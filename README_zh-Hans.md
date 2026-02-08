[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver STTS22H

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/stts22h/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

STTS22H是一款超低功耗、高精度的数字温度传感器，在整个工作温度范围内均能提供高性能。STTS22H是一款带隙温度传感器，集成了A/D转换器、信号处理逻辑以及I²C/SMBus 3.0接口，全部集成于单个专用集成电路（ASIC）中。该传感器采用小型2 x 2 x 0.50 mm 6引脚UDFN封装，底部设有裸露焊盘，以更好地与周围环境温度匹配。STTS22H在出厂前已进行校准，客户无需进行额外校准。

LibDriver STTS22H是LibDriver推出的STTS22H的全功能驱动，该驱动提供连续模式温度读取、单次模式温度读取等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver STTS22H的源文件。

/interface目录包含了LibDriver STTS22H与平台无关的IIC总线模板。

/test目录包含了LibDriver STTS22H驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver STTS22H编程范例。

/doc目录包含了LibDriver STTS22H离线文档。

/datasheet目录包含了STTS22H数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

#### example basic

```C
#include "driver_stts22h_basic.h"

uint8_t res;
uint32_t i;
float celsius_deg;

/* basic init */
res = stts22h_basic_init(STTS22H_ADDRESS_GND);
if (res != 0)
{
    return 1;
}

...
    
/* loop */
for (i = 0; i < 3; i++)
{
    /* delay 1000ms */
    stts22h_interface_delay_ms(1000);

    /* read data */
    res = stts22h_basic_read(&celsius_deg);
    if (res != 0)
    {
        (void)stts22h_basic_deinit();

        return 1;
    }

    /* output */
    stts22h_interface_debug_print("%d/%d\n", (uint32_t)(i + 1), (uint32_t)3);
    stts22h_interface_debug_print("temperature is %0.2fC.\n", celsius_deg);
    
    ...
}

...
    
/* deinit */
(void)stts22h_basic_deinit();

return 0;
```

#### example shot

```C
#include "driver_stts22h_shot.h"

uint8_t res;
uint32_t i;
float celsius_deg;

/* shot init */
res = stts22h_shot_init(STTS22H_ADDRESS_GND);
if (res != 0)
{
    return 1;
}

...
    
/* loop */
for (i = 0; i < 3; i++)
{
    /* delay 1000ms */
    stts22h_interface_delay_ms(1000);

    /* read data */
    res = stts22h_shot_read(&celsius_deg);
    if (res != 0)
    {
        (void)stts22h_shot_deinit();

        return 1;
    }

    /* output */
    stts22h_interface_debug_print("%d/%d\n", (uint32_t)(i + 1), (uint32_t)3);
    stts22h_interface_debug_print("temperature is %0.2fC.\n", celsius_deg);
    
    ...
}

...
    
/* deinit */
(void)stts22h_shot_deinit();

return 0;
```

#### example interrupt

```C
#include "driver_stts22h_interrupt.h"

uint8_t res;
float low = 20.0f;
float high = 30.0f;
volatile uint8_t gs_flag;
uint8_t (*g_gpio_irq)(void) = NULL;

static void a_receive_callback(uint8_t type)
{
    switch (type)
    {
        case STTS22H_STATUS_UNDER_LOW_LIMIT :
        {
            float celsius_deg = 0.0f;
            
            stts22h_interface_debug_print("stts22h: irq under low limit.\n");
            (void)stts22h_interrupt_read(&celsius_deg);
            stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
            gs_flag = 1;
            
            break;
        }
        case STTS22H_STATUS_OVER_HIGH_LIMIT :
        {
            float celsius_deg = 0.0f;
            
            stts22h_interface_debug_print("stts22h: irq over high limit.\n");
            (void)stts22h_interrupt_read(&celsius_deg);
            stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
            gs_flag = 1;
            
            break;
        }
        default :
        {
            stts22h_interface_debug_print("stts22h: unknown code.\n");
            
            break;
        }
    }
}

/* gpio interrupt init */
g_gpio_irq = stts22h_interrupt_irq_handler;
res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}

...
    
/* interrupt init */
res = stts22h_interrupt_init(STTS22H_ADDRESS_GND, low, high, a_receive_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

...
    
/* wait for interrupt */
gs_flag = 0;
while (gs_flag == 0)
{
    ....
}
stts22h_interface_debug_print("stts22h: find interrupt.\n");

...
    
/* gpio interrupt deinit */
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/stts22h/index.html](https://www.libdriver.com/docs/stts22h/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。