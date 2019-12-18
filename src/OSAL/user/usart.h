#ifndef __USART_H
#define __USART_H

#include <stdio.h>
#include <stdarg.h>
#include "type.h"

void Usart_Init(uint32 bound);
void Usart_Transmit(unsigned char data);
void Usart_Puts(char *string);
void Usart_Printf(char *format, ...);

#endif
