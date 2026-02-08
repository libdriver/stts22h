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
 * @file      driver_stts22h.c
 * @brief     driver stts22h source file
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

#include "driver_stts22h.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "STMicroelectronic STTS22H"        /**< chip name */
#define MANUFACTURER_NAME         "STMicroelectronic"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.5f                               /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                               /**< chip max supply voltage */
#define MAX_CURRENT               0.18f                              /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                             /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f                             /**< chip max operating temperature */
#define DRIVER_VERSION            1000                               /**< driver version */

/**
 * @brief chip register definition
 */
#define STTS22H_REG_WHOAMI              0x01      /**< who am i register */
#define STTS22H_REG_TEMP_H_LIMIT        0x02      /**< temperature high limit register */
#define STTS22H_REG_TEMP_L_LIMIT        0x03      /**< temperature low limit register */
#define STTS22H_REG_CTRL                0x04      /**< control register */
#define STTS22H_REG_STATUS              0x05      /**< status register */
#define STTS22H_REG_TEMP_L_OUT          0x06      /**< temperature lsb register */
#define STTS22H_REG_TEMP_H_OUT          0x07      /**< temperature msb register */

/**
 * @brief      iic interface read bytes
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_stts22h_read(stts22h_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_addr_auto_inc != 0)                                                         /* address auto inc */
    {
        if (handle->iic_read(handle->iic_addr, reg, buf, len) != 0)                             /* read data */
        {
            return 1;                                                                           /* return error */
        }
    }
    else
    {
        uint16_t i;
        
        for (i = 0; i < len; i++)                                                               /* read all data */
        {
            if (handle->iic_read(handle->iic_addr, (uint8_t)(reg + i), &buf[i], 1) != 0)        /* read data */
            {
                return 1;                                                                       /* return error */
            }
        }
    }
    
    return 0;                                                                                   /* success return 0 */
}

/**
 * @brief     iic interface write bytes
 * @param[in] *handle pointer to a stts22h handle structure
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_stts22h_write(stts22h_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_addr_auto_inc != 0)                                                         /* address auto inc */
    {
        if (handle->iic_write(handle->iic_addr, reg, buf, len) != 0)                            /* write data */
        {
            return 1;                                                                           /* return error */
        }
    }
    else
    {
        uint16_t i;
        
        for (i = 0; i < len; i++)                                                               /* write all data */
        {
            if (handle->iic_write(handle->iic_addr, (uint8_t)(reg + i), &buf[i], 1) != 0)       /* write data */
            {
                return 1;                                                                       /* return error */
            }
        }
    }
    
    return 0;                                                                                   /* success return 0 */
}

/**
 * @brief     set the iic address pin
 * @param[in] *handle pointer to a stts22h handle structure
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t stts22h_set_addr_pin(stts22h_handle_t *handle, stts22h_address_t addr_pin)
{
    if (handle == NULL)                        /* check handle */
    {
        return 2;                              /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr_pin;      /* set pin */
    
    return 0;                                  /* success return 0 */
}

/**
 * @brief      get the iic address pin
 * @param[in]  *handle pointer to a stts22h handle structure
 * @param[out] *addr_pin pointer to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t stts22h_get_addr_pin(stts22h_handle_t *handle, stts22h_address_t *addr_pin)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    
    *addr_pin = (stts22h_address_t)(handle->iic_addr);        /* get pin */
    
    return 0;                                                 /* success return 0 */
}

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
uint8_t stts22h_set_temperature_high_limit(stts22h_handle_t *handle, uint8_t raw)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    prev = raw;                                                                        /* set raw data */
    res = a_stts22h_write(handle, STTS22H_REG_TEMP_H_LIMIT, &prev, 1);                 /* write limit */
    if (res != 0)                                                                      /* check the result */
    {
        handle->debug_print("stts22h: write temperature high limit failed.\n");        /* write temperature high limit failed */
        
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t stts22h_get_temperature_high_limit(stts22h_handle_t *handle, uint8_t *raw)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_TEMP_H_LIMIT, &prev, 1);                 /* read limit */
    if (res != 0)                                                                     /* check the result */
    {
        handle->debug_print("stts22h: read temperature high limit failed.\n");        /* read temperature high limit failed */
        
        return 1;                                                                     /* return error */
    }
    *raw = prev;                                                                      /* set raw data */
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t stts22h_set_temperature_low_limit(stts22h_handle_t *handle, uint8_t raw)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    prev = raw;                                                                        /* set raw data */
    res = a_stts22h_write(handle, STTS22H_REG_TEMP_L_LIMIT, &prev, 1);                 /* write limit */
    if (res != 0)                                                                      /* check the result */
    {
        handle->debug_print("stts22h: write temperature low limit failed.\n");         /* write temperature low limit failed */
        
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t stts22h_get_temperature_low_limit(stts22h_handle_t *handle, uint8_t *raw)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_TEMP_L_LIMIT, &prev, 1);                /* read limit */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("stts22h: read temperature low limit failed.\n");        /* read temperature low limit failed */
        
        return 1;                                                                    /* return error */
    }
    *raw = prev;                                                                     /* set raw data */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t stts22h_set_output_data_rate(stts22h_handle_t *handle, stts22h_output_data_rate_t rate)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_CTRL, &prev, 1);           /* read control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: read control failed.\n");         /* read control failed */
        
        return 1;                                                       /* return error */
    }
    if (rate == STTS22H_OUTPUT_DATA_RATE_1HZ)                           /* 1hz */
    {
        prev |= 1 << 7;                                                 /* set low power bit */
    }
    else
    {
        prev &= ~(1 << 7);                                              /* clear low power bit */
        prev &= ~(3 << 4);                                              /* clear settings */
        prev |= rate << 4;                                              /* set rate */
    }
    res = a_stts22h_write(handle, STTS22H_REG_CTRL, &prev, 1);          /* write control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: write control failed.\n");        /* write control failed */
        
        return 1;                                                       /* return error */
    }
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t stts22h_get_output_data_rate(stts22h_handle_t *handle, stts22h_output_data_rate_t *rate)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_CTRL, &prev, 1);            /* read control */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("stts22h: read control failed.\n");          /* read control failed */
        
        return 1;                                                        /* return error */
    }
    
    if (((prev >> 7) & 0x01) != 0)                                       /* low power is clear */
    {
        *rate = STTS22H_OUTPUT_DATA_RATE_1HZ;                            /* set 1hz */
    }
    else
    {
        *rate = (stts22h_output_data_rate_t)((prev >> 4) & 0x03);        /* set output data rate */
    }
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t stts22h_set_block_data_update(stts22h_handle_t *handle, stts22h_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_CTRL, &prev, 1);           /* read control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: read control failed.\n");         /* read control failed */
        
        return 1;                                                       /* return error */
    }
    prev &= ~(1 << 6);                                                  /* clear settings */
    prev |= enable << 6;                                                /* set bool */
    res = a_stts22h_write(handle, STTS22H_REG_CTRL, &prev, 1);          /* write control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: write control failed.\n");        /* write control failed */
        
        return 1;                                                       /* return error */
    }
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t stts22h_get_block_data_update(stts22h_handle_t *handle, stts22h_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_CTRL, &prev, 1);           /* read control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: read control failed.\n");         /* read control failed */
        
        return 1;                                                       /* return error */
    }
    *enable = (stts22h_bool_t)((prev >> 6) & 0x01);                     /* set bool */
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t stts22h_set_iic_address_auto_increment(stts22h_handle_t *handle, stts22h_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_CTRL, &prev, 1);           /* read control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: read control failed.\n");         /* read control failed */
        
        return 1;                                                       /* return error */
    }
    prev &= ~(1 << 3);                                                  /* clear settings */
    prev |= enable << 3;                                                /* set bool */
    res = a_stts22h_write(handle, STTS22H_REG_CTRL, &prev, 1);          /* write control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: write control failed.\n");        /* write control failed */
        
        return 1;                                                       /* return error */
    }
    handle->iic_addr_auto_inc = (uint8_t)enable;                        /* set bool */
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t stts22h_get_iic_address_auto_increment(stts22h_handle_t *handle, stts22h_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_CTRL, &prev, 1);           /* read control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: read control failed.\n");         /* read control failed */
        
        return 1;                                                       /* return error */
    }
    *enable = (stts22h_bool_t)((prev >> 3) & 0x01);                     /* set bool */
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t stts22h_set_disable_smbus_timeout(stts22h_handle_t *handle, stts22h_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_CTRL, &prev, 1);           /* read control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: read control failed.\n");         /* read control failed */
        
        return 1;                                                       /* return error */
    }
    prev &= ~(1 << 1);                                                  /* clear settings */
    prev |= enable << 1;                                                /* set bool */
    res = a_stts22h_write(handle, STTS22H_REG_CTRL, &prev, 1);          /* write control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: write control failed.\n");        /* write control failed */
        
        return 1;                                                       /* return error */
    }
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t stts22h_get_disable_smbus_timeout(stts22h_handle_t *handle, stts22h_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_CTRL, &prev, 1);           /* read control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: read control failed.\n");         /* read control failed */
        
        return 1;                                                       /* return error */
    }
    *enable = (stts22h_bool_t)((prev >> 1) & 0x01);                     /* set bool */
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t stts22h_get_status(stts22h_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_STATUS, &prev, 1);        /* read status */
    if (res != 0)                                                      /* check the result */
    {
        handle->debug_print("stts22h: read status failed.\n");         /* read status failed */
        
        return 1;                                                      /* return error */
    }
    *status = prev;                                                    /* set status */
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t stts22h_single_read(stts22h_handle_t *handle, int16_t *raw, float *celsius_deg)
{
    uint8_t res;
    uint8_t prev;
    uint8_t buf[2];
    uint16_t timeout;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_CTRL, &prev, 1);            /* read control */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("stts22h: read control failed.\n");          /* read control failed */
        
        return 1;                                                        /* return error */
    }
    prev &= ~(1 << 7);                                                   /* clear low power */
    prev &= ~(1 << 2);                                                   /* clear free run */
    prev |= (1 << 0);                                                    /* set one shot */
    res = a_stts22h_write(handle, STTS22H_REG_CTRL, &prev, 1);           /* write control */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("stts22h: write control failed.\n");         /* write control failed */
        
        return 1;                                                        /* return error */
    }
    
    timeout = 500;                                                       /* set timeout 5000ms */
    while (timeout != 0)
    {
        res = a_stts22h_read(handle, STTS22H_REG_STATUS, &prev, 1);      /* read status */
        if (res != 0)                                                    /* check the result */
        {
            handle->debug_print("stts22h: read status failed.\n");       /* read status failed */
            
            return 1;                                                    /* return error */
        }
        if ((prev & 0x01) == 0)                                          /* conversion is completed */
        {
            break;                                                       /* break */
        }
        handle->delay_ms(10);                                            /* delay 10ms */
        timeout--;                                                       /* timeout-- */
    }
    if (timeout == 0)                                                    /* check timeout */
    {
        handle->debug_print("stts22h: read timeout.\n");                 /* read timeout */
        
        return 4;                                                        /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_TEMP_L_OUT, buf, 2);        /* read temperature */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("stts22h: read failed.\n");                  /* read failed */
        
        return 1;                                                        /* return error */
    }
    *raw = (int16_t)((uint16_t)((uint16_t)buf[1] << 8) | buf[0]);        /* set raw data */
    *celsius_deg = (float)(*raw) / 100.0f;                               /* convert data */
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t stts22h_start_continuous_read(stts22h_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_CTRL, &prev, 1);           /* read control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: read control failed.\n");         /* read control failed */
        
        return 1;                                                       /* return error */
    }
    prev |= 1 << 2;                                                     /* enable */
    prev &= ~(1 << 0);                                                  /* clear one shot */
    res = a_stts22h_write(handle, STTS22H_REG_CTRL, &prev, 1);          /* write control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: write control failed.\n");        /* write control failed */
        
        return 1;                                                       /* return error */
    }
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t stts22h_stop_continuous_read(stts22h_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_CTRL, &prev, 1);           /* read control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: read control failed.\n");         /* read control failed */
        
        return 1;                                                       /* return error */
    }
    prev &= ~(1 << 2);                                                  /* disable */
    prev &= ~(1 << 0);                                                  /* clear one shot */
    res = a_stts22h_write(handle, STTS22H_REG_CTRL, &prev, 1);          /* write control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: write control failed.\n");        /* write control failed */
        
        return 1;                                                       /* return error */
    }
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t stts22h_continuous_read(stts22h_handle_t *handle, int16_t *raw, float *celsius_deg)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_TEMP_L_OUT, buf, 2);        /* read temperature */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("stts22h: read failed.\n");                  /* read failed */
        
        return 1;                                                        /* return error */
    }
    *raw = (int16_t)((uint16_t)((uint16_t)buf[1] << 8) | buf[0]);        /* set raw data */
    *celsius_deg = (float)(*raw) / 100.0f;                               /* convert data */
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t stts22h_irq_handler(stts22h_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_STATUS, &prev, 1);              /* read status */
    if (res != 0)                                                            /* check the result */
    {
        handle->debug_print("stts22h: read status failed.\n");               /* read status failed */
        
        return 1;                                                            /* return error */
    }
    if ((prev & STTS22H_STATUS_UNDER_LOW_LIMIT) != 0)                        /* check flag */
    {
        if (handle->receive_callback != NULL)                                /* not null */
        {
            handle->receive_callback(STTS22H_STATUS_UNDER_LOW_LIMIT);        /* run the callback */
        }
    }
    if ((prev & STTS22H_STATUS_OVER_HIGH_LIMIT) != 0)                        /* check flag */
    {
        if (handle->receive_callback != NULL)                                /* not null */
        {
            handle->receive_callback(STTS22H_STATUS_OVER_HIGH_LIMIT);        /* run the callback */
        }
    }
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t stts22h_temperature_convert_to_register(stts22h_handle_t *handle, float celsius_deg, uint8_t *reg)
{
    if (handle == NULL)                                /* check handle */
    {
        return 2;                                      /* return error */
    }
    if (handle->inited != 1)                           /* check handle initialization */
    {
        return 3;                                      /* return error */
    }
    
    *reg = (uint8_t)(celsius_deg / 0.64f) + 63;        /* convert real data to register data */
    
    return 0;                                          /* success return 0 */
}

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
uint8_t stts22h_temperature_convert_to_data(stts22h_handle_t *handle, uint8_t reg, float *celsius_deg)
{
    if (handle == NULL)                                   /* check handle */
    {
        return 2;                                         /* return error */
    }
    if (handle->inited != 1)                              /* check handle initialization */
    {
        return 3;                                         /* return error */
    }
    
    *celsius_deg = ((float)(reg) - 63.0f) * 0.64f;        /* convert raw data to real data */
    
    return 0;                                             /* success return 0 */
}

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
uint8_t stts22h_init(stts22h_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    uint8_t id;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->debug_print == NULL)                                        /* check debug_print */
    {
        return 3;                                                           /* return error */
    }
    if (handle->iic_init == NULL)                                           /* check iic_init */
    {
        handle->debug_print("stts22h: iic_init is null.\n");                /* iic_init is null */
        
        return 3;                                                           /* return error */
    }
    if (handle->iic_deinit == NULL)                                         /* check iic_deinit */
    {
        handle->debug_print("stts22h: iic_deinit is null.\n");              /* iic_deinit is null */
        
        return 3;                                                           /* return error */
    }
    if (handle->iic_read == NULL)                                           /* check iic_read */
    {
        handle->debug_print("stts22h: iic_read is null.\n");                /* iic_read is null */
        
        return 3;                                                           /* return error */
    }
    if (handle->iic_write == NULL)                                          /* check iic_write */
    {
        handle->debug_print("stts22h: iic_write is null.\n");               /* iic_write is null */
        
        return 3;                                                           /* return error */
    }
    if (handle->delay_ms == NULL)                                           /* check delay_ms */
    {
        handle->debug_print("stts22h: delay_ms is null.\n");                /* delay_ms is null */
        
        return 3;                                                           /* return error */
    }
    if (handle->receive_callback == NULL)                                   /* check receive_callback */
    {
        handle->debug_print("stts22h: receive_callback is null.\n");        /* receive_callback is null */
        
        return 3;                                                           /* return error */
    }
    
    if (handle->iic_init() != 0)                                            /* iic init */
    {
        handle->debug_print("stts22h: iic init failed.\n");                 /* iic init failed */
        
        return 1;                                                           /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_WHOAMI, &id, 1);               /* read who am i */
    if (res != 0)                                                           /* check the result */
    {
        handle->debug_print("stts22h: read who am i failed.\n");            /* read who am i failed */
        (void)handle->iic_deinit();                                         /* iic deinit */
        
        return 4;                                                           /* return error */
    }
    if (id != 0xA0)                                                         /* check id */
    {
        handle->debug_print("stts22h: id is invalid.\n");                   /* id is invalid */
        (void)handle->iic_deinit();                                         /* iic deinit */
        
        return 4;                                                           /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_CTRL, &prev, 1);               /* read control */
    if (res != 0)                                                           /* check the result */
    {
        handle->debug_print("stts22h: read control failed.\n");             /* read control failed */
        (void)handle->iic_deinit();                                         /* iic deinit */
        
        return 1;                                                           /* return error */
    }
    prev &= ~(1 << 3);                                                      /* clear settings */
    prev |= 1 << 3;                                                         /* set bool */
    res = a_stts22h_write(handle, STTS22H_REG_CTRL, &prev, 1);              /* write control */
    if (res != 0)                                                           /* check the result */
    {
        handle->debug_print("stts22h: write control failed.\n");            /* write control failed */
        (void)handle->iic_deinit();                                         /* iic deinit */
        
        return 1;                                                           /* return error */
    }
    handle->iic_addr_auto_inc = 1;                                          /* set enable */
    handle->inited = 1;                                                     /* flag finish initialization */
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t stts22h_deinit(stts22h_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_stts22h_read(handle, STTS22H_REG_CTRL, &prev, 1);           /* read control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: read control failed.\n");         /* read control failed */
        
        return 4;                                                       /* return error */
    }
    prev &= ~(1 << 7);                                                  /* clear settings */
    prev &= ~(1 << 2);                                                  /* clear settings */
    prev &= ~(1 << 0);                                                  /* clear settings */
    res = a_stts22h_write(handle, STTS22H_REG_CTRL, &prev, 1);          /* write control */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("stts22h: write control failed.\n");        /* write control failed */
        
        return 4;                                                       /* return error */
    }
     
    if (handle->iic_deinit() != 0)                                      /* iic deinit */
    {
        handle->debug_print("stts22h: iic deinit failed.\n");           /* return error */
        
        return 1;                                                       /* iic deinit failed */
    }   
    handle->inited = 0;                                                 /* flag close */
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t stts22h_set_reg(stts22h_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                   /* check handle */
    {
        return 2;                                         /* return error */
    }
    if (handle->inited != 1)                              /* check handle initialization */
    {
        return 3;                                         /* return error */
    } 
    
    return a_stts22h_write(handle, reg, buf, len);        /* write data */
}

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
uint8_t stts22h_get_reg(stts22h_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                  /* check handle */
    {
        return 2;                                        /* return error */
    }
    if (handle->inited != 1)                             /* check handle initialization */
    {
        return 3;                                        /* return error */
    } 
    
    return a_stts22h_read(handle, reg, buf, len);        /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a stts22h info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t stts22h_info(stts22h_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(stts22h_info_t));                        /* initialize stts22h info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
