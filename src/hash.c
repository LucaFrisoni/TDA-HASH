#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CAPACIDAD_MINIMA 3
#define FACTOR_MAX 0.75
#define MULTIPLICADOR_CAPACIDAD 2

typedef struct nodo {
	char *clave;
	void *valor;
	struct nodo *siguiente;
} nodo_t;

struct hash {
	size_t capacidad;
	size_t cantidad;
	nodo_t **buckets;
};

size_t funcion_hash(const char *clave)
{
	size_t hash = 5381;
	int c;

	while ((c = *clave++)) {
		hash = ((hash << 5) + hash) + (unsigned char)c;
	}

	return hash;
}

nodo_t *buscando_nodo_y_anterior(hash_t *hash, const char *clave,
				 nodo_t **nodo_anterior, size_t *indice_ref)
{
	size_t indice = funcion_hash(clave) % hash->capacidad;

	nodo_t *nodo_actual = hash->buckets[indice];
	nodo_t *anterior = NULL;

	while (nodo_actual) {
		if (strcmp(nodo_actual->clave, clave) == 0)
			break;
		anterior = nodo_actual;
		nodo_actual = nodo_actual->siguiente;
	}

	if (nodo_anterior)
		*nodo_anterior = anterior;
	if (indice_ref)
		*indice_ref = indice;

	return nodo_actual;
}
// --------------------------------------------------------------------------------------------------------Funciones de Hash
nodo_t *creando_nodo(char *clave, void *valor)
{
	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
	if (!nuevo_nodo)
		return NULL;

	nuevo_nodo->clave = clave;
	nuevo_nodo->valor = valor;

	return nuevo_nodo;
}

nodo_t **crear_buckets(size_t capacidad)
{
	nodo_t **buckets = calloc(capacidad, sizeof(nodo_t *));
	if (!buckets)
		return NULL;

	return buckets;
}

hash_t *crear_hash_estructura_base(size_t capacidad)
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	if (!hash)
		return NULL;

	hash->capacidad = capacidad;

	hash->buckets = crear_buckets(capacidad);
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

	hash_t *hash = crear_hash_estructura_base(capacidad_inicial);
	if (!hash)
		return NULL;

	return hash;
}
// --------------------------------------------------------------------------------------------------
size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;

	return hash->cantidad;
}
// --------------------------------------------------------------------------------------------------
bool hash_necesita_rehash(hash_t *hash)
{
	return ((double)hash->cantidad / (double)hash->capacidad) > FACTOR_MAX;
}

void reinsertar_nodos(hash_t *hash, nodo_t **buckets_nuevos,
		      size_t capacidad_nueva)
{
	nodo_t **buckets_viejos = hash->buckets;
	size_t i;

	for (i = 0; i < hash->capacidad; i++) {
		nodo_t *nodo_actual = buckets_viejos[i];
		while (nodo_actual) {
			nodo_t *nodo_siguiente = nodo_actual->siguiente;

			size_t nuevo_indice = funcion_hash(nodo_actual->clave) %
					      capacidad_nueva;

			// Inserta al principio del nuevo bucket
			nodo_actual->siguiente = buckets_nuevos[nuevo_indice];
			buckets_nuevos[nuevo_indice] = nodo_actual;

			nodo_actual = nodo_siguiente;
		}
	}
}

void reemplazar_buckets(hash_t *hash, nodo_t **nuevos_buckets,
			size_t nueva_capacidad)
{
	free(hash->buckets);
	hash->buckets = nuevos_buckets;
	hash->capacidad = nueva_capacidad;
}

bool hash_rehash(hash_t *hash)
{
	size_t nueva_capacidad = hash->capacidad * MULTIPLICADOR_CAPACIDAD;

	nodo_t **nuevos_buckets = crear_buckets(nueva_capacidad);
	if (!nuevos_buckets)
		return false;

	reinsertar_nodos(hash, nuevos_buckets, nueva_capacidad);
	reemplazar_buckets(hash, nuevos_buckets, nueva_capacidad);

	return true;
}

bool insertar_nodo(hash_t *hash, nodo_t *nodo_anterior, size_t indice,
		   char *clave, void *valor, void **encontrado)
{
	nodo_t *nuevo_nodo = creando_nodo(clave, valor);
	if (!nuevo_nodo)
		return false;

	//Inserto nodo
	if (!nodo_anterior)
		hash->buckets[indice] = nuevo_nodo;
	else
		nodo_anterior->siguiente = nuevo_nodo;

	if (encontrado)
		*encontrado = NULL;

	hash->cantidad++;

	if (hash_necesita_rehash(hash))
		return hash_rehash(hash);

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

	size_t indice = 0;
	nodo_t *nodo_anterior = NULL;

	nodo_t *nodo_encontrado =
		buscando_nodo_y_anterior(hash, clave, &nodo_anterior, &indice);

	//Si encuentra un nodo significa que la clave coincide
	if (nodo_encontrado)
		return caso_clave_existente(nodo_encontrado, valor, encontrado);

	char *copia_clave = malloc(strlen(clave) + 1);
	if (!copia_clave)
		return false;
	strcpy(copia_clave, clave);

	return insertar_nodo(hash, nodo_anterior, indice, copia_clave, valor,
			     encontrado);
}
// --------------------------------------------------------------------------------------------------
void *hash_buscar(hash_t *hash, char *clave)
{
	if (!hash || !clave)
		return NULL;

	nodo_t *nodo_encontrado =
		buscando_nodo_y_anterior(hash, clave, NULL, NULL);
	return nodo_encontrado ? nodo_encontrado->valor : NULL;
}
// --------------------------------------------------------------------------------------------------
bool hash_contiene(hash_t *hash, char *clave)
{
	if (!hash || !clave)
		return false;
	return buscando_nodo_y_anterior(hash, clave, NULL, NULL) != NULL;
}
// --------------------------------------------------------------------------------------------------
void *quitando_nodo_y_devolviendo_valor(hash_t *hash, size_t indice,
					nodo_t *nodo_encontrado,
					nodo_t *nodo_anterior)
{
	// Caso nodo en el medio o al final
	if (nodo_anterior)
		nodo_anterior->siguiente = nodo_encontrado->siguiente;
	// Caso primer nodo del bucket
	else
		hash->buckets[indice] = nodo_encontrado->siguiente;

	void *valor = nodo_encontrado->valor;
	free(nodo_encontrado);
	hash->cantidad--;
	return valor;
}

void *hash_quitar(hash_t *hash, char *clave)
{
	if (!hash || !clave)
		return NULL;

	size_t indice = 0;
	nodo_t *nodo_anterior = NULL;

	nodo_t *nodo_encontrado =
		buscando_nodo_y_anterior(hash, clave, &nodo_anterior, &indice);

	if (nodo_encontrado)
		return quitando_nodo_y_devolviendo_valor(
			hash, indice, nodo_encontrado, nodo_anterior);

	return NULL;
}
// --------------------------------------------------------------------------------------------------
size_t hash_iterar(hash_t *hash, bool (*f)(char *, void *, void *), void *ctx)
{
	if (!hash || !f)
		return 0;

	size_t contador = 0;
	size_t i;

	for (i = 0; i < hash->capacidad && hash->cantidad > 0; i++) {
		nodo_t *nodo_actual = hash->buckets[i];

		while (nodo_actual) {
			nodo_t *nodo_siguiente = nodo_actual->siguiente;

			bool ok =
				f(nodo_actual->clave, nodo_actual->valor, ctx);
			contador++;
			if (!ok)
				return contador;

			nodo_actual = nodo_siguiente;
		}
	}
	return contador;
}
// --------------------------------------------------------------------------------------------------
void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}
// --------------------------------------------------------------------------------------------------
void destruir_elementos(hash_t *hash, void (*destructor)(void *))
{
	size_t i;

	for (i = 0; i < hash->capacidad && hash->cantidad > 0; i++) {
		nodo_t *nodo_actual = hash->buckets[i];

		while (nodo_actual) {
			nodo_t *nodo_siguiente = nodo_actual->siguiente;

			if (destructor)
				destructor(nodo_actual->valor);

			free(nodo_actual->clave);
			free(nodo_actual);
			hash->cantidad--;

			nodo_actual = nodo_siguiente;
		}
	}
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;

	destruir_elementos(hash, destructor);

	free(hash->buckets);
	free(hash);
}