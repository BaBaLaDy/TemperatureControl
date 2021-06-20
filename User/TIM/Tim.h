#ifndef __Tim_H
#define __Tim_H

#include "stm32f10x.h"

extern float Water;

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);

#endif