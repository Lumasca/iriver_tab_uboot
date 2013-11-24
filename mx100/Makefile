#
# uboot mx100 sys routine.
# jhlim

include $(TOPDIR)/config.mk

LIB	= $(obj)libmx100.a

CONFIG_MX100=y

COBJS-$(CONFIG_MX100) += _gfx.o _i2c.o _tools.o _sys.o _gpio.o _gpio_i2c.o _main.o melfas_download.o _timer.o

#intc.o jpeg.o jpeg_test.o

COBJS	:= $(COBJS-y)
SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

all:$(LIB)

$(LIB):	$(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend
