#include "pa2m.h"
#include "src/hash.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX_CLAVE_RANDOM 32
#define CANT_PRUEBA_ESTRES 5000

void insertar_muchos_elementos(hash_t *hash, size_t cantidad)
{
	char clave[MAX_CLAVE_RANDOM];

	for (size_t i = 0; i < cantidad; i++) {
		int *valor = malloc(sizeof(int));
		if (!valor)
			return;
		*valor = (int)i;
		sprintf(clave, "key_%zu", i);
		hash_insertar(hash, clave, valor, NULL);
	}
}
//-------------------------------------------------------------------------Test Unitarios

void test_hash_crear_capacidad_inicial_menor_3()
{
	hash_t *hash = hash_crear(2);
	pa2m_afirmar(
		hash != NULL,
		"Se crea correctamente el hash al pasarle una capacidad menor a 3");
	hash_destruir(hash);
}
void test_hash_crear_capacidad_inicial_mayor_3()
{
	hash_t *hash = hash_crear(6);
	pa2m_afirmar(
		hash != NULL,
		"Se crea correctamente el hash al pasarle una capacidad mayor a 3");
	hash_destruir(hash);
}
void test_hash_crear_capacidad_inicial_extremadamante_grande()
{
	hash_t *hash = hash_crear(10000);
	pa2m_afirmar(
		hash != NULL,
		"Se crea correctamente el hash al pasarle una capacidad extremadamante grande");
	hash_destruir(hash);
}

void tests_hash_crear()
{
	test_hash_crear_capacidad_inicial_menor_3();
	test_hash_crear_capacidad_inicial_mayor_3();
	test_hash_crear_capacidad_inicial_extremadamante_grande();
}
//-------------------------------------------------------------------------
void test_hash_cantidad_hash_null()
{
	pa2m_afirmar(hash_cantidad(NULL) == 0, "Pasar un hash NULL devuelve 0");
}
void test_hash_cantidad_al_crear()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "Un hash recien creado inicializa con la cantidad en 0");
	hash_destruir(hash);
}

void tests_hash_cantidad()
{
	test_hash_cantidad_hash_null();
	test_hash_cantidad_al_crear();
}
//-------------------------------------------------------------------------
void test_hash_insertar_hash_null()
{
	pa2m_afirmar(hash_insertar(NULL, "clave", "valor", NULL) == false,
		     "Insertar en un hash NULL devuelve false");
}
void test_hash_insertar_clave_null()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash_insertar(hash, NULL, "valor", NULL) == false,
		     "Insertar una clave NULL devuelve false");
	hash_destruir(hash);
}
void test_hash_insertar_inserta_correctamente_hash_vacio()
{
	hash_t *hash = hash_crear(10);
	bool inserto = hash_insertar(hash, "clave", "valor", NULL);

	pa2m_afirmar(inserto == true,
		     "Se puede insertar un elemento en un hash vacío");
	pa2m_afirmar(
		hash_cantidad(hash) == 1,
		"La cantidad del hash pasa a ser 1 tras insertar un elemento");

	hash_destruir(hash);
}
void test_hash_insertar_inserta_correctamente_hash_varios_elementos()
{
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "clave1", "valor1", NULL);
	hash_insertar(hash, "clave2", "valor2", NULL);
	hash_insertar(hash, "clave3", "valor3", NULL);

	pa2m_afirmar(hash_cantidad(hash) == 3,
		     "Se pueden insertar múltiples elementos en un hash");
}
void test_hash_insertar_inserta_correctamente_coliciones()
{
	hash_t *hash = hash_crear(10);

	//Todas estas claves colicionan
	bool inserto1 = hash_insertar(hash, "a", "valor1", NULL);
	bool inserto2 = hash_insertar(hash, "k", "valor2", NULL);
	bool inserto3 = hash_insertar(hash, "u", "valor3", NULL);
	bool inserto4 = hash_insertar(hash, "ad", "valor4", NULL);
	bool inserto5 = hash_insertar(hash, "an", "valor5", NULL);

	pa2m_afirmar(
		inserto1 && inserto2 && inserto3 && inserto4 && inserto5,
		"Se pueden insertar correctamente elementos que colisionan en la misma posición");

	pa2m_afirmar(
		hash_cantidad(hash) == 5,
		"La cantidad del hash es correcto al insertar elementos que colicionan");

	hash_destruir(hash);
}
void test_hash_insertar_modifica_correctamente_valor_clave_existente()
{
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "clave", "valor1", NULL);
	hash_insertar(hash, "clave", "valor2", NULL);

	pa2m_afirmar(
		hash_cantidad(hash) == 1,
		"La cantidad no aumenta al reemplazar el valor de una clave existente");

	hash_destruir(hash);
}
void test_hash_insertar_aumenta_cantidad()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "Al inicio el hash tiene cantidad 0");

	hash_insertar(hash, "clave", "valor", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 1,
		     "Inserta correctamente y aumenta cantidad a 1");

	hash_insertar(hash, "otra_clave", "otro_valor", NULL);
	hash_insertar(hash, "otra_clave2", "otro_valor", NULL);
	hash_insertar(hash, "otra_clave3", "otro_valor", NULL);
	pa2m_afirmar(
		hash_cantidad(hash) == 4,
		"Inserta varios elementos aumenta la cantidad correctamente");

	hash_destruir(hash);
}
void test_hash_insertar_hace_rehash_si_supera_factor()
{
	hash_t *hash = hash_crear(5);

	insertar_muchos_elementos(hash, 50);

	pa2m_afirmar(
		hash_cantidad(hash) == 50,
		"Al Insertar muchos elementos que supera factor de carga se hace el rehash correctamente");

	hash_destruir_todo(hash, free);
}
void test_hash_insertar_encontrado_null()
{
	hash_t *hash = hash_crear(10);

	bool inserto = hash_insertar(hash, "clave1", "valor1", NULL);
	pa2m_afirmar(inserto, "Puedo insertar sin pasar puntero de encontrado");

	hash_destruir(hash);
}
void test_hash_insertar_pasando_encontrado_clave_existente()
{
	hash_t *hash = hash_crear(10);
	void *encontrado = NULL;

	hash_insertar(hash, "clave1", "valor1", NULL);
	hash_insertar(hash, "clave1", "nuevoValor", &encontrado);

	pa2m_afirmar(
		strcmp(encontrado, "valor1") == 0,
		"Encontrado guarda correctamente el valor que fue modificado");

	hash_destruir(hash);
}
void test_hash_insertar_pasando_encontrado_insertando_nodo()
{
	hash_t *hash = hash_crear(10);
	void *encontrado = NULL;

	hash_insertar(hash, "clave1", "valor1", &encontrado);

	pa2m_afirmar(encontrado == NULL,
		     "Al insertar nueva clave, encontrado es NULL");

	hash_destruir(hash);
}
void test_hash_insertar_insertando_varios_tipos_datos()
{
	hash_t *hash = hash_crear(10);

	int entero = 42;
	double decimal = 3.14159;
	char caracter = 'x';
	bool booleano = true;

	pa2m_afirmar(hash_insertar(hash, "int", &entero, NULL) == true,
		     "Insertar un int devuelve true");
	pa2m_afirmar(hash_insertar(hash, "double", &decimal, NULL) == true,
		     "Insertar un double devuelve true");
	pa2m_afirmar(hash_insertar(hash, "char", &caracter, NULL) == true,
		     "Insertar un char devuelve true");
	pa2m_afirmar(hash_insertar(hash, "bool", &booleano, NULL) == true,
		     "Insertar un bool devuelve true");

	int arr[] = { 1, 2, 3 };
	struct {
		int a;
		double b;
	} st = { 7, 2.5 };

	pa2m_afirmar(hash_insertar(hash, "array", arr, NULL) == true,
		     "Insertar un array devuelve true");
	pa2m_afirmar(hash_insertar(hash, "struct", &st, NULL) == true,
		     "Insertar un struct devuelve true");

	pa2m_afirmar(
		true,
		"Se puede insertar diferentes tipos de datos sin ningún problema");

	hash_destruir(hash);
}
void test_hash_insertar_insertando_valor_null()
{
	hash_t *hash = hash_crear(10);
	bool inserto = hash_insertar(hash, "clave1", NULL, NULL);

	pa2m_afirmar(inserto, "Se puede insertar un valor NULL sin error");

	hash_destruir(hash);
}
void test_hash_insertar_insertando_mismo_valor()
{
	hash_t *hash = hash_crear(10);
	int valor = 22;

	bool inserto = hash_insertar(hash, "clave1", &valor, NULL);
	bool inserto2 = hash_insertar(hash, "clave2", &valor, NULL);

	pa2m_afirmar(
		inserto && inserto2,
		"Se puede insertar un mismo valor con clave distinta correctamente");

	hash_destruir(hash);
}
void test_insertar_prueba_estres()
{
	hash_t *hash = hash_crear(50);

	insertar_muchos_elementos(hash, CANT_PRUEBA_ESTRES);

	pa2m_afirmar(
		hash_cantidad(hash) == CANT_PRUEBA_ESTRES,
		"Prueba estres:Se pudieron insertar 5000 elementos correctamente");
	hash_destruir_todo(hash, free);
}

void tests_hash_insertar()
{
	test_hash_insertar_hash_null();
	test_hash_insertar_clave_null();
	test_hash_insertar_inserta_correctamente_hash_vacio();
	test_hash_insertar_inserta_correctamente_hash_varios_elementos();
	test_hash_insertar_inserta_correctamente_coliciones();
	test_hash_insertar_modifica_correctamente_valor_clave_existente();
	test_hash_insertar_aumenta_cantidad();
	test_hash_insertar_hace_rehash_si_supera_factor();
	test_hash_insertar_encontrado_null();
	test_hash_insertar_pasando_encontrado_clave_existente();
	test_hash_insertar_pasando_encontrado_insertando_nodo();
	test_hash_insertar_insertando_varios_tipos_datos();
	test_hash_insertar_insertando_valor_null();
	test_hash_insertar_insertando_mismo_valor();
	test_insertar_prueba_estres();
}
//-------------------------------------------------------------------------
void test_hash_buscar_hash_null()
{
	pa2m_afirmar(hash_buscar(NULL, "clave") == NULL,
		     "Buscar con hash NULL devuelve NULL");
}
void test_hash_buscar_clave_null()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash_buscar(hash, NULL) == NULL,
		     "Buscar con clave NULL devuelve NULL");
	hash_destruir(hash);
}
void test_hash_buscar_hash_vacio()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash_buscar(hash, "clave") == NULL,
		     "Buscar con hash vacío devuelve NULL");
	hash_destruir(hash);
}
void test_hash_buscar_encontro_con_unico_elemento()
{
	hash_t *hash = hash_crear(10);
	int valor = 42;
	hash_insertar(hash, "unica", &valor, NULL);

	pa2m_afirmar(hash_buscar(hash, "unica") == &valor,
		     "Buscar un elemento único lo encuentra correctamente");

	hash_destruir(hash);
}
void test_hash_buscar_encontro_con_varios_elementos()
{
	hash_t *hash = hash_crear(10);
	int v1 = 1, v2 = 2, v3 = 3;

	hash_insertar(hash, "primero", &v1, NULL);
	hash_insertar(hash, "segundo", &v2, NULL);
	hash_insertar(hash, "tercero", &v3, NULL);

	pa2m_afirmar(
		hash_buscar(hash, "segundo") == &v2,
		"Buscar un elemento entre varios lo encuentra correctamente");

	hash_destruir(hash);
}
void test_hash_buscar_no_modifica_cantidad()
{
	hash_t *hash = hash_crear(10);
	int valor = 100;

	hash_insertar(hash, "clave", &valor, NULL);
	size_t cantidad_antes = hash_cantidad(hash);

	hash_buscar(hash, "clave");
	size_t cantidad_despues = hash_cantidad(hash);

	pa2m_afirmar(cantidad_antes == cantidad_despues,
		     "Buscar no modifica la cantidad de elementos del hash");

	hash_destruir(hash);
}
void test_hash_buscar_no_existe_elemento()
{
	hash_t *hash = hash_crear(10);
	int valor = 10;
	hash_insertar(hash, "existe", &valor, NULL);

	pa2m_afirmar(hash_buscar(hash, "no_existe") == NULL,
		     "Buscar una clave que no existe devuelve NULL");

	hash_destruir(hash);
}
void test_hash_buscar_mismo_elemento()
{
	hash_t *hash = hash_crear(10);
	int valor = 123;
	hash_insertar(hash, "repetida", &valor, NULL);

	void *primera_busqueda = hash_buscar(hash, "repetida");
	void *segunda_busqueda = hash_buscar(hash, "repetida");

	pa2m_afirmar(
		primera_busqueda == segunda_busqueda,
		"Buscar el mismo elemento varias veces devuelve la misma referencia al mismo valor");
	hash_destruir(hash);
}
void test_hash_buscar_varios_tipo_datos()
{
	hash_t *hash = hash_crear(10);

	int entero = 1;
	double real = 2.5;
	char caracter = 'z';
	int vector[] = { 3, 4, 5 };
	struct {
		int a;
		double b;
	} st = { 7, 2.5 };
	bool booleano = true;

	hash_insertar(hash, "entero", &entero, NULL);
	hash_insertar(hash, "real", &real, NULL);
	hash_insertar(hash, "caracter", &caracter, NULL);
	hash_insertar(hash, "vector", vector, NULL);
	hash_insertar(hash, "estructura", &st, NULL);
	hash_insertar(hash, "booleano", &booleano, NULL);

	pa2m_afirmar(*(int *)hash_buscar(hash, "entero") == 1,
		     "Buscar entero devuelve el valor esperado");
	pa2m_afirmar(*(double *)hash_buscar(hash, "real") == 2.5,
		     "Buscar double devuelve el valor esperado");
	pa2m_afirmar(*(char *)hash_buscar(hash, "caracter") == 'z',
		     "Buscar char devuelve el valor esperado");
	pa2m_afirmar(((int *)hash_buscar(hash, "vector"))[2] == 5,
		     "Buscar array devuelve el valor esperado");
	pa2m_afirmar(((struct {
			     int a;
			     double b;
		     } *)hash_buscar(hash, "estructura"))
				     ->a == 7,
		     "Buscar struct devuelve el valor esperado");
	pa2m_afirmar(*(bool *)hash_buscar(hash, "booleano") == true,
		     "Buscar booleano devuelve el valor esperado");

	hash_destruir(hash);
}
void test_buscar_prueba_estres()
{
	hash_t *hash = hash_crear(50);

	insertar_muchos_elementos(hash, CANT_PRUEBA_ESTRES);

	bool todo_ok = true;
	for (size_t i = 0; i < CANT_PRUEBA_ESTRES; i++) {
		char clave[MAX_CLAVE_RANDOM];
		sprintf(clave, "key_%zu", i);
		int *valor = hash_buscar(hash, clave);

		if (!valor) {
			todo_ok = false;
			break;
		}
	}

	pa2m_afirmar(
		todo_ok,
		"Prueba estrés: Se puede buscar correctamente con +5000 elementos");

	hash_destruir_todo(hash, free);
}

void tests_hash_buscar()
{
	test_hash_buscar_hash_null();
	test_hash_buscar_clave_null();
	test_hash_buscar_hash_vacio();
	test_hash_buscar_encontro_con_unico_elemento();
	test_hash_buscar_encontro_con_varios_elementos();
	test_hash_buscar_no_modifica_cantidad();
	test_hash_buscar_no_existe_elemento();
	test_hash_buscar_mismo_elemento();
	test_buscar_prueba_estres();
}
//-------------------------------------------------------------------------
void test_hash_contiene_hash_null()
{
	pa2m_afirmar(!hash_contiene(NULL, "clave"),
		     "Pasar hash NULL devuelve false");
}
void test_hash_contiene_clave_null()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(!hash_contiene(hash, NULL),
		     "Pasar clave NULL devuelve false");
	hash_destruir(hash);
}
void test_hash_contiene_hash_vacio()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(!hash_contiene(hash, "clave"),
		     "Hash vacío no contiene ninguna clave");
	hash_destruir(hash);
}
void test_hash_contiene_con_unico_elemento()
{
	hash_t *hash = hash_crear(10);
	int valor = 42;
	hash_insertar(hash, "clave1", &valor, NULL);

	pa2m_afirmar(
		hash_contiene(hash, "clave1"),
		"Si hay solo 1 clave insertada y buscas la clave que existe, devuelve true");
	pa2m_afirmar(
		!hash_contiene(hash, "clave2"),
		"Si hay solo 1 clave insertada y buscas una que no existe, devuelve false");

	hash_destruir(hash);
}
void test_hash_contiene_con_varios_elementos()
{
	hash_t *hash = hash_crear(10);
	int v1 = 1, v2 = 2, v3 = 3;
	hash_insertar(hash, "c1", &v1, NULL);
	hash_insertar(hash, "c2", &v2, NULL);
	hash_insertar(hash, "c3", &v3, NULL);

	pa2m_afirmar(
		hash_contiene(hash, "c2"),
		"Funciona correctamente al buscar una clave existente con varios elementos");
	pa2m_afirmar(
		!hash_contiene(hash, "c4"),
		"Funciona correctamente al buscar una clave inexistente con varios elementos");

	hash_destruir(hash);
}
void test_hash_contiene_no_modifica_cantidad()
{
	hash_t *hash = hash_crear(10);
	int valor = 5;
	hash_insertar(hash, "clave", &valor, NULL);
	size_t cant = hash_cantidad(hash);

	hash_contiene(hash, "clave");
	hash_contiene(hash, "otra");

	pa2m_afirmar(
		hash_cantidad(hash) == cant,
		"Ver si una clave esta contenida en el Hash no modifica cantidad");

	hash_destruir(hash);
}
void test_hash_contiene_no_existe_elemento()
{
	hash_t *hash = hash_crear(10);
	int valor = 3;
	hash_insertar(hash, "clave1", &valor, NULL);

	pa2m_afirmar(!hash_contiene(hash, "clave2"),
		     "hash_contiene devuelve false para clave no existente");

	hash_destruir(hash);
}
void test_hash_contiene_mismo_elemento()
{
	hash_t *hash = hash_crear(10);
	int valor = 7;
	hash_insertar(hash, "clave", &valor, NULL);
	bool ok = hash_contiene(hash, "clave");
	bool ok2 = hash_contiene(hash, "clave");
	bool ok3 = hash_contiene(hash, "clave");
	pa2m_afirmar(ok && ok2 && ok3,
		     "Devuelve true si buscas la misma clave varias veces");

	hash_destruir(hash);
}
void test_hash_contiene_varios_tipo_datos()
{
	hash_t *hash = hash_crear(10);

	int i = 42;
	double d = 3.14;
	char c = 'x';
	int arr[] = { 1, 2, 3 };
	bool booleano = true;

	typedef struct {
		int a;
		double b;
	} ejemplo_struct;

	ejemplo_struct st = { 7, 2.5 };

	// Insertar todos los elementos
	hash_insertar(hash, "int", &i, NULL);
	hash_insertar(hash, "double", &d, NULL);
	hash_insertar(hash, "char", &c, NULL);
	hash_insertar(hash, "array", arr, NULL);
	hash_insertar(hash, "bool", &booleano, NULL);
	hash_insertar(hash, "struct", &st, NULL);

	// Verificar que hash_contiene funcione correctamente
	pa2m_afirmar(hash_contiene(hash, "int"),
		     "Funciona correctamente con int");
	pa2m_afirmar(hash_contiene(hash, "double"),
		     "Funciona correctamente con double");
	pa2m_afirmar(hash_contiene(hash, "char"),
		     "Funciona correctamente con char");
	pa2m_afirmar(hash_contiene(hash, "array"),
		     "Funciona correctamente con array");
	pa2m_afirmar(hash_contiene(hash, "bool"),
		     "Funciona correctamente con bool");
	pa2m_afirmar(hash_contiene(hash, "struct"),
		     "Funciona correctamente con struct");

	hash_destruir(hash);
}
void test_hash_contiene_valor_null()
{
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "clave", NULL, NULL);

	pa2m_afirmar(hash_contiene(hash, "clave"),
		     "Funciona correctamante al buscar clave con valor NULL");

	hash_destruir(hash);
}
void test_integracion_contiene_prueba_estres()
{
	hash_t *hash = hash_crear(50);

	insertar_muchos_elementos(hash, CANT_PRUEBA_ESTRES);

	bool todo_ok = true;
	for (size_t i = 0; i < CANT_PRUEBA_ESTRES; i++) {
		char clave[MAX_CLAVE_RANDOM];
		sprintf(clave, "key_%zu", i);

		if (!hash_contiene(hash, clave)) {
			todo_ok = false;
			break;
		}
	}

	pa2m_afirmar(
		todo_ok,
		"Prueba estrés: Funciona correctamente con +5000 elementos");

	hash_destruir_todo(hash, free);
}

void tests_hash_contiene()
{
	test_hash_contiene_hash_null();
	test_hash_contiene_clave_null();
	test_hash_contiene_hash_vacio();
	test_hash_contiene_con_unico_elemento();
	test_hash_contiene_no_modifica_cantidad();
	test_hash_contiene_no_existe_elemento();
	test_hash_contiene_mismo_elemento();
	test_integracion_contiene_prueba_estres();
}
//-------------------------------------------------------------------------
void test_hash_quitar_hash_null()
{
	pa2m_afirmar(hash_quitar(NULL, "clave") == NULL,
		     "Quitar en hash NULL devuelve NULL");
}
void test_hash_quitar_clave_null()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash_quitar(hash, NULL) == NULL,
		     "Quitar pasando clave NULL devuelve NULL");
	hash_destruir(hash);
}
void test_hash_quitar_hash_vacio()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash_quitar(hash, "clave") == NULL,
		     "Quitar de hash vacío devuelve NULL");
	hash_destruir(hash);
}
void test_hash_quitar_unico_elemento()
{
	hash_t *hash = hash_crear(10);
	int valor = 42;
	hash_insertar(hash, "clave", &valor, NULL);

	void *valor_quitado = hash_quitar(hash, "clave");
	pa2m_afirmar(*(int *)valor_quitado == 42,
		     "Se puede quitar un único elemento correctamente");
	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "Cantidad queda en 0 después de quitar el único elemento");

	hash_destruir(hash);
}
void test_hash_quitar_varios_elementos()
{
	hash_t *hash = hash_crear(10);
	int v1 = 1, v2 = 2, v3 = 3;
	hash_insertar(hash, "c1", &v1, NULL);
	hash_insertar(hash, "c2", &v2, NULL);
	hash_insertar(hash, "c3", &v3, NULL);

	void *q1 = hash_quitar(hash, "c1");
	void *q2 = hash_quitar(hash, "c2");
	void *q3 = hash_quitar(hash, "c3");

	pa2m_afirmar(*(int *)q1 == 1 && *(int *)q2 == 2 && *(int *)q3 == 3,
		     "Se pueden quitar correctamente varios elementos");
	pa2m_afirmar(
		hash_cantidad(hash) == 0,
		"Cantidad queda en 0 después de quitar todos los elementos");

	hash_destruir(hash);
}
void test_hash_quitar_reduce_cantidad()
{
	hash_t *hash = hash_crear(10);
	int v1 = 10, v2 = 20;
	hash_insertar(hash, "c1", &v1, NULL);
	hash_insertar(hash, "c2", &v2, NULL);

	hash_quitar(hash, "c1");
	pa2m_afirmar(hash_cantidad(hash) == 1,
		     "Cantidad se reduce correctamente al quitar un elemento");

	hash_destruir(hash);
}
void test_hash_quitar_elementos_modificados()
{
	hash_t *hash = hash_crear(10);
	int v1 = 5, v2 = 10;
	hash_insertar(hash, "c1", &v1, NULL);
	hash_insertar(hash, "c1", &v2, NULL);

	void *q = hash_quitar(hash, "c1");
	pa2m_afirmar(
		*(int *)q == 10,
		"Se quita correctamente un elemento cuyo valor fue modificado");

	hash_destruir(hash);
}
void test_hash_quitar_elementos_valor_repetido()
{
	hash_t *hash = hash_crear(10);
	int v1 = 5, v2 = 5;
	hash_insertar(hash, "c1", &v1, NULL);
	hash_insertar(hash, "c2", &v2, NULL); // sobrescribiendo

	void *q = hash_quitar(hash, "c1");
	pa2m_afirmar(
		*(int *)q == 5,
		"Se quita correctamente un elemento con un valor repetido");

	hash_destruir(hash);
}
void test_hash_quitar_no_existe_elemento()
{
	hash_t *hash = hash_crear(10);
	int valor = 7;
	hash_insertar(hash, "c1", &valor, NULL);

	pa2m_afirmar(hash_quitar(hash, "c2") == NULL,
		     "Intentar quitar un elemento que no existe devuelve NULL");
	pa2m_afirmar(
		hash_cantidad(hash) == 1,
		"Cantidad no cambia al intentar quitar elemento inexistente");

	hash_destruir(hash);
}
void test_hash_quitar_varios_tipos_datos()
{
	hash_t *hash = hash_crear(10);

	int entero = 1;
	double real = 2.5;
	char caracter = 'z';
	int vector[] = { 3, 4, 5 };
	bool booleano = true;

	// Definimos la estructura explícitamente
	typedef struct {
		int a;
		double b;
	} ejemplo_struct;

	ejemplo_struct st = { 7, 2.5 };

	// Insertar en el hash
	hash_insertar(hash, "entero", &entero, NULL);
	hash_insertar(hash, "real", &real, NULL);
	hash_insertar(hash, "caracter", &caracter, NULL);
	hash_insertar(hash, "vector", vector, NULL);
	hash_insertar(hash, "bool", &booleano, NULL);
	hash_insertar(hash, "estructura", &st, NULL);

	// Quitar y verificar
	pa2m_afirmar(*(int *)hash_quitar(hash, "entero") == 1,
		     "Quitar int devuelve el valor esperado");
	pa2m_afirmar(*(double *)hash_quitar(hash, "real") == 2.5,
		     "Quitar double devuelve el valor esperado");
	pa2m_afirmar(*(char *)hash_quitar(hash, "caracter") == 'z',
		     "Quitar char devuelve el valor esperado");
	pa2m_afirmar(((int *)hash_quitar(hash, "vector"))[2] == 5,
		     "Quitar array devuelve el valor esperado");
	pa2m_afirmar(*(bool *)hash_quitar(hash, "bool") == true,
		     "Quitar bool devuelve el valor esperado");
	pa2m_afirmar(((ejemplo_struct *)hash_quitar(hash, "estructura"))->a ==
			     7,
		     "Quitar struct devuelve el valor esperado");

	hash_destruir(hash);
}
void test_hash_quitar_valor_null()
{
	hash_t *hash = hash_crear(10);

	hash_insertar(hash, "clave", NULL, NULL);

	pa2m_afirmar(hash_quitar(hash, "clave") == NULL,
		     "Quitar un valor NULL devuelve NULL");

	hash_destruir(hash);
}
void tests_hash_quitar_prueba_estres()
{
	hash_t *hash = hash_crear(50);

	insertar_muchos_elementos(hash, CANT_PRUEBA_ESTRES);

	bool todo_ok = true;
	for (size_t i = 0; i < CANT_PRUEBA_ESTRES; i++) {
		char clave[MAX_CLAVE_RANDOM];
		int valor_esperado = (int)i;
		sprintf(clave, "key_%zu", i);

		int *valor_obtenido = hash_quitar(hash, clave);
		if (!valor_obtenido || *valor_obtenido != valor_esperado) {
			todo_ok = false;
			break;
		}
	}

	pa2m_afirmar(
		todo_ok && hash_cantidad(hash) == 0,
		"Prueba estrés: Funciona correctamente quitando +5000 elementos");

	hash_destruir_todo(hash, free);
}

void tests_hash_quitar()
{
	test_hash_quitar_hash_null();
	test_hash_quitar_clave_null();
	test_hash_quitar_hash_vacio();
	test_hash_quitar_unico_elemento();
	test_hash_quitar_varios_elementos();
	test_hash_quitar_reduce_cantidad();
	test_hash_quitar_elementos_modificados();
	test_hash_quitar_elementos_valor_repetido();
	test_hash_quitar_no_existe_elemento();
	test_hash_quitar_varios_tipos_datos();
	test_hash_quitar_valor_null();
	tests_hash_quitar_prueba_estres();
}
//-------------------------------------------------------------------------
bool contar_elementos(char *clave, void *valor, void *ctx)
{
	size_t *contador = (size_t *)ctx;
	(*contador)++;
	return true;
}
bool funcion_true(char *clave, void *valor, void *ctx)
{
	return true;
}
bool cortar_antes(char *clave, void *valor, void *ctx)
{
	size_t *contador = (size_t *)ctx;
	(*contador)++;
	return false; // corta inmediatamente
}
bool funcion_personalizada(char *clave, void *valor, void *ctx)
{
	int *suma = (int *)ctx;
	*suma += *(int *)valor;
	return true;
}
bool sumar_con_ctx(char *clave, void *valor, void *ctx)
{
	int *acumulador = (int *)ctx;
	*acumulador += *(int *)valor;
	return true;
}

void test_hash_iterar_hash_null()
{
	size_t recorridos = hash_iterar(NULL, contar_elementos, NULL);
	pa2m_afirmar(recorridos == 0, "hash_iterar con hash NULL devuelve 0");
}
void test_hash_iterar_funcion_null()
{
	hash_t *hash = hash_crear(10);
	size_t recorridos = hash_iterar(hash, NULL, NULL);
	pa2m_afirmar(recorridos == 0,
		     "hash_iterar con función NULL devuelve 0");
	hash_destruir(hash);
}
void test_hash_iterar_hash_vacio()
{
	hash_t *hash = hash_crear(10);
	size_t recorridos = hash_iterar(hash, contar_elementos, NULL);
	pa2m_afirmar(recorridos == 0, "hash_iterar en hash vacío devuelve 0");
	hash_destruir(hash);
}
void test_hash_iterar_recorrido_completo()
{
	hash_t *hash = hash_crear(10);
	int a = 1, b = 2, c = 3;
	hash_insertar(hash, "a", &a, NULL);
	hash_insertar(hash, "b", &b, NULL);
	hash_insertar(hash, "c", &c, NULL);

	size_t contador = 0;
	size_t recorridos = hash_iterar(hash, contar_elementos, &contador);
	pa2m_afirmar(recorridos == 3 && contador == 3,
		     "Recorre correctamante todos los elementos");
	hash_destruir(hash);
}
void test_hash_iterar_recorrido_cortado()
{
	hash_t *hash = hash_crear(10);
	int a = 1, b = 2;
	hash_insertar(hash, "a", &a, NULL);
	hash_insertar(hash, "b", &b, NULL);

	size_t contador = 0;
	size_t recorridos = hash_iterar(hash, cortar_antes, &contador);
	pa2m_afirmar(recorridos == 1 && contador == 1,
		     "Se detiene correctamente al devolver false la funcion");
	hash_destruir(hash);
}
void test_hash_iterar_funcion_personalizada()
{
	hash_t *hash = hash_crear(10);
	int a = 1, b = 2, c = 3;
	hash_insertar(hash, "a", &a, NULL);
	hash_insertar(hash, "b", &b, NULL);
	hash_insertar(hash, "c", &c, NULL);

	int suma = 0;
	hash_iterar(hash, funcion_personalizada, &suma);
	pa2m_afirmar(
		suma == 6,
		"Aplica correctamente la función personalizada a todos los elementos");
	hash_destruir(hash);
}
void test_hash_iterar_ctx_funciona_correctamente()
{
	hash_t *hash = hash_crear(10);
	int a = 2, b = 4;
	hash_insertar(hash, "a", &a, NULL);
	hash_insertar(hash, "b", &b, NULL);

	int total = 0;
	hash_iterar(hash, sumar_con_ctx, &total);
	pa2m_afirmar(total == 6,
		     "Pasa correctamente el ctx a la función de callback");
	hash_destruir(hash);
}
void tests_hash_iterar_prueba_estres()
{
	hash_t *hash = hash_crear(50);

	insertar_muchos_elementos(hash, CANT_PRUEBA_ESTRES);

	size_t contador = hash_iterar(hash, funcion_true, NULL);

	pa2m_afirmar(contador == CANT_PRUEBA_ESTRES,
		     "Prueba estrés: Recorre correctamente +5000 elementos");

	hash_destruir_todo(hash, free);
}

void tests_hash_iterar()
{
	test_hash_iterar_hash_null();
	test_hash_iterar_funcion_null();
	test_hash_iterar_hash_vacio();
	test_hash_iterar_recorrido_completo();
	test_hash_iterar_recorrido_cortado();
	test_hash_iterar_funcion_personalizada();
	test_hash_iterar_ctx_funciona_correctamente();
	tests_hash_iterar_prueba_estres();
}
//-------------------------------------------------------------------------
void test_hash_destruir_hash_null()
{
	hash_destruir(NULL);
	pa2m_afirmar(true, "Pasar hash NULL no provoca errores");
}
void test_hash_destruir_hash_vacio()
{
	hash_t *hash = hash_crear(10);
	hash_destruir(hash);
	pa2m_afirmar(true, "Hash vacío es liberado correctamente");
}
void test_hash_destruir_destruye_correctamente_unico_elemento()
{
	hash_t *hash = hash_crear(10);
	int x = 5;
	hash_insertar(hash, "clave", &x, NULL);
	hash_destruir(hash);
	pa2m_afirmar(true, "Libera correctamente hash con un elemento");
}
void test_hash_destruir_destruye_correctamente_varios_elemento()
{
	hash_t *hash = hash_crear(10);
	int x = 5;
	int y = 15;
	int z = 150;
	int a = 151;

	hash_insertar(hash, "clave", &x, NULL);
	hash_insertar(hash, "clave2", &y, NULL);
	hash_insertar(hash, "clave2", &z, NULL);
	hash_insertar(hash, "clave2", &a, NULL);

	hash_destruir(hash);

	pa2m_afirmar(true, "Libera correctamente hash con varios elementos");
}

void tests_hash_destruir()
{
	test_hash_destruir_hash_null();
	test_hash_destruir_hash_vacio();
	test_hash_destruir_destruye_correctamente_unico_elemento();
	test_hash_destruir_destruye_correctamente_varios_elemento();
}
//-------------------------------------------------------------------------
void destructor(void *valor)
{
	free(valor);
}

void test_hash_destruir_todo_hash_null()
{
	hash_destruir_todo(NULL, NULL);
	pa2m_afirmar(true, "Pasarle hash NULL no provoca errores");
}
void test_hash_destruir_todo_hash_vacio()
{
	hash_t *hash = hash_crear(10);
	hash_destruir_todo(hash, NULL);
	pa2m_afirmar(true, "Hash vacío es liberado correctamente");
}
void test_hash_destruir_todo_destruye_correctamente_unico_elemento()
{
	hash_t *hash = hash_crear(10);
	int *x = malloc(sizeof(int));
	*x = 42;
	hash_insertar(hash, "clave", x, NULL);
	hash_destruir_todo(hash, destructor);
	pa2m_afirmar(true, "Libera correctamente hash con un elemento");
}
void test_hash_destruir_todo_destruye_correctamente_varios_elementos()
{
	hash_t *hash = hash_crear(10);
	int *x = malloc(sizeof(int));
	*x = 42;
	int *y = malloc(sizeof(int));
	*y = 420;
	int *z = malloc(sizeof(int));
	*z = 12;

	hash_insertar(hash, "clave", x, NULL);
	hash_insertar(hash, "clave2", y, NULL);
	hash_insertar(hash, "clave3", z, NULL);

	hash_destruir_todo(hash, destructor);
	pa2m_afirmar(true, "Libera correctamente hash con varios elemento");
}
void test_hash_destruir_todo_destruye_correctamente_varios_tipos_datos()
{
	hash_t *hash = hash_crear(10);

	// Tipos de datos
	int *i = malloc(sizeof(int));
	*i = 1;
	double *d = malloc(sizeof(double));
	*d = 2.5;
	char *c = malloc(sizeof(char));
	*c = 'z';
	int *arr = malloc(3 * sizeof(int));
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	bool *b = malloc(sizeof(bool));
	*b = true;

	struct {
		int a;
		double b;
	} *st = malloc(sizeof(*st));
	st->a = 7;
	st->b = 2.5;

	// Insertar en hash
	hash_insertar(hash, "int", i, NULL);
	hash_insertar(hash, "double", d, NULL);
	hash_insertar(hash, "char", c, NULL);
	hash_insertar(hash, "array", arr, NULL);
	hash_insertar(hash, "bool", b, NULL);
	hash_insertar(hash, "struct", st, NULL);

	hash_destruir_todo(hash, destructor);

	pa2m_afirmar(
		true,
		"hash_destruir_todo libera correctamente varios tipos de datos (int, double, char, array, struct, bool)");
}
void test_hash_destruir_todo_destruye_correctamente_destructor_null()
{
	hash_t *hash = hash_crear(10);
	int x = 5;
	hash_insertar(hash, "clave", &x, NULL);
	hash_destruir_todo(hash, NULL);
	pa2m_afirmar(true, "Funciona correctamente con destructor NULL");
}
void test_hash_destruir_todo_destruye_correctamente_valor_null()
{
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "clave", NULL, NULL);
	hash_destruir_todo(hash, NULL);
	pa2m_afirmar(true, "Funciona correctamente liberando con valores NULL");
}

void tests_hash_destruir_todo()
{
	test_hash_destruir_todo_hash_null();
	test_hash_destruir_todo_hash_vacio();
	test_hash_destruir_todo_destruye_correctamente_unico_elemento();
	test_hash_destruir_todo_destruye_correctamente_varios_elementos();
	test_hash_destruir_todo_destruye_correctamente_varios_tipos_datos();
	test_hash_destruir_todo_destruye_correctamente_destructor_null();
}
//-------------------------------------------------------------------------Test Integracion
void test_integracion_insertar_despues_de_quitar()
{
	hash_t *hash = hash_crear(10);
	int valor = 1;

	hash_insertar(hash, "clave1", &valor, NULL);
	hash_quitar(hash, "clave1");

	valor = 2;
	pa2m_afirmar(
		hash_insertar(hash, "clave1", &valor, NULL) == true,
		"Se puede insertar correctamente un elemento después de quitarlo");
	pa2m_afirmar(*(int *)hash_buscar(hash, "clave1") == 2,
		     "El valor insertado después de quitar es correcto");

	hash_destruir(hash);
}
void test_integracion_insertar_despues_de_buscar()
{
	hash_t *hash = hash_crear(10);
	int valor = 10;

	hash_insertar(hash, "clave1", &valor, NULL);
	hash_buscar(hash, "clave1"); // buscamos antes de insertar

	valor = 20;
	pa2m_afirmar(hash_insertar(hash, "clave2", &valor, NULL) == true,
		     "Se puede insertar después de hacer búsquedas");
	pa2m_afirmar(*(int *)hash_buscar(hash, "clave2") == 20,
		     "El valor insertado después de buscar es correcto");

	hash_destruir(hash);
}
void test_integracion_insertar_modifica_correctamente_valor_clave_existente()
{
	hash_t *hash = hash_crear(10);
	int valor1 = 5;
	int valor2 = 15;

	hash_insertar(hash, "clave1", &valor1, NULL);
	hash_insertar(hash, "clave1", &valor2, NULL);

	pa2m_afirmar(
		*(int *)hash_buscar(hash, "clave1") == 15,
		"Insertar en clave existente modifica correctamente el valor");

	hash_destruir(hash);
}
void test_integracion_insertar_despues_de_contiene()
{
	hash_t *hash = hash_crear(10);
	int valor = 7;

	hash_insertar(hash, "clave1", &valor, NULL);
	hash_contiene(hash, "clave1"); // consultamos si contiene

	valor = 8;
	pa2m_afirmar(hash_insertar(hash, "clave2", &valor, NULL) == true,
		     "Se puede insertar después de usar contiene");
	pa2m_afirmar(*(int *)hash_buscar(hash, "clave2") == 8,
		     "El valor insertado después de contiene es correcto");

	hash_destruir(hash);
}
void test_integracion_insertar_despues_de_iterar()
{
	hash_t *hash = hash_crear(10);
	int valor = 3;

	hash_insertar(hash, "clave1", &valor, NULL);

	hash_iterar(hash, funcion_true, NULL);

	valor = 4;
	pa2m_afirmar(hash_insertar(hash, "clave2", &valor, NULL) == true,
		     "Se puede insertar después de iterar sobre el hash");
	pa2m_afirmar(*(int *)hash_buscar(hash, "clave2") == 4,
		     "El valor insertado después de iterar es correcto");

	hash_destruir(hash);
}

void tests_integracion_insertar()
{
	test_integracion_insertar_despues_de_quitar();
	test_integracion_insertar_despues_de_buscar();
	test_integracion_insertar_modifica_correctamente_valor_clave_existente();
	test_integracion_insertar_despues_de_contiene();
	test_integracion_insertar_despues_de_iterar();
}
//-------------------------------------------------------------------------
void test_integracion_buscar_despues_de_quitar()
{
	hash_t *hash = hash_crear(10);
	int valor = 10;

	hash_insertar(hash, "clave1", &valor, NULL);
	hash_quitar(hash, "clave1");

	pa2m_afirmar(hash_buscar(hash, "clave1") == NULL,
		     "Buscar después de quitar devuelve NULL");

	hash_destruir(hash);
}
void test_integracion_buscar_despues_de_contiene()
{
	hash_t *hash = hash_crear(10);
	int valor = 20;

	hash_insertar(hash, "clave1", &valor, NULL);
	hash_contiene(hash, "clave1");

	pa2m_afirmar(
		*(int *)hash_buscar(hash, "clave1") == 20,
		"Buscar después de usar contiene devuelve el valor correcto");

	hash_destruir(hash);
}
void test_integracion_buscar_despues_de_iterar()
{
	hash_t *hash = hash_crear(10);
	int valor = 30;

	hash_insertar(hash, "clave1", &valor, NULL);
	hash_iterar(hash, funcion_true, NULL);

	pa2m_afirmar(*(int *)hash_buscar(hash, "clave1") == 30,
		     "Buscar después de iterar devuelve el valor correcto");

	hash_destruir(hash);
}
void test_integracion_buscar_modificacion_valor_correctamente()
{
	hash_t *hash = hash_crear(10);
	int valor1 = 5;
	int valor2 = 50;

	hash_insertar(hash, "clave1", &valor1, NULL);
	hash_insertar(hash, "clave1", &valor2, NULL);

	pa2m_afirmar(*(int *)hash_buscar(hash, "clave1") == 50,
		     "Buscar devuelve el valor modificado correctamente");

	hash_destruir(hash);
}
void test_integracion_buscar_elemento_colicionado()
{
	hash_t *hash = hash_crear(3);
	int valor1 = 1, valor2 = 2;

	// Sabemos que estas claves colisionarán según la función hash
	hash_insertar(hash, "a", &valor1, NULL);
	hash_insertar(hash, "k", &valor2, NULL);

	pa2m_afirmar(*(int *)hash_buscar(hash, "a") == 1,
		     "Buscar devuelve valor correcto de clave con colisión");

	hash_destruir(hash);
}

void tests_integracion_buscar()
{
	test_integracion_buscar_despues_de_quitar();
	test_integracion_buscar_despues_de_contiene();
	test_integracion_buscar_despues_de_iterar();
	test_integracion_buscar_modificacion_valor_correctamente();
	test_integracion_buscar_elemento_colicionado();
}
//-------------------------------------------------------------------------
void test_integracion_contiene_despues_de_quitar()
{
	hash_t *hash = hash_crear(10);
	int valor = 10;

	hash_insertar(hash, "clave1", &valor, NULL);
	hash_quitar(hash, "clave1");

	pa2m_afirmar(!hash_contiene(hash, "clave1"),
		     "Contiene después de quitar devuelve false");

	hash_destruir(hash);
}
void test_integracion_contiene_despues_de_buscar()
{
	hash_t *hash = hash_crear(10);
	int valor = 20;

	hash_insertar(hash, "clave1", &valor, NULL);
	hash_buscar(hash, "clave1");

	pa2m_afirmar(hash_contiene(hash, "clave1"),
		     "Contiene después de buscar funciona correctamente");

	hash_destruir(hash);
}
void test_integracion_contiene_despues_de_iterar()
{
	hash_t *hash = hash_crear(10);
	int valor = 30;

	hash_insertar(hash, "clave1", &valor, NULL);
	hash_iterar(hash, funcion_true, NULL);

	pa2m_afirmar(hash_contiene(hash, "clave1"),
		     "Contiene después de iterar funciona correctamente");

	hash_destruir(hash);
}
void test_integracion_contiene_elemento_colicionado()
{
	hash_t *hash = hash_crear(3);
	int valor1 = 1, valor2 = 2;

	hash_insertar(hash, "a", &valor1, NULL);
	hash_insertar(hash, "k", &valor2, NULL);

	pa2m_afirmar(
		hash_contiene(hash, "k"),
		"Contiene funciona correctamente para elementos con colisión");

	hash_destruir(hash);
}
void test_integracion_contiene_modificacion_valor_correctamente()
{
	hash_t *hash = hash_crear(10);
	int valor1 = 5;
	int valor2 = 10;

	hash_insertar(hash, "clave1", &valor1, NULL);
	hash_insertar(hash, "clave1", &valor2, NULL);

	pa2m_afirmar(
		hash_contiene(hash, "clave1"),
		"Contiene funciona correctamente para elementos cuyo valor fue modificado");

	hash_destruir(hash);
}

void tests_integracion_contiene()
{
	test_integracion_contiene_despues_de_quitar();
	test_integracion_contiene_despues_de_buscar();
	test_integracion_contiene_despues_de_iterar();
	test_integracion_contiene_elemento_colicionado();
	test_integracion_contiene_modificacion_valor_correctamente();
}
//-------------------------------------------------------------------------
void test_integracion_quitar_despues_de_contiene()
{
	hash_t *hash = hash_crear(10);
	int valor = 42;

	hash_insertar(hash, "clave1", &valor, NULL);

	hash_contiene(hash, "clave1");
	int *valor_quitado = hash_quitar(hash, "clave1");
	pa2m_afirmar(*valor_quitado == 42,
		     "Se puede quitar un elemento después de contiene");

	hash_destruir(hash);
}
void test_integracion_quitar_despues_de_buscar()
{
	hash_t *hash = hash_crear(10);
	int valor = 42;

	hash_insertar(hash, "clave1", &valor, NULL);
	hash_buscar(hash, "clave1");

	int *valor_quitado = hash_quitar(hash, "clave1");
	pa2m_afirmar(*valor_quitado == 42,
		     "Se puede quitar un elemento después de buscar");

	hash_destruir(hash);
}
void test_integracion_quitar_despues_de_iterar()
{
	hash_t *hash = hash_crear(10);
	int valor = 42;

	hash_insertar(hash, "clave1", &valor, NULL);
	hash_iterar(hash, funcion_true, NULL);

	int *valor_quitado = hash_quitar(hash, "clave1");
	pa2m_afirmar(*valor_quitado == 42,
		     "Se puede quitar un elemento después de iterar");

	hash_destruir(hash);
}
void test_integracion_quitar_elemento_colicionado()
{
	hash_t *hash = hash_crear(3);
	int v1 = 1, v2 = 2;

	hash_insertar(hash, "a", &v1, NULL);
	hash_insertar(hash, "k", &v2, NULL);

	int *valor_quitado = hash_quitar(hash, "a");
	pa2m_afirmar(*valor_quitado == 1,
		     "Se quitan correctamente elementos colisionados");

	hash_destruir(hash);
}
void test_integracion_quitar_modificacion_valor_correctamente()
{
	hash_t *hash = hash_crear(10);
	int valor1 = 10;
	int valor2 = 20;

	hash_insertar(hash, "clave1", &valor1, NULL);
	hash_insertar(hash, "clave1", &valor2, NULL); // modificamos valor

	int *valor_quitado = hash_quitar(hash, "clave1");
	pa2m_afirmar(*valor_quitado == 20,
		     "Se quita el valor modificado correctamente");

	hash_destruir(hash);
}

void tests_integracion_quitar()
{
	test_integracion_quitar_despues_de_contiene();
	test_integracion_quitar_despues_de_buscar();
	test_integracion_quitar_despues_de_iterar();
	test_integracion_quitar_elemento_colicionado();
	test_integracion_quitar_modificacion_valor_correctamente();
}
//-------------------------------------------------------------------------
void test_integracion_iterar_despues_de_quitar()
{
	hash_t *hash = hash_crear(10);
	int valor = 42;
	hash_insertar(hash, "clave1", &valor, NULL);

	hash_quitar(hash, "clave1");

	size_t cont = hash_iterar(hash, funcion_true, NULL);
	pa2m_afirmar(cont == 0, "Iterar después de quitar itera correctamente");

	hash_destruir(hash);
}
void test_integracion_iterar_despues_de_buscar()
{
	hash_t *hash = hash_crear(10);
	int valor = 42;
	hash_insertar(hash, "clave1", &valor, NULL);

	hash_buscar(hash, "clave1");

	size_t cont = hash_iterar(hash, funcion_true, NULL);
	pa2m_afirmar(cont == 1, "Iterar después de buscar itera correctamente");

	hash_destruir(hash);
}
void test_integracion_iterar_despues_de_contiene()
{
	hash_t *hash = hash_crear(10);
	int valor = 42;
	hash_insertar(hash, "clave1", &valor, NULL);

	hash_contiene(hash, "clave1");
	size_t cont = hash_iterar(hash, funcion_true, NULL);
	pa2m_afirmar(cont == 1,
		     "Iterar después de contiene itera correctamente");

	hash_destruir(hash);
}
void test_integracion_iterar_elementos_colicionados()
{
	hash_t *hash = hash_crear(3);
	int v1 = 1, v2 = 2;

	hash_insertar(hash, "a", &v1, NULL);
	hash_insertar(hash, "k", &v2, NULL);

	size_t cant = hash_iterar(hash, funcion_true, NULL);

	pa2m_afirmar(cant == 2,
		     "Iterar itera correctamente elementos colisionados");

	hash_destruir(hash);
}

void tests_integracion_iterar()
{
	test_integracion_iterar_despues_de_quitar();
	test_integracion_iterar_despues_de_buscar();
	test_integracion_iterar_despues_de_contiene();
	test_integracion_iterar_elementos_colicionados();
}

int main()
{
	pa2m_nuevo_grupo("============== Tests Unitarios ===============");
	pa2m_nuevo_grupo("---hash_crear---");
	tests_hash_crear();

	pa2m_nuevo_grupo("---hash_cantidad---");
	tests_hash_cantidad();

	pa2m_nuevo_grupo("---hash_insertar---");
	tests_hash_insertar();

	pa2m_nuevo_grupo("---hash_buscar---");
	tests_hash_buscar();

	pa2m_nuevo_grupo("---hash_contiene---");
	tests_hash_contiene();

	pa2m_nuevo_grupo("---hash_quitar---");
	tests_hash_quitar();

	pa2m_nuevo_grupo("---hash_iterar---");
	tests_hash_iterar();

	pa2m_nuevo_grupo("---hash_destruir---");
	tests_hash_destruir();

	pa2m_nuevo_grupo("---hash_destruir_todo---");
	tests_hash_destruir_todo();

	pa2m_nuevo_grupo("============== Tests de Integracion ===============");
	pa2m_nuevo_grupo("---hash_insertar---");
	tests_integracion_insertar();

	pa2m_nuevo_grupo("---hash_buscar---");
	tests_integracion_buscar();

	pa2m_nuevo_grupo("---hash_contiene---");
	tests_integracion_contiene();

	pa2m_nuevo_grupo("---hash_quitar---");
	tests_integracion_quitar();

	pa2m_nuevo_grupo("---hash_iterar---");
	tests_integracion_iterar();

	return pa2m_mostrar_reporte();
}
