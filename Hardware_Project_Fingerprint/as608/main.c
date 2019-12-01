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

#define enroll_key_press() ((PINC & 0x01) == 0)
#define finger_Scann_Key_press() ((PINC & 0x04) == 0)

void keys_init();
void finger_enrollment();
void finger_scan();
void process_result(uint16_t ret);


int main(void)
{
	LCDInit(LS_NONE);
	finger_init();
	keys_init();
	DDRC |= 1<<PINC5;
	PORTC |= 1<<PINC5;
	
    
    while (1) 
    { 
		LCDWriteStringXY(0,0,"Welcome!");
		
		if(enroll_key_press())
		{
			finger_enrollment();
		}
		else if (finger_Scann_Key_press())
		{
			finger_scan();
		}
			
		
    }
}

void keys_init()
{
	DDRC &= ~(1<<PINC0); //Enroll Key
	DDRC &= ~(1<<PINC2); //Scan key
	PORTC |= 1<<PINC0;
	PORTC |= 1<<PINC2;
	
}

void finger_enrollment()
{
	LCDClear();
	LCDWriteStringXY(0,0,"Deleting");
	process_result(finger_delete_all());
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
	while(finger_read()!=0x00);
	LCDClear();
	LCDWriteStringXY(0,0,"Scanning...");
	_delay_ms(1000);
	LCDWriteStringXY(0,1,"Completed");
	_delay_ms(1000);
	LCDClear();
	LCDWriteStringXY(0,0,"generating char");
	process_result(finger_generate_char_file(1));
	LCDClear();
	LCDWriteStringXY(0,0,"Searching...");
	code = finger_search();
	process_result(code);
	if(code==0x00)
	{
		PORTC &=~(1<<PINC5);
		_delay_ms(5000);
		PORTC |= 1<<PINC5;
	}
	_delay_ms(4000);
	LCDClear();
}

void process_result(uint16_t ret)
{
   LCDClearLineXY(0,1);
   LCDWriteStringXY(0,1,finger_get_response_string(ret));
   _delay_ms(2000);	
}
