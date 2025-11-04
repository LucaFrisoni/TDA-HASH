#include "src/hash.h"
#include "src/tp1.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define CAPACIDAD_HASH 10
#define NUM_PARAMETRO_ARCHIVO 1
#define NUM_PARAMETRO_OPERACION 2
#define NUM_PARAMETRO_BUSQUEDA 3
#define NUM_PARAMATRO_VALOR_BUSQUEDA 4
#define CANTIDAD_DE_PARAMETROS 5
#define TAMAÑO_CTX 2

// ----------------------- Callback para guardar en hash -----------------------
bool guardar_en_hash(struct pokemon *poke, void *extra)
{
	hash_t *hash = extra;
	return hash_insertar(hash, poke->nombre, poke, NULL);
}
// ----------------------- Validaciones -----------------------
bool validando_formato_csv(const char *archivo)
{
	if (!archivo)
		return false;

	const char *ext = strrchr(archivo, '.');
	if (!ext || strcmp(ext, ".csv") != 0)
		return false;

	return true;
}

bool validando_params(int argc, char *argv[])
{
	if (argc < CANTIDAD_DE_PARAMETROS) {
		printf("Se deben de pasar los 5 parametros permitidos <ejecutable> <archivo_pokemones.csv> buscar nombre|id <valor>\n");
		return false;
	}
	if (argc > CANTIDAD_DE_PARAMETROS) {
		printf("Solo 5 parametros permitidos <ejecutable> <archivo_pokemones.csv> buscar nombre|id <valor>\n");
		return false;
	}
	if (!validando_formato_csv(argv[NUM_PARAMETRO_ARCHIVO])) {
		printf("El archivo pasado debe de estar en formato 'csv'\n");
		return false;
	}
	if (strcmp(argv[NUM_PARAMETRO_OPERACION], "buscar") != 0) {
		printf("El tercer argumento debe ser 'buscar'\n");
		return false;
	}
	if (strcmp(argv[NUM_PARAMETRO_BUSQUEDA], "nombre") != 0 &&
	    strcmp(argv[NUM_PARAMETRO_BUSQUEDA], "id") != 0) {
		printf("El cuarto argumento para buscar debe ser 'nombre' o 'id'\n");
		return false;
	}
	return true;
}

bool validando_func(tp1_t *tp1, hash_t *hash)
{
	if (tp1 && !hash) {
		tp1_destruir(tp1);
		return false;
	}
	if (!tp1 && hash) {
		hash_destruir(hash);
		return false;
	}
	if (!tp1 && !hash)
		return false;
	return true;
}

bool cargando_pokemons(tp1_t *tp1, hash_t *hash)
{
	size_t cant_pokemones =
		tp1_con_cada_pokemon(tp1, guardar_en_hash, hash);
	size_t cant_pk_hash = hash_cantidad(hash);
	if (cant_pk_hash != cant_pokemones || cant_pokemones == 0)
		return false;
	return true;
}
// ----------------------- Mostrar -----------------------
const char *convert_tipo_a_string(enum tipo_pokemon tipo)
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

void mostrar_pokemon(struct pokemon *p)
{
	if (!p)
		return;
	printf("Id: %d\n", p->id);
	printf("Nombre: %s\n", p->nombre);
	printf("Tipo: %s\n", convert_tipo_a_string(p->tipo));
	printf("Ataque: %d\n", p->ataque);
	printf("Defensa: %d\n", p->defensa);
	printf("Velocidad: %d\n\n", p->velocidad);
}
// ----------------------- Búsqueda  -----------------------
bool buscando_pk_id(char *clave, void *pk, void *ctx)
{
	struct pokemon *p_hash = (struct pokemon *)pk;

	void **param = (void **)ctx;

	struct pokemon **p = (struct pokemon **)param[0];
	int id = *(int *)param[1];

	if (p_hash->id == id) {
		*p = p_hash;
		return false;
	}
	return true;
}

struct pokemon *pokemon_encontrado(char *argv[], hash_t *hash)
{
	struct pokemon *p = NULL;
	// Buscar por nombre
	if (strcmp(argv[NUM_PARAMETRO_BUSQUEDA], "nombre") == 0)
		p = hash_buscar(hash, argv[NUM_PARAMATRO_VALOR_BUSQUEDA]);

	// Buscar por id
	if (strcmp(argv[NUM_PARAMETRO_BUSQUEDA], "id") == 0) {
		int id = atoi(argv[NUM_PARAMATRO_VALOR_BUSQUEDA]);
		void *ctx[TAMAÑO_CTX] = { &p, &id };
		hash_iterar(hash, buscando_pk_id, ctx);
	}
	return p;
}

int main(int argc, char *argv[])
{
	if (!validando_params(argc, argv))
		return -1;

	tp1_t *tp1 = tp1_leer_archivo(argv[1]);
	hash_t *hash = hash_crear(CAPACIDAD_HASH);
	if (!validando_func(tp1, hash))
		return -2;

	if (!cargando_pokemons(tp1, hash))
		return -3;

	struct pokemon *p = pokemon_encontrado(argv, hash);

	if (p)
		mostrar_pokemon(p);
	else
		printf("No se encontró el pokemon\n");

	hash_destruir(hash);
	tp1_destruir(tp1);
	return 0;
}