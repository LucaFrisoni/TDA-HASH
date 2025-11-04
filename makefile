VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g

compilar:
	gcc $(CFLAGS) main.c src/*.c -o programa
compilar_t:
	gcc $(CFLAGS) pruebas_alumno.c src/*.c -o tests
valgrind:
	valgrind $(VALGRIND_FLAGS) ./programa assets/pokemones.csv buscar nombre Pikachu
valgrind_t:
	valgrind $(VALGRIND_FLAGS) ./tests