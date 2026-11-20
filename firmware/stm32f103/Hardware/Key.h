#ifndef __KEY_H
#define __KEY_H

// 按钮引脚定义
#define KEY_EMERGENCY_PIN GPIO_Pin_1    // 急停按钮
#define KEY_START_PIN     GPIO_Pin_8   // 启动/停止按钮
#define KEY_RESET_PIN     GPIO_Pin_9   // 计数重置按钮

void Key_Init(void);
uint8_t Key_GetNum(void);
uint8_t Key_CheckEmergency(void);
uint8_t Key_CheckStart(void);
uint8_t Key_CheckReset(void);

#endif
