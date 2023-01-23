#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int8_t write_data_buffer1[1024];

int main()
{
	int fd;

	fd = open("/dev/file2_device",O_RDWR);
	if(fd < 0)
	{
		printf("Unable to open the file\n");
		return -1;
	}
	else
		printf("File opened successfully from location /dev/");

	printf("Enter the data to write: ");
	scanf("%s",write_data_buffer1);
	write(fd,write_data_buffer1,sizeof(write_data_buffer1));
	printf("Data write done.\n");

	close(fd);

	return 0;
}
