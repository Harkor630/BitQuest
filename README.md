# BitQuest
Explorador de Matrices con C y NASM
BitQuest: Explorador de Matrices con C y NASM
=============================================

Descripcion
-----------
Videojuego de laberinto en consola desarrollado con programacion
hibrida C + NASM de 64 bits. El jugador recorre mapas de 60x60,
recolecta monedas, busca una llave, abre una puerta y llega a
la salida para avanzar de nivel. Solo se muestra una ventana
visible de 20x20 del mapa completo.

Estructura del proyecto
-----------------------
  main.c       -> Punto de entrada, menu y control de niveles
  juego.c      -> Logica del juego (render, movimiento, resumenes)
  juego.h      -> Prototipos, constantes y definiciones
  mapas.h      -> Los tres mapas de 60x60
  rutinas.asm  -> Cinco funciones obligatorias en NASM x86-64
  build.bat    -> Script de compilacion para Windows
  README.txt   -> Este archivo

Requisitos
----------
  - NASM (https://www.nasm.us/)
  - GCC / MinGW-w64 para Windows, o gcc para Linux/macOS

Compilacion en Windows
----------------------
  Ejecutar build.bat desde la carpeta del proyecto:

    build.bat

  O manualmente:
    nasm -f win64 -D WIN64 rutinas.asm -o rutinas.obj
    gcc -Wall -o BitQuest.exe main.c juego.c rutinas.obj
    BitQuest.exe

Compilacion en Linux
--------------------
    nasm -f elf64 rutinas.asm -o rutinas.o
    gcc -Wall -o BitQuest main.c juego.c rutinas.o
    ./BitQuest

Compilacion en macOS
--------------------
  En macOS los simbolos externos llevan prefijo '_'. Editar
  rutinas.asm y agregar guion bajo a los nombres globales, o
  usar el flag de GCC -Wl,-no_pie segun la version del sistema.

    nasm -f macho64 rutinas.asm -o rutinas.o
    gcc -Wall -o BitQuest main.c juego.c rutinas.o
    ./BitQuest

Controles
---------
  W  ->  Mover arriba
  S  ->  Mover abajo
  A  ->  Mover izquierda
  D  ->  Mover derecha
  Q  ->  Salir del juego

Funciones NASM implementadas
-----------------------------
  1. contar_caracter   -> Cuenta un caracter en el mapa (monedas)
  2. validar_movimiento -> Verifica si una celda es transitable
  3. calcular_puntaje   -> Calcula el puntaje final del jugador
  4. detectar_objeto    -> Detecta objetos en una celda especifica
  5. contar_libres      -> Cuenta celdas libres en el mapa

Formula del puntaje
-------------------
  puntaje = (monedas * 100) + (niveles_completados * 500) - pasos
  Minimo: 0 puntos.