#!/bin/sh
#openocd -c "gdb_port 3342; telnet_port 4442" -f interface/vsllink_swd.cfg -f target/stm32f1x.cfg
openocd -f ./openocd-vsllink.cfg -f interface/vsllink_swd.cfg -f target/stm32f4x.cfg
