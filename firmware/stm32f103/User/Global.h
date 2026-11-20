#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "stm32f10x.h"

typedef enum
{
    MODE_STANDBY = 1,
    MODE_RUNNING,
    MODE_ALARM,
    MODE_EMERGENCY
} SystemMode;

typedef SystemMode SystemState;

#define STATE_STANDBY MODE_STANDBY
#define STATE_RUNNING MODE_RUNNING
#define STATE_ALARM MODE_ALARM
#define STATE_EMERGENCY MODE_EMERGENCY
#define currentState CurrMode

extern volatile uint8_t CurrMode;
extern volatile uint8_t NextMode;
extern uint8_t KeyNum;
extern uint32_t lastOLEDTime;
extern uint8_t breatheEnabled;

void Global_Loop(void);
void Global_Tick(void);

#endif
