#
# Copyright (c) 2003,08,09,16,17 Bernd Walter
# All rights reserved.
#
# $URL$
# $Date$
# $Author$
# $Rev$
#

ARCH=		cortex-m0plus

PART=		SAMD21J18A
LDSCRIPT=	samd21j18a_flash.ld
FAMILY=		SAMD21
OPENOCDBRD=	board/atmel_samd21_xplained_pro.cfg

ASF=		contrib/xdk-asf
FREERTOS=	contrib/FreeRTOS

BIN=		main
SRCS=		main.c
SRCS+=		uart.c
SRCS+=		gcode.c
SRCS+=		sbrk.c

CROSS=		arm-none-eabi-
CC=		$(CROSS)gcc
LD=		$(CROSS)ld
OBJCOPY=	$(CROSS)objcopy

XCFLAGS=	-fomit-frame-pointer -Os -g -Wall
XCFLAGS+=	-mcpu=$(ARCH) -mfloat-abi=soft
XCFLAGS+=	-D=__$(PART)__ -D=TYPE_$(FAMILY)
XCFLAGS+=	-Wno-format-y2k
XCFLAGS+=	-W -Wpointer-arith
XCFLAGS+=	-Wreturn-type -Wswitch -Wshadow -Wcast-align
XCFLAGS+=	-Wbad-function-cast -Wchar-subscripts
# some of the ASF spits tons of such unused-parameter warnings
XCFLAGS+=	-Wno-unused-parameter
XCFLAGS+=	-fstrict-aliasing

XCFLAGS+=	-Iconfig
XCFLAGS+=	-I.

.if $(FAMILY) == SAMD20
XCFLAGS+=	-I$(ASF)/sam0/utils/cmsis/samd20/include
XCFLAGS+=	-I$(ASF)/sam0/utils/cmsis/samd20/source/
.elif $(FAMILY) == SAMD21
XCFLAGS+=	-I$(ASF)/sam0/utils/cmsis/samd21/include
XCFLAGS+=	-I$(ASF)/sam0/utils/cmsis/samd21/source/
.endif
XCFLAGS+=	-I$(ASF)/thirdparty/CMSIS/Include
XCFLAGS+=	-I$(ASF)/sam0/utils
XCFLAGS+=	-I$(ASF)/common/utils
XCFLAGS+=	-I$(ASF)/sam0/utils/preprocessor
XCFLAGS+=	-I$(ASF)/sam0/utils/header_files
XCFLAGS+=	-I$(ASF)/sam0/drivers/system/clock
.if $(FAMILY) == SAMD20
XCFLAGS+=	-I$(ASF)/sam0/drivers/system/clock/clock_samd20
.elif $(FAMILY) == SAMD21
XCFLAGS+=	-I$(ASF)/sam0/drivers/system/clock/clock_samd21_r21_da_ha1
.endif
XCFLAGS+=	-I$(ASF)/sam0/drivers/system
XCFLAGS+=	-I$(ASF)/sam0/drivers/system/pinmux
XCFLAGS+=	-I$(ASF)/sam0/drivers/system/interrupt
.if $(FAMILY) == SAMD20 || $(FAMILY) == SAMD21
XCFLAGS+=	-I$(ASF)/sam0/drivers/system/power/power_sam_d_r_h
XCFLAGS+=	-I$(ASF)/sam0/drivers/system/reset/reset_sam_d_r_h
.endif
.if $(FAMILY) == SAMD20
XCFLAGS+=	-I$(ASF)/sam0/drivers/system/interrupt/system_interrupt_samd20
.elif $(FAMILY) == SAMD21
XCFLAGS+=	-I$(ASF)/sam0/drivers/system/interrupt/system_interrupt_samd21
.endif
XCFLAGS+=	-I$(ASF)/common/services/usb
XCFLAGS+=	-I$(ASF)/common/services/usb/udc
XCFLAGS+=	-I$(ASF)/common/services/usb/class/cdc
XCFLAGS+=	-I$(ASF)/common/services/usb/class/cdc/device
XCFLAGS+=	-I$(ASF)/sam0/drivers/usb
XCFLAGS+=	-I$(ASF)/sam0/drivers/extint
XCFLAGS+=	-I$(ASF)/sam0/drivers/port
XCFLAGS+=	-I$(ASF)/common/services/sleepmgr

.if defined(USB_SERIAL)
XCFLAGS+=       -DUSB_SERIAL=\"${USB_SERIAL}\"
.endif

SRCS+=		$(ASF)/sam0/drivers/system/system.c
.if $(FAMILY) == SAMD20
SRCS+=		$(ASF)/sam0/drivers/system/clock/clock_samd20/clock.c
SRCS+=		$(ASF)/sam0/drivers/system/clock/clock_samd20/gclk.c
.elif $(FAMILY) == SAMD21
SRCS+=		$(ASF)/sam0/drivers/system/clock/clock_samd21_r21_da_ha1/clock.c
SRCS+=		$(ASF)/sam0/drivers/system/clock/clock_samd21_r21_da_ha1/gclk.c
.endif
SRCS+=		$(ASF)/common/utils/interrupt/interrupt_sam_nvic.c
.if $(FAMILY) == SAMD20
SRCS+=		$(ASF)/sam0/utils/cmsis/samd20/source/gcc/startup_samd20.c
.elif $(FAMILY) == SAMD21
SRCS+=		$(ASF)/sam0/utils/cmsis/samd21/source/gcc/startup_samd21.c
.endif
SRCS+=		$(ASF)/common/services/usb/udc/udc.c
SRCS+=		$(ASF)/sam0/drivers/usb/stack_interface/usb_device_udd.c
SRCS+=		$(ASF)/sam0/drivers/usb/usb_sam_d_r/usb.c
SRCS+=		$(ASF)/sam0/drivers/system/pinmux/pinmux.c
SRCS+=		$(ASF)/sam0/drivers/usb/stack_interface/usb_dual.c
SRCS+=		$(ASF)/common/services/sleepmgr/samd/sleepmgr.c
SRCS+=		$(ASF)/common/services/usb/class/cdc/device/udi_cdc_desc.c
SRCS+=		$(ASF)/common/services/usb/class/cdc/device/udi_cdc.c

XCFLAGS+=	-I$(FREERTOS)/FreeRTOS/Source/include
XCFLAGS+=	-I$(FREERTOS)/FreeRTOS/Source/portable/GCC/ARM_CM0

SRCS+=		$(FREERTOS)/FreeRTOS/Source/portable/GCC/ARM_CM0/port.c
SRCS+=		$(FREERTOS)/FreeRTOS/Source/tasks.c
SRCS+=		$(FREERTOS)/FreeRTOS/Source/queue.c
SRCS+=		$(FREERTOS)/FreeRTOS/Source/list.c
SRCS+=		$(FREERTOS)/FreeRTOS/Source/timers.c
SRCS+=		$(FREERTOS)/FreeRTOS/Source/portable/MemMang/heap_2.c

CFLAGS=		-mthumb $(XCFLAGS)
LDFLAGS+=	-nostartfiles
LDFLAGS+=	-T ldscripts/$(LDSCRIPT) -Wl,-Map=$(BIN).map,--cref,--no-warn-mismatch

#
#
#

.SUFFIXES: .c .h .o

OBJS= $(SRCS:.c=.o)

all:		$(BIN).rom $(BIN).bin

clean:
		rm -f $(OBJS) $(BIN).rom $(BIN).map $(BIN) $(BIN).bin

$(BIN):		$(OBJS)
		$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(BIN)

$(BIN).rom:	$(BIN)
		$(OBJCOPY) -j .text -j .data -O ihex $(BIN) $(BIN).rom

$(BIN).bin:	$(BIN)
		$(OBJCOPY) -j .text -j .data -O binary $(BIN) $(BIN).bin

prog:
		openocd -f interface/cmsis-dap.cfg -f $(OPENOCDBRD) -c "program main verify reset exit"

debug:
		openocd -f interface/cmsis-dap.cfg -f $(OPENOCDBRD)

gdb:
		arm-none-eabi-gdb -x "target extended-remote wb1:3333" main

