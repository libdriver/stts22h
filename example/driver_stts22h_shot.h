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
 * @file      driver_stts22h_shot.h
 * @brief     driver stts22h shot header file
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

#ifndef DRIVER_STTS22H_SHOT_H
#define DRIVER_STTS22H_SHOT_H

#include "driver_stts22h_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup stts22h_example_driver
 * @{
 */

/**
 * @brief stts22h shot example default definition
 */

/**
 * @brief stts22h shot example default definition
 */
#define STTS22H_SHOT_DEFAULT_DISABLE_SMBUS_TIMEOUT             STTS22H_BOOL_FALSE                   /**< disable */
#define STTS22H_SHOT_DEFAULT_BLOCK_DATA_UPDATE                 STTS22H_BOOL_FALSE                   /**< disable block data update */
#define STTS22H_SHOT_DEFAULT_IIC_ADDRESS_AUTO_INCREMENT        STTS22H_BOOL_TRUE                    /**< enable */
#define STTS22H_SHOT_DEFAULT_OUTPUT_DATA_RATE                  STTS22H_OUTPUT_DATA_RATE_50HZ        /**< 50hz */

/**
 * @brief     shot example init
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t stts22h_shot_init(stts22h_address_t addr_pin);

/**
 * @brief  shot example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t stts22h_shot_deinit(void);

/**
 * @brief      shot example read
 * @param[out] *celsius_deg pointer to a celsius degrees buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t stts22h_shot_read(float *celsius_deg);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
