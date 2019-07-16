                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ANSI-C Compiler
                                      3 ; Version 3.9.1 #11292 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module spi
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _SPI_init
                                     12 	.globl _SPI_read
                                     13 	.globl _SPI_write
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
                                     51 ;	../../lib/spi.c: 4: void SPI_init() {
                                     52 ;	-----------------------------------------
                                     53 ;	 function SPI_init
                                     54 ;	-----------------------------------------
      0080FA                         55 _SPI_init:
                                     56 ;	../../lib/spi.c: 5: SPI_CR1 = (1 << SPI_CR1_MSTR) | (1 << SPI_CR1_SPE) | (1 << SPI_CR1_BR1);
      0080FA 35 54 52 00      [ 1]   57 	mov	0x5200+0, #0x54
                                     58 ;	../../lib/spi.c: 6: SPI_CR2 = (1 << SPI_CR2_SSM) | (1 << SPI_CR2_SSI) | (1 << SPI_CR2_BDM) | (1 << SPI_CR2_BDOE);
      0080FE 35 C3 52 01      [ 1]   59 	mov	0x5201+0, #0xc3
                                     60 ;	../../lib/spi.c: 7: }
      008102 81               [ 4]   61 	ret
                                     62 ;	../../lib/spi.c: 9: uint8_t SPI_read() {
                                     63 ;	-----------------------------------------
                                     64 ;	 function SPI_read
                                     65 ;	-----------------------------------------
      008103                         66 _SPI_read:
                                     67 ;	../../lib/spi.c: 10: SPI_write(0xFF);
      008103 4B FF            [ 1]   68 	push	#0xff
      008105 CD 81 13         [ 4]   69 	call	_SPI_write
      008108 84               [ 1]   70 	pop	a
                                     71 ;	../../lib/spi.c: 11: while (!(SPI_SR & (1 << SPI_SR_RXNE)));
      008109                         72 00101$:
      008109 C6 52 03         [ 1]   73 	ld	a, 0x5203
      00810C 44               [ 1]   74 	srl	a
      00810D 24 FA            [ 1]   75 	jrnc	00101$
                                     76 ;	../../lib/spi.c: 12: return SPI_DR;
      00810F C6 52 04         [ 1]   77 	ld	a, 0x5204
                                     78 ;	../../lib/spi.c: 13: }
      008112 81               [ 4]   79 	ret
                                     80 ;	../../lib/spi.c: 15: void SPI_write(uint8_t data) {
                                     81 ;	-----------------------------------------
                                     82 ;	 function SPI_write
                                     83 ;	-----------------------------------------
      008113                         84 _SPI_write:
                                     85 ;	../../lib/spi.c: 16: SPI_DR = data;
      008113 AE 52 04         [ 2]   86 	ldw	x, #0x5204
      008116 7B 03            [ 1]   87 	ld	a, (0x03, sp)
      008118 F7               [ 1]   88 	ld	(x), a
                                     89 ;	../../lib/spi.c: 17: while (!(SPI_SR & (1 << SPI_SR_TXE)));
      008119                         90 00101$:
      008119 C6 52 03         [ 1]   91 	ld	a, 0x5203
      00811C A5 02            [ 1]   92 	bcp	a, #0x02
      00811E 27 F9            [ 1]   93 	jreq	00101$
                                     94 ;	../../lib/spi.c: 18: }
      008120 81               [ 4]   95 	ret
                                     96 	.area CODE
                                     97 	.area CONST
                                     98 	.area INITIALIZER
                                     99 	.area CABS (ABS)
