#include <stdio.h>
#include <stdlib.h>
#include "arduino-serial-lib.c"

int main()
{
	int fd = serialport_init("/dev/tty.usbserial-A6031NAG", 9600);
	char buf[1000];
	serialport_read_until(fd, buf, '\n', 1000, 10000);
	printf("%s\n", buf);

	// Move small from middle to far
	serialport_write(fd, "-60 140 210 0 0");
	sleep(2);
	serialport_write(fd, "-60 140 10 0 0");
	sleep(2);
	serialport_write(fd, "-60 140 10 0 1");
	sleep(2);
	serialport_write(fd, "-60 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-30 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-30 140 -20 0 1");
	sleep(2);
	serialport_write(fd, "-30 140 -20 0 0");
	sleep(2);
	serialport_write(fd, "-30 140 210 0 0");
	sleep(2);

	// Move medium from middle to close
	serialport_write(fd, "-60 140 210 0 0");
	sleep(2);
	serialport_write(fd, "-60 140 -20 0 0");
	sleep(2);
	serialport_write(fd, "-60 140 -20 0 1");
	sleep(2);
	serialport_write(fd, "-60 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-90 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-90 140 -20 0 1");
	sleep(2);
	serialport_write(fd, "-90 140 -20 0 0");
	sleep(2);
	serialport_write(fd, "-90 140 210 0 0");
	sleep(2);

	// Move small from far to close
	serialport_write(fd, "-30 140 210 0 0");
	sleep(2);
	serialport_write(fd, "-30 140 -20 0 0");
	sleep(2);
	serialport_write(fd, "-30 140 -20 0 1");
	sleep(2);
	serialport_write(fd, "-30 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-90 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-90 140 -10 0 1");
	sleep(2);
	serialport_write(fd, "-90 140 -10 0 0");
	sleep(2);

	// Move big from middle to far
	serialport_write(fd, "-60 140 210 0 0");
	sleep(2);
	serialport_write(fd, "-60 140 -20 0 0");
	sleep(2);
	serialport_write(fd, "-60 140 -20 0 1");
	sleep(2);
	serialport_write(fd, "-60 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-30 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-30 140 -20 0 1");
	sleep(2);
	serialport_write(fd, "-30 140 -20 0 0");
	sleep(2);
	serialport_write(fd, "-30 140 210 0 0");
	sleep(2);

	// Move small from close to middle
	serialport_write(fd, "-90 140 210 0 0");
	sleep(2);
	serialport_write(fd, "-90 140 -20 0 0");
	sleep(2);
	serialport_write(fd, "-90 140 -20 0 1");
	sleep(2);
	serialport_write(fd, "-90 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-60 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-60 140 -20 0 1");
	sleep(2);
	serialport_write(fd, "-60 140 -20 0 0");
	sleep(2);
	serialport_write(fd, "-60 140 210 0 0");
	sleep(2);

	// Move medium from close to far
	serialport_write(fd, "-90 140 210 0 0");
	sleep(2);
	serialport_write(fd, "-90 140 -20 0 0");
	sleep(2);
	serialport_write(fd, "-90 140 -20 0 1");
	sleep(2);
	serialport_write(fd, "-90 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-30 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-30 140 -20 0 1");
	sleep(2);
	serialport_write(fd, "-30 140 -20 0 0");
	sleep(2);
	serialport_write(fd, "-30 140 210 0 0");
	sleep(2);

	// @TODO: Arm seems to bug out and reset without this move!
	serialport_write(fd, "30 140 210 0 0");
	sleep(2);

	// Move small from middle to far
	serialport_write(fd, "-60 140 210 0 0");
	sleep(2);
	serialport_write(fd, "-60 140 -20 0 0");
	sleep(2);
	serialport_write(fd, "-60 140 -20 0 1");
	sleep(2);
	serialport_write(fd, "-60 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-30 140 210 0 1");
	sleep(2);
	serialport_write(fd, "-30 140 10 0 1");
	sleep(2);
	serialport_write(fd, "-30 140 10 0 0");
	sleep(2);
	serialport_write(fd, "-30 140 210 0 0");
	sleep(2);
	serialport_write(fd, "-60 140 210 0 0");
}
