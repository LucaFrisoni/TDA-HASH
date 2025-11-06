<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA HASH

## Repositorio de Luca Frisoni - 113497 - lfrisoni@fi.uba.ar / frisoniluca1@gmail.com(Github)

- Para compilar:

```bash
make compilar
```

- Para ejecutar:

```bash
./programa
```

- Para ejecutar con valgrind:

```bash
make valgrind
```

---

## 🛠️Funcionamiento

### 🏛️Estructuras

Para este trabajo práctico se definieron distintas estructuras de datos que permiten organizar y manipular los elementos de forma eficiente.  
A continuación se detallan las estructuras principales:

---

#### 🔹`nodo_t`

Representa un **nodo** enlazado.

- Contiene una clave char que permite el acceso al dato.
- Contiene un puntero genérico al dato almacenado.
- Guarda un puntero al siguiente nodo.

📊 **Diagrama de memoria:**  
![Imagen nodo_t](assets/img/image-nodo_t.png)

---

#### 🔹`hash_t`

Representa la **tabla de hash**.

- Tiene un array de punteros que apuntan a **nodo_t**.
- Lleva un contador con la capacidad total de la tabla y la cantidad total de elementos.

📊 **Diagrama de memoria:**  
![Imagen hash_t](assets/img/image-hash_t.png)

---

### ⚙️Funcionamiento General

---

### 1. Validación de parámetros

Antes de iniciar cualquier procesamiento, el programa valida que los parámetros de entrada sean correctos con `validando_params(argc, argv)`:

- 🔹 Cantidad de argumentos: deben ser **exactamente 5** (`<ejecutable> <archivo.csv> buscar nombre|id <valor>`).
- 🔹 Formato del archivo: se comprueba que el archivo pasado como `argv[1]` tenga la extensión **.csv** con `validando_formato_csv()`.
- 🔹 Comando de operación: el argumento `argv[2]` debe ser **'buscar'**.
- 🔹 Tipo de búsqueda: el argumento `argv[3]` debe ser **"nombre"** o **"id"**.

---

### 2.Flujo de Ejecución

---

### 2.1 Lectura principal (según `argv[1]`)

Se realiza la lectura de un archivo **.csv** cuya ubicacion fue pasado por parametro con `tp1_leer_archivo()`.

```c
tp1_t *tp1_leer_archivo(const char *nombre);
```

**📌¿Qué hace esta función?:**

- 🔹 Abre el archivo .csv indicado por parámetro
- 🔹 Lee cada línea del archivo y verifica que tenga el formato correcto.
- 🔹 Extrae la información de cada Pokémon contenido en el archivo.
- 🔹 Guarda todos los Pokémon válidos dentro de una estructura **tp1_t**.
- 🔹 Al finalizar, ordena los Pokémon por su id de manera ascendente.
- 🔹 Devuelve la estructura completa con todos los datos cargados.

- ⚠️ En caso de que el archivo no respete el formato, no se pueda abrir o falle la reserva de memoria, la función devuelve `NULL`.

---

### 2.2 Creacion de Hash

Se crea la estructura principial de este TDA con `hash_crear()` donde se almacenarán los Pokémon como pares clave → valor.

```c
hash_t *hash_crear(size_t capacidad_inicial);
```

---

### 2.3 Validacion de Funciones

Antes de continuar, se valida que ambas estructuras **(tp1_t y hash_t)** hayan sido creadas correctamente:

```c
bool validando_func(tp1_t *tp1, hash_t *hash);
```

⚠️ Si alguna falló, se liberan los recursos y finaliza el programa.

---

### 2.4 Cargando pokemones

Al pasar las validaciones, se insertan los pokemones en el Hash

```c
bool cargando_pokemons(tp1_t *tp1, hash_t *hash);
```

**📌¿Qué hace esta función?:**

- 🔹 Recorre cada Pokémon guardado en tp1 con su iterador interno.
- 🔹 Se llama a la función callback `guardar_en_hash()`, que inserta el Pokémon en el hash usando su nombre como clave.
- 🔹 Se valida que haya pokemones.

⚠️ Si la cantidad de elementos insertados no coincide con la cantidad total de pokemones, se liberan los recursos y finaliza el programa.

---

### 2.5 Buscando pokemon según operacion(según `argv[2]`)

El sistema permite al usuario hacer una busqueda entre 2 operaciones predefinidas.

#### 🔍 buscar

**Parámetros:** `<tipo_busqueda> <valor>`

- 🔹Segun el **tipo de busqueda**:

  - `nombre`: Busca directamente en el hash con `hash_buscar()` ya que el nombre es la clave.
  - `id`: Como el hash no tiene el id como clave, se itera con `hash_iterar()` e internamente se compara cada Pokémon con el id buscado mediante el callback `buscando_pk_id()`.

- 📌Si lo encuentra, se retorna el Pokémon.
- 📌Si no existe, devuelve **NULL**

---

### 2.6 Mostrando Pokemon

Si el Pokémon fue encontrado, se llama a:

```c
void mostrar_pokemon(struct pokemon *p);
```

📄 Se imprime:

- Id
- Nombre
- Tipo
- Ataque, Defensa, Velocidad

⚠️ Si no fue encontrado, se imprime un mensaje indicando lo mismo

---

### 3. Liberación de Memoria

Al finalizar la ejecución:

- 🗑️Se destruye la tabla de hash de Pokémones con `hash_destruir(hash)`.

- 🗑️Se libera la estructura principal del TP con `tp1_destruir(tp1)`.

Esto asegura que no queden memory leaks ni recursos sin liberar.

---

## Tests Unitarios

Esta sección describe cómo se verifican todas las funciones primitivas del proyecto mediante pruebas unitarias y de integracion.

### Objetivo

- 🔹Comprobar que cada función del proyecto se ejecute correctamente en distintos escenarios.
- 🔹Asegurar que las operaciones sobre las estructuras dinámicas se realicen sin errores de memoria.
- 🔹Validar el correcto funcionamiento de cada una de las Estructura de Datos.
- 🔹Confirmar que las funciones devuelvan los resultados esperados.
- 🔹Asegurarse el buen funcionamiento en casos demandantes

### Cobertura de pruebas

Se realizan 114 pruebas

Estos tests incluyen:

- 🔹**Pruebas de creacion**
- 🔹**Pruebas de incercion**
- 🔹**Pruebas de eliminacion**
- 🔹**Pruebas de busqueda**
- 🔹**Pruebas de liberacion de memoria**
- 🔹**Pruebas de iteracion**
- 🔹**Pruebas de combinacion de funciones**
- 🔹**Pruebas de estres**

### Compilación y ejecución de tests

Para compilar los tests:

```bash
make compilar_t
```

Para ejecutar los tests:

```bash
./tests
```

Para ejecutar con valgrind:

```bash
make valgrind_t
```

## 📈Complejidad computacional de las funciones primitivas

- **`hash_crear`** → O(1)
  La función `hash_crear` realiza un conjunto fijo de operaciones:
  Por lo tanto, la complejidad es O(1) en todos los casos.

- **`hash_cantidad`** → O(1)
  Solo accede al campo cantidad de la estructura **hash_t**, lo que es una operación de acceso directo a memoria. No recorre ni calcula nada adicional.
  Por lo tanto, la complejidad es O(1) en todos los casos.

- **`hash_insertar`** → O(n) en el peor caso, O(k) amortizado promedio
  En el caso promedio, se calcula el bucket en tiempo constante y se recorre una lista de hasta k elementos para insertar. Como el factor de carga se mantiene bajo, k es pequeño, por lo que la operación es O(k).
  En el peor caso, todas las claves colisionan en el mismo bucket y se recorren hasta n elementos. Además, si se produce un rehash, se reubica todo el contenido del hash, lo que es O(n).

- **`hash_buscar`** → O(n) en el peor caso, O(k) amortizado promedio
  Se calcula el índice del bucket en tiempo constante y luego se recorre la lista que contiene hasta k elementos para encontrar la clave.
  En el promedio, k es pequeño (gracias al control del factor de carga), así que la operación es O(k). Pero si todas las claves colisionan en un único bucket, se pueden recorrer hasta n elementos, resultando en O(n).

- **`hash_contiene`** → O(n) en el peor caso, O(k) amortizado promedio
  Funciona igual que `hash_buscar:` se localiza el bucket en tiempo constante y se recorre su lista de colisiones, que contiene hasta k elementos.
  Con un factor de carga controlado, k es pequeño, así que la operación es O(k). Si todas las claves colisionan, se recorren hasta n elementos, quedando O(n).

- **`hash_quitar`** → O(n) en el peor caso, O(k) amortizado promedio
  Se calcula el bucket en tiempo constante y luego se recorre la lista de colisiones hasta encontrar y extraer el nodo correspondiente, lo cual toma O(k) en promedio.
  Si todas las claves colisionan en un mismo bucket, se recorren hasta n elementos. Por eso, en el peor caso la complejidad es O(n).

- **`hash_iterar`** → O(n)
  La función recorre todos los buckets de la tabla, pero solo avanza dentro de cada uno cuando encuentra nodos. Dado que cada nodo del hash se visita exactamente una vez y la cantidad total de nodos almacenados es n, el tiempo total de ejecución depende directamente de esa cantidad.

- **`hash_destruir`** → O(n)
  La función `hash_destruir` solo llama a `hash_destruir_todo`, por lo que su complejidad depende directamente de esa función. Por tanto, su complejidad es O(n), siendo n la cantidad de elementos almacenados en el hash.

- **`hash_destruir_todo`** → O(n)
  `hash_destruir_todo` recorre internamente todo el contenido del hash, liberando cada nodo y opcionalmente cada dato. Como visita y destruye cada elemento una sola vez, su complejidad es O(n), donde n es la cantidad total de elementos almacenados.

## 📚 Respuestas a las preguntas teóricas(explicar con graficos)

### 1. Qué es un diccionario

Un diccionario es una estructura de datos que permite almacenar y relacionar pares de valores, donde cada par está compuesto por:

- 🔹Una clave🔑 → usada para identificar el dato
- 🔹Un valor🪙 → la información asociada a esa clave

📌 **Características principales:**

- 🔹Permite insertar,modificar, buscar y eliminar elementos.
- 🔹Las claves son únicas: no puede haber dos elementos con la misma clave.
- 🔹El acceso a un valor se realiza a través de su clave, no mediante índices como en un arreglo.

### 2. Explicá 3 formas diferentes de implementar un diccionario y qué es una tabla de Hash y los diferentes métodos de resolución de colisiones vistos

1.🧱**Tabla Hash**
Una tabla hash usa una función hash para transformar la clave en un índice dentro de un arreglo llamado buckets. Cada bucket puede contener uno o varios elementos (por colisiones).

🔍 Tipos de manejo de colisiones

Como no es posible evitar totalmente que dos claves diferentes produzcan el mismo índice (colisión), las tablas hash implementan alguna estrategia de resolución de colisiones:

_Hashing con direccionamiento abierto_

En este método, cada bucket contiene una estructura auxiliar, usualmente una lista enlazada o una estructura más compleja como un árbol balanceado. Si varias claves colisionan en el mismo índice, se almacenan como nodos dentro de la lista o árbol de ese bucket.

📊 **Diagrama**  
![Imagen hash abierto](assets/img/image-dichashabierto1.png)

✏️**Características:**

- 🔹El tamaño del arreglo de buckets puede ser fijo, pero la cantidad de elementos en la tabla es ilimitada, ya que las colisiones no dependen del tamaño del arreglo.

- 🔹El tiempo promedio para acceder a un elemento depende del factor de carga (n/b): si es bajo, se mantiene eficiente.

- 🔹Un factor de carga grande no detiene la inserción, pero aumenta el número promedio de colisiones (k)

- 🔹Permite un rehash sin mover todos los datos uno por uno.

_Hashing con direccionamiento cerrado_
Aquí no se usan estructuras auxiliares. Todos los elementos se guardan directamente en el arreglo, y cuando ocurre una colisión, se busca una posición alternativa en el arreglo siguiendo una estrategia:

-**Linear probing:** Avanza secuencialmente al siguiente índice.
![Imagen hash cerrado1](assets/img/image-hashcerrado1.png)

-**Quadratic probing:** Salta hacia adelante usando un incremento cuadrático.
![Imagen hash cerrado2](assets/img/image-hashcerrado2.png)

-**Double hashing:** Busca el siguiente índice aplicando una segunda función hash.
![Imagen hash cerrado3](assets/img/image-hashcerrado3.png)

✏️**Características:**

- 🔹Ahorra memoria al evitar punteros o estructuras auxiliares.

- 🔹Una alta ocupación del arreglo provoca largos tiempos de búsqueda y degradación severa de rendimiento. Necesita rehash antes de llegar al 100%.

- 🔹Eliminar elementos requiere marcar posiciones como eliminadas en lugar de simplemente vaciarlas, o bien reinsertar todo el bloque hasta la siguiente posición vacía.

- 🔹Requiere que la capacidad del arreglo sea lo suficientemente grande para evitar sobrecarga.

📈**Complejidades**

En ambas variantes:

Promedio: O(k), donde k es el tamaño promedio del bucket o la longitud promedio de una secuencia de probes. Bajo un buen diseño de hash y rehashing oportuno, k suele ser una constante pequeña.

Peor caso: O(n), cuando todos los elementos colisionan en el mismo bucket (hash abierta) o cuando el arreglo está casi lleno y la búsqueda se vuelve secuencial (hash cerrada).

⚙️**Rehashing**

Para evitar degradación de performance, cuando el factor de carga (número de elementos / capacidad de la tabla) supera un cierto factor, la tabla se rehashea:

- 🔹Se crea una tabla de mayor tamaño.

- 🔹Todos los elementos se vuelven a insertar aplicando la función hash nuevamente (O(n) costo).

Esto garantiza que k se mantenga bajo.

---

2.🌲**Árbol Binario de Búsqueda Balanceado**
Un Árbol Binario de Búsqueda (ABB) guarda sus elementos de forma ordenada: cada nodo tiene una clave y un valor, y está posicionado según la clave. Para asegurarse de que las operaciones no empeoren con el tiempo, se usan versiones balanceadas del ABB como los árboles AVL o Red-Black, que mantienen la altura del árbol lo más baja posible.

📊 **Diagramas**  
![Imagen abb diccionario](assets/img/image-dicabb1.png)

📌 **Características principales:**

- ✅ La búsqueda, la inserción y la eliminación son todas O(log n) en el peor caso porque la altura del árbol se mantiene balanceada (h ≈ log n).

- ✅ Es posible realizar un recorrido in-order para obtener las claves ordenadas alfabéticamente o por su valor comparativo.

- ❌ Más compleja de implementar que una tabla hash simple: requiere rotaciones y reequilibrio.

- 🔁 Mantiene orden, a diferencia de la tabla hash: esto es útil para operaciones como obtener los primeros N elementos o rangos entre claves.

- 📦 Se usa en bases de datos, compiladores y sistemas donde se requiere acceso rápido y ordenado a datos.

---

3.📝**Lista de pares clave-valor**
En esta implementación, el diccionario se representa como una lista (enlazada o arreglo dinámico) donde cada elemento almacena una clave y su valor asociado. Es la forma más sencilla de implementar un diccionario.

📊 **Diagramas**  
![Imagen lista diccionario](assets/img/image-diclista1.png)

📌 **Características principales:**

- ✅ Simplicidad extrema: no requiere estructuras complejas ni funciones hash.

- ❌ Todas las operaciones básicas (buscar, insertar, eliminar) son O(n) en el peor caso porque es necesario recorrer la lista hasta encontrar la clave deseada.

- ✅ Puede ser útil para pequeños volúmenes de datos o en contextos donde el orden de inserción tiene importancia (por ejemplo, logs o buffers).

- ❌ No es adecuada para aplicaciones donde se requieran muchas búsquedas o actualizaciones de forma eficiente.

### 3. Qué es una función de hash y qué características debe tener para nuestro problema en particular

Una función de hash es un algoritmo que, a partir de una clave de entrada (generalmente una cadena de caracteres o un número), devuelve un valor entero llamado hash. Ese valor se utiliza para determinar la posición donde se almacenará el elemento en una tabla de hash.

🔍 En el contexto de nuestro problema, donde trabajamos con una tabla de hash para almacenar pares clave–valor, la función de hash debe cumplir con estas características:

- **Determinística:**
  Para una misma clave, siempre debe producir el mismo resultado. Esto garantiza que podamos encontrar el dato nuevamente cuando lo necesitemos.

- **Uniforme:**
  Debe distribuir las claves lo más equitativamente posible a lo largo de la tabla. Así se reduce la cantidad de colisiones (es decir, que dos claves distintas terminen en la misma posición).

- **Eficiente:**
  Tiene que ser rápida de calcular, ya que se usará muchas veces (al insertar o buscar elementos).

- **Independiente del tamaño de la tabla (en lo posible):**
  Aunque el valor final suele ajustarse con un módulo (% capacidad_tabla), la función base debería generar valores razonablemente dispersos para cualquier tamaño de tabla.

- **Adaptable a las claves que manejamos:**
  En nuestro caso, las claves pueden ser por ejemplo cadenas de texto. Así que la función debe saber procesarlas (por ejemplo, recorriendo cada carácter y combinándolo mediante sumas, multiplicaciones, desplazamientos, etc.).

### 4. Explique por qué es importante el tamaño de la tabla (tanto para tablas abiertas como cerradas)

El tamaño de la tabla de hash es un aspecto crítico en el diseño de una estructura de datos basada en hashing, tanto si utilizamos direcciones abiertas como cerradas

**🔐En tablas con direcciones cerradas**

En este modelo, cada posición de la tabla contiene una lista (o estructura similar) donde se guardan todos los elementos que colisionan en esa posición.

✅ Importancia del tamaño de la tabla:

- Cuanto más grande sea la tabla, menos probabilidad de colisión habrá, porque hay más posiciones posibles para distribuir las claves.

- Una tabla pequeña provoca que muchas claves distintas terminen en la misma posición y, por lo tanto, se formen listas largas. Esto degrada el tiempo promedio de búsqueda, inserción y borrado, que deja de ser cercano a O(1) y se acerca a O(n) (n = cantidad de claves).

🔎 Objetivo: Mantener bajo el factor de carga (α = cantidad de elementos / tamaño de la tabla), idealmente menor a 1. Si crece demasiado, conviene rehashar la tabla a un tamaño mayor.

**🔓 En tablas con direcciones abiertas (sondeo lineal, cuadrático, hashing doble, etc.)**

En este caso, todas las claves se almacenan dentro del arreglo principal, y cuando ocurre una colisión, se busca otra posición disponible según alguna técnica de sondeo.

✅ Importancia del tamaño de la tabla:

- Cuanto más grande es la tabla, más espacio libre tenemos, lo que reduce las secuencias de colisiones y hace que las operaciones sean más rápidas.

- Si la tabla está demasiado llena (factor de carga alto), aumenta la probabilidad de largas secuencias de sondeo para encontrar una celda vacía o el elemento buscado. Esto empeora la complejidad, especialmente para búsquedas e inserciones.
