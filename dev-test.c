/* ********************************************
 * dev-test.c 
 * Author: Youngdeok
 * Desc: system call function test 
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

int main( int argc, char **argv ) {
    int fd;
    int err;
    int msgtype = 0;
	int msg = 0;
    int i;
	int data;

	// to get command line arguments
	if( argc<2 ) {
		printf("USAGE: dev-test [0|1|2|3]\n");
		printf("0:block/wakeup test, 1:led on, 2:led off, 3:get state test\n");
		exit(1);
	}

    msgtype = atoi(argv[1]);

	// device open
    fd = open("/dev/led-driver", O_RDWR);
	if (fd < 0) {
        fd = open("/dev/misc/led-driver", O_RDWR);
        if (fd < 0) {
            printf("open driver failed\n");
            return -1;
        }
    }
   
	// POINT: to interpret msgtype and cmd
	switch (msgtype) {

		case 0:
			while(1) {
				msg = 0;
				// POINT: blocking mode, just wait...
            	printf("[user] read() Blocking...\n");
				read(fd, (void *)&msg, sizeof(int) );
				printf("[user]read from the kernel(msg) = %d\n", msg);
				fflush(stdout);
				if ( msg == 9) break; // condition value
			}
			break;
		
		case 1:
            printf("[user] ioctl(..DEV_LED_ON..)\n");
			ioctl(fd, DEV_LED_ON, &msgtype);
			break;
		
		case 2:
            printf("[user] ioctl(..DEV_LED_OFF..)\n");
			ioctl(fd, DEV_LED_OFF, &msgtype);
			break;
	
		case 3:
            printf("[user] ioctl(..DEV_GETSTATE, &data)\n");
			ioctl(fd, DEV_GETSTATE, &data);
			printf("[user] data from kernel = %d\n", data);
			break;

		default:
			break;
	}
    
	close(fd);

    return 0;
}

