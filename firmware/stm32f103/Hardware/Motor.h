#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_SetSpeed(int16_t Speed);
void Motor_FaultCheck(void);
uint8_t Motor_GetFaultStatus(void);
void Motor_ResetFaultStatus(void);
void Motor_Enable(void);
void Motor_Disable(void);

#endif
