# usage

There is a demo application inside `app` directory which toggles PD4 via interrupts. To upload the application short PD3 to ground, power-cycle the MCU and run the uploader utility. DTR pin on UART-USB converter can be connected to RESET pin on STM8 for automatic reset.

``` bash
$ cd app && make
$ python ../boot.py -p /dev/ttyUSB0 firmware.bin
```
