# TDA Hash

Se pide implementar un diccionario basado en una **Tabla de Hash abierta** (direccionamiento cerrado) en C. Para ello se brindan las firmas de las funciones públicas a implementar y **se deja a criterio del alumno la creación de las estructuras y funciones privadas del TDA** para el correcto funcionamiento de la **Tabla de Hash** cumpliendo con las buenas prácticas de programación. La función de **Hash** a utilizar será interna de la implementación y también debe ser decidida por el alumno. Para esta implementación las claves admitidas por la tabla serán solamente strings (no nulos). Adicionalmente se pide la creación de un iterador interno que sea capaz de recorrer las claves almacenadas en la tabla.


En el archivo **main.c** se debe implementar un programa que por línea de comandos reciba el nombre de un archivo y utilizando las primitivas del **TP1** cargue un archivo de pokemones en una tabla de hash. El programa debe implementar las funcionalidades de búsqueda de pokemon por nombre y por id utilizando las primitivas de hash si es posible. Cuando no sea posible implementarlo de otra forma y explicar en el informe por qué. No se permite modificar los archivos de cabecera.

El TDA entregado deberá compilar y pasar las pruebas dispuestas por la cátedra sin errores, adicionalmente estas pruebas deberán ser ejecutadas **sin pérdida de memoria**.

Para la resolución de este trabajo se recomienda utilizar una **metodología orientada a pruebas**. A tal fin, se incluye un archivo **pruebas.c** que debe ser completado con las pruebas pertinentes de cada una de las diferentes primitivas del TDA. El archivo de pruebas **forma parte de la entrega y por lo tanto de la nota final**. Aún mas importante, las pruebas van a resultar fundamentales para lograr no solamente una implementación correcta, si no también una experiencia de desarrollo menos turbulenta.

## Consejos para la elaboración del trabajo

Intente comprender primero el funcionamiento de la tabla y del mecanismo de resolución de colisiones. Utilice lápiz y papel para dibujar algunas tablas y realice varias operaciones de inserción, eliminación y búsqueda. Asegúrese de entender bien cómo se relacionan las operaciones y cómo los datos se almacenan en la tabla antes de comenzar la implementación.

Y como siempre, las recomendaciones generales que no pueden faltar: recuerde que al escribir pruebas **no se busca en el código de pruebas la encapsulación ni simplificación de las mismas**. No es incorrecto tener pruebas con código repetitivo. Las pruebas son una **especificación** del comportamiento deseado de las primitivas. Como tal, deben ser fáciles de leer y entender su objetivo.

En general para todo el código: utilice nombres claros de variables y funciones auxiliares. Nombres claros facilitan la comprensión de la implementación (tanto por el implementador como por el corrector). Respete las convenciones de estilo propuestas por la cátedra.

**NO** escribas código a lo loco sin compilar cada tanto. Implementá la solución de a poco y compilando a cada paso. Dejar la compilación para el final es uno de los peores errores que podés cometer. Para la compilación de las pruebas se provee un **Makefile**. Utilizá el comando **make** frecuentemente para compilar y correr las pruebas.

**NO** avances en la implementación si quedan errores sin resolver en alguna prueba. Cada vez que escribas una prueba implementá toda la funcionalidad necesaria para que funcione correctamente. Esto incluye liberar memoria y accesos inválidos a la misma. Solamente una vez que hayas logrado que la prueba pase exitosamente es que podés comenzar a escribir la próxima prueba para continuar el trabajo.

**NO** está permitido modificar los archivos **.h**. Se pueden hacer modificaciones al **makefile**, pero recordá que el trabajo será compilado por el sistema de entregas con las cabeceras y el **makefile** original.

## Parte teórica

Explicá teóricamente los siguientes puntos (no necesariamente en orden, pero por favor usando diagramas):

   - Qué es un diccionario
    - Explicá 3 formas diferentes de implementar un diccionario (tabla de hash cuenta como 1 sola implementación posible)
   - Qué es una función de hash y qué características debe tener para nuestro problema en particular
   - Qué es una tabla de Hash y los diferentes métodos de resolución de colisiones vistos (encadenamiento, probing, etc)
   - Explique por qué es importante el tamaño de la tabla (tanto para tablas abiertas como cerradas)
     - Dado que en una tabla abierta se pueden encadenar colisiones sin importar el tamaño de la tabla, ¿Realmente importa el tamaño?
   - Mas te vale que expliques con dibujos
