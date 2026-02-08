/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2026-02-02
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2026/02/02  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_stts22h_basic.h"
#include "driver_stts22h_shot.h"
#include "driver_stts22h_interrupt.h"
#include "driver_stts22h_register_test.h"
#include "driver_stts22h_read_test.h"
#include "driver_stts22h_interrupt_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "getopt.h"
#include "gpio.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */
volatile uint8_t gs_flag;                  /**< interrupt flag */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0)
    {
        if (g_gpio_irq != NULL)
        {
            g_gpio_irq();
        }
    }
}

/**
 * @brief     interface receive callback
 * @param[in] type irq type
 * @note      none
 */
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

/**
 * @brief     stts22h full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t stts22h(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"high", required_argument, NULL, 2},
        {"low", required_argument, NULL, 3},
        {"times", required_argument, NULL, 4},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    stts22h_address_t addr = STTS22H_ADDRESS_GND;
    float low = 20.0f;
    float high = 30.0f;
    uint32_t times = 3;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* addr */
            case 1 :
            {
                if (strcmp("vdd", optarg) == 0)
                {
                    addr = STTS22H_ADDRESS_VDD;
                }
                else if (strcmp("15k", optarg) == 0)
                {
                    addr = STTS22H_ADDRESS_15K;
                }
                else if (strcmp("56k", optarg) == 0)
                {
                    addr = STTS22H_ADDRESS_56K;
                }
                else if (strcmp("gnd", optarg) == 0)
                {
                    addr = STTS22H_ADDRESS_GND;
                }
                else
                {
                    return 5;
                }
                
                break;
            } 
            
            /* high */
            case 2 :
            {
                /* set the high threshold */
                high = (float)atof(optarg);
                
                break;
            } 
            
            /* low */
            case 3 :
            {
                /* set the low threshold */
                low = (float)atof(optarg);
                
                break;
            } 
            
            /* running times */
            case 4 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);
    
    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (stts22h_register_test(addr) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (stts22h_read_test(addr, times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_int", type) == 0)
    {
        uint8_t res;
        
        /* gpio interrupt init */
        g_gpio_irq = stts22h_interrupt_test_irq_handler;
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* run read test */
        if (stts22h_interrupt_test(addr, low, high) != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* gpio interrupt deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float celsius_deg;
        
        /* basic init */
        res = stts22h_basic_init(addr);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
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
            stts22h_interface_debug_print("%d/%d\n", (uint32_t)(i + 1), (uint32_t)times);
            stts22h_interface_debug_print("temperature is %0.2fC.\n", celsius_deg);
        }
        
        /* deinit */
        (void)stts22h_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_shot", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float celsius_deg;
        
        /* shot init */
        res = stts22h_shot_init(addr);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
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
            stts22h_interface_debug_print("%d/%d\n", (uint32_t)(i + 1), (uint32_t)times);
            stts22h_interface_debug_print("temperature is %0.2fC.\n", celsius_deg);
        }
        
        /* deinit */
        (void)stts22h_shot_deinit();
        
        return 0;
    }
    else if (strcmp("e_int", type) == 0)
    {
        uint8_t res;
        
        /* gpio interrupt init */
        g_gpio_irq = stts22h_interrupt_irq_handler;
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* interrupt init */
        res = stts22h_interrupt_init(addr, low, high, a_receive_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* wait for interrupt */
        gs_flag = 0;
        while (gs_flag == 0)
        {
        }
        stts22h_interface_debug_print("stts22h: find interrupt.\n");
        
        /* gpio interrupt deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        stts22h_interface_debug_print("Usage:\n");
        stts22h_interface_debug_print("  stts22h (-i | --information)\n");
        stts22h_interface_debug_print("  stts22h (-h | --help)\n");
        stts22h_interface_debug_print("  stts22h (-p | --port)\n");
        stts22h_interface_debug_print("  stts22h (-t reg | --test=reg) [--addr=<vdd | 15k | 56k | gnd>]\n");
        stts22h_interface_debug_print("  stts22h (-t read | --test=read) [--addr=<vdd | 15k | 56k | gnd>] [--times=<num>]\n");
        stts22h_interface_debug_print("  stts22h (-t int | --test=int) [--addr=<vdd | 15k | 56k | gnd>] [--low=<threshold>] [--high=<threshold>]\n");
        stts22h_interface_debug_print("  stts22h (-e read | --example=read) [--addr=<vdd | 15k | 56k | gnd>] [--times=<num>]\n");
        stts22h_interface_debug_print("  stts22h (-e shot | --example=shot) [--addr=<vdd | 15k | 56k | gnd>] [--times=<num>]\n");
        stts22h_interface_debug_print("  stts22h (-e int | --example=int) [--addr=<vdd | 15k | 56k | gnd>] [--low=<threshold>] [--high=<threshold>]\n");
        stts22h_interface_debug_print("\n");
        stts22h_interface_debug_print("Options:\n");
        stts22h_interface_debug_print("      --addr=<vdd | 15k | 56k | gnd>\n");
        stts22h_interface_debug_print("                                 Set the device address.([default: gnd])\n");
        stts22h_interface_debug_print("  -e <read | shot | int>, --example=<read | shot | int>\n");
        stts22h_interface_debug_print("                                 Run the driver example.\n");
        stts22h_interface_debug_print("  -h, --help                     Show the help.\n");
        stts22h_interface_debug_print("      --high=<threshold>         Set the interrupt high threshold.([default: 20])\n");
        stts22h_interface_debug_print("      --low=<threshold>          Set the interrupt low threshold.([default: 20])\n");
        stts22h_interface_debug_print("  -i, --information              Show the chip information.\n");
        stts22h_interface_debug_print("  -p, --port                     Display the pin connections of the current board.\n");
        stts22h_interface_debug_print("  -t <reg | read | int>, --test=<reg | read | int>\n");
        stts22h_interface_debug_print("                                 Run the driver test.\n");
        stts22h_interface_debug_print("      --times=<num>              Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        stts22h_info_t info;
        
        /* print stts22h info */
        stts22h_info(&info);
        stts22h_interface_debug_print("stts22h: chip is %s.\n", info.chip_name);
        stts22h_interface_debug_print("stts22h: manufacturer is %s.\n", info.manufacturer_name);
        stts22h_interface_debug_print("stts22h: interface is %s.\n", info.interface);
        stts22h_interface_debug_print("stts22h: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        stts22h_interface_debug_print("stts22h: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        stts22h_interface_debug_print("stts22h: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        stts22h_interface_debug_print("stts22h: max current is %0.2fmA.\n", info.max_current_ma);
        stts22h_interface_debug_print("stts22h: max temperature is %0.1fC.\n", info.temperature_max);
        stts22h_interface_debug_print("stts22h: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        stts22h_interface_debug_print("stts22h: SCL connected to GPIOB PIN8.\n");
        stts22h_interface_debug_print("stts22h: SDA connected to GPIOB PIN9.\n");
        stts22h_interface_debug_print("stts22h: INT connected to GPIOB PIN0.\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register stts22h function */
    shell_init();
    shell_register("stts22h", stts22h);
    uart_print("stts22h: welcome to libdriver stts22h.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("stts22h: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("stts22h: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("stts22h: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("stts22h: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("stts22h: param is invalid.\n");
            }
            else
            {
                uart_print("stts22h: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
