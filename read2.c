#include<stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int8_t read_data_buffer1[1024];

int main()
{
	int fd = 0;

	fd = open("/dev/file2_device",O_RDWR);
	if(fd < 0)
	{
		printf("Unable to open file\n");
		return -1;
	}
	else
		printf("File opened successfully\n");

	printf("Reading data...\n");
	read(fd,read_data_buffer1,sizeof(read_data_buffer1));
	printf("Data read is: %s",read_data_buffer1);
	printf("Read data successfully/n");

	close(fd);

	return 0;
}
