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
 * @file      driver_stts22h_register_test.c
 * @brief     driver stts22h register test source file
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

#include "driver_stts22h_register_test.h"
#include <stdlib.h>

static stts22h_handle_t gs_handle;        /**< stts22h handle */

/**
 * @brief     register test
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t stts22h_register_test(stts22h_address_t addr_pin)
{
    uint8_t res;
    uint8_t reg;
    uint8_t reg_check;
    uint8_t status;
    float celsius_deg;
    float celsius_deg_check;
    stts22h_address_t addr_pin_check;
    stts22h_info_t info;
    stts22h_output_data_rate_t rate;
    stts22h_bool_t enable;
    
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
    
    /* start register test */
    stts22h_interface_debug_print("stts22h: start register test.\n");
    
    /* stts22h_set_addr_pin/stts22h_get_addr_pin test */
    stts22h_interface_debug_print("stts22h: stts22h_set_addr_pin/stts22h_get_addr_pin test.\n");
    
    /* set addr pin vdd */
    res = stts22h_set_addr_pin(&gs_handle, STTS22H_ADDRESS_VDD);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set addr pin failed.\n");
       
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set addr pin vdd.\n");
    res = stts22h_get_addr_pin(&gs_handle, &addr_pin_check);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get addr pin failed.\n");
       
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check addr pin %s.\n", (addr_pin_check == STTS22H_ADDRESS_VDD) ? "ok" : "error");
    
    /* set addr pin 15k */
    res = stts22h_set_addr_pin(&gs_handle, STTS22H_ADDRESS_15K);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set addr pin failed.\n");
       
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set addr pin 15k.\n");
    res = stts22h_get_addr_pin(&gs_handle, &addr_pin_check);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get addr pin failed.\n");
       
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check addr pin %s.\n", (addr_pin_check == STTS22H_ADDRESS_15K) ? "ok" : "error");
    
    /* set addr pin 56k */
    res = stts22h_set_addr_pin(&gs_handle, STTS22H_ADDRESS_56K);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set addr pin failed.\n");
       
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set addr pin 56k.\n");
    res = stts22h_get_addr_pin(&gs_handle, &addr_pin_check);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get addr pin failed.\n");
       
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check addr pin %s.\n", (addr_pin_check == STTS22H_ADDRESS_56K) ? "ok" : "error");
    
    /* set addr pin gnd */
    res = stts22h_set_addr_pin(&gs_handle, STTS22H_ADDRESS_GND);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set addr pin failed.\n");
       
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set addr pin gnd.\n");
    res = stts22h_get_addr_pin(&gs_handle, &addr_pin_check);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get addr pin failed.\n");
       
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check addr pin %s.\n", (addr_pin_check == STTS22H_ADDRESS_GND) ? "ok" : "error");
    
    /* set addr pin */
    res = stts22h_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set addr pin failed.\n");
       
        return 1;
    }
    
    /* init */
    res = stts22h_init(&gs_handle);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: init failed.\n");
       
        return 1;
    }
    
    /* stts22h_set_temperature_high_limit/stts22h_get_temperature_high_limit test */
    stts22h_interface_debug_print("stts22h: stts22h_set_temperature_high_limit/stts22h_get_temperature_high_limit test.\n");
    
    reg = rand() % 0xFF;
    res = stts22h_set_temperature_high_limit(&gs_handle, reg);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set temperature high limit failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set temperature high limit 0x%02X.\n", reg);
    res = stts22h_get_temperature_high_limit(&gs_handle, &reg_check);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get temperature high limit failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check temperature high limit %s.\n", (reg == reg_check) ? "ok" : "error");
    
    /* stts22h_set_temperature_low_limit/stts22h_get_temperature_low_limit test */
    stts22h_interface_debug_print("stts22h: stts22h_set_temperature_low_limit/stts22h_get_temperature_low_limit test.\n");
    
    reg = rand() % 0xFF;
    res = stts22h_set_temperature_low_limit(&gs_handle, reg);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set temperature low limit failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set temperature low limit 0x%02X.\n", reg);
    res = stts22h_get_temperature_low_limit(&gs_handle, &reg_check);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get temperature low limit failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check temperature low limit %s.\n", (reg == reg_check) ? "ok" : "error");
    
    /* stts22h_set_output_data_rate/stts22h_get_output_data_rate test */
    stts22h_interface_debug_print("stts22h: stts22h_set_output_data_rate/stts22h_get_output_data_rate test.\n");
    
    /* set output data rate 25hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_25HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 25hz.\n");
    res = stts22h_get_output_data_rate(&gs_handle, &rate);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check output data rate %s.\n", (rate == STTS22H_OUTPUT_DATA_RATE_25HZ) ? "ok" : "error");
    
    /* set output data rate 50hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_50HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 50hz.\n");
    res = stts22h_get_output_data_rate(&gs_handle, &rate);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check output data rate %s.\n", (rate == STTS22H_OUTPUT_DATA_RATE_50HZ) ? "ok" : "error");
    
    /* set output data rate 100hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_100HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 100hz.\n");
    res = stts22h_get_output_data_rate(&gs_handle, &rate);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check output data rate %s.\n", (rate == STTS22H_OUTPUT_DATA_RATE_100HZ) ? "ok" : "error");
    
    /* set output data rate 200hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_200HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 200hz.\n");
    res = stts22h_get_output_data_rate(&gs_handle, &rate);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check output data rate %s.\n", (rate == STTS22H_OUTPUT_DATA_RATE_200HZ) ? "ok" : "error");
    
    /* set output data rate 1hz */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_OUTPUT_DATA_RATE_1HZ);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set output data rate 1hz.\n");
    res = stts22h_get_output_data_rate(&gs_handle, &rate);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check output data rate %s.\n", (rate == STTS22H_OUTPUT_DATA_RATE_1HZ) ? "ok" : "error");
    
    /* stts22h_set_block_data_update/stts22h_get_block_data_update test */
    stts22h_interface_debug_print("stts22h: stts22h_set_block_data_update/stts22h_get_block_data_update test.\n");
    
    /* enable block data update */
    res = stts22h_set_block_data_update(&gs_handle, STTS22H_BOOL_TRUE);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set block data update failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: enable block data update.\n");
    res = stts22h_get_block_data_update(&gs_handle, &enable);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get block data update failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check block data update %s.\n", (enable == STTS22H_BOOL_TRUE) ? "ok" : "error");
    
    /* disable block data update */
    res = stts22h_set_block_data_update(&gs_handle, STTS22H_BOOL_FALSE);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set block data update failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: disable block data update.\n");
    res = stts22h_get_block_data_update(&gs_handle, &enable);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get block data update failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check block data update %s.\n", (enable == STTS22H_BOOL_FALSE) ? "ok" : "error");
    
    /* stts22h_set_iic_address_auto_increment/stts22h_get_iic_address_auto_increment test */
    stts22h_interface_debug_print("stts22h: stts22h_set_iic_address_auto_increment/stts22h_get_iic_address_auto_increment test.\n");
    
    /* enable iic address auto increment */
    res = stts22h_set_iic_address_auto_increment(&gs_handle, STTS22H_BOOL_TRUE);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set iic address auto increment failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: enable iic address auto increment.\n");
    res = stts22h_get_iic_address_auto_increment(&gs_handle, &enable);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get iic address auto increment failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check iic address auto increment %s.\n", (enable == STTS22H_BOOL_TRUE) ? "ok" : "error");
    
    /* disable iic address auto increment */
    res = stts22h_set_iic_address_auto_increment(&gs_handle, STTS22H_BOOL_FALSE);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set iic address auto increment failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: disable iic address auto increment.\n");
    res = stts22h_get_iic_address_auto_increment(&gs_handle, &enable);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get iic address auto increment failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check iic address auto increment %s.\n", (enable == STTS22H_BOOL_FALSE) ? "ok" : "error");
    
    /* stts22h_set_disable_smbus_timeout/stts22h_get_disable_smbus_timeout test */
    stts22h_interface_debug_print("stts22h: stts22h_set_disable_smbus_timeout/stts22h_get_disable_smbus_timeout test.\n");
    
    /* enable disable smbus timeout */
    res = stts22h_set_disable_smbus_timeout(&gs_handle, STTS22H_BOOL_TRUE);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set disable smbus timeout failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: enable disable smbus timeout.\n");
    res = stts22h_get_disable_smbus_timeout(&gs_handle, &enable);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get disable smbus timeout failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check enable disable smbus timeout %s.\n", (enable == STTS22H_BOOL_TRUE) ? "ok" : "error");
    
    /* disable disable smbus timeout */
    res = stts22h_set_disable_smbus_timeout(&gs_handle, STTS22H_BOOL_FALSE);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set disable smbus timeout failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: disable disable smbus timeout.\n");
    res = stts22h_get_disable_smbus_timeout(&gs_handle, &enable);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get disable smbus timeout failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check enable disable smbus timeout %s.\n", (enable == STTS22H_BOOL_FALSE) ? "ok" : "error");
    
    /* stts22h_get_status test */
    stts22h_interface_debug_print("stts22h: stts22h_get_status test.\n");
    
    /* get status */
    res = stts22h_get_status(&gs_handle, &status);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: get status failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: status is 0x%02X.\n", status);
    
    /* stts22h_temperature_convert_to_register/stts22h_temperature_convert_to_data test */
    stts22h_interface_debug_print("stts22h: stts22h_temperature_convert_to_register/stts22h_temperature_convert_to_data test.\n");
    
    celsius_deg = (float)(rand() % 1000) / 10.0f;
    res = stts22h_temperature_convert_to_register(&gs_handle, celsius_deg, &reg);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: temperature convert to register failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: set celsius degrees %0.2fC.\n", celsius_deg);
    res = stts22h_temperature_convert_to_data(&gs_handle, reg, &celsius_deg_check);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: temperature convert to data failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    stts22h_interface_debug_print("stts22h: check celsius degrees %0.2fC.\n", celsius_deg_check);
    
    /* finish register test */
    stts22h_interface_debug_print("stts22h: finish register test.\n");
    (void)stts22h_deinit(&gs_handle);
    
    return 0;
}
