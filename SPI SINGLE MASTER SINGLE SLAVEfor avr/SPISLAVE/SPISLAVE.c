#include<avr/io.h>
#define F_CPU 8000000
#include<util/delay.h>
#include<stdio.h>
#include<lcdavr.h>
 
#define MOSI PB5
#define MISO PB6
#define SCK PB7
#define SS PB4
 
void SPI_Init()					/* SPI Initialize function */
{
	DDRB &= ~((1<<MOSI)|(1<<SCK)|(1<<SS));  /* Make MOSI, SCK, SS as
 						input pins */
	DDRB |= (1<<MISO);			/* Make MISO pin as 
						output pin */
	SPCR = (1<<SPE);			/* Enable SPI in slave mode */
}


char SPI_Receive()			/* SPI Receive data function */
{
	while(!(SPSR & (1<<SPIF)));	/* Wait till reception complete */
	return(SPDR);			/* Return received data */
}

int main(void)
{
	uint8_t count;
	char buffer[5];
	DDRA=0XFF;
	
	lcd_init();
	SPI_Init();
	
	lcd_string("Slave Device");
	lcd_command(0xc0);
	lcd_string("Receive Data:    ");
	while (1)			/* Receive count continuous */
	{
		count = SPI_Receive();
		sprintf(buffer, "%d   ", count);
		lcd_command(0xce);
		lcd_string(buffer);
	}

}
