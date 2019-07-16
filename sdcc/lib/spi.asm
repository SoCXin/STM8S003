;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.9.1 #11292 (Linux)
;--------------------------------------------------------
	.module spi
	.optsdcc -mstm8
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _SPI_init
	.globl _SPI_read
	.globl _SPI_write
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area DABS (ABS)

; default segment ordering for linker
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area CONST
	.area INITIALIZER
	.area CODE

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area GSINIT
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;	../../lib/spi.c: 4: void SPI_init() {
;	-----------------------------------------
;	 function SPI_init
;	-----------------------------------------
_SPI_init:
;	../../lib/spi.c: 5: SPI_CR1 = (1 << SPI_CR1_MSTR) | (1 << SPI_CR1_SPE) | (1 << SPI_CR1_BR1);
	mov	0x5200+0, #0x54
;	../../lib/spi.c: 6: SPI_CR2 = (1 << SPI_CR2_SSM) | (1 << SPI_CR2_SSI) | (1 << SPI_CR2_BDM) | (1 << SPI_CR2_BDOE);
	mov	0x5201+0, #0xc3
;	../../lib/spi.c: 7: }
	ret
;	../../lib/spi.c: 9: uint8_t SPI_read() {
;	-----------------------------------------
;	 function SPI_read
;	-----------------------------------------
_SPI_read:
;	../../lib/spi.c: 10: SPI_write(0xFF);
	push	#0xff
	call	_SPI_write
	pop	a
;	../../lib/spi.c: 11: while (!(SPI_SR & (1 << SPI_SR_RXNE)));
00101$:
	ld	a, 0x5203
	srl	a
	jrnc	00101$
;	../../lib/spi.c: 12: return SPI_DR;
	ld	a, 0x5204
;	../../lib/spi.c: 13: }
	ret
;	../../lib/spi.c: 15: void SPI_write(uint8_t data) {
;	-----------------------------------------
;	 function SPI_write
;	-----------------------------------------
_SPI_write:
;	../../lib/spi.c: 16: SPI_DR = data;
	ldw	x, #0x5204
	ld	a, (0x03, sp)
	ld	(x), a
;	../../lib/spi.c: 17: while (!(SPI_SR & (1 << SPI_SR_TXE)));
00101$:
	ld	a, 0x5203
	bcp	a, #0x02
	jreq	00101$
;	../../lib/spi.c: 18: }
	ret
	.area CODE
	.area CONST
	.area INITIALIZER
	.area CABS (ABS)
