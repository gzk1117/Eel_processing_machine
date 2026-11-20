#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Motor.h"
#include "CountSensor.h"
#include "OLED.h"
#include "Timer.h"
#include "PWM.h"
// #include "Relay.h"
#include "Global.h"
#include "MODE_STANDBY.h"
#include "MODE_RUNNING.h"
#include "MODE_ALARM.h"
#include "MODE_EMERGENCY.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NextMode = MODE_STANDBY;

    LED_Init();
    PWM_Init();
    Key_Init();
    OLED_Init();
    OLED_Clear();
    Timer_Init();
    CountSensor_Init();
    Motor_Init();
    // Relay_Init();
    
    OLED_ShowString(1,1,"Count:");
    OLED_ShowString(2,1,"State:");
    
    Mode_Standby_Init();
    CurrMode = MODE_STANDBY;
    
    while (1)
    {
        Global_Loop();
        
        if (CurrMode != NextMode)
        {
            switch (CurrMode)
            {
                case MODE_STANDBY: Mode_Standby_Exit(); break;
                case MODE_RUNNING: Mode_Running_Exit(); break;
                case MODE_ALARM: Mode_Alarm_Exit(); break;
                case MODE_EMERGENCY: Mode_Emergency_Exit(); break;
            }

            switch (NextMode)
            {
                case MODE_STANDBY: Mode_Standby_Init(); break;
                case MODE_RUNNING: Mode_Running_Init(); break;
                case MODE_ALARM: Mode_Alarm_Init(); break;
                case MODE_EMERGENCY: Mode_Emergency_Init(); break;
            }

            CurrMode = NextMode;
			 KeyNum = 0;
        }
        else
        {
            switch (CurrMode)
            {
                case MODE_STANDBY: Mode_Standby_Loop(); break;
                case MODE_RUNNING: Mode_Running_Loop(); break;
                case MODE_ALARM: Mode_Alarm_Loop(); break;
                case MODE_EMERGENCY: Mode_Emergency_Loop(); break;
            }
        }
    }
}

