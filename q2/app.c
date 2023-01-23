#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define WR_DATA _IOW('a','a',int32_t *)
#define RD_DATA _IOR('a','b',int32_t *)


int8_t write_buf[1024];
int8_t read_buf[1024];

int main()
{
        int fd;
        
        fd = open("/dev/kernel_module", O_RDWR);       
        if(fd > 0) 
                printf("Device File Opened successfully...\n");
        
        else 
                printf("Cannot open device file...\n");
                
        printf("Enter to write data:");
        scanf("  %[^\t\n]s", write_buf);
        printf("Data Writing ...");
        ioctl(fd, WR_DATA,write_buf);
        printf("Done!\n");
                        
                        
        printf("Data Reading ...");
        ioctl(fd, RD_DATA,read_buf);
        printf("Done!\n\n");
        printf("Data = %s\n\n", read_buf);
        
        close(fd);
        
        return 0;
}
