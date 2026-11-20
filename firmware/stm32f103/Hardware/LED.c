#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Timer.h"

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; // 0绿灯,1红灯
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);// 开始都处于熄灭状态
}

void LED_ON(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_ResetBits(GPIOx, GPIO_Pin);
}

void LED_OFF(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_SetBits(GPIOx, GPIO_Pin);
}

void LED_All_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
	PWM_SetCompare1(0); // 关闭蓝色LED的PWM输出
}

// 呼吸灯状态变量
static uint8_t breatheDir = 1;
static uint8_t breatheBrightness = 0;
static uint32_t lastBreatheTime = 0;

void LED_Breathe(void)
{
	uint32_t now = Get_ms();  // 只读取一次当前时间
	// 非阻塞式呼吸灯
	if ((uint32_t)(now - lastBreatheTime) >= 10U)
	{
		lastBreatheTime = now;

		if (breatheDir)
		{
			breatheBrightness++;
			if (breatheBrightness >= 99U)   // 与 PWM ARR=99 对齐
			{
				breatheDir = 0;
			}
		}
		else
		{
			if (breatheBrightness > 0U)
			{
				breatheBrightness--;
			}
			if (breatheBrightness == 0U)
			{
				breatheDir = 1;
			}
		}

		PWM_SetCompare1(breatheBrightness);
	}
}
