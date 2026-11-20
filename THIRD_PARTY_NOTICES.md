# 第三方组件说明

本仓库保留原始电控工程随附的第三方文件，未修改其中的版权头与声明。

| 位置 | 组件与来源 |
| --- | --- |
| `firmware/stm32f103/Library/` | STMicroelectronics STM32F10x Standard Peripheral Library V3.5.0，MCD Application Team，2011 |
| `firmware/stm32f103/Start/core_cm3.c`、`core_cm3.h` | ARM Cortex-M3 CMSIS 核心接口，具体版权与使用条件见文件头 |
| `firmware/stm32f103/Start/stm32f10x.h`、`system_stm32f10x.*`、`startup_stm32f10x_*.s` | STM32F10x 设备定义、系统与启动文件，具体版权与声明见各文件 |
| `firmware/stm32f103/User/stm32f10x_conf.h`、`stm32f10x_it.*` | STM32 外设配置与中断模板及项目适配内容 |

上述组件保留各自的适用条件，项目展示与源码归档不改变其原有声明。本次归档没有为整个仓库额外指定新的开源许可证。
