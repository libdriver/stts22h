[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver STTS22H

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/stts22h/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der STTS22H ist ein extrem stromsparender, hochpräziser digitaler Temperatursensor, der über den gesamten Betriebstemperaturbereich hinweg hohe Leistung bietet. Der STTS22H ist ein Bandgap-Temperatursensor mit integriertem A/D-Wandler, Signalverarbeitungslogik und I²C/SMBus 3.0-Schnittstelle – alles in einem einzigen ASIC. Der Sensor ist in einem kleinen 2 x 2 x 0,50 mm großen 6-poligen UDFN-Gehäuse untergebracht, dessen Kontaktfläche nach unten zeigt, um eine optimale Temperaturanpassung an die Umgebung zu gewährleisten. Der STTS22H ist werkseitig kalibriert und erfordert keine zusätzliche Kalibrierung seitens des Kunden.

LibDriver STTS22H ist ein umfassender Treiber für STTS22H, entwickelt von LibDriver. Er bietet kontinuierliches und einzelnes Auslesen sowie weitere Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver STTS22H-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver STTS22H IIC.

/test enthält den Testcode des LibDriver STTS22H-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver STTS22H-Beispielcode.

/doc enthält das LibDriver STTS22H-Offlinedokument.

/Datenblatt enthält STTS22H-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

```c
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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/stts22h/index.html](https://www.libdriver.com/docs/stts22h/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.