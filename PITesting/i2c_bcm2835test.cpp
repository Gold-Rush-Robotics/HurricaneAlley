#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include "bcm2835.h"
#include "pca9685.h"



int main()
{
bcm2835_i2c_begin();
int length = 10;

char buffer[10]={0};
char[]* address = &buffer;

bcm2835_i2c_setSlaveAddress(0x29);
	
bcm2835_i2c_read(address , length);


for(int i=0 ; i<3 ; ++i)
{
  cout << buffer[i];
}
return 0;
}