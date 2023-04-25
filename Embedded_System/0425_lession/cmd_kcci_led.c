#include <common.h>
#include <command.h>
#include <asm/io.h>

#define IOMUXC_SW_MUX_CTL_PAD_SD1_DATA0 0x020E0340
#define IOMUXC_SW_MUX_CTL_PAD_SD1_DATA1 0x020E033C
#define IOMUXC_SW_MUX_CTL_PAD_SD1_CMD 0x020E0348
#define IOMUXC_SW_MUX_CTL_PAD_SD1_DATA2 0x020E034C
#define GPIO1_DR                  0x0209C000
#define GPIO1_GDIR                0x0209C004
#define GPIO1_PSR                 0x0209C008

void led_init(void)
{
	unsigned long temp;
	temp = 0x05;
	writel(temp,IOMUXC_SW_MUX_CTL_PAD_SD1_DATA0);
    writel(temp,IOMUXC_SW_MUX_CTL_PAD_SD1_DATA1);
    writel(temp,IOMUXC_SW_MUX_CTL_PAD_SD1_CMD);
    writel(temp,IOMUXC_SW_MUX_CTL_PAD_SD1_DATA2);

	temp = 0x0f << 16;
	temp = readl(GPIO1_GDIR) | temp;
	writel(temp,GPIO1_GDIR);

	temp = ~(0x0f << 16);
	temp = readl(GPIO1_DR) & temp;
	writel(temp,GPIO1_DR);
}
void led_write(unsigned long led_data)
{
	led_data = led_data <<16;
	writel(led_data, GPIO1_DR);
}

void led_status(unsigned long led_data)
{
  int i;
  char data[5];
  unsigned long temp = 0x01;
  unsigned long n = led_data;
  for(i=0; i < 4; i++)
  {
	 unsigned long m = n & temp;
    	if(m)
	 {
      	data[i] = 'O';
 	 }
    	else if(m==0)
	 {
	   	data[i] = 'X';
     }
    	temp = temp << 1;
  }
  data[4] = '\0';
  printf("%s\n", data);
  
}


static int do_KCCI_LED(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	unsigned long led_data;
	if(argc !=2)
	{
		cmd_usage(cmdtp);
		return 1;
	}
	printf("*LED TEST START(작성자 : KYH)\n");
	led_init();
	led_data = simple_strtoul(argv[1],NULL,16);
	led_read(&led_data);
	led_status(led_data);
	printf("*LED TEST END(%s : %#04x)\n\n", argv[0],(unsigned int)led_data);

	return 0;
}

U_BOOT_CMD(
		led,2,0,do_KCCI_LED,
		"led - kcci LED Test",
		"number - Input argument is only one.(led [0x00~0x0f])\n");


