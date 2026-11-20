#include "stm32f10x.h"                  // Device header
#include "PWM2.h"
#include "Timer.h"
#include "Global.h"

// 故障检测引脚定义
#define FAULT_DETECT_PIN GPIO_Pin_11
#define FAULT_DETECT_PORT GPIOA

// STAY引脚定义（使用原继电器引脚PA7）
#define STAY_PIN GPIO_Pin_7
#define STAY_PORT GPIOA

static uint8_t motorFault = 0; // 电机故障标志
static uint32_t lastMotorCheckTime = 0; // 记录上次电机故障检测的时间

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 初始化故障检测引脚（PA11）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_InitStructure.GPIO_Pin = FAULT_DETECT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FAULT_DETECT_PORT, &GPIO_InitStructure);
	
	// 初始化STAY引脚（PA7，原继电器控制引脚）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = STAY_PIN;
	GPIO_Init(STAY_PORT, &GPIO_InitStructure);
	
	// 初始状态：启用TB6612（STAY高电平）
	GPIO_SetBits(STAY_PORT, STAY_PIN);
	
	PWM2_Init();
}

void Motor_SetSpeed(int16_t Speed)
{
	// 速度限幅，PWM周期为99，所以速度范围为0-99
	if (Speed > 99)
	{
		Speed = 99;
	}
	else if (Speed < -99)
	{
		Speed = -99;
	}
	
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		PWM2_SetCompare1(Speed);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		GPIO_SetBits(GPIOB, GPIO_Pin_13);
		PWM2_SetCompare1(-Speed);
	}
}

// 电机故障检测
void Motor_FaultCheck(void)
{
	// 这里实现基于硬件跳接的故障检测逻辑
	// 原理：模拟BLDC驱动器的ALM信号，故障时输出低电平
	static uint8_t faultCounter = 0;
	uint32_t now = Get_ms();
	
	// 每100ms检测一次
	if ((uint32_t)(now - lastMotorCheckTime) >= 100U)
	{
		lastMotorCheckTime = now;
		
		if (CurrMode == MODE_RUNNING)
		{
			// 读取故障检测引脚状态（PA11）
			// 平时为高电平，故障时为低电平
			if (GPIO_ReadInputDataBit(FAULT_DETECT_PORT, FAULT_DETECT_PIN) == 0)
			{
				// 检测到故障信号
				faultCounter++;
				if (faultCounter >= 3) // 连续3次检测到故障
				{
					motorFault = 1;
					faultCounter = 0;
				}
			}
			else
			{
				faultCounter = 0;
			}
		}
		else
		{
			faultCounter = 0;
		}
	}
}

/*
电机故障检测说明：

1. 硬件跳接检测逻辑：
   - 每100ms检测一次故障信号
   - 检测原理：使用PA11引脚作为故障检测输入，模拟BLDC驱动器的ALM信号
   - 平时为高电平，故障时为低电平
   - 连续3次检测到低电平才触发报警，防止误报

2. 故障模拟操作：
   - 用一根杜邦线，一头接PA11，另一头瞬间碰一下GND
   - 这样可以模拟BLDC驱动器发出的故障信号

3. 实际硬件实现：
   - 未来替换为真实的BLDC驱动器，其ALM引脚直接连接到PA11
   - 当BLDC驱动器检测到故障（如堵转、过流、过热等）时，会将ALM引脚拉低

4. 故障处理流程：
   - 检测到故障后，设置motorFault标志
   - main.c中的主循环检测到故障后，进入报警状态
   - 报警状态下，电机停止，对应故障的电机亮红灯
   - 需要人工排除故障后，再次按下启动键才能重新启动

5. 与继电器的配合：
   - 电机故障时，只停止电机，不切断总电源
   - 只有急停按钮才会切断总电源
   - 这样可以确保芯片在故障状态下仍然能够正常工作，显示故障信息
*/

// 获取电机故障状态
uint8_t Motor_GetFaultStatus(void)
{
	return motorFault;
}

// 重置电机故障状态
void Motor_ResetFaultStatus(void)
{
	motorFault = 0;
}

// 启用TB6612（STAY高电平）
void Motor_Enable(void)
{
	GPIO_SetBits(STAY_PORT, STAY_PIN);
}

// 禁用TB6612（STAY低电平）
void Motor_Disable(void)
{
	GPIO_ResetBits(STAY_PORT, STAY_PIN);
}
