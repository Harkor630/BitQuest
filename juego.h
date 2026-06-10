#ifndef JUEGO_H
#define JUEGO_H

#define FILAS 60 
#define COLS 60 
#define VISTA 20

//Caracteres para representar el mapa
#define PARED   '#'
#define CAMINO  '.'
#define JUGADOR 'P'
#define MONEDA  'M'
#define LLAVE   'K'
#define PUERTA  'D'
#define SALIDA  'E'

//Estado del Jugador
typedef struct {
    int fila;
    int col;
    int tiene_llave;
    int monedas;
    int pasos;
} Jugador;

//Funciones en C
void imprimir_mapa(char mapa[FILAS][COLS], Jugador *j, int nivel);
void mover_jugador(char mapa[FILAS][COLS], Jugador *j);
void resumen_nivel(Jugador *j, int total_monedas, int nivel);
void resumen_final(int monedas_total, int pasos_total, int puntaje);

//Funciones en NASM
extern long long contar_caracter(char *mapa, int total, char c);
extern long long validar_movimiento(char *mapa, int cols, int fila, int col);
extern long long calcular_puntaje(int monedas, int pasos, int niveles);
extern long long detectar_objeto(char *mapa, int cols, int fila, int col, char obj);
extern long long contar_libres(char *mapa, int total);

#endif 
