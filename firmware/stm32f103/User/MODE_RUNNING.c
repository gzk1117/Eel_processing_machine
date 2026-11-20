#include "stm32f10x.h"
#include "LED.h"
#include "Motor.h"
// #include "Relay.h"
#include "OLED.h"
#include "Global.h"

void Mode_Running_Init(void)
{
    LED_ON(GPIOA, GPIO_Pin_0);
    LED_OFF(GPIOA, GPIO_Pin_1);
    Motor_SetSpeed(99);
    Motor_Enable();
    Motor_ResetFaultStatus();
    breatheEnabled = 1;
    OLED_ShowString(2,7,"         ");
    OLED_ShowString(2,7,"RUNNING");
}

void Mode_Running_Loop(void)
{
    if (KeyNum == 1)
    {
        NextMode = MODE_EMERGENCY;
        return;
    }

    if (KeyNum == 2)
    {
        NextMode = MODE_STANDBY;
        return;
    }

    Motor_FaultCheck();
    
    if (Motor_GetFaultStatus())
    {
        NextMode = MODE_ALARM;
    }
}

void Mode_Running_Exit(void)
{
    Motor_SetSpeed(0);
}
