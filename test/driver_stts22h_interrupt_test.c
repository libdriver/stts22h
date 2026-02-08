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
 * @file      driver_stts22h_interrupt_test.c
 * @brief     driver stts22h interrupt test source file
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

#include "driver_stts22h_interrupt_test.h"

static stts22h_handle_t gs_handle;        /**< stts22h handle */
static volatile uint8_t gs_flag;          /**< local flag */

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
            int16_t raw;
            float celsius_deg = 0.0f;
            
            stts22h_interface_debug_print("stts22h: irq under low limit.\n");
            (void)stts22h_continuous_read(&gs_handle, &raw, &celsius_deg);
            stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
            gs_flag = 1;
            
            break;
        }
        case STTS22H_STATUS_OVER_HIGH_LIMIT :
        {
            int16_t raw;
            float celsius_deg = 0.0f;
            
            stts22h_interface_debug_print("stts22h: irq over high limit.\n");
            (void)stts22h_continuous_read(&gs_handle, &raw, &celsius_deg);
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
 * @brief  interrupt test irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t stts22h_interrupt_test_irq_handler(void)
{
    if (stts22h_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     interrupt test
 * @param[in] addr_pin address pin
 * @param[in] low_celsius_deg low threshold celsius degrees
 * @param[in] high_celsius_deg high threshold celsius degrees
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t stts22h_interrupt_test(stts22h_address_t addr_pin, float low_celsius_deg, float high_celsius_deg)
{
    uint8_t res;
    uint8_t reg;
    stts22h_info_t info;
    
    /* link interface function */
    DRIVER_STTS22H_LINK_INIT(&gs_handle, stts22h_handle_t);
    DRIVER_STTS22H_LINK_IIC_INIT(&gs_handle, stts22h_interface_iic_init);
    DRIVER_STTS22H_LINK_IIC_DEINIT(&gs_handle, stts22h_interface_iic_deinit);
    DRIVER_STTS22H_LINK_IIC_READ(&gs_handle, stts22h_interface_iic_read);
    DRIVER_STTS22H_LINK_IIC_WRITE(&gs_handle, stts22h_interface_iic_write);
    DRIVER_STTS22H_LINK_DELAY_MS(&gs_handle, stts22h_interface_delay_ms);
    DRIVER_STTS22H_LINK_DEBUG_PRINT(&gs_handle, stts22h_interface_debug_print);
    DRIVER_STTS22H_LINK_RECEIVE_CALLBACK(&gs_handle, a_receive_callback);
    
    /* get stts22h info */
    res = stts22h_info(&info);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get info failed.\n");
        
        return 1;
    }
    else
    {
        /* print chip information */
        stts22h_interface_debug_print("stts22h: chip is %s.\n", info.chip_name);
        stts22h_interface_debug_print("stts22h: manufacturer is %s.\n", info.manufacturer_name);
        stts22h_interface_debug_print("stts22h: interface is %s.\n", info.interface);
        stts22h_interface_debug_print("stts22h: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        stts22h_interface_debug_print("stts22h: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        stts22h_interface_debug_print("stts22h: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        stts22h_interface_debug_print("stts22h: max current is %0.2fmA.\n", info.max_current_ma);
        stts22h_interface_debug_print("stts22h: max temperature is %0.1fC.\n", info.temperature_max);
        stts22h_interface_debug_print("stts22h: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* set addr pin */
    res = stts22h_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set addr pin failed.\n");
       
        return 1;
    }
    
    /* stts22h init */
    res = stts22h_init(&gs_handle);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: init failed.\n");
       
        return 1;
    }
    
    /* enable smbus timeout */
    res = stts22h_set_disable_smbus_timeout(&gs_handle, STTS22H_BOOL_FALSE);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set disable smbus timeout failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable block data update */
    res = stts22h_set_block_data_update(&gs_handle, STTS22H_BOOL_FALSE);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set block data update failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable iic address auto increment */
    res = stts22h_set_iic_address_auto_increment(&gs_handle, STTS22H_BOOL_TRUE);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set iic address auto increment failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set output data rate 1hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_1HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* temperature convert to register */
    res = stts22h_temperature_convert_to_register(&gs_handle, low_celsius_deg, &reg);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: temperature convert to register failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set temperature low limit */
    res = stts22h_set_temperature_low_limit(&gs_handle, reg);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set temperature low limit failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* temperature convert to register */
    res = stts22h_temperature_convert_to_register(&gs_handle, high_celsius_deg, &reg);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: temperature convert to register failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set temperature high limit */
    res = stts22h_set_temperature_high_limit(&gs_handle, reg);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set temperature high limit failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait for interrupt */
    gs_flag = 0;
    while (gs_flag == 0)
    {
    }
    stts22h_interface_debug_print("stts22h: find interrupt.\n");
    
    /* finish interrupt test */
    stts22h_interface_debug_print("stts22h: finish interrupt test.\n");
    (void)stts22h_deinit(&gs_handle);
    
    return 0;
}
