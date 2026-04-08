all : flash

TARGET:=User/main

TARGET_MCU?=CH32V003

ADDITIONAL_C_FILES:=User/tmp102.c User/breathing.c User/color_map.c

include ch32v003fun/ch32fun/ch32fun.mk

flash : cv_flash
clean : cv_clean
