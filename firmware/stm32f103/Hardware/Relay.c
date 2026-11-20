#include "stm32f10x.h"                  // Device header
#include "Relay.h"

// 继电器初始化
void Relay_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = RELAY_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RELAY_PORT, &GPIO_InitStructure);
	
	// 初始状态：继电器闭合（电源开启）
	GPIO_SetBits(RELAY_PORT, RELAY_PIN);
}

// 继电器控制
void Relay_Control(uint8_t state)
{
	if (state)
	{
		// 闭合继电器（电源开启）
		GPIO_SetBits(RELAY_PORT, RELAY_PIN);
	}
	else
	{
		// 断开继电器（电源关闭）
		GPIO_ResetBits(RELAY_PORT, RELAY_PIN);
	}
}

/*
继电器硬件连接说明：

1. 控制端（低压侧）：
   - VCC：接STM32开发板的5V或3.3V
   - GND：接STM32的GND（共地）
   - IN（信号线）：接STM32的GPIOA7引脚

2. 输出端（高压侧）：
   - COM（公共端）：接入动力电源的正极
   - NO（常开端）：接电机的正极
   - NC（常闭端）：暂时不接

3. 急停按钮连接：
   - 急停按钮应串联在继电器的VCC线路上
   - 正常时：按钮导通，继电器有电，受STM32控制
   - 急停时：按钮断开，继电器瞬间没电，自动弹开，切断动力电源
   - 同时，按钮的另一组信号接给STM32，使其进入紧急状态并报警

4. 电源分路：
   - 控制电：12V电池 → 降压模块（5V） → STM32的5V引脚
   - 动力电：12V电池正极 → 继电器COM口 → 继电器NO口 → 电机驱动模块 → 电机
*/
