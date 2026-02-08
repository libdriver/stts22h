### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

GPIO Pin: INT PB0.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. STTS22H

#### 3.1 Command Instruction

1. Show stts22h chip and driver information.

   ```shell
   stts22h (-i | --information)
   ```

2. Show stts22h help.

   ```shell
   stts22h (-h | --help)
   ```

3. Show stts22h pin connections of the current board.

   ```shell
   stts22h (-p | --port)
   ```

4. Run stts22h register test.

   ```shell
   stts22h (-t reg | --test=reg) [--addr=<vdd | 15k | 56k | gnd>]
   ```

5. Run stts22h read test, num means test times.

   ```shell
   stts22h (-t read | --test=read) [--addr=<vdd | 15k | 56k | gnd>] [--times=<num>]
   ```
6. Run stts22h interrupt test, threshold means the threshold in celsius degrees.

   ```shell
   stts22h (-t int | --test=int) [--addr=<vdd | 15k | 56k | gnd>] [--low=<threshold>] [--high=<threshold>]
   
7. Run stts22h read function, num means read times.

   ```shell
   stts22h (-e read | --example=read) [--times=<num>]
   ```

8. Run stts22h shot function, num means read times.

   ```shell
   stts22h (-e shot | --example=shot) [--times=<num>]
   ```
   
9. Run stts22h read test, threshold means the threshold in celsius degrees.

   ```shell
   stts22h (-e int | --example=int) [--addr=<vdd | 15k | 56k | gnd>] [--low=<threshold>] [--high=<threshold>]
#### 3.2 Command Example

```shell
stts22h -i

stts22h: chip is STMicroelectronic STTS22H.
stts22h: manufacturer is STMicroelectronic.
stts22h: interface is IIC.
stts22h: driver version is 1.0.
stts22h: min supply voltage is 1.5V.
stts22h: max supply voltage is 3.6V.
stts22h: max current is 0.18mA.
stts22h: max temperature is 125.0C.
stts22h: min temperature is -40.0C.
```

```shell
stts22h -p

stts22h: SCL connected to GPIOB PIN8.
stts22h: SDA connected to GPIOB PIN9.
stts22h: INT connected to GPIOB PIN0.
```

```shell
stts22h -t reg --addr=gnd

stts22h: chip is STMicroelectronic STTS22H.
stts22h: manufacturer is STMicroelectronic.
stts22h: interface is IIC.
stts22h: driver version is 1.0.
stts22h: min supply voltage is 1.5V.
stts22h: max supply voltage is 3.6V.
stts22h: max current is 0.18mA.
stts22h: max temperature is 125.0C.
stts22h: min temperature is -40.0C.
stts22h: start register test.
stts22h: stts22h_set_addr_pin/stts22h_get_addr_pin test.
stts22h: set addr pin vdd.
stts22h: check addr pin ok.
stts22h: set addr pin 15k.
stts22h: check addr pin ok.
stts22h: set addr pin 56k.
stts22h: check addr pin ok.
stts22h: set addr pin gnd.
stts22h: check addr pin ok.
stts22h: stts22h_set_temperature_high_limit/stts22h_get_temperature_high_limit test.
stts22h: set temperature high limit 0xEE.
stts22h: check temperature high limit ok.
stts22h: stts22h_set_temperature_low_limit/stts22h_get_temperature_low_limit test.
stts22h: set temperature low limit 0xDE.
stts22h: check temperature low limit ok.
stts22h: stts22h_set_output_data_rate/stts22h_get_output_data_rate test.
stts22h: set output data rate 25hz.
stts22h: check output data rate ok.
stts22h: set output data rate 50hz.
stts22h: check output data rate ok.
stts22h: set output data rate 100hz.
stts22h: check output data rate ok.
stts22h: set output data rate 200hz.
stts22h: check output data rate ok.
stts22h: set output data rate 1hz.
stts22h: check output data rate ok.
stts22h: stts22h_set_block_data_update/stts22h_get_block_data_update test.
stts22h: enable block data update.
stts22h: check block data update ok.
stts22h: disable block data update.
stts22h: check block data update ok.
stts22h: stts22h_set_iic_address_auto_increment/stts22h_get_iic_address_auto_increment test.
stts22h: enable iic address auto increment.
stts22h: check iic address auto increment ok.
stts22h: disable iic address auto increment.
stts22h: check iic address auto increment ok.
stts22h: stts22h_set_disable_smbus_timeout/stts22h_get_disable_smbus_timeout test.
stts22h: enable disable smbus timeout.
stts22h: check enable disable smbus timeout ok.
stts22h: disable disable smbus timeout.
stts22h: check enable disable smbus timeout ok.
stts22h: stts22h_get_status test.
stts22h: status is 0x00.
stts22h: stts22h_temperature_convert_to_register/stts22h_temperature_convert_to_data test.
stts22h: set celsius degrees 40.90C.
stts22h: check celsius degrees 40.32C.
stts22h: finish register test.
```

```shell
stts22h -t read --addr=gnd --times=3

stts22h: chip is STMicroelectronic STTS22H.
stts22h: manufacturer is STMicroelectronic.
stts22h: interface is IIC.
stts22h: driver version is 1.0.
stts22h: min supply voltage is 1.5V.
stts22h: max supply voltage is 3.6V.
stts22h: max current is 0.18mA.
stts22h: max temperature is 125.0C.
stts22h: min temperature is -40.0C.
stts22h: single read test.
stts22h: set output data rate 1hz.
stts22h: temperature is 28.29C.
stts22h: temperature is 28.31C.
stts22h: temperature is 28.29C.
stts22h: set output data rate 25hz.
stts22h: temperature is 28.23C.
stts22h: temperature is 28.24C.
stts22h: temperature is 28.24C.
stts22h: set output data rate 50hz.
stts22h: temperature is 28.26C.
stts22h: temperature is 28.26C.
stts22h: temperature is 28.25C.
stts22h: set output data rate 100hz.
stts22h: temperature is 28.25C.
stts22h: temperature is 28.24C.
stts22h: temperature is 28.24C.
stts22h: set output data rate 200hz.
stts22h: temperature is 28.27C.
stts22h: temperature is 28.27C.
stts22h: temperature is 28.27C.
stts22h: continuous read test.
stts22h: set output data rate 1hz.
stts22h: temperature is 28.27C.
stts22h: temperature is 28.27C.
stts22h: temperature is 28.30C.
stts22h: set output data rate 25hz.
stts22h: temperature is 28.25C.
stts22h: temperature is 28.24C.
stts22h: temperature is 28.24C.
stts22h: set output data rate 50hz.
stts22h: temperature is 28.22C.
stts22h: temperature is 28.23C.
stts22h: temperature is 28.26C.
stts22h: set output data rate 100hz.
stts22h: temperature is 28.27C.
stts22h: temperature is 28.24C.
stts22h: temperature is 28.23C.
stts22h: set output data rate 200hz.
stts22h: temperature is 28.26C.
stts22h: temperature is 28.26C.
stts22h: temperature is 28.28C.
stts22h: disable iic address auto increment.
stts22h: temperature is 28.26C.
stts22h: temperature is 28.29C.
stts22h: temperature is 28.28C.
stts22h: finish read test.
```

```shell
stts22h -t int --addr=gnd --low=20.0 --high=30.0

stts22h: chip is STMicroelectronic STTS22H.
stts22h: manufacturer is STMicroelectronic.
stts22h: interface is IIC.
stts22h: driver version is 1.0.
stts22h: min supply voltage is 1.5V.
stts22h: max supply voltage is 3.6V.
stts22h: max current is 0.18mA.
stts22h: max temperature is 125.0C.
stts22h: min temperature is -40.0C.
stts22h: irq over high limit.
stts22h: temperature is 30.21C.
stts22h: find interrupt.
stts22h: finish interrupt test.
```

```shell
stts22h -e read --addr=gnd --times=3

1/3
temperature is 28.86C.
2/3
temperature is 28.87C.
3/3
temperature is 28.82C.
```

```shell
stts22h -e shot --addr=gnd --times=3

1/3
temperature is 28.63C.
2/3
temperature is 28.65C.
3/3
temperature is 28.65C.
```

```shell
stts22h -e int --addr=gnd --low=20.0 --high=30.0

stts22h: irq over high limit.
stts22h: temperature is 30.21C.
stts22h: find interrupt.
```

```shell
stts22h -h

Usage:
  stts22h (-i | --information)
  stts22h (-h | --help)
  stts22h (-p | --port)
  stts22h (-t reg | --test=reg) [--addr=<vdd | 15k | 56k | gnd>]
  stts22h (-t read | --test=read) [--addr=<vdd | 15k | 56k | gnd>] [--times=<num>]
  stts22h (-t int | --test=int) [--addr=<vdd | 15k | 56k | gnd>] [--low=<threshold>] [--high=<threshold>]
  stts22h (-e read | --example=read) [--addr=<vdd | 15k | 56k | gnd>] [--times=<num>]
  stts22h (-e shot | --example=shot) [--addr=<vdd | 15k | 56k | gnd>] [--times=<num>]
  stts22h (-e int | --example=int) [--addr=<vdd | 15k | 56k | gnd>] [--low=<threshold>] [--high=<threshold>]

Options:
      --addr=<vdd | 15k | 56k | gnd>
                                 Set the device address.([default: gnd])
  -e <read | shot | int>, --example=<read | shot | int>
                                 Run the driver example.
  -h, --help                     Show the help.
      --high=<threshold>         Set the interrupt high threshold.([default: 20])
      --low=<threshold>          Set the interrupt low threshold.([default: 20])
  -i, --information              Show the chip information.
  -p, --port                     Display the pin connections of the current board.
  -t <reg | read | int>, --test=<reg | read | int>
                                 Run the driver test.
      --times=<num>              Set the running times.([default: 3])
```

