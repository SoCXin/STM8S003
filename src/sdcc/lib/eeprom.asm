;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.9.1 #11292 (Linux)
;--------------------------------------------------------
	.module eeprom
	.optsdcc -mstm8
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _eeprom_unlock
	.globl _option_bytes_unlock
	.globl _eeprom_lock
	.globl _eeprom_wait_busy
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
;	../../lib/eeprom.c: 3: void eeprom_unlock() {
;	-----------------------------------------
;	 function eeprom_unlock
;	-----------------------------------------
_eeprom_unlock:
;	../../lib/eeprom.c: 4: FLASH_DUKR = FLASH_DUKR_KEY1;
	mov	0x5064+0, #0xae
;	../../lib/eeprom.c: 5: FLASH_DUKR = FLASH_DUKR_KEY2;
	mov	0x5064+0, #0x56
;	../../lib/eeprom.c: 6: while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_DUL)));
00101$:
	ld	a, 0x505f
	bcp	a, #0x08
	jreq	00101$
;	../../lib/eeprom.c: 7: }
	ret
;	../../lib/eeprom.c: 9: void option_bytes_unlock() {
;	-----------------------------------------
;	 function option_bytes_unlock
;	-----------------------------------------
_option_bytes_unlock:
;	../../lib/eeprom.c: 10: FLASH_CR2 |= (1 << FLASH_CR2_OPT);
	bset	20571, #7
;	../../lib/eeprom.c: 11: FLASH_NCR2 &= ~(1 << FLASH_NCR2_NOPT);
	bres	20572, #7
;	../../lib/eeprom.c: 12: }
	ret
;	../../lib/eeprom.c: 14: void eeprom_lock() {
;	-----------------------------------------
;	 function eeprom_lock
;	-----------------------------------------
_eeprom_lock:
;	../../lib/eeprom.c: 15: FLASH_IAPSR &= ~(1 << FLASH_IAPSR_DUL);
	bres	20575, #3
;	../../lib/eeprom.c: 16: }
	ret
;	../../lib/eeprom.c: 18: void eeprom_wait_busy() {
;	-----------------------------------------
;	 function eeprom_wait_busy
;	-----------------------------------------
_eeprom_wait_busy:
;	../../lib/eeprom.c: 19: while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_EOP)));
00101$:
	ld	a, 0x505f
	bcp	a, #0x04
	jreq	00101$
;	../../lib/eeprom.c: 20: }
	ret
	.area CODE
	.area CONST
	.area INITIALIZER
	.area CABS (ABS)
