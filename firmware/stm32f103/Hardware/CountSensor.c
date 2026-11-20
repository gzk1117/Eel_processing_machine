#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "Global.h"

volatile uint16_t CountSensor_Count;
static uint32_t Last_Trigger_Time = 0;

void CountSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // 电平从 0 变 1 时触发
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t CountSensor_Get(void)
{
    uint16_t temp;
    uint32_t primask_bit;

    // 步骤1：备份当前的PRIMASK寄存器值（记录中断是否处于开启状态）
    primask_bit = __get_PRIMASK(); 
    // 步骤2：关闭全局中断（进入原子操作临界区）
    __disable_irq();               
    // 步骤3：读取计数值（此时中断无法修改，保证数据完整）
    temp = CountSensor_Count;
    // 步骤4：恢复之前的中断状态（而非直接开中断）
    __set_PRIMASK(primask_bit);    
    return temp;
}
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		uint32_t Current_Time = Get_ms(); 
		// 只有在运行状态下才计数
		if (CurrMode == MODE_RUNNING)
		{
			 if (Current_Time - Last_Trigger_Time > 20)
			{
                // 执行真正的计数
                CountSensor_Count++;
                
                // 更新时间戳，为下一次消抖做准备
                Last_Trigger_Time = Current_Time; 
            }
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
void CountSensor_Clear(void) // 带原子保护的清零函数
{
	uint32_t primask_bit = __get_PRIMASK();
	__disable_irq(); // 关闭中断，防止清零时被中断修改
	CountSensor_Count = 0;
	__set_PRIMASK(primask_bit); // 恢复之前的中断状态
}
