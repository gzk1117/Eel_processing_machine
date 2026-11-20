#include "stm32f10x.h"
#include "LED.h"
#include "Motor.h"
// #include "Relay.h"
#include "OLED.h"
#include "Global.h"

void Mode_Emergency_Init(void)
{
    LED_All_OFF();
    Motor_SetSpeed(0);
    Motor_Disable();
    breatheEnabled = 0;
    OLED_ShowString(2,7,"         ");
    OLED_ShowString(2,7,"EMERGENCY");
}

void Mode_Emergency_Loop(void)
{
    if (KeyNum == 2)
    {
        NextMode = MODE_STANDBY;
    }
}

void Mode_Emergency_Exit(void)
{
    Motor_Enable();
	 KeyNum = 0;
}
