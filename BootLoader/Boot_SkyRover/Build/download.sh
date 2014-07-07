#!/bin/bash
make
~/stm32ld /dev/tty.SLAB_USBtoUART 115200 main.bin
