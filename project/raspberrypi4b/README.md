### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

GPIO Pin: INT GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(stts22h REQUIRED)
```

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
./stts22h -i

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
./stts22h -p

stts22h: SCL connected to GPIO3(BCM).
stts22h: SDA connected to GPIO2(BCM).
stts22h: INT connected to GPIO17(BCM).
```

```shell
./stts22h -t reg --addr=gnd

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
stts22h: set temperature high limit 0xA3.
stts22h: check temperature high limit ok.
stts22h: stts22h_set_temperature_low_limit/stts22h_get_temperature_low_limit test.
stts22h: set temperature low limit 0x97.
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
stts22h: set celsius degrees 77.70C.
stts22h: check celsius degrees 77.44C.
stts22h: finish register test.
```

```shell
./stts22h -t read --addr=gnd --times=3

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
stts22h: temperature is 27.97C.
stts22h: temperature is 27.95C.
stts22h: temperature is 27.98C.
stts22h: set output data rate 25hz.
stts22h: temperature is 27.94C.
stts22h: temperature is 27.94C.
stts22h: temperature is 27.95C.
stts22h: set output data rate 50hz.
stts22h: temperature is 27.95C.
stts22h: temperature is 27.95C.
stts22h: temperature is 27.93C.
stts22h: set output data rate 100hz.
stts22h: temperature is 27.92C.
stts22h: temperature is 27.95C.
stts22h: temperature is 27.93C.
stts22h: set output data rate 200hz.
stts22h: temperature is 27.91C.
stts22h: temperature is 27.92C.
stts22h: temperature is 27.91C.
stts22h: continuous read test.
stts22h: set output data rate 1hz.
stts22h: temperature is 27.79C.
stts22h: temperature is 27.88C.
stts22h: temperature is 27.78C.
stts22h: set output data rate 25hz.
stts22h: temperature is 27.88C.
stts22h: temperature is 27.89C.
stts22h: temperature is 27.87C.
stts22h: set output data rate 50hz.
stts22h: temperature is 27.84C.
stts22h: temperature is 27.87C.
stts22h: temperature is 27.80C.
stts22h: set output data rate 100hz.
stts22h: temperature is 27.87C.
stts22h: temperature is 27.89C.
stts22h: temperature is 27.85C.
stts22h: set output data rate 200hz.
stts22h: temperature is 27.81C.
stts22h: temperature is 27.83C.
stts22h: temperature is 27.76C.
stts22h: disable iic address auto increment.
stts22h: temperature is 27.88C.
stts22h: temperature is 27.94C.
stts22h: temperature is 27.83C.
stts22h: finish read test.
```

```shell
./stts22h -t int --addr=gnd --low=20.0 --high=30.0

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
./stts22h -e read --addr=gnd --times=3

1/3
temperature is 27.77C.
2/3
temperature is 27.88C.
3/3
temperature is 27.81C.
```

```shell
./stts22h -e shot --addr=gnd --times=3

1/3
temperature is 27.82C.
2/3
temperature is 27.86C.
3/3
temperature is 27.84C.
```

```shell
./stts22h -e int --addr=gnd --low=20.0 --high=30.0

stts22h: irq over high limit.
stts22h: temperature is 30.21C.
stts22h: find interrupt.
```

```shell
./stts22h -h

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

