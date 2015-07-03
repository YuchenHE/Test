#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <string.h>
#include <linux/spi/spidev.h>

#include "RegCtl_SpiHw.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

//uint8_t SPI1_send(uint8_t data);
//void SPI1_GetReg(uint8_t *buf);

static const char *device = "/dev/spidev1.0";
static uint8_t mode = 0;  // CPHA=0, CPOL=0, CS_LOW, MSB_FIRST
static uint32_t speed = 1000000; //1 MHz

int fd = 0;

/* Open /dev/spidev1.0, Set SPI mode parameters,*/
void RegCtl_SpiHwInit(void)
{
	int ret = 0;
	//int fd = 0;
	
	fd = open(device, O_RDWR);
	if (fd < 0){
		perror("can't open device");
		return;
	}
	
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1){
		perror("can't set spi mode");
		return;
	}
	
	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1){
		perror("can't get spi mode");
		return;
	}
	
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1){
		perror("can't set max speed hz");
		return;
	}

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1){
		perror("can't get max speed hz");
		return;
	}

	printf("spi mode: %d\n", mode);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
}


uint16_t RegCtl_SpiHwGetReg(uint8_t address, uint8_t *reg_data)
{
	uint8_t buff[2];
	//buff[0] =address; /* buff[1] is set in SPI1_GetReg as 0x01*/
	buff[0] = (uint8_t) (address<<1)| 0x01U; /* Debug */
	
	//TODO
	//..
	SPI1_GetReg(buff);
	reg_data[1]= buff[1];
	
	return 0;
}

void SPI1_GetReg(uint8_t *buf)
{
	/*dummy = */SPI1_send(*buf);
	buf++;
	*buf = SPI1_send(0x01);
}

uint8_t SPI1_send(uint8_t data)
{
	struct spi_ioc_transfer xfer[2];
	uint8_t buf[2], *bp;
	int status, len;
	
	memset(xfer, 0, sizeof xfer);
	memset(buf, 0, sizeof buf);
	
	len = 1;
	buf[0] = data;
	xfer[0].tx_buf = (unsigned long)buf;
	xfer[0].len = 1;
	xfer[1].rx_buf = (unsigned long) buf;
	xfer[1].len = 1;
	
	status = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
	if (status < 0){
		perror("SPI_IOC_MESSAGE");
		return 0xFF;
	}
	printf("TX Byte:  %02x RX Byte:  %02x\n", data, buf[0]);
	
	return 0;
}


