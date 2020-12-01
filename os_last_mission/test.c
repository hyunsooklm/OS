#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>


#define MY_IOCTL_NUMBER 100
#define MY_IOCTL_READ _IOR(MY_IOCTL_NUMBER,0,int)
#define MY_IOCTL_WRITE _IOW(MY_IOCTL_NUMBER,1,int)

#define BUFF_SIZE 1024

int main(){

	int fd;
	int debug_flag=0;
	char *str_hello="Hello, VIrtual BUffer!!";
	char buff[BUFF_SIZE];
	printf("Device driver test.\n");
	
	if((fd=open("/dev/virtual_buffer",O_RDWR))>0)
	{	
		printf("before ioctl_read debug:%d\n",debug_flag);
		ioctl(fd,MY_IOCTL_READ,&debug_flag);
		printf("ioctl_read-debug:%d\n",debug_flag);
		debug_flag=debug_flag==1?0:1;
		//debug_flag!=debug_flag;
		printf("ioctl_write_debug:%d\n",debug_flag);
		ioctl(fd,MY_IOCTL_WRITE,&debug_flag);
		write(fd,str_hello,strlen(str_hello)+1);
		read(fd,buff,BUFF_SIZE);
		printf("read from device:%s\n",buff);
		close(fd);
	}
	/*
	if((fd=open("/dev/virtual_buffer",O_RDWR))>0)
	{
		ioctl(fd,MY_IOCTL_READ,&debug_flag);
		debug_flag=!debug_flag;
		ioctl(fd,MY_IOCTL_WRITE,&debug_flag);
		close(fd);
	}*/
	return 0;
}
