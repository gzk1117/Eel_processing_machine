#include "stm32f10x.h"
#include "LED.h"
#include "Motor.h"
// #include "Relay.h"
#include "OLED.h"
#include "Global.h"

void Mode_Standby_Init(void)
{
    LED_All_OFF();
    Motor_SetSpeed(0);
    Motor_Enable();
    breatheEnabled = 1;
    OLED_ShowString(2,7,"         ");
    OLED_ShowString(2,7,"STANDBY");
}

void Mode_Standby_Loop(void)
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

void Mode_Standby_Exit(void)
{
}
