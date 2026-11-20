#include "stm32f10x.h"
#include "LED.h"
#include "Motor.h"
#include "OLED.h"
#include "Global.h"

void Mode_Alarm_Init(void)
{
    LED_ON(GPIOA, GPIO_Pin_1);
    LED_OFF(GPIOA, GPIO_Pin_0);
    Motor_SetSpeed(0);
    OLED_ShowString(2,7,"         ");
    OLED_ShowString(2,7,"ALARM");
}

void Mode_Alarm_Loop(void)
{
    if (KeyNum == 2)
    {
        NextMode = MODE_RUNNING;
    }
    if (KeyNum == 1)
    {
        NextMode = MODE_EMERGENCY;
    }
}

void Mode_Alarm_Exit(void)
{
}
