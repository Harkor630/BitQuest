/* ============================================================
   main.c  -  Punto de entrada de BitQuest
   Controla el flujo general: menu, niveles y resumen final.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "juego.h"
#include "mapas.h"

/* ------------------------------------------------------------
   Posiciones de inicio del jugador en cada nivel
   (deben coincidir con la 'P' inicial del mapa)
   ------------------------------------------------------------ */
static const int INICIO_FILA[3] = {1, 1, 1};
static const int INICIO_COL[3]  = {1, 1, 1};

/* ------------------------------------------------------------
   pantalla_inicio
   ------------------------------------------------------------ */
static void pantalla_inicio(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("\n");
    printf("  ============================================\n");
    printf("   BitQuest: Explorador de Matrices con NASM  \n");
    printf("  ============================================\n");
    printf("\n");
    printf("          Explorador de Matrices con C y NASM\n");
    printf("\n");
    printf("  Controles:\n");
    printf("    W / A / S / D  ->  Mover jugador\n");
    printf("    Q              ->  Salir del juego\n");
    printf("\n");
    printf("  Objetivo:\n");
    printf("    Recoge la llave [K], abre la puerta [D]\n");
    printf("    y llega a la salida [E] para avanzar de nivel.\n");
    printf("    Recoge monedas [M] para aumentar tu puntaje.\n");
    printf("\n");
    printf("  Presiona cualquier tecla para comenzar...\n");
    getchar();
}

/* ------------------------------------------------------------
   jugar_nivel
   Inicializa el estado del jugador, copia el mapa original
   y ejecuta el loop del nivel.
   Devuelve 0 si el nivel se completo, 1 si el jugador salio.
   ------------------------------------------------------------ */
static int jugar_nivel(char mapa_orig[FILAS][COLS], int num_nivel,
                       int *monedas_out, int *pasos_out) {
    /* Copiar el mapa para no modificar el original */
    char mapa[FILAS][COLS];
    memcpy(mapa, mapa_orig, sizeof(mapa));

    /* Inicializar jugador */
    Jugador j;
    j.fila       = INICIO_FILA[num_nivel - 1];
    j.col        = INICIO_COL[num_nivel - 1];
    j.tiene_llave = 0;
    j.monedas    = 0;
    j.pasos      = 0;

    /* --- Funcion NASM 1: contar monedas totales del nivel --- */
    long long total_monedas = contar_caracter((char *)mapa,
                                              FILAS * COLS, MONEDA);

    /* --- Funcion NASM 5: contar celdas libres al inicio --- */
    long long libres = contar_libres((char *)mapa, FILAS * COLS);
    printf("  [Nivel %d] Celdas libres en el mapa: %lld\n",
           num_nivel, libres);
    printf("  [Nivel %d] Monedas disponibles: %lld\n",
           num_nivel, total_monedas);
    printf("  Presiona cualquier tecla para iniciar...\n");
    getchar();

    /* Borrar la 'P' inicial del mapa (el jugador se dibuja dinamicamente) */
    mapa[j.fila][j.col] = CAMINO;

    /* Loop principal del nivel */
    while (1) {
        imprimir_mapa(mapa, &j, num_nivel);

        int resultado = mover_jugador(mapa, &j);

        if (resultado == 1) {
            /* El jugador quiso salir */
            *monedas_out = j.monedas;
            *pasos_out   = j.pasos;
            return 1;
        }

        if (resultado == 2) {
            /* Llego a la salida */
            *monedas_out = j.monedas;
            *pasos_out   = j.pasos;
            resumen_nivel(&j, (int)total_monedas, num_nivel);
            return 0;
        }
    }
}

/* ============================================================
   main
   ============================================================ */
int main(void) {
    pantalla_inicio();

    int monedas_acum   = 0;  /* monedas totales recolectadas */
    int pasos_acum     = 0;  /* pasos totales                */
    int monedas_nivel;
    int pasos_nivel;

    /* Total de monedas posibles en los 3 niveles */
    long long total_m1 = contar_caracter((char *)mapa1, FILAS * COLS, MONEDA);
    long long total_m2 = contar_caracter((char *)mapa2, FILAS * COLS, MONEDA);
    long long total_m3 = contar_caracter((char *)mapa3, FILAS * COLS, MONEDA);
    int monedas_posibles = (int)(total_m1 + total_m2 + total_m3);

    /* --- Nivel 1 --- */
    printf("\n  === NIVEL 1 ===\n");
    if (jugar_nivel(mapa1, 1, &monedas_nivel, &pasos_nivel)) {
        printf("\n  Juego terminado por el jugador.\n");
        return 0;
    }
    monedas_acum += monedas_nivel;
    pasos_acum   += pasos_nivel;

    /* --- Nivel 2 --- */
    printf("\n  === NIVEL 2 ===\n");
    if (jugar_nivel(mapa2, 2, &monedas_nivel, &pasos_nivel)) {
        printf("\n  Juego terminado por el jugador.\n");
        return 0;
    }
    monedas_acum += monedas_nivel;
    pasos_acum   += pasos_nivel;

    /* --- Nivel 3 --- */
    printf("\n  === NIVEL 3 ===\n");
    if (jugar_nivel(mapa3, 3, &monedas_nivel, &pasos_nivel)) {
        printf("\n  Juego terminado por el jugador.\n");
        return 0;
    }
    monedas_acum += monedas_nivel;
    pasos_acum   += pasos_nivel;

    /* --- Funcion NASM 3: calcular puntaje final --- */
    long long puntaje = calcular_puntaje(monedas_acum, pasos_acum, 3);

    /* Resumen final */
    resumen_final(monedas_acum, monedas_posibles, pasos_acum, (int)puntaje);

    return 0;
}