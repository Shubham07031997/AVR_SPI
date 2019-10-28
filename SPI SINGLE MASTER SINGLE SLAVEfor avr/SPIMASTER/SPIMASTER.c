#define F_CPU 8000000UL			/* Define CPU Frequency 8MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include Delay header file */
#include <stdio.h>			/* Include Std. i/p o/p file */
#include<lcdavr.h>

#define MOSI PB5
#define MISO PB6
#define SCK PB7
#define SS PB4

void SPI_Init()					/* SPI Initialize function */
{
	DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS);	/* Make MOSI, SCK, SS 
						as Output pin */
	DDRB &= ~(1<<MISO);			/* Make MISO pin 
						as input pin */
	PORTB |= (1<<SS);			/* Make high on SS pin */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);	/* Enable SPI in master mode
						with Fosc/16 */
	SPSR &= ~(1<<SPI2X);			/* Disable speed doubler */
}

void SPI_Write(char data)		/* SPI write data function */
{
	char flush_buffer;
	SPDR = data;			/* Write data to SPI data register */
	while(!(SPSR & (1<<SPIF)));	/* Wait till transmission complete */
	flush_buffer = SPDR;		/* Flush received data */

/* Note: SPIF flag is cleared by first reading SPSR (with SPIF set) and then accessing SPDR hence flush buffer used here to access SPDR after SPSR read */
}

char SPI_Read()				/* SPI read data function */
{
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));	/* Wait till reception complete */
	return(SPDR);			/* Return received data */
}


int main(void)
{
DDRA=0XFF;
	uint8_t count;
	char buffer[5];
	
	lcd_init();
	SPI_Init();

	lcd_string("MASTER DEVICE");
	lcd_command(0xc0);
	lcd_string("SENDING DATA:-");
	
	
	//SS_Enable;
	count = 0;
	while (1)			/* Send Continuous count */
	{
		SPI_Write(count);
		sprintf(buffer, "%d   ", count);
		lcd_command(0xce);
		lcd_string(buffer);
		count++;
		_delay_ms(100);
	}
		
}
