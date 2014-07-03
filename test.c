#include <stdio.h>
#include <stdlib.h>
#include "arduino-serial-lib.c"

int main()
{
	int fd = serialport_init("/dev/tty.usbserial-A6031NAG", 9600);
	char buf[1000];
	serialport_read_until(fd, buf, '\n', 1000, 10000);
	printf("%s\n", buf);

	int i;
	for (i = 0; i < 17; i++) {
		// arm rotation, arm stretch, arm height, hand rotation, gripping
		char spfbuf[40];
		sprintf(spfbuf, "%d 120 100 0 %d\n", -90 + i * 10, i % 2);
		serialport_write(fd, spfbuf);

		sleep(2);
	}
}
