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
 * @file      driver_stts22h_read_test.c
 * @brief     driver stts22h read test source file
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

#include "driver_stts22h_read_test.h"

static stts22h_handle_t gs_handle;        /**< stts22h handle */

/**
 * @brief     read test
 * @param[in] addr_pin address pin
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t stts22h_read_test(stts22h_address_t addr_pin, uint32_t times)
{
    uint8_t res; 
    uint32_t i;
    stts22h_info_t info;
    
    /* link interface function */
    DRIVER_STTS22H_LINK_INIT(&gs_handle, stts22h_handle_t);
    DRIVER_STTS22H_LINK_IIC_INIT(&gs_handle, stts22h_interface_iic_init);
    DRIVER_STTS22H_LINK_IIC_DEINIT(&gs_handle, stts22h_interface_iic_deinit);
    DRIVER_STTS22H_LINK_IIC_READ(&gs_handle, stts22h_interface_iic_read);
    DRIVER_STTS22H_LINK_IIC_WRITE(&gs_handle, stts22h_interface_iic_write);
    DRIVER_STTS22H_LINK_DELAY_MS(&gs_handle, stts22h_interface_delay_ms);
    DRIVER_STTS22H_LINK_DEBUG_PRINT(&gs_handle, stts22h_interface_debug_print);
    DRIVER_STTS22H_LINK_RECEIVE_CALLBACK(&gs_handle, stts22h_interface_receive_callback);
    
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
    
    /* single read test */
    stts22h_interface_debug_print("stts22h: single read test.\n");
    
    /* set output data rate 1hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_1HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 1hz.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float celsius_deg;
        
        /* delay 1000ms */
        stts22h_interface_delay_ms(1000);
        
        /* read data */
        res = stts22h_single_read(&gs_handle, &raw, &celsius_deg);
        if (res != 0)
        {
            stts22h_interface_debug_print("stts22h: single read failed.\n");
            (void)stts22h_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
    }
    
    /* set output data rate 25hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_25HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 25hz.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float celsius_deg;
        
        /* delay 1000ms */
        stts22h_interface_delay_ms(1000);
        
        /* read data */
        res = stts22h_single_read(&gs_handle, &raw, &celsius_deg);
        if (res != 0)
        {
            stts22h_interface_debug_print("stts22h: single read failed.\n");
            (void)stts22h_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
    }
    
    /* set output data rate 50hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_50HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 50hz.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float celsius_deg;
        
        /* delay 1000ms */
        stts22h_interface_delay_ms(1000);
        
        /* read data */
        res = stts22h_single_read(&gs_handle, &raw, &celsius_deg);
        if (res != 0)
        {
            stts22h_interface_debug_print("stts22h: single read failed.\n");
            (void)stts22h_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
    }
    
    /* set output data rate 100hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_100HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 100hz.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float celsius_deg;
        
        /* delay 1000ms */
        stts22h_interface_delay_ms(1000);
        
        /* read data */
        res = stts22h_single_read(&gs_handle, &raw, &celsius_deg);
        if (res != 0)
        {
            stts22h_interface_debug_print("stts22h: single read failed.\n");
            (void)stts22h_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
    }
    
    /* set output data rate 200hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_200HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 200hz.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float celsius_deg;
        
        /* delay 1000ms */
        stts22h_interface_delay_ms(1000);
        
        /* read data */
        res = stts22h_single_read(&gs_handle, &raw, &celsius_deg);
        if (res != 0)
        {
            stts22h_interface_debug_print("stts22h: single read failed.\n");
            (void)stts22h_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
    }
    
    /* continuous read test */
    stts22h_interface_debug_print("stts22h: continuous read test.\n");
    
    /* start continuous read */
    res = stts22h_start_continuous_read(&gs_handle);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: start continuous read failed.\n");
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
    stts22h_interface_debug_print("stts22h: set output data rate 1hz.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float celsius_deg;
        
        /* delay 1000ms */
        stts22h_interface_delay_ms(1000);
        
        /* read data */
        res = stts22h_continuous_read(&gs_handle, &raw, &celsius_deg);
        if (res != 0)
        {
            stts22h_interface_debug_print("stts22h: single read failed.\n");
            (void)stts22h_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
    }
    
    /* set output data rate 25hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_25HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 25hz.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float celsius_deg;
        
        /* delay 1000ms */
        stts22h_interface_delay_ms(1000);
        
        /* read data */
        res = stts22h_continuous_read(&gs_handle, &raw, &celsius_deg);
        if (res != 0)
        {
            stts22h_interface_debug_print("stts22h: single read failed.\n");
            (void)stts22h_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
    }
    
    /* set output data rate 50hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_50HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 50hz.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float celsius_deg;
        
        /* delay 1000ms */
        stts22h_interface_delay_ms(1000);
        
        /* read data */
        res = stts22h_continuous_read(&gs_handle, &raw, &celsius_deg);
        if (res != 0)
        {
            stts22h_interface_debug_print("stts22h: single read failed.\n");
            (void)stts22h_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
    }
    
    /* set output data rate 100hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_100HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 100hz.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float celsius_deg;
        
        /* delay 1000ms */
        stts22h_interface_delay_ms(1000);
        
        /* read data */
        res = stts22h_continuous_read(&gs_handle, &raw, &celsius_deg);
        if (res != 0)
        {
            stts22h_interface_debug_print("stts22h: single read failed.\n");
            (void)stts22h_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
    }
    
    /* set output data rate 200hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_200HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 200hz.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float celsius_deg;
        
        /* delay 1000ms */
        stts22h_interface_delay_ms(1000);
        
        /* read data */
        res = stts22h_continuous_read(&gs_handle, &raw, &celsius_deg);
        if (res != 0)
        {
            stts22h_interface_debug_print("stts22h: single read failed.\n");
            (void)stts22h_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
    }
    
    /* disable iic address auto increment */
    res = stts22h_set_iic_address_auto_increment(&gs_handle, STTS22H_BOOL_FALSE);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set iic address auto increment failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: disable iic address auto increment.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float celsius_deg;
        
        /* delay 1000ms */
        stts22h_interface_delay_ms(1000);
        
        /* read data */
        res = stts22h_continuous_read(&gs_handle, &raw, &celsius_deg);
        if (res != 0)
        {
            stts22h_interface_debug_print("stts22h: single read failed.\n");
            (void)stts22h_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        stts22h_interface_debug_print("stts22h: temperature is %0.2fC.\n", celsius_deg);
    }
    
    /* stop continuous read */
    res = stts22h_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: stop continuous read failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish read test */
    stts22h_interface_debug_print("stts22h: finish read test.\n");
    (void)stts22h_deinit(&gs_handle);
    
    return 0;
}
