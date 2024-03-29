#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_FILENAME  "/dev/ledkey_dev"

void print_led(unsigned char);
void print_key(unsigned char);
int main(int argc,char * argv[])
{
    int dev;
    char buff = 0;
	char oldBuff = 0;
    int ret;
	if(argc < 2)
	{
		printf("USAGE : %s [ledval] \n",argv[0]);
		return 1;
	}
	buff = atoi(argv[1]);

    dev = open( DEVICE_FILENAME, O_RDWR );
	if(dev<0)
	{
		perror("open()");
		return 2;
	}
    ret = write(dev,&buff,sizeof(buff));
	if(ret < 0)
	{
		perror("write()");
		return 3;
	}
	print_led(buff);
	
	buff = 0;
	do {
		read(dev,&buff,sizeof(buff));
		if((buff != 0) && (oldBuff != buff))
		{
			printf("key : %d\n",buff);
			print_key(buff);
    		write(dev,&buff,sizeof(buff));
			print_led(buff);
			oldBuff = buff;
			if(buff == 8) //key:8
				break;
		}
	} while(1);


    close(dev);
    return 0;
}
void print_led(unsigned char led)
{
	int i;
	puts("1:2:3:4");
	for(i=0;i<=3;i++)
	{
		if(led & (0x01 << i))
			putchar('O');
		else
			putchar('X');
		if(i < 3 )
			putchar(':');
		else
			putchar('\n');
	}
	return;
}

void print_key(unsigned char key)
{
	int i;
	puts("1:2:3:4:5:6:7:8");
	for(i=0;i<=7;i++)
	{
		if(i+1 == key)
			putchar('O');
		else
			putchar('X');

		if(i < 7 )
			putchar(':');
		else
			putchar('\n');
	}
	return;
}
