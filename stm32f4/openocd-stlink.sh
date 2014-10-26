#!/bin/sh
#openocd -c "gdb_port 3342; telnet_port 4442" -f interface/vsllink_swd.cfg -f target/stm32f1x.cfg
  openocd -f "board/stm32f4discovery.cfg" -f "/interface/stlink-v2.cfg" -c "targets"  -c "program ./build/usb_sfot_disp.elf verify reset" 

