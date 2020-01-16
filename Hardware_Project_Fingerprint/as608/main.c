/*
 * as608.c
 *
 * Created: 19/10/2018 22:59:58
 * Author : Pavindu Lakshan
 * Fingerprint code running on atmega328
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "r305.h"
#include "lcd.h"

#define MUX_A PC4
#define MUX_B PC5

#define FINGER_ENABLE 1
#define GSM_ENABLE 2

#define enroll_key_press() ((PINC & 0x01) == 0)
#define finger_Scann_Key_press() ((PINC & 0x04) == 0)

void keys_init();
void finger_enrollment();
void finger_scan();
void process_result(uint16_t ret);

void mux(int);

void spi_init_master (void);
char spi_tranceiver(char data);


int main(void)
{
	LCDInit(LS_NONE);
	finger_init();
	keys_init();
	DDRC |= 1<<PINC5;
	PORTC |= 1<<PINC5 | 1<<PINC4;
	
	mux(FINGER_ENABLE);
	LCDClear();
    while (1) 
    { 
		LCDWriteStringXY(0,0,"Welcome!");
		_delay_ms(2000);
		
		finger_scan();
		//finger_enrollment();
		//while(finger_read()!=0x00);
			
		
    }
}

void mux(int val){
	if(val == FINGER_ENABLE){
		PORTC |= (1<<MUX_A);	
		PORTC &= ~(1<<MUX_B);
	}else{
		PORTC &= ~(1<<MUX_A);
		PORTC &= ~(1<<MUX_B);
	}
}

void keys_init()
{
	DDRC &= ~(1<<PINC0); //init Enroll Key as input
	DDRC &= ~(1<<PINC2); //init Scan key as input
	PORTC |= 1<<PINC0;
	PORTC |= 1<<PINC2;
	
}

void finger_enrollment()
{
	LCDClear();
	//LCDWriteStringXY(0,0,"Deleting");
	//process_result(finger_delete_all());
	LCDClear();
	LCDWriteStringXY(0,0,"Enrollment");
	LCDWriteStringXY(0,1,"Place Finger");
	_delay_ms(2000);
	LCDClear();
	LCDWriteStringXY(0,0,"Finger Reading 1");
	process_result(finger_read());
	LCDClear();
	LCDWriteStringXY(0,0,"Generating char")
	process_result(finger_generate_char_file(1));
	LCDClear();
	LCDWriteStringXY(0,0,"Finger Reading 2");
	process_result(finger_read());
	LCDClear();
	LCDWriteStringXY(0,0,"Generating char")
	process_result(finger_generate_char_file(2));
	LCDClear();
	LCDWriteStringXY(0,0,"Generate temp");
	process_result(finger_generate_template());
	LCDClear();
	LCDWriteStringXY(0,0,"Storing..");
	process_result(finger_store(2));
	_delay_ms(4000);	
	LCDClear();
}


void finger_scan()
{   uint16_t code;
	LCDClear();
	LCDWriteStringXY(0,0,"Welcome!");
	LCDWriteStringXY(0,1,"Place finger");
	process_result(finger_read());
	LCDClear();
	LCDWriteStringXY(0,0,"Scanning...");
	_delay_ms(1000);
	LCDWriteStringXY(0,1,"Completed");
	_delay_ms(1000);
	LCDClear();
	LCDWriteStringXY(0,0,"generating char");
	process_result(finger_generate_char_file(1));
	process_result(finger_generate_char_file(2));
	process_result(finger_generate_template());
	LCDClear();
	LCDWriteStringXY(0,0,"Searching...");
	code = finger_search();
	process_result(code);
	//upImg();
	//upChar(1);
	//finger_store(1);
	_delay_ms(4000);
	LCDClear();
}

void process_result(uint16_t ret)
{
   LCDClearLineXY(0,1);
   LCDWriteStringXY(0,1,finger_get_response_string(ret));
   _delay_ms(2000);	
}

//Initialize SPI Master Device
void spi_init_master (void)
{
	DDRB = (1<<PINB5)|(1<<PINB3)|(1<<PINB2);              //Set MOSI, SCK , SS as Output
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1); //Enable SPI, Set as Master
	SPCR &= ~(1<<SPR0);//f/64
}

//Function to send and receive data
char spi_tranceiver (char data)
{
	PORTB &= ~(1<<PINB2); //sending low signal to activate slave
	SPDR = data;                       //Load data into the buffer
	while(!((SPSR)&(1<<SPIF)));          //wait till transmission complete
	return(SPDR);                      //Return received data
}