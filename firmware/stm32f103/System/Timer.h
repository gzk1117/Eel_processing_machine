#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"

/**
  * @brief  全局毫秒计数变量
  * @note   由 TIM2 每 1ms 触发一次中断进行自增
  *         加了 volatile 确保在 main 函数读取的是实时值
  */
extern volatile uint32_t msTicks;

/**
  * @brief  初始化定时器 TIM2 为 1ms 定时中断
  * @param  无
  * @retval 无
  */
void Timer_Init(void);

/**
  * @brief  获取自系统启动以来的总毫秒数
  * @param  无
  * @retval 当前的毫秒值
  */
uint32_t Get_ms(void);

#endif
