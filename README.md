# BitQuest

Repositorio sobre el minijuego realizado como Proyecto Final
Lenguaje Ensamblador - UAA - 4to "B"
INTEGRANTES: 
-Judith Lizeth Paredes Rodriguez
-Maximiliano Padilla Miramontes
-Monserrat Cortez Vargas

# BitQuest
Explorador de Matrices con C y NASM
BitQuest: Explorador de Matrices con C y NASM
=============================================

Descripcion:
-----------
Videojuego de laberinto en consola desarrollado con programacion
hibrida C + NASM de 64 bits. El jugador recorre mapas de 60x60,
recolecta monedas, busca una llave, abre una puerta y llega a
la salida para avanzar de nivel. Solo se muestra una ventana
visible de 20x20 del mapa completo.

Estructura del proyecto:
-----------------------
  main.c       -> Punto de entrada, menu y control de niveles
  juego.c      -> Logica del juego (render, movimiento, resumenes)
  juego.h      -> Prototipos, constantes y definiciones
  mapas.h      -> Los tres mapas de 60x60
  rutinas.asm  -> Cinco funciones obligatorias en NASM x86-64
  build.bat    -> Script de compilacion para Windows
  README.txt   -> Este archivo

Requisitos:
----------
  - NASM (https://www.nasm.us/)
  - GCC / MinGW-w64 para Windows, o gcc para Linux/macOS

Compilacion en Windows:
----------------------
  Ejecutar build.bat desde la carpeta del proyecto:

    build.bat

