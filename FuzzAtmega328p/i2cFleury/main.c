/*
	Educational purpose only
	tuomokup
	AVR ATMEGA328p with external 16Mhz crystal voltage measuring using GROVE LCD RED/BLUE V2.0
	Many thanks to Peter Fleury and Grove
*/
#ifndef F_CPU						// if F_CPU was not defined in Project -> Properties
#define F_CPU 16000000UL			// define it now as 16 MHz unsigned long
#endif

#include <avr/io.h>				// this is always included in AVR programs
#include <util/delay.h>			// add this to use the delay function
#include "i2cmaster.h"
#include "rgb_lcd.h"

#define addr 0x3E      // device address of Grove LCD, see datasheet

//#include <stdlib.h>       //for itoa()

int main(void)
{
	_delay_ms(30);
	i2c_init();                             // initialize I2C library
	
	_delay_ms(2);
	// write 0x75 to EEPROM address 5 (Byte Write)
	i2c_start_wait(addr<<I2C_WRITE);     // set device address and write mode
	_delay_us(20);
	i2c_write(LCD_SETDDRAMADDR);
	_delay_us(20);
	i2c_write(0x28);	//Brightness
	i2c_stop();
	_delay_ms(2);
	
	INSTRUCTIONset(addr, LCD_SETDDRAMADDR, 0x0C);
	INSTRUCTIONset(addr, LCD_SETDDRAMADDR, LCD_CLEARDISPLAY);
	INSTRUCTIONset(addr, LCD_SETDDRAMADDR, 0x06);  //INPUT SET
	_delay_ms(2);
	
	adc_init();
	while(1)
	{
		//READING FIRST VALUE
		float analogValue = read_adc(0); // READ PC0 pin
		_delay_ms(20);
		float n = 12*analogValue/1024;
		char buf[20];
		ftoa(n,buf,2); // ftoa Converts a floating-point/double number to a string.
		_delay_ms(20);
		INSTRUCTIONset(addr, LCD_SETDDRAMADDR, 0x03);  // Shift to the first line
		_delay_ms(20);
		Print(addr, "Q1:-");
		_delay_ms(20);
		Print(addr, buf);
		Print(addr, " ");
		
		//READING SECOND VALUE
		INSTRUCTIONset(addr, LCD_SETDDRAMADDR, 0xC0);  // Shift to the second line
		analogValue = read_adc(1); // READ PC1 pin
		_delay_ms(20);
		n = 12*analogValue/1024;
		ftoa(n,buf,2);
		_delay_ms(20);
		Print(addr, "Q2:-");
		Print(addr, buf);
		
		//READING THIRD VALUE
		analogValue = read_adc(2); // READ PC2 pin
		_delay_ms(20);
		n = 12*analogValue/1024;
		ftoa(n,buf,2);
		_delay_ms(20);
		Print(addr, " V:");
		Print(addr, buf);
		Print(addr, " ");
		_delay_ms(20);
	}
}