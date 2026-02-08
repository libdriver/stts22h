[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver STTS22H

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/stts22h/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The STTS22H is an ultralow-power, high-accuracy, digital temperature sensor offering high performance over the entire operating temperature range. The STTS22H is a bandgap temperature sensor coupled with an A/D converter, signal processing logic, and an I²C/SMBus 3.0 interface all in a single ASIC. This sensor is housed in a small 2 x 2 x 0.50 mm 6-lead UDFN package with exposed pad down for a better temperature match with the surrounding environment. The STTS22H is factory calibrated and requires no additional calibration efforts on the customer side.

LibDriver STTS22H is a full-featured driver for STTS22H, launched by LibDriver.It provides continuous reading, single reading and additional features. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver STTS22H source files.

/interface includes LibDriver STTS22H IIC platform independent template.

/test includes LibDriver STTS22H driver test code and this code can test the chip necessary function simply.

/example includes LibDriver STTS22H sample code.

/doc includes LibDriver STTS22H offline document.

/datasheet includes STTS22H datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

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

### Document

Online documents: [https://www.libdriver.com/docs/stts22h/index.html](https://www.libdriver.com/docs/stts22h/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.