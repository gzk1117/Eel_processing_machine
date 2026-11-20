#ifndef __LED_H
#define __LED_H

void LED_Init(void);
void LED_ON(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void LED_OFF(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void LED_All_OFF(void);
void LED_Breathe(void);

#endif
