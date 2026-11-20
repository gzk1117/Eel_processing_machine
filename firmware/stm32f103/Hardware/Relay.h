#ifndef __RELAY_H
#define __RELAY_H

// 继电器控制引脚定义
#define RELAY_PIN GPIO_Pin_7
#define RELAY_PORT GPIOA

void Relay_Init(void);
void Relay_Control(uint8_t state);

#endif
