#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "CountSensor.h"
#include "OLED.h"
#include "Timer.h"

// 全局变量定义
uint8_t KeyNum;
uint32_t lastOLEDTime = 0;
uint8_t breatheEnabled = 1;
volatile uint8_t CurrMode = 0;
volatile uint8_t NextMode = 0;

// 全局循环函数
void Global_Loop(void)
{
    // 读取按键
    KeyNum = Key_GetNum();
    
    // 优化 OLED 刷新频率，每100ms刷新一次
    uint32_t now = Get_ms();
    if ((uint32_t)(now - lastOLEDTime) >= 100U)
    {
        lastOLEDTime = now;
        OLED_ShowNum(1,7,CountSensor_Get(),5);
    }
    
    // 按下计数清零按钮，计数归零
    if (KeyNum == 3)
    {
        CountSensor_Clear();  // 硬件层面清除计数
    }
    
    // 呼吸灯功能，只有在非急停状态下运行
    if (breatheEnabled)
    {
        LED_Breathe();
    }
    else
    {
        Delay_ms(10); // 如果系统没启动，给个小的 Delay 防止跑太快浪费功耗
    }
}

// 全局定时函数
void Global_Tick(void)
{
    // 这里可以添加全局定时任务
}
