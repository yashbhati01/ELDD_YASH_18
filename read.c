#include<stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int8_t read_data_buffer[1024];

int main()
{
	int fd = 0;

	fd = open("/dev/file1_device",O_RDWR);
	if(fd < 0)
	{
		printf("Unable to open file\n");
		return -1;
	}
	else
		printf("File opened successfully\n");

	printf("Reading data...\n");
	read(fd,read_data_buffer,sizeof(read_data_buffer));
	printf("Data read is: %s",read_data_buffer);
	printf("Read data successfully/n");

	close(fd);

	return 0;
}
