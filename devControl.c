/* ********************************************
 * devControl.c 
 * Author: Youngdeok (medusakiller@gmail.com)
 * Desc: a library source
 * create: arm...ar rscv lib{name}.a {name}.o
 * ********************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "led-test.h"

int fd;
int err;
int data;

// implementation of the library
int device_init() {

    fd = open("/dev/led-driver", O_RDWR);
	if (fd < 0) {
        fd = open("/dev/misc/led-driver", O_RDWR);
        if (fd < 0) {
            printf("open driver failed\n");
            return -1;
			exit(1);
        }
    }
	printf("[lib] device open sucessful.\n");
	return 0;
}
            
void led_on() {
#ifdef DEBUG
	printf("[lib] ioctl(..DEV_LED_ON..)\n");
#endif
	if(ioctl(fd, DEV_LED_ON, NULL) < 0) {
		perror("ioctl error"); // for errno
	}
}

void led_off() {
#ifdef DEBUG
	printf("[lib] ioctl(..DEV_LED_OFF..)\n");
#endif
	if(ioctl(fd, DEV_LED_OFF, NULL) < 0) {
		perror("ioctl error");
	}

}

int getLed3State() {
#ifdef DEBUG
	printf("[lib] ioctl(..DEV_GETSTATE..)\n");
#endif
	if(ioctl(fd, DEV_GETSTATE, &data) < 0) {
		perror("ioctl error");
	}
	return data;
}

void device_close() {
	close(fd);
	printf("close the device\n");
}

