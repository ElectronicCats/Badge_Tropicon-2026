all : build

TARGET:=User/main

TARGET_MCU?=CH32V003

ADDITIONAL_C_FILES:=User/tmp102.c User/breathing.c User/color_map.c

include ch32v003fun/ch32fun/ch32fun.mk

build : $(TARGET).bin
flash : $(TARGET).bin
	wlink flash $(TARGET).bin
clean : cv_clean
