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
 * @file      driver_stts22h_interrupt.h
 * @brief     driver stts22h interrupt header file
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

#ifndef DRIVER_STTS22H_INTERRUPT_H
#define DRIVER_STTS22H_INTERRUPT_H

#include "driver_stts22h_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup stts22h_example_driver stts22h example driver function
 * @brief    stts22h example driver modules
 * @ingroup  stts22h_driver
 * @{
 */

/**
 * @brief stts22h interrupt example default definition
 */
#define STTS22H_INTERRUPT_DEFAULT_DISABLE_SMBUS_TIMEOUT             STTS22H_BOOL_FALSE                   /**< disable */
#define STTS22H_INTERRUPT_DEFAULT_BLOCK_DATA_UPDATE                 STTS22H_BOOL_FALSE                   /**< disable block data update */
#define STTS22H_INTERRUPT_DEFAULT_IIC_ADDRESS_AUTO_INCREMENT        STTS22H_BOOL_TRUE                    /**< enable */
#define STTS22H_INTERRUPT_DEFAULT_OUTPUT_DATA_RATE                  STTS22H_OUTPUT_DATA_RATE_50HZ        /**< 50hz */

/**
 * @brief  interrupt irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t stts22h_interrupt_irq_handler(void);

/**
 * @brief     interrupt example init
 * @param[in] addr_pin address pin
 * @param[in] low_celsius_deg low threshold celsius degrees
 * @param[in] high_celsius_deg high threshold celsius degrees
 * @param[in] *callback pointer to a callback address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t stts22h_interrupt_init(stts22h_address_t addr_pin, float low_celsius_deg, 
                               float high_celsius_deg, void (*callback)(uint8_t type));

/**
 * @brief  interrupt example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t stts22h_interrupt_deinit(void);

/**
 * @brief      interrupt example read
 * @param[out] *celsius_deg pointer to a celsius degrees buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t stts22h_interrupt_read(float *celsius_deg);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
