;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.9.1 #11292 (Linux)
;--------------------------------------------------------
	.module i2c
	.optsdcc -mstm8
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _i2c_init
	.globl _i2c_start
	.globl _i2c_stop
	.globl _i2c_write
	.globl _i2c_write_addr
	.globl _i2c_read
	.globl _i2c_read_arr
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
;	../../lib/i2c.c: 4: void i2c_init() {
;	-----------------------------------------
;	 function i2c_init
;	-----------------------------------------
_i2c_init:
;	../../lib/i2c.c: 5: I2C_FREQR = (1 << I2C_FREQR_FREQ1);
	mov	0x5212+0, #0x02
;	../../lib/i2c.c: 6: I2C_CCRL = 0x0A; // 100kHz
	mov	0x521b+0, #0x0a
;	../../lib/i2c.c: 7: I2C_OARH = (1 << I2C_OARH_ADDMODE); // 7-bit addressing
	mov	0x5214+0, #0x80
;	../../lib/i2c.c: 8: I2C_CR1 = (1 << I2C_CR1_PE);
	mov	0x5210+0, #0x01
;	../../lib/i2c.c: 9: }
	ret
;	../../lib/i2c.c: 11: void i2c_start() {
;	-----------------------------------------
;	 function i2c_start
;	-----------------------------------------
_i2c_start:
;	../../lib/i2c.c: 12: I2C_CR2 |= (1 << I2C_CR2_START);
	bset	21009, #0
;	../../lib/i2c.c: 13: while (!(I2C_SR1 & (1 << I2C_SR1_SB)));
00101$:
	ld	a, 0x5217
	srl	a
	jrnc	00101$
;	../../lib/i2c.c: 14: }
	ret
;	../../lib/i2c.c: 16: void i2c_stop() {
;	-----------------------------------------
;	 function i2c_stop
;	-----------------------------------------
_i2c_stop:
;	../../lib/i2c.c: 17: I2C_CR2 |= (1 << I2C_CR2_STOP);
	bset	21009, #1
;	../../lib/i2c.c: 18: while (I2C_SR3 & (1 << I2C_SR3_MSL));
00101$:
	ld	a, 0x5219
	srl	a
	jrc	00101$
;	../../lib/i2c.c: 19: }
	ret
;	../../lib/i2c.c: 21: void i2c_write(uint8_t data) {
;	-----------------------------------------
;	 function i2c_write
;	-----------------------------------------
_i2c_write:
;	../../lib/i2c.c: 22: I2C_DR = data;
	ldw	x, #0x5216
	ld	a, (0x03, sp)
	ld	(x), a
;	../../lib/i2c.c: 23: while (!(I2C_SR1 & (1 << I2C_SR1_TXE)));
00101$:
	ld	a, 0x5217
	jrpl	00101$
;	../../lib/i2c.c: 24: }
	ret
;	../../lib/i2c.c: 26: void i2c_write_addr(uint8_t addr) {
;	-----------------------------------------
;	 function i2c_write_addr
;	-----------------------------------------
_i2c_write_addr:
;	../../lib/i2c.c: 27: I2C_DR = addr;
	ldw	x, #0x5216
	ld	a, (0x03, sp)
	ld	(x), a
;	../../lib/i2c.c: 28: while (!(I2C_SR1 & (1 << I2C_SR1_ADDR)));
00101$:
	ld	a, 0x5217
	bcp	a, #0x02
	jreq	00101$
;	../../lib/i2c.c: 29: (void) I2C_SR3; // check BUS_BUSY
	ld	a, 0x5219
;	../../lib/i2c.c: 30: I2C_CR2 |= (1 << I2C_CR2_ACK);
	bset	21009, #2
;	../../lib/i2c.c: 31: }
	ret
;	../../lib/i2c.c: 33: uint8_t i2c_read() {
;	-----------------------------------------
;	 function i2c_read
;	-----------------------------------------
_i2c_read:
;	../../lib/i2c.c: 34: I2C_CR2 &= ~(1 << I2C_CR2_ACK);
	bres	21009, #2
;	../../lib/i2c.c: 35: i2c_stop();
	call	_i2c_stop
;	../../lib/i2c.c: 36: while (!(I2C_SR1 & (1 << I2C_SR1_RXNE)));
00101$:
	ld	a, 0x5217
	bcp	a, #0x40
	jreq	00101$
;	../../lib/i2c.c: 37: return I2C_DR;
	ld	a, 0x5216
;	../../lib/i2c.c: 38: }
	ret
;	../../lib/i2c.c: 40: void i2c_read_arr(uint8_t *buf, int len) {
;	-----------------------------------------
;	 function i2c_read_arr
;	-----------------------------------------
_i2c_read_arr:
;	../../lib/i2c.c: 41: while (len-- > 1) {
00104$:
	ldw	y, (0x05, sp)
	ldw	x, y
	decw	x
	ldw	(0x05, sp), x
;	../../lib/i2c.c: 44: *(buf++) = I2C_DR;
	ldw	x, (0x03, sp)
;	../../lib/i2c.c: 41: while (len-- > 1) {
	cpw	y, #0x0001
	jrsle	00106$
;	../../lib/i2c.c: 42: I2C_CR2 |= (1 << I2C_CR2_ACK);
	bset	21009, #2
;	../../lib/i2c.c: 43: while (!(I2C_SR1 & (1 << I2C_SR1_RXNE)));
00101$:
	ld	a, 0x5217
	bcp	a, #0x40
	jreq	00101$
;	../../lib/i2c.c: 44: *(buf++) = I2C_DR;
	ld	a, 0x5216
	ld	(x), a
	incw	x
	ldw	(0x03, sp), x
	jra	00104$
00106$:
;	../../lib/i2c.c: 46: *buf = i2c_read();
	pushw	x
	call	_i2c_read
	popw	x
	ld	(x), a
;	../../lib/i2c.c: 47: }
	ret
	.area CODE
	.area CONST
	.area INITIALIZER
	.area CABS (ABS)
