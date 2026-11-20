#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "Timer.h" 

#define KEY_DEBOUNCE_MS 20U             // 消抖时间（毫秒）

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY_EMERGENCY_PIN | KEY_START_PIN | KEY_RESET_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if (Key_CheckEmergency())
	{
		KeyNum = 1;
	}
	else if (Key_CheckStart())
	{
		KeyNum = 2;
	}
	else if (Key_CheckReset())
	{
		KeyNum = 3;
	}
	
	return KeyNum;
}

// 按键状态结构体
typedef struct
{
	uint8_t state;       // 稳定状态：0-未按下，1-已按下（用于“单次触发”锁存）
	uint8_t timing;      // 是否正在计时：0-否，1-是
	uint32_t startMs;    // 按下后开始计时的时间戳（ms）
} KeyState;

// 按键状态变量
static KeyState emergencyKey = {0, 0};
static KeyState startKey = {0, 0};
static KeyState resetKey = {0, 0};

uint8_t Key_CheckEmergency(void)
{
	uint32_t now = Get_ms();

	// 读取到低电平：按键被按下
	if (GPIO_ReadInputDataBit(GPIOB, KEY_EMERGENCY_PIN) == 0)
	{
		// 只在“当前还未触发按下事件”时处理
		if (emergencyKey.state == 0)
		{
			// 第一次检测到按下，启动计时
			if (emergencyKey.timing == 0)
			{
				emergencyKey.timing = 1;
				emergencyKey.startMs = now;
			}
			// 按下持续达到消抖时间，判定为有效按下，触发一次
			else if ((uint32_t)(now - emergencyKey.startMs) >= KEY_DEBOUNCE_MS)
			{
				emergencyKey.state = 1;
				return 1;
			}
		}
	}
	else
	{
		// 松开后清状态：允许下一次按下再次触发
		emergencyKey.state = 0;
		emergencyKey.timing = 0;
		emergencyKey.startMs = 0;
	}

	return 0;
}

uint8_t Key_CheckStart(void)
{
	uint32_t now = Get_ms();

	if (GPIO_ReadInputDataBit(GPIOB, KEY_START_PIN) == 0)
	{
		if (startKey.state == 0)
		{
			if (startKey.timing == 0)
			{
				startKey.timing = 1;
				startKey.startMs = now;
			}
			else if ((uint32_t)(now - startKey.startMs) >= KEY_DEBOUNCE_MS)
			{
				startKey.state = 1;
				return 1;
			}
		}
	}
	else
	{
		startKey.state = 0;
		startKey.timing = 0;
		startKey.startMs = 0;
	}

	return 0;
}

uint8_t Key_CheckReset(void)
{
	uint32_t now = Get_ms();

	if (GPIO_ReadInputDataBit(GPIOB, KEY_RESET_PIN) == 0)
	{
		if (resetKey.state == 0)
		{
			if (resetKey.timing == 0)
			{
				resetKey.timing = 1;
				resetKey.startMs = now;
			}
			else if ((uint32_t)(now - resetKey.startMs) >= KEY_DEBOUNCE_MS)
			{
				resetKey.state = 1;
				return 1;
			}
		}
	}
	else
	{
		resetKey.state = 0;
		resetKey.timing = 0;
		resetKey.startMs = 0;
	}

	return 0;
}
