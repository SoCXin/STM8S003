                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ANSI-C Compiler
                                      3 ; Version 3.9.1 #11292 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module eeprom
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _eeprom_unlock
                                     12 	.globl _option_bytes_unlock
                                     13 	.globl _eeprom_lock
                                     14 	.globl _eeprom_wait_busy
                                     15 ;--------------------------------------------------------
                                     16 ; ram data
                                     17 ;--------------------------------------------------------
                                     18 	.area DATA
                                     19 ;--------------------------------------------------------
                                     20 ; ram data
                                     21 ;--------------------------------------------------------
                                     22 	.area INITIALIZED
                                     23 ;--------------------------------------------------------
                                     24 ; absolute external ram data
                                     25 ;--------------------------------------------------------
                                     26 	.area DABS (ABS)
                                     27 
                                     28 ; default segment ordering for linker
                                     29 	.area HOME
                                     30 	.area GSINIT
                                     31 	.area GSFINAL
                                     32 	.area CONST
                                     33 	.area INITIALIZER
                                     34 	.area CODE
                                     35 
                                     36 ;--------------------------------------------------------
                                     37 ; global & static initialisations
                                     38 ;--------------------------------------------------------
                                     39 	.area HOME
                                     40 	.area GSINIT
                                     41 	.area GSFINAL
                                     42 	.area GSINIT
                                     43 ;--------------------------------------------------------
                                     44 ; Home
                                     45 ;--------------------------------------------------------
                                     46 	.area HOME
                                     47 	.area HOME
                                     48 ;--------------------------------------------------------
                                     49 ; code
                                     50 ;--------------------------------------------------------
                                     51 	.area CODE
                                     52 ;	../../lib/eeprom.c: 3: void eeprom_unlock() {
                                     53 ;	-----------------------------------------
                                     54 ;	 function eeprom_unlock
                                     55 ;	-----------------------------------------
      0080D4                         56 _eeprom_unlock:
                                     57 ;	../../lib/eeprom.c: 4: FLASH_DUKR = FLASH_DUKR_KEY1;
      0080D4 35 AE 50 64      [ 1]   58 	mov	0x5064+0, #0xae
                                     59 ;	../../lib/eeprom.c: 5: FLASH_DUKR = FLASH_DUKR_KEY2;
      0080D8 35 56 50 64      [ 1]   60 	mov	0x5064+0, #0x56
                                     61 ;	../../lib/eeprom.c: 6: while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_DUL)));
      0080DC                         62 00101$:
      0080DC C6 50 5F         [ 1]   63 	ld	a, 0x505f
      0080DF A5 08            [ 1]   64 	bcp	a, #0x08
      0080E1 27 F9            [ 1]   65 	jreq	00101$
                                     66 ;	../../lib/eeprom.c: 7: }
      0080E3 81               [ 4]   67 	ret
                                     68 ;	../../lib/eeprom.c: 9: void option_bytes_unlock() {
                                     69 ;	-----------------------------------------
                                     70 ;	 function option_bytes_unlock
                                     71 ;	-----------------------------------------
      0080E4                         72 _option_bytes_unlock:
                                     73 ;	../../lib/eeprom.c: 10: FLASH_CR2 |= (1 << FLASH_CR2_OPT);
      0080E4 72 1E 50 5B      [ 1]   74 	bset	20571, #7
                                     75 ;	../../lib/eeprom.c: 11: FLASH_NCR2 &= ~(1 << FLASH_NCR2_NOPT);
      0080E8 72 1F 50 5C      [ 1]   76 	bres	20572, #7
                                     77 ;	../../lib/eeprom.c: 12: }
      0080EC 81               [ 4]   78 	ret
                                     79 ;	../../lib/eeprom.c: 14: void eeprom_lock() {
                                     80 ;	-----------------------------------------
                                     81 ;	 function eeprom_lock
                                     82 ;	-----------------------------------------
      0080ED                         83 _eeprom_lock:
                                     84 ;	../../lib/eeprom.c: 15: FLASH_IAPSR &= ~(1 << FLASH_IAPSR_DUL);
      0080ED 72 17 50 5F      [ 1]   85 	bres	20575, #3
                                     86 ;	../../lib/eeprom.c: 16: }
      0080F1 81               [ 4]   87 	ret
                                     88 ;	../../lib/eeprom.c: 18: void eeprom_wait_busy() {
                                     89 ;	-----------------------------------------
                                     90 ;	 function eeprom_wait_busy
                                     91 ;	-----------------------------------------
      0080F2                         92 _eeprom_wait_busy:
                                     93 ;	../../lib/eeprom.c: 19: while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_EOP)));
      0080F2                         94 00101$:
      0080F2 C6 50 5F         [ 1]   95 	ld	a, 0x505f
      0080F5 A5 04            [ 1]   96 	bcp	a, #0x04
      0080F7 27 F9            [ 1]   97 	jreq	00101$
                                     98 ;	../../lib/eeprom.c: 20: }
      0080F9 81               [ 4]   99 	ret
                                    100 	.area CODE
                                    101 	.area CONST
                                    102 	.area INITIALIZER
                                    103 	.area CABS (ABS)
