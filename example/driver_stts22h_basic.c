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
 * @file      driver_stts22h_basic.c
 * @brief     driver stts22h basic source file
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

static stts22h_handle_t gs_handle;        /**< stts22h handle */

/**
 * @brief     basic example init
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t stts22h_basic_init(stts22h_address_t addr_pin)
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_STTS22H_LINK_INIT(&gs_handle, stts22h_handle_t);
    DRIVER_STTS22H_LINK_IIC_INIT(&gs_handle, stts22h_interface_iic_init);
    DRIVER_STTS22H_LINK_IIC_DEINIT(&gs_handle, stts22h_interface_iic_deinit);
    DRIVER_STTS22H_LINK_IIC_READ(&gs_handle, stts22h_interface_iic_read);
    DRIVER_STTS22H_LINK_IIC_WRITE(&gs_handle, stts22h_interface_iic_write);
    DRIVER_STTS22H_LINK_DELAY_MS(&gs_handle, stts22h_interface_delay_ms);
    DRIVER_STTS22H_LINK_DEBUG_PRINT(&gs_handle, stts22h_interface_debug_print);
    DRIVER_STTS22H_LINK_RECEIVE_CALLBACK(&gs_handle, stts22h_interface_receive_callback);
    
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
    
    /* set default smbus timeout */
    res = stts22h_set_disable_smbus_timeout(&gs_handle, STTS22H_BASIC_DEFAULT_DISABLE_SMBUS_TIMEOUT);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set disable smbus timeout failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default block data update */
    res = stts22h_set_block_data_update(&gs_handle, STTS22H_BASIC_DEFAULT_BLOCK_DATA_UPDATE);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set block data update failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default iic address auto increment */
    res = stts22h_set_iic_address_auto_increment(&gs_handle, STTS22H_BASIC_DEFAULT_IIC_ADDRESS_AUTO_INCREMENT);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set iic address auto increment failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default output data rate */
    res = stts22h_set_output_data_rate(&gs_handle, STTS22H_BASIC_DEFAULT_OUTPUT_DATA_RATE);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: set output data rate failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start continuous read */
    res = stts22h_start_continuous_read(&gs_handle);
    if (res != 0)
    {
        stts22h_interface_debug_print("stts22h: start continuous read failed.\n");
        (void)stts22h_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *celsius_deg pointer to a celsius degrees buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t stts22h_basic_read(float *celsius_deg)
{
    int16_t raw;
    
    /* read data */
    if (stts22h_continuous_read(&gs_handle, &raw, celsius_deg) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t stts22h_basic_deinit(void)
{
    uint8_t res;
    
    /* stop continuous read*/
    res = stts22h_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    /* close stts22h */
    if (stts22h_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}
