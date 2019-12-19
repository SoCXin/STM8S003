#ifndef __LED_H
#define __LED_H

#include "iostm8s003f3.h"

#define LED0		PD_ODR_ODR3

void LED_Init(void);

#endif
