; ============================================================
; rutinas.asm  -  BitQuest  (Win64, convencion Microsoft x64)
; Argumentos: rcx, rdx, r8, r9  |  Retorno: rax
; ============================================================

section .text

global contar_caracter
global validar_movimiento
global calcular_puntaje
global detectar_objeto
global contar_libres

; ============================================================
; contar_caracter(char *mapa, int total, char c)
;   rcx = mapa   rdx = total   r8 = caracter
;   retorna en rax cuantas veces aparece el caracter
; ============================================================
contar_caracter:
    xor     rax, rax            ; contador = 0
    xor     r9,  r9             ; indice   = 0
    movzx   r10, r8b            ; caracter buscado

.loop:
    cmp     r9,  rdx
    jge     .fin
    movzx   r11, byte [rcx + r9]
    cmp     r11, r10
    jne     .sig
    inc     rax
.sig:
    inc     r9
    jmp     .loop
.fin:
    ret


; ============================================================
; validar_movimiento(char *mapa, int cols, int fila, int col)
;   rcx = mapa   rdx = cols   r8 = fila   r9 = col
;   retorna 1 si puede moverse, 0 si es pared
; ============================================================
validar_movimiento:
    imul    r8, rdx             ; fila * cols
    add     r8, r9              ; + col  => indice
    movzx   rax, byte [rcx + r8]
    cmp     al, '#'
    je      .pared
    mov     rax, 1
    ret
.pared:
    xor     rax, rax
    ret


; ============================================================
; calcular_puntaje(int monedas, int pasos, int niveles)
;   rcx = monedas   rdx = pasos   r8 = niveles
;   formula: (monedas*100) + (niveles*500) - pasos
; ============================================================
calcular_puntaje:
    imul    rcx, 100            ; monedas * 100
    imul    r8,  500            ; niveles * 500
    mov     rax, rcx
    add     rax, r8
    sub     rax, rdx            ; - pasos
    test    rax, rax
    jns     .ok
    xor     rax, rax
.ok:
    ret


; ============================================================
; detectar_objeto(char *mapa, int cols, int fila, int col, char obj)
;   rcx = mapa   rdx = cols   r8 = fila   r9 = col
;   obj = quinto argumento en pila: [rsp+40] (tras shadow space)
;   retorna 1 si el objeto esta en esa celda, 0 si no
; ============================================================
detectar_objeto:
    movzx   r10, byte [rsp+40]  ; obj (5to argumento)
    imul    r8,  rdx            ; fila * cols
    add     r8,  r9             ; + col
    movzx   rax, byte [rcx + r8]
    cmp     rax, r10
    je      .encontrado
    xor     rax, rax
    ret
.encontrado:
    mov     rax, 1
    ret


; ============================================================
; contar_libres(char *mapa, int total)
;   rcx = mapa   rdx = total
;   retorna cantidad de celdas '.'
; ============================================================
contar_libres:
    xor     rax, rax
    xor     r8,  r8
.loop:
    cmp     r8,  rdx
    jge     .fin
    movzx   r9,  byte [rcx + r8]
    cmp     r9b, '.'
    jne     .sig
    inc     rax
.sig:
    inc     r8
    jmp     .loop
.fin:
    ret