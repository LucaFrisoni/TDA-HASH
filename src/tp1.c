#include "tp1.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define CANT_INICIAL 128
struct tp1 {
	struct pokemon **pokemones; //Array de puntero a pokemones
	size_t cantidad;
};

// -----------------------------------------MIS FUNCIONES---------------------------------------------------
void bubbleSort_pokemones_alfabeticamente_asc_optimizado(tp1_t *tp1)
{
	if (!tp1 || tp1->cantidad == 0)
		return;

	size_t i, j;
	bool hubo_intercambio = true;
	i = 1;

	while (i < tp1->cantidad && hubo_intercambio) {
		hubo_intercambio = false;
		for (j = 0; j < tp1->cantidad - i; j++) {
			if (strcmp(tp1->pokemones[j]->nombre,
				   tp1->pokemones[j + 1]->nombre) > 0) {
				hubo_intercambio = true;
				struct pokemon *pokemon_aux = tp1->pokemones[j];
				tp1->pokemones[j] = tp1->pokemones[j + 1];
				tp1->pokemones[j + 1] = pokemon_aux;
			}
		}
		i++;
	}
}
void bubbleSort_pokemones_id_asc_optimizado(tp1_t *tp1)
{
	if (!tp1 || tp1->cantidad == 0)
		return;

	size_t i, j;
	bool hubo_intercambio = true;
	i = 1;

	while (i < tp1->cantidad && hubo_intercambio) {
		hubo_intercambio = false;
		for (j = 0; j < tp1->cantidad - i; j++) {
			if (tp1->pokemones[j]->id > tp1->pokemones[j + 1]->id) {
				hubo_intercambio = true;
				struct pokemon *pokemon_aux = tp1->pokemones[j];
				tp1->pokemones[j] = tp1->pokemones[j + 1];
				tp1->pokemones[j + 1] = pokemon_aux;
			}
		}
		i++;
	}
}
// --------------------------------------------------------------------------------------------------------
char *resize_buffer(char *buffer, size_t *capacidad)
{
	if (!capacidad || *capacidad <= 0)
		return NULL;

	*capacidad *= 2;
	char *tmp = realloc(buffer, *capacidad);
	if (!tmp) {
		free(buffer);
		return NULL;
	}
	return tmp;
}
// --------------------------------------------------------------------------------------------------------
bool validandoModo(const char *modo)
{
	if (!modo)
		return false;

	return strcmp(modo, "r") == 0 || strcmp(modo, "r+") == 0 ||
	       strcmp(modo, "w") == 0 || strcmp(modo, "w+") == 0 ||
	       strcmp(modo, "a") == 0 || strcmp(modo, "a+") == 0;
}
FILE *manejador_de_archivos_open(const char *nombre_archivo, char *s)
{
	if (!nombre_archivo || strlen(nombre_archivo) == 0)
		return NULL;
	if (!validandoModo(s))
		return NULL;

	FILE *archivo = fopen(nombre_archivo, s);
	if (!archivo)
		return NULL;

	return archivo;
}
// --------------------------------------------------------------------------------------------------------
const char *tipo_a_string(enum tipo_pokemon tipo)
{
	switch (tipo) {
	case TIPO_ELEC:
		return "ELEC";
	case TIPO_FUEG:
		return "FUEG";
	case TIPO_PLAN:
		return "PLAN";
	case TIPO_AGUA:
		return "AGUA";
	case TIPO_NORM:
		return "NORM";
	case TIPO_FANT:
		return "FANT";
	case TIPO_PSI:
		return "PSI";
	case TIPO_LUCH:
		return "LUCH";
	default:
		return "DESCONOCIDO";
	}
}

char *leer_linea(FILE *archivo, size_t *capacidad)
{
	size_t longitud = 0;
	bool leyendo = true;
	char *buffer;

	if (!archivo || !capacidad || *capacidad <= 0)
		return NULL;

	buffer = malloc(*capacidad);

	while (leyendo) {
		if (fgets(&buffer[longitud], (int)(*capacidad - longitud),
			  archivo) == NULL) {
			if (longitud == 0) { //Final del archivo
				free(buffer);
				return NULL;
			}
			return buffer; //Final del archivo pero terminaba sin \n;
		}
		//buffer es solo lo que se leyo
		longitud += strlen(buffer + longitud);

		// Línea completa leída
		if (buffer[longitud - 1] == '\n') {
			buffer[longitud - 1] = '\0';
			return buffer;
		}

		// Buffer lleno pero no hay '\n', duplicamos el tamaño
		buffer = resize_buffer(buffer, capacidad);
	}
	return NULL;
}
bool escribiendo_linea(tp1_t *tp1, FILE *archivo)
{
	if (!tp1 || !archivo)
		return false;

	for (size_t i = 0; i < tp1->cantidad; i++) {
		struct pokemon *p = tp1->pokemones[i];
		if (fprintf(archivo, "%d,%s,%s,%d,%d,%d\n", p->id, p->nombre,
			    tipo_a_string(p->tipo), p->ataque, p->defensa,
			    p->velocidad) < 0) {
			return false; // Error de escritura
		}
	}
	return true;
}
// --------------------------------------------------------------------------------------------------------
bool buscando_duplicados(tp1_t *tp1, struct pokemon *pk)
{
	for (size_t i = 0; i < tp1->cantidad; i++) {
		if (tp1->pokemones[i]->id == pk->id)
			return true; // Es duplicado
	}
	return false;
}

enum tipo_pokemon parse_tipo(const char *buffer)
{
	for (int t = TIPO_ELEC; t <= TIPO_LUCH; t++) {
		if (strcmp(buffer, tipo_a_string(t)) == 0) {
			return t; // Encontro el ENUM
		}
	}
	return -1; // tipo inválido
}

struct pokemon *liberando_memoria_parse_pokemon_y_null(struct pokemon *p)
{
	if (!p)
		return NULL;
	if (p->nombre)
		free(p->nombre);
	free(p);
	return NULL;
}

bool switch_pokemon(struct pokemon *p, int campo, const char *linea)
{
	switch (campo) {
	case 0: // ID
		p->id = atoi(linea);
		return true;
	case 1: // Nombre
		p->nombre = malloc(strlen(linea) + 1);
		if (!p->nombre)
			return false;
		strcpy(p->nombre, linea);
		return true;
	case 2: // Tipo
		p->tipo = parse_tipo(linea);
		if ((int)p->tipo == -1)
			return false;
		return true;
	case 3: // Ataque
		p->ataque = atoi(linea);
		return true;
	case 4: // Defensa
		p->defensa = atoi(linea);
		return true;
	case 5: // Velocidad
		p->velocidad = atoi(linea);
		return true;
	default:
		return false; // campo inválido
	}
}

struct pokemon *procesando_parse_pokemon(struct pokemon *p, char *linea)
{
	int campo = 0;
	char *coma = NULL;

	while ((coma = strchr(linea, ',')) != NULL) {
		*coma = '\0'; // corto el campo
		if (!switch_pokemon(p, campo, linea))
			return liberando_memoria_parse_pokemon_y_null(p);
		linea = coma + 1;
		campo++;
	}

	// procesar último campo (después de la última coma)
	if (!switch_pokemon(p, campo, linea))
		return liberando_memoria_parse_pokemon_y_null(p);
	campo++;

	// validar cantidad de campos
	if (campo != 6)
		return liberando_memoria_parse_pokemon_y_null(p);

	return p;
}

struct pokemon *parsear_pokemon(char *linea)
{
	struct pokemon *p;

	if (!linea || linea[0] == '\0' || strchr(linea, ';') != NULL)
		return NULL;

	p = calloc(1, sizeof(struct pokemon));
	if (!p)
		return NULL;

	return procesando_parse_pokemon(p, linea);
}

struct pokemon *procesando_copia_pokemon(struct pokemon *pk)
{
	struct pokemon *nuevo_pk = malloc(sizeof(struct pokemon));
	if (!nuevo_pk)
		return NULL;

	// Copio todo el pokemon
	nuevo_pk->id = pk->id;
	nuevo_pk->tipo = pk->tipo;
	nuevo_pk->ataque = pk->ataque;
	nuevo_pk->defensa = pk->defensa;
	nuevo_pk->velocidad = pk->velocidad;

	nuevo_pk->nombre = malloc(strlen(pk->nombre) + 1);
	if (!nuevo_pk->nombre) {
		free(nuevo_pk);
		return NULL;
	}
	strcpy(nuevo_pk->nombre, pk->nombre);

	return nuevo_pk;
}

void agregar_pokemon(tp1_t *tp1, struct pokemon *pk, bool copiar)
{
	if (buscando_duplicados(tp1, pk)) {
		free(pk->nombre);
		free(pk);
		return; // si es duplicado
	}

	struct pokemon *nuevo_pk = pk;

	//Esto lo agregue por un segmention fault con tp1_destruir
	if (copiar) {
		nuevo_pk = procesando_copia_pokemon(pk);
		if (!nuevo_pk)
			return;
	}

	// Redimensiono arreglo de punteros
	struct pokemon **tmp = realloc(
		tp1->pokemones, sizeof(struct pokemon *) * (tp1->cantidad + 1));
	if (!tmp) {
		if (copiar) {
			free(nuevo_pk->nombre);
			free(nuevo_pk);
		}
		return;
	}

	tp1->pokemones = tmp;
	tp1->pokemones[tp1->cantidad] = nuevo_pk;
	tp1->cantidad++;
}

// ----------------------------------------------TP1 FUNCIONES------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
tp1_t *tp1_leer_archivo(const char *nombre)
{
	tp1_t *tp1;
	FILE *archivo;
	char *linea;
	size_t capacidad = CANT_INICIAL;
	struct pokemon *pk;

	archivo = manejador_de_archivos_open(nombre, "r");
	if (!archivo)
		return NULL;

	tp1 = calloc(1, sizeof(tp1_t));

	while ((linea = leer_linea(archivo, &capacidad))) {
		pk = parsear_pokemon(linea);
		if (pk)
			agregar_pokemon(tp1, pk, false);
		free(linea);
	}

	// Significa que todos los pokemones eran invalidos o que el archivo estaba vacio
	if (!tp1->pokemones) {
		free(tp1->pokemones);
		free(tp1);
		fclose(archivo);
		return NULL;
	}

	bubbleSort_pokemones_id_asc_optimizado(tp1);
	fclose(archivo);
	return tp1;
}
// --------------------------------------------------------------------------------------------------------
size_t tp1_cantidad(tp1_t *tp1)
{
	if (!tp1) {
		return 0;
	} else {
		return tp1->cantidad;
	}
}
// --------------------------------------------------------------------------------------------------------
tp1_t *tp1_guardar_archivo(tp1_t *tp1, const char *nombre)
{
	FILE *archivo;

	if (!tp1)
		return NULL;

	archivo = manejador_de_archivos_open(nombre, "w");
	if (!archivo)
		return NULL;

	if (!escribiendo_linea(tp1, archivo)) {
		fclose(archivo);
		return NULL;
	}

	fclose(archivo);

	return tp1;
}
// --------------------------------------------------------------------------------------------------------
int obtener_max_id(tp1_t *tp)
{
	return tp->pokemones[tp->cantidad - 1]->id;
}

tp1_t *crear_tp_si_valido(tp1_t *tp1, tp1_t *tp2)
{
	if (!tp1 || !tp2)
		return NULL;

	if (tp1->cantidad == 0 && tp2->cantidad == 0)
		return NULL;

	tp1_t *nuevo = calloc(1, sizeof(tp1_t));
	if (!nuevo)
		return NULL;

	return nuevo;
}

void copiar_pokemones(tp1_t *destino, tp1_t *origen, bool *ids_existentes)
{
	for (size_t i = 0; i < origen->cantidad; i++) {
		int id = origen->pokemones[i]->id;
		if (!ids_existentes[id]) {
			agregar_pokemon(destino, origen->pokemones[i], true);
			ids_existentes[id] = true;
		}
	}
}

tp1_t *tp1_union(tp1_t *un_tp, tp1_t *otro_tp)
{
	tp1_t *tp3 = crear_tp_si_valido(un_tp, otro_tp);
	if (!tp3)
		return NULL;

	// Obtengo el ID máximo de ambos
	int max_id_tp1 = obtener_max_id(un_tp);
	int max_id_tp2 = obtener_max_id(otro_tp);
	int max_id = (max_id_tp1 > max_id_tp2) ? max_id_tp1 : max_id_tp2;

	// Vector de control para saber qué IDs ya se agregaron
	bool *ids_existentes = calloc((size_t)max_id + 1, sizeof(bool));
	if (!ids_existentes) {
		free(tp3);
		return NULL;
	}

	// Agrego primero todos los de un_tp
	copiar_pokemones(tp3, un_tp, ids_existentes);

	// Agrego los de otro_tp, solo si no estaban ya
	copiar_pokemones(tp3, otro_tp, ids_existentes);

	free(ids_existentes);

	return tp3;
}
// --------------------------------------------------------------------------------------------------------
tp1_t *tp1_interseccion(tp1_t *un_tp, tp1_t *otro_tp)
{
	tp1_t *tp3 = crear_tp_si_valido(un_tp, otro_tp);
	if (!tp3)
		return NULL;
	size_t i = 0, j = 0;

	while (i < un_tp->cantidad && j < otro_tp->cantidad) {
		int id_1 = un_tp->pokemones[i]->id;
		int id_2 = otro_tp->pokemones[j]->id;

		if (id_1 == id_2) {
			agregar_pokemon(tp3, un_tp->pokemones[i], true);
			i++;
			j++;
		} else if (id_1 < id_2) {
			i++;
		} else {
			j++;
		}
	}

	return tp3;
}
// --------------------------------------------------------------------------------------------------------
void marcar_ids_tp(tp1_t *tp, bool *vector)
{
	for (size_t i = 0; i < tp->cantidad; i++) {
		int id = tp->pokemones[i]->id;
		vector[id] = true;
	}
}

tp1_t *tp1_diferencia(tp1_t *un_tp, tp1_t *otro_tp)
{
	tp1_t *tp3 = crear_tp_si_valido(un_tp, otro_tp);
	if (!tp3)
		return NULL;

	// Obtengo el ID máximo de ambos
	int max_id_tp1 = obtener_max_id(un_tp);
	int max_id_tp2 = obtener_max_id(otro_tp);
	int max_id = (max_id_tp1 > max_id_tp2) ? max_id_tp1 : max_id_tp2;

	// Vector de control para saber qué IDs ya se agregaron
	bool *ids_existentes = calloc((size_t)max_id + 1, sizeof(bool));
	if (!ids_existentes) {
		free(tp3);
		return NULL;
	}

	// Marco todos los IDs de tp2
	marcar_ids_tp(otro_tp, ids_existentes);

	// Agrego todos los de tp1 que no estén en tp2
	copiar_pokemones(tp3, un_tp, ids_existentes);

	free(ids_existentes);
	return tp3;
}
// --------------------------------------------------------------------------------------------------------
struct pokemon *tp1_buscar_nombre(tp1_t *tp, const char *nombre)
{
	if (!tp || !nombre || tp->cantidad == 0)
		return NULL;

	size_t i;
	for (i = 0; i < tp->cantidad; i++) {
		if (strcmp(tp->pokemones[i]->nombre, nombre) == 0)
			return tp->pokemones[i];
	}
	return NULL;
}
// --------------------------------------------------------------------------------------------------------
struct pokemon *tp1_buscar_id(tp1_t *tp, int id)
{
	if (!tp || tp->cantidad == 0 || id < 0)
		return NULL;

	size_t i;
	for (i = 0; i < tp->cantidad; i++) {
		if (tp->pokemones[i]->id == id)
			return tp->pokemones[i];
	}
	return NULL;
}
// --------------------------------------------------------------------------------------------------------
size_t tp1_con_cada_pokemon(tp1_t *un_tp, bool (*f)(struct pokemon *, void *),
			    void *extra)
{
	size_t i;
	size_t count = 0;
	if (!un_tp || !f)
		return 0;

	for (i = 0; i < un_tp->cantidad; i++) {
		count++;
		if (!f(un_tp->pokemones[i], extra)) {
			break;
		}
	}
	return count;
}
// --------------------------------------------------------------------------------------------------------
void tp1_destruir(tp1_t *tp1)
{
	if (!tp1)
		return;

	for (size_t i = 0; i < tp1->cantidad; i++) {
		if (tp1->pokemones[i]) {
			free(tp1->pokemones[i]->nombre);
			free(tp1->pokemones[i]);
		}
	}

	free(tp1->pokemones);
	free(tp1);
}