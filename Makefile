##CC := gcc
CROSS_COMPILE = arm-none-linux-gnueabi-
CC := $(CROSS_COMPILE)gcc
CFLAGS = -g
#LDFLAGS =
LIBS = -lpthread
PRJ_ROOT = /work/rootfs/root/led_package
INC_DIRS = -I$(PRJ_ROOT)/include
PREFIX = /work/rootfs/usr/bin

## Objects and the Target name
OBJS = dev-test.o

TARGET = dev-test

.SUFFIXES: .c .o

all: $(TARGET) 

%.o : %.c
	$(CC) $(CFLAGS) $(DBG_FLAGS) $(INC_DIRS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS) 

clean:
	rm -f $(OBJS) $(TARGET)

install:
	install -m 0755 $(TARGET) $(PREFIX)

#dev-test.o: dev-test.c dev-test.h
