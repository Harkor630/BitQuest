/* ============================================================
juego.c  -  Logica principal del juego BitQuest
Funciones: imprimir mapa (ventana 20x20), mover jugador,
            resumen de nivel y resumen final.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
  #include <conio.h>    /* _getch() en Windows */
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "juego.h"

/* ------------------------------------------------------------
Lectura de tecla sin buffering (portable)
   ------------------------------------------------------------ */
#ifndef _WIN32
static char leer_tecla(void) {
    struct termios viejo, nuevo;
    char c;
    tcgetattr(STDIN_FILENO, &viejo);
    nuevo = viejo;
    nuevo.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &nuevo);
    read(STDIN_FILENO, &c, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &viejo);
    return c;
}
#else
static char leer_tecla(void) {
    return (char)_getch();
}
#endif

/* ------------------------------------------------------------
limpiar_pantalla
   ------------------------------------------------------------ */
static void limpiar_pantalla(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* ============================================================
imprimir_mapa
Muestra una ventana de VISTA x VISTA centrada en el jugador.
Si el jugador esta cerca de un borde, la ventana se ajusta.
   ============================================================ */
void imprimir_mapa(char mapa[FILAS][COLS], Jugador *j, int nivel) {
    /* Calcular esquina superior-izquierda de la ventana */
    int inicio_fila = j->fila - VISTA / 2;
    int inicio_col  = j->col  - VISTA / 2;

    /* Ajustar para no salirse del mapa */
    if (inicio_fila < 0) inicio_fila = 0;
    if (inicio_col  < 0) inicio_col  = 0;
    if (inicio_fila + VISTA > FILAS) inicio_fila = FILAS - VISTA;
    if (inicio_col  + VISTA > COLS)  inicio_col  = COLS  - VISTA;

    limpiar_pantalla();

    /* Encabezado de estado */
    printf("==================== BitQuest ====================\n");
    printf(" Nivel: %d   |   Monedas: %d   |   Llave: %s   |   Pasos: %d\n",
        nivel,
        j->monedas,
        j->tiene_llave ? "Si" : "No",
        j->pasos);
    printf("==================================================\n");

    /* Imprimir ventana visible */
    for (int f = inicio_fila; f < inicio_fila + VISTA; f++) {
        for (int c = inicio_col; c < inicio_col + VISTA; c++) {
            char celda = mapa[f][c];
            /* Mostrar al jugador en su posicion */
            if (f == j->fila && c == j->col)
                putchar(JUGADOR);
            else
                putchar(celda);
        }
        putchar('\n');
    }

    printf("--------------------------------------------------\n");
    printf(" [W/A/S/D] Mover   [Q] Salir\n");
}

/* ============================================================
mover_jugador
Lee una tecla, valida el movimiento con NASM y actualiza
el estado del jugador y del mapa.
Devuelve:
    0  -> continuar jugando
    1  -> jugador quiso salir (Q)
    2  -> jugador llego a la salida (E)
   ============================================================ */
int mover_jugador(char mapa[FILAS][COLS], Jugador *j) {
    char tecla = leer_tecla();

    int nueva_fila = j->fila;
    int nueva_col  = j->col;

    switch (tecla) {
        case 'W': case 'w': nueva_fila--; break;
        case 'S': case 's': nueva_fila++; break;
        case 'A': case 'a': nueva_col--;  break;
        case 'D': case 'd': nueva_col++;  break;
        case 'Q': case 'q': return 1;     /* salir del juego */
        default: return 0;                /* tecla ignorada  */
    }

    /* Verificar limites del mapa */
    if (nueva_fila < 0 || nueva_fila >= FILAS ||
        nueva_col  < 0 || nueva_col  >= COLS)
        return 0;

    /* --- Funcion NASM 2: validar movimiento (paredes) --- */
    long long puede = validar_movimiento((char *)mapa, COLS, nueva_fila, nueva_col);
    if (!puede) return 0;

    /* --- Funcion NASM 4: detectar objetos en la celda destino --- */
    char celda = mapa[nueva_fila][nueva_col];

    /* Puerta: solo si tiene llave */
    if (celda == PUERTA) {
        if (!j->tiene_llave) return 0;  /* bloqueado sin llave */
        /* Abrir puerta: convertir en camino */
        mapa[nueva_fila][nueva_col] = CAMINO;
    }

    /* Llave */
    long long hay_llave = detectar_objeto((char *)mapa, COLS,
                                        nueva_fila, nueva_col, LLAVE);
    if (hay_llave) {
        j->tiene_llave = 1;
        mapa[nueva_fila][nueva_col] = CAMINO;
    }

    /* Moneda */
    long long hay_moneda = detectar_objeto((char *)mapa, COLS,
                                        nueva_fila, nueva_col, MONEDA);
    if (hay_moneda) {
        j->monedas++;
        mapa[nueva_fila][nueva_col] = CAMINO;
    }

    /* Salida */
    long long hay_salida = detectar_objeto((char *)mapa, COLS,
                                        nueva_fila, nueva_col, SALIDA);

    /* Mover al jugador */
    j->fila = nueva_fila;
    j->col  = nueva_col;
    j->pasos++;

    if (hay_salida) return 2;
    return 0;
}

/* ============================================================
resumen_nivel
Muestra estadisticas al terminar cada nivel.
   ============================================================ */
void resumen_nivel(Jugador *j, int total_monedas, int nivel) {
    printf("\n=================================\n");
    printf(" Nivel %d completado\n", nivel);
    printf(" Monedas recolectadas: %d / %d\n", j->monedas, total_monedas);
    printf(" Pasos realizados: %d\n", j->pasos);
    printf("=================================\n");
    printf("\n Presiona cualquier tecla para continuar...\n");
    leer_tecla();
}

/* ============================================================
resumen_final
Muestra el resumen global al terminar los tres niveles.
   ============================================================ */
void resumen_final(int monedas_total, int monedas_posibles,
                int pasos_total, int puntaje) {
    printf("\n=================================\n");
    printf(" Juego completado!\n");
    printf(" Monedas totales recolectadas: %d / %d\n",
        monedas_total, monedas_posibles);
    printf(" Pasos totales: %d\n", pasos_total);
    printf(" Niveles completados: 3\n");
    printf(" Puntaje final: %d\n", puntaje);
    printf("=================================\n");
}