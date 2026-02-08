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
 * @file      driver_stts22h.h
 * @brief     driver stts22h header file
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

#ifndef DRIVER_STTS22H_H
#define DRIVER_STTS22H_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup stts22h_driver stts22h driver function
 * @brief    stts22h driver modules
 * @{
 */

/**
 * @addtogroup stts22h_base_driver
 * @{
 */

/**
 * @brief stts22h address enumeration definition
 */
typedef enum
{
    STTS22H_ADDRESS_VDD = 0x70,        /**< addr pin connected to the VDD */
    STTS22H_ADDRESS_15K = 0x78,        /**< addr pin connected to the 15K pull-up resistor */
    STTS22H_ADDRESS_56K = 0x7C,        /**< addr pin connected to the 56K pull-up resistor */
    STTS22H_ADDRESS_GND = 0x7E,        /**< addr pin connected to the GND */
} stts22h_address_t;

/**
 * @brief stts22h bool enumeration definition
 */
typedef enum
{
    STTS22H_BOOL_FALSE = 0x00,        /**< false */
    STTS22H_BOOL_TRUE  = 0x01,        /**< true */
} stts22h_bool_t;

/**
 * @brief stts22h output data rate enumeration definition
 */
typedef enum
{
    STTS22H_OUTPUT_DATA_RATE_25HZ  = 0x00,        /**< 25hz */
    STTS22H_OUTPUT_DATA_RATE_50HZ  = 0x01,        /**< 50hz */
    STTS22H_OUTPUT_DATA_RATE_100HZ = 0x02,        /**< 100hz */
    STTS22H_OUTPUT_DATA_RATE_200HZ = 0x03,        /**< 200hz */
    STTS22H_OUTPUT_DATA_RATE_1HZ   = 0x04,        /**< 1hz */
} stts22h_output_data_rate_t;

/**
 * @brief stts22h status enumeration definition
 */
typedef enum
{
    STTS22H_STATUS_UNDER_LOW_LIMIT = (1 << 2),        /**< under low limit */
    STTS22H_STATUS_OVER_HIGH_LIMIT = (1 << 1),        /**< over high limit */
} stts22h_status_t;

/**
 * @brief stts22h handle structure definition
 */
typedef struct stts22h_handle_s
{
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    uint8_t inited;                                                                     /**< inited flag */
    uint8_t iic_addr;                                                                   /**< iic address */
    uint8_t iic_addr_auto_inc;                                                          /**< iic address auto inc */
} stts22h_handle_t;

/**
 * @brief stts22h information structure definition
 */
typedef struct stts22h_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} stts22h_info_t;

/**
 * @}
 */

/**
 * @defgroup stts22h_link_driver stts22h link driver function
 * @brief    stts22h link driver modules
 * @ingroup  stts22h_driver
 * @{
 */

/**
 * @brief     initialize stts22h_handle_t structure
 * @param[in] HANDLE pointer to a stts22h handle structure
 * @param[in] STRUCTURE stts22h_handle_t
 * @note      none
 */
#define DRIVER_STTS22H_LINK_INIT(HANDLE, STRUCTURE)          memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to a stts22h handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_STTS22H_LINK_IIC_INIT(HANDLE, FUC)           (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to a stts22h handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_STTS22H_LINK_IIC_DEINIT(HANDLE, FUC)         (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE pointer to a stts22h handle structure
 * @param[in] FUC pointer to an iic_read function address
 * @note      none
 */
#define DRIVER_STTS22H_LINK_IIC_READ(HANDLE, FUC)           (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE pointer to a stts22h handle structure
 * @param[in] FUC pointer to an iic_write function address
 * @note      none
 */
#define DRIVER_STTS22H_LINK_IIC_WRITE(HANDLE, FUC)          (HANDLE)->iic_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to a stts22h handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_STTS22H_LINK_DELAY_MS(HANDLE, FUC)           (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to a stts22h handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_STTS22H_LINK_DEBUG_PRINT(HANDLE, FUC)        (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE pointer to a stts22h handle structure
 * @param[in] FUC pointer to a receive_callback function address
 * @note      none
 */
#define DRIVER_STTS22H_LINK_RECEIVE_CALLBACK(HANDLE, FUC)   (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup stts22h_base_driver stts22h base driver function
 * @brief    stts22h base driver modules
 * @ingroup  stts22h_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a stts22h info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t stts22h_info(stts22h_info_t *info);

/**
 * @brief     set the iic address pin
 * @param[in] *handle pointer to a stts22h handle structure
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t stts22h_set_addr_pin(stts22h_handle_t *handle, stts22h_address_t addr_pin);

/**
 * @brief      get the iic address pin
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[out] *addr_pin pointer to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t stts22h_get_addr_pin(stts22h_handle_t *handle, stts22h_address_t *addr_pin);

/**
 * @brief     irq handler
 * @param[in] *handle pointer to a stts22h handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stts22h_irq_handler(stts22h_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to a stts22h handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 id is invalid
 * @note      none
 */
uint8_t stts22h_init(stts22h_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to a stts22h handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t stts22h_deinit(stts22h_handle_t *handle);

/**
 * @brief      read data from the chip once
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[out] *raw pointer to a raw adc buffer
 * @param[out] *celsius_deg pointer to a celsius degrees buffer
 * @return     status code
 *             - 0 success
 *             - 1 single read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 read timeout
 * @note       none
 */
uint8_t stts22h_single_read(stts22h_handle_t *handle, int16_t *raw, float *celsius_deg);

/**
 * @brief     start the chip reading
 * @param[in] *handle pointer to a stts22h handle structure
 * @return    status code
 *            - 0 success
 *            - 1 start continuous read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stts22h_start_continuous_read(stts22h_handle_t *handle);

/**
 * @brief     stop the chip reading
 * @param[in] *handle pointer to a stts22h handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop continuous read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stts22h_stop_continuous_read(stts22h_handle_t *handle);

/**
 * @brief      read data from the chip continuously
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[out] *raw pointer to a raw adc buffer
 * @param[out] *celsius_deg pointer to a celsius degrees buffer
 * @return     status code
 *             - 0 success
 *             - 1 continuous read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       this function can be used only after run stts22h_start_continuous_read
 *             and can be stopped by stts22h_stop_continuous_read
 */
uint8_t stts22h_continuous_read(stts22h_handle_t *handle, int16_t *raw, float *celsius_deg);

/**
 * @brief     set temperature high limit
 * @param[in] *handle pointer to a stts22h handle structure
 * @param[in] raw raw data
 * @return    status code
 *            - 0 success
 *            - 1 set temperature high limit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stts22h_set_temperature_high_limit(stts22h_handle_t *handle, uint8_t raw);

/**
 * @brief      get temperature high limit
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[out] *raw pointer to a raw data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature high limit failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stts22h_get_temperature_high_limit(stts22h_handle_t *handle, uint8_t *raw);

/**
 * @brief     set temperature low limit
 * @param[in] *handle pointer to a stts22h handle structure
 * @param[in] raw raw data
 * @return    status code
 *            - 0 success
 *            - 1 set temperature low limit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stts22h_set_temperature_low_limit(stts22h_handle_t *handle, uint8_t raw);

/**
 * @brief      get temperature low limit
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[out] *raw pointer to a raw data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature low limit failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stts22h_get_temperature_low_limit(stts22h_handle_t *handle, uint8_t *raw);

/**
 * @brief     set output data rate
 * @param[in] *handle pointer to a stts22h handle structure
 * @param[in] rate output data rate
 * @return    status code
 *            - 0 success
 *            - 1 set output data rate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stts22h_set_output_data_rate(stts22h_handle_t *handle, stts22h_output_data_rate_t rate);

/**
 * @brief      get output data rate
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[out] *rate pointer to an output data rate buffer
 * @return     status code
 *             - 0 success
 *             - 1 get output data rate failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stts22h_get_output_data_rate(stts22h_handle_t *handle, stts22h_output_data_rate_t *rate);

/**
 * @brief     enable or disable block data update
 * @param[in] *handle pointer to a stts22h handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set block data update failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stts22h_set_block_data_update(stts22h_handle_t *handle, stts22h_bool_t enable);

/**
 * @brief      get block data update status
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get block data update failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stts22h_get_block_data_update(stts22h_handle_t *handle, stts22h_bool_t *enable);

/**
 * @brief     enable or disable iic address auto increment
 * @param[in] *handle pointer to a stts22h handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic address auto increment failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stts22h_set_iic_address_auto_increment(stts22h_handle_t *handle, stts22h_bool_t enable);

/**
 * @brief      get iic address auto increment status
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic address auto increment failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stts22h_get_iic_address_auto_increment(stts22h_handle_t *handle, stts22h_bool_t *enable);

/**
 * @brief     enable or disable disable smbus timeout
 * @param[in] *handle pointer to a stts22h handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set disable smbus timeout failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stts22h_set_disable_smbus_timeout(stts22h_handle_t *handle, stts22h_bool_t enable);

/**
 * @brief      get disable smbus timeout status
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get disable smbus timeout failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stts22h_get_disable_smbus_timeout(stts22h_handle_t *handle, stts22h_bool_t *enable);

/**
 * @brief      get status
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[out] *status pointer to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stts22h_get_status(stts22h_handle_t *handle, uint8_t *status);

/**
 * @brief      convert the temperature to the register raw data
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[in]  celsius_deg celsius degrees
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stts22h_temperature_convert_to_register(stts22h_handle_t *handle, float celsius_deg, uint8_t *reg);

/**
 * @brief      convert the register raw data to the temperature
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[in]  reg register raw data
 * @param[out] *celsius_deg pointer to a celsius degrees buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stts22h_temperature_convert_to_data(stts22h_handle_t *handle, uint8_t reg, float *celsius_deg);

/**
 * @}
 */

/**
 * @defgroup stts22h_extend_driver stts22h extend driver function
 * @brief    stts22h extend driver modules
 * @ingroup  stts22h_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle pointer to a stts22h handle structure
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t stts22h_set_reg(stts22h_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t stts22h_get_reg(stts22h_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */
 
#ifdef __cplusplus
}
#endif

#endif
