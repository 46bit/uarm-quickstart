#include <stdio.h>
#include <stdlib.h>
#include "arduino-serial-lib.c"

int main()
{
	int fd = serialport_init("/dev/tty.usbserial-A6031NAG", 9600);
	char buf[1000];
	serialport_read_until(fd, buf, '\n', 1000, 10000);
	printf("%s\n", buf);

	while(1)
	{
		size_t bufsize;
		char *linebuf = NULL;
		int gls = getline((char **) &linebuf, &bufsize, stdin);

		if (gls == 0) continue;
		if (gls == -1) break;

		printf("%s\n", linebuf);
		serialport_write(fd, linebuf);
		free(linebuf);
	}
}
