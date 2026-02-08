[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver STTS22H

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/stts22h/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

STTS22H是一款超低功耗、高精度的數位溫度感測器，在整個工作溫度範圍內均能提供高性能。STTS22H是一款帶隙溫度感測器，集成了A/D轉換器、訊號處理邏輯以及I²C/SMBus 3.0介面，全部集成於單個專用積體電路（ASIC）中。該感測器採用小型2 x 2 x 0.50 mm 6引腳UDFN封裝，底部設有裸露焊盤，以更好地與周圍環境溫度匹配。STTS22H在出廠前已進行校準，客戶無需進行額外校準。

LibDriver STTS22H是LibDriver推出的STTS22H的全功能驅動，該驅動提供連續模式溫度讀取、單次模式溫度讀取等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver STTS22H的源文件。

/interface目錄包含了LibDriver STTS22H與平台無關的IIC總線模板。

/test目錄包含了LibDriver STTS22H驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver STTS22H編程範例。

/doc目錄包含了LibDriver STTS22H離線文檔。

/datasheet目錄包含了STTS22H數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/stts22h/index.html](https://www.libdriver.com/docs/stts22h/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。