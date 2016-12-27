// one wire interface for ds18b20
//
// compiler: avr tools
// device:   atmega328p @ 8MHz
//
// 27-12-16
//
// file: trm.h
//


#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#ifndef SET_BIT
#define SET_BIT(reg, bit) (reg |= (1<<bit))
#endif
#ifndef CLR_BIT
#define CLR_BIT(reg, bit) (reg &= (~(1<<bit)))
#endif


//*****************************************************************************
#define TRM_PORT_REG  PORTC
#define TRM_DDR_REG   DDRC
#define TRM_PIN_REG   PINC
#define TRM_PIN       1


#define TRM_INPUT   CLR_BIT(TRM_DDR_REG, TRM_PIN)
#define TRM_OUTPUT  SET_BIT(TRM_DDR_REG, TRM_PIN)
#define TRM_LOW     CLR_BIT(TRM_PORT_REG, TRM_PIN)
#define TRM_HIGH    SET_BIT(TRM_PORT_REG, TRM_PIN)

#define TRM_READ    (TRM_PIN_REG & (1<<TRM_PIN))


//-----------------------------------------------------------------------------
uint8_t trm_reset()
	{
	TRM_LOW;
	TRM_OUTPUT;
	_delay_us(480);

	TRM_INPUT;
	_delay_us(60);

	uint8_t i = TRM_READ;
	_delay_us(420);

	return i;  //0 = OK
	}


//-----------------------------------------------------------------------------
void trm_write_bit(uint8_t bit)
	{
	cli();
	TRM_LOW;
	TRM_OUTPUT;
	_delay_us(1);

	if(bit) TRM_INPUT;

	_delay_us(60);
	TRM_INPUT;
	sei();
	}


//-----------------------------------------------------------------------------
uint8_t trm_read_bit(void)
	{
	uint8_t bit=0;

	cli();
	TRM_LOW;
	TRM_OUTPUT;
	_delay_us(1);

	TRM_INPUT;
	_delay_us(14);

	if(TRM_READ) bit=1;

	_delay_us(45);

	sei();
	return bit;
	}


//-----------------------------------------------------------------------------
uint8_t trm_read_byte(void) //read byte
	{
	uint8_t b=0;

	for(uint8_t k=0; k<8; k++) b|=(trm_read_bit()<<k);

	return b;
	}


//-----------------------------------------------------------------------------
void trm_write_byte(uint8_t byte) //send byte
	{
	for(uint8_t mask=1; mask; mask<<=1) trm_write_bit(byte&mask);
	}

