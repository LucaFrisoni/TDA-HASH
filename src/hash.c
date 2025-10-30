#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDAD_MINIMA 3

typedef struct nodo {
	char *clave;
	void *valor;
	struct nodo *siguiente;
} nodo_t;

struct hash {
	size_t capacidad;
	nodo_t **buckets; //Array de doble punteros a una estructura externa
};

unsigned int funcion_hash(const char *clave)
{
	if (!clave)
		return 0;

	unsigned int hash = 0;
	while (*clave) {
		hash += (unsigned char)(*clave); // suma simple de ASCII
		clave++;
	}

	return hash;
}

// ----------------------------------------------------------------------Funciones de Hash

nodo_t *creando_nodo()
{
	nodo_t *nodo = calloc(1, sizeof(nodo_t));
	if (!nodo)
		return NULL;
	return nodo;
}

hash_t *hash_estructura_base(size_t capacidad)
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	if (!hash)
		return NULL;

	hash->capacidad = capacidad;

	hash->buckets = calloc(capacidad, sizeof(nodo_t *));
	if (!hash->buckets) {
		free(hash);
		return NULL;
	}

	return hash;
}

hash_t *hash_crear(size_t capacidad_inicial)
{
	if (!capacidad_inicial)
		return NULL;

	if (capacidad_inicial < CAPACIDAD_MINIMA)
		capacidad_inicial = CAPACIDAD_MINIMA;

	hash_t *hash = hash_estructura_base(capacidad_inicial);
	if (!hash)
		return NULL;

	return hash;
}
// -------------------------------------------------
size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;

	return hash->capacidad;
}
// -------------------------------------------------

nodo_t *buscar_nodo_o_ultimo(nodo_t *nodo, const char *clave)
{
	if (!nodo)
		return NULL;

	if (strcmp(nodo->clave, clave) == 0)
		return nodo;

	if (!nodo->siguiente)
		return nodo; // este es el último nodo (no se encontró la clave)

	return buscar_nodo_o_ultimo(nodo->siguiente, clave);
}

bool insertar_nodo(hash_t *hash, size_t indice, nodo_t *nodo_encontrado,
		   char *clave, void *valor, void **encontrado)
{
	nodo_t *nuevo = creando_nodo();
	if (!nuevo)
		return false;

	nuevo->clave = clave;
	nuevo->valor = valor;

	if (!nodo_encontrado) // Caso hash vacio
		hash->buckets[indice] = nuevo;
	else // Caso ya había nodos => agregar al final
		nodo_encontrado->siguiente = nuevo;

	if (encontrado)
		*encontrado = NULL;

	return true;
}

bool caso_clave_existente(nodo_t *nodo_existente, void *valor,
			  void **encontrado)
{
	if (encontrado)
		*encontrado = nodo_existente->valor;

	nodo_existente->valor = valor;
	return true;
}

bool hash_insertar(hash_t *hash, char *clave, void *valor, void **encontrado)
{
	if (!hash || !clave)
		return false;

	size_t indice = funcion_hash(clave) % hash->capacidad;
	nodo_t *nodo = hash->buckets[indice];
	nodo_t *nodo_encontrado = buscar_nodo_o_ultimo(nodo, clave);

	if (strcmp(nodo_encontrado->clave, clave) != 0)
		return insertar_nodo(hash, indice, nodo_encontrado, clave,
				     valor, encontrado);

	return caso_clave_existente(nodo_encontrado, valor, encontrado);
}
// -------------------------------------------------
void *hash_buscar(hash_t *hash, char *clave)
{
}
// -------------------------------------------------
bool hash_contiene(hash_t *hash, char *clave)
{
}
// -------------------------------------------------
void *hash_quitar(hash_t *hash, char *clave)
{
}
// -------------------------------------------------
size_t hash_iterar(hash_t *hash, bool (*f)(char *, void *, void *), void *ctx)
{
}
// -------------------------------------------------
void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}
// -------------------------------------------------
void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
}