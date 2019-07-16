                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ANSI-C Compiler
                                      3 ; Version 3.9.1 #11292 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module uart
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _uart_init
                                     12 	.globl _uart_write
                                     13 	.globl _uart_read
                                     14 ;--------------------------------------------------------
                                     15 ; ram data
                                     16 ;--------------------------------------------------------
                                     17 	.area DATA
                                     18 ;--------------------------------------------------------
                                     19 ; ram data
                                     20 ;--------------------------------------------------------
                                     21 	.area INITIALIZED
                                     22 ;--------------------------------------------------------
                                     23 ; absolute external ram data
                                     24 ;--------------------------------------------------------
                                     25 	.area DABS (ABS)
                                     26 
                                     27 ; default segment ordering for linker
                                     28 	.area HOME
                                     29 	.area GSINIT
                                     30 	.area GSFINAL
                                     31 	.area CONST
                                     32 	.area INITIALIZER
                                     33 	.area CODE
                                     34 
                                     35 ;--------------------------------------------------------
                                     36 ; global & static initialisations
                                     37 ;--------------------------------------------------------
                                     38 	.area HOME
                                     39 	.area GSINIT
                                     40 	.area GSFINAL
                                     41 	.area GSINIT
                                     42 ;--------------------------------------------------------
                                     43 ; Home
                                     44 ;--------------------------------------------------------
                                     45 	.area HOME
                                     46 	.area HOME
                                     47 ;--------------------------------------------------------
                                     48 ; code
                                     49 ;--------------------------------------------------------
                                     50 	.area CODE
                                     51 ;	../../lib/uart.c: 4: void uart_init() {
                                     52 ;	-----------------------------------------
                                     53 ;	 function uart_init
                                     54 ;	-----------------------------------------
      0080AD                         55 _uart_init:
                                     56 ;	../../lib/uart.c: 8: UART1_BRR2 = ((div >> 8) & 0xF0) + (div & 0x0F);
      0080AD 35 01 52 33      [ 1]   57 	mov	0x5233+0, #0x01
                                     58 ;	../../lib/uart.c: 9: UART1_BRR1 = div >> 4;
      0080B1 A6 01            [ 1]   59 	ld	a, #0x01
      0080B3 C7 52 32         [ 1]   60 	ld	0x5232, a
                                     61 ;	../../lib/uart.c: 11: UART1_CR2 = (1 << UART1_CR2_TEN) | (1 << UART1_CR2_REN);
      0080B6 35 0C 52 35      [ 1]   62 	mov	0x5235+0, #0x0c
                                     63 ;	../../lib/uart.c: 12: }
      0080BA 81               [ 4]   64 	ret
                                     65 ;	../../lib/uart.c: 14: void uart_write(uint8_t data) {
                                     66 ;	-----------------------------------------
                                     67 ;	 function uart_write
                                     68 ;	-----------------------------------------
      0080BB                         69 _uart_write:
                                     70 ;	../../lib/uart.c: 15: UART1_DR = data;
      0080BB AE 52 31         [ 2]   71 	ldw	x, #0x5231
      0080BE 7B 03            [ 1]   72 	ld	a, (0x03, sp)
      0080C0 F7               [ 1]   73 	ld	(x), a
                                     74 ;	../../lib/uart.c: 16: while (!(UART1_SR & (1 << UART1_SR_TC)));
      0080C1                         75 00101$:
      0080C1 C6 52 30         [ 1]   76 	ld	a, 0x5230
      0080C4 A5 40            [ 1]   77 	bcp	a, #0x40
      0080C6 27 F9            [ 1]   78 	jreq	00101$
                                     79 ;	../../lib/uart.c: 17: }
      0080C8 81               [ 4]   80 	ret
                                     81 ;	../../lib/uart.c: 19: uint8_t uart_read() {
                                     82 ;	-----------------------------------------
                                     83 ;	 function uart_read
                                     84 ;	-----------------------------------------
      0080C9                         85 _uart_read:
                                     86 ;	../../lib/uart.c: 20: while (!(UART1_SR & (1 << UART1_SR_RXNE)));
      0080C9                         87 00101$:
      0080C9 C6 52 30         [ 1]   88 	ld	a, 0x5230
      0080CC A5 20            [ 1]   89 	bcp	a, #0x20
      0080CE 27 F9            [ 1]   90 	jreq	00101$
                                     91 ;	../../lib/uart.c: 21: return UART1_DR;
      0080D0 C6 52 31         [ 1]   92 	ld	a, 0x5231
                                     93 ;	../../lib/uart.c: 22: }
      0080D3 81               [ 4]   94 	ret
                                     95 	.area CODE
                                     96 	.area CONST
                                     97 	.area INITIALIZER
                                     98 	.area CABS (ABS)
