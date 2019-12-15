#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include "USART_Interrupt.h" 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include "gsm.h" 
#include "lcd.h" 

//uint16_t getID();
//void formatID(uint16_t );
void GSM_setup();
void sendMsg(unsigned char p_status,char* num);
void query_database(char* id);
//void issue_tutorial();
//void messaging();

//supplementary functions
void GSM_interact(char* ,char* ,char* );
void GSM_Send_Msg(char* ,char* );
void LCD_Write_xy(char a,char b,char* str);
void parse_data(char* data);

char buff[160];
char data[160];
char name[20];
char phone[13];

volatile int buffer_pointer;
int position = 0;

int main(void)
{
	DDRD = 0b11110000;
	DDRC = 0b11000000;
	buffer_pointer = 0;
	Lcd4_Init();
	Lcd4_Clear();
	LCD_Write_xy(1,4,"Group 35");
	_delay_ms(1000);
	LCD_Write_xy(2,1,"   Starting..   ");
	_delay_ms(1000);
	USART_Init(9600);	//initialize USART
	sei();//enable global interrupts
	GSM_setup();
	_delay_ms(1000);
query_database("1");
	while(1)
	{
		//get ID from finger print scanner
		//send ID to database
		//get response 	sendMsg(1,"+94716830842");
		//take action based on whether student has paid or not
	}
}

//Interrupt Service Routine for USART communication
ISR(USART_RXC_vect)
{
	buff[buffer_pointer] = UDR;	/* copy UDR (received value) to buffer */
	buffer_pointer++;
}

void GSM_setup(){
	GSM_interact(GSM_BEGIN,"began","Error");
 	GSM_interact(SET_GPRS,"SET GPRS","GPRS Error");
 	GSM_interact(SET_APN,"SET APN","APN Error");
 	GSM_interact(ENABLE_GPRS,"EN GPRS","GPRS is disable");
}

void GSM_interact(char* AT_command,char* success,char* show_error){
	while(1)
	{
		buffer_pointer = 0;
		USART_SendString(AT_command);
		//USART_TxChar(0x1A);
		_delay_ms(1000);
		if(strstr(buff,"OK"))
		{			
			Lcd4_Clear();
			LCD_Write_xy(1,1,success);
			_delay_ms(1000);
			memset(buff,0,160);
			break;
		}
		else
		{
			Lcd4_Clear();
			LCD_Write_xy(2,0,show_error);
			_delay_ms(3000);
			Lcd4_Clear();
			memset(buff,0,160);
		}
	}
}

void sendMsg(unsigned char p_status,char* num){
	//GSM_interact(DISABLE_GPRS,"gprs disable","gprs active");
	GSM_interact(TXT_MSG_MODE,"txt mode ok","TXT mode fail");
	if(p_status){
		//send message
		GSM_Send_Msg(num,"Student came to class");
		//issue tutorial
	}
	else{
		//send message
		GSM_Send_Msg(num,"Student haven't paid");
		//show error
		Lcd4_Clear();
		LCD_Write_xy(2,0,"You haven't paid");
	}
}

void query_database(char* id){
	GSM_interact(ENABLE_HTTP,"HTTP Enabled","HTTP is disable");
	//GSM_interact(ENABLE_HTTPS,"HTTPS is disable");
	GSM_interact(SET_PROFILE,"Profile set","Profile error");
	
	char SET_URL[400];
	strcpy(SET_URL,"AT+HTTPPARA=\"URL\",\"http://hardware-back.herokuapp.com/students/");
	strcat(SET_URL, id);
	strcat(SET_URL,"\"\r");
	
	GSM_interact(SET_URL,"URL Success","URL error");
	
	//GSM_interact("AT+HTTPSSL?\r","NOT HTTPS");
	GSM_interact(START_GET,"GET success","GET error");
	_delay_ms(3000);//This delay is essential for retrieving information.don't remove it.
	memset(buff,0,160);
	//_delay_ms(3000);
	buffer_pointer = 0;
	_delay_ms(1000); 
	Lcd4_Clear();
	LCD_Write_xy(1,1,"0");
	USART_SendString(READ_DATA);
	Lcd4_Clear();
	LCD_Write_xy(1,1,"1");
	_delay_ms(5000);
	
	/*int q = 0;
	while(q < strlen(buff)){
		strncpy(data, buff+q,16);
		Lcd4_Clear();
		LCD_Write_xy(1,1,data);
		_delay_ms(4500);
		q += 16;
	}*/
	
	/*while(buff[buffer_pointer] != '{'){
		buffer_pointer++;
	}
	Lcd4_Clear();
	LCD_Write_xy(1,1,"2");
	strncpy(data, buff+30,buffer_pointer);*/
	
	
	strcpy(data,buff);
	Lcd4_Clear();
	LCD_Write_xy(1,1,"Please wait");
	_delay_ms(3000);
	parse_data(data);
	Lcd4_Clear();
	//LCD_Write_xy(1,1,data);
	_delay_ms(3000);
	Lcd4_Clear();
	Lcd4_Write_String(name);
	_delay_ms(3000);
	Lcd4_Clear();
	Lcd4_Write_String(phone);
	_delay_ms(3000);
	
	memset(buff,0,160);
	GSM_interact(TERMINATE_SESSION,"Terminated","Terminate Error");
	
	memset(data,0,50);
	GSM_interact(DISABLE_GPRS,"GPRS disable","ERROR DISABLING");
}

void GSM_Send_Msg(char *num,char *sms)
{
	char sms_buffer[35];
	buffer_pointer=0;
	sprintf(sms_buffer,"AT+CMGS=\"%s\"\r",num);
	USART_SendString(sms_buffer); /*send command AT+CMGS="Mobile No."\r */
	_delay_ms(200);
	while(1)
	{
		if(buff[buffer_pointer]==0x3e) /* wait for '>' character*/
		{
			buffer_pointer = 0;
			memset(buff,0,strlen(buff));
			USART_SendString(sms); /* send msg to given no. */
			USART_TxChar(0x1a); /* send Ctrl+Z */
			break;
		}
		buffer_pointer++;
	}
	_delay_ms(300);
	buffer_pointer = 0;
	memset(buff,0,strlen(buff));
	memset(sms_buffer,0,strlen(sms_buffer));
}

void LCD_Write_xy(char a,char b,char* str){
	Lcd4_Set_Cursor(a,b);
	Lcd4_Write_String(str);
}

void parse_data(char* data){
	int name_start = 0;
	int j=0;
	int i=0;
	int mobile_start = 0;

	while(i < strlen(data)-1){
		if(j==2){
			break;
		}
		if(data[i] == ':' && data[i+1] == '"'){
			j++;
			if(j==1){
				name_start = i+2;
			}
			else if(j==2){
				mobile_start = i+1;
			}
		}
		i++;
	}
	strncpy(name,data+name_start,mobile_start-name_start-6);
	strncpy(phone,data+mobile_start+1,12);
}