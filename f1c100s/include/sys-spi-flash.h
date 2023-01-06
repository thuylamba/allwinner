#ifndef __SYS_SPI_FLASH_H__
#define __SYS_SPI_FLASH_H__
void sys_spi_flash_init(void);
void sys_spi_flash_read(int addr, void * buf, int count);
void sys_spi_flash_write(int addr, void * data, int cnt);
#endif	//	__SYS_SPI_FLASH_H__
