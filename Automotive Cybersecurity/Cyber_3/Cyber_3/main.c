/*
 * Cyber_3.c
 *
 * Created: 11/11/2023 11:59:11 AM
 * Author : Dell
 */ 

#define F_CPU 8000000
#include "LIB/BIT_MATH.h"
#include "LIB/STD_TYPES.h"
#include <stdlib.h>

#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/EXTI/EXTI_interface.h"
#include "MCAL/UART/UART_interface.h"
#include "MCAL/TIMERS/TIMER0/TIMER0_interface.h"

#include "HAL/LCD/LCD_interface.h"
#include <util/delay.h>

void decimalToHex(u8 decimalNumber, u8 hexNumber[]) {
	if (decimalNumber == 0) {
		sprintf(hexNumber, "00");
		return;
	}
	// Convert decimal to hex
	if(decimalNumber >= 16)
	{
		sprintf(hexNumber, "%X", decimalNumber);
	}
	else
	{
		sprintf(hexNumber[0], '0');
		sprintf(hexNumber[1], "%X", decimalNumber);
	}
}

#define  KEY_VAL	35

volatile static u8 Received_St_Buffer[20];
volatile static u8 Received_He_Buffer[20];

volatile static u8 ID_St_Buffer;
volatile static u8 Secure_FLAG ;

volatile static u8 randomNum[4];
volatile static u8 HashSeed[4];

void	StringToHex(u8 *str, u8 *_Received_He_Buffer);
void	SendSeed(void);
u8		CheckSeed(void);

int main(void)
{
	GPIO_Init();
	LCD_Init();
	USART_Init();
	TIMER0_voidInit();
	_delay_ms(1000);
	LCD_WriteString("    START APP  ");
	USART_Transmit_String( "Send Request", '\0' , 20);
	_delay_ms(1000);
	LCD_ClearDisplay();
	TIMER0_voidStart();
	while (1)
	{
		_delay_ms(100);
		LCD_GoTo_xy(0,0);
		USART_Receive_String(&Received_St_Buffer,20);
		_delay_ms(500);
		ID_St_Buffer = Received_St_Buffer[0];
		if (ID_St_Buffer != '0')
		{
			LCD_WriteString(Received_St_Buffer);
			_delay_ms(500);
			StringToHex(Received_St_Buffer,Received_He_Buffer);
			switch(ID_St_Buffer)
			{
				case '2':
					if (Received_St_Buffer[1] == '7')
					{
						switch(Received_St_Buffer[3])
						{
							case  '1':
								{
									LCD_ClearDisplay();
									_delay_ms(500);
									LCD_WriteString("6701");
									SendSeed();
									break;
								}
							case  '2':
								{
									Secure_FLAG = CheckSeed();
									
									USART_Transmit_String("6702",'\0',5);
									for(u8 i = 0; i<8 ; i++)
									{
										USART_Transmit(Received_St_Buffer[4+i]);
									}
									
									if (Secure_FLAG)
									{
										LCD_ClearDisplay();
										_delay_ms(500);
										LCD_WriteString("6702");
										USART_Transmit_String("6702",'\0',5);
										for(u8 i = 0; i<8 ; i++)
										{
											LCD_WriteChar(Received_St_Buffer[4+i]); 
										}
									}
									else
									{
										LCD_ClearDisplay();
										_delay_ms(500);
										LCD_GoTo_xy(0,0);
										LCD_WriteString("Security Key Error!");
										GPIO_SetValue_Pin(GPIO_PA5,HIGH);
									}
									break;
								}
						default:
							break;
						}
					} else { USART_Transmit_String( "Error\0", '\0' , 20); /* wrong message. */ }
					break;
					
				case '3':
					if (Received_St_Buffer[1] == '1')
					{
						if (Secure_FLAG)
						{
							if (Received_He_Buffer[4] == 0xA && Received_He_Buffer[5] == 0xA && Received_He_Buffer[6] == 0 && Received_He_Buffer[7] == 0)
							{
								USART_Transmit_String( "7101AA00\0", '\0' , 20); // positive response.
								GPIO_SetValue_Pin(GPIO_PA7,HIGH);
							}
						}
						else
						{
							USART_Transmit_String( "NRC7F3135\0", '\0' , 20); // unsecured error message.
						}
					} else { USART_Transmit_String( "Error\0", '\0' , 20); /* wrong message. */}
					break;
					
				default:
					break;
			}
			USART_Transmit_String( "\r\n", '\0' , 3);
			USART_Transmit_String( "\r\nSend Request", '\0' , 20);
		}
		ID_St_Buffer = 0;
	}
}

void	StringToHex(u8 *str ,u8 * _Received_He_Buffer)
{
	u8 local_counter = 0;
	for (u8 i =0;i<20;i++)
	{
		_Received_He_Buffer[local_counter] = 0;
	}
	while(str[local_counter] != '\0')
	{
		switch(str[local_counter])
		{
			case 'A':
				_Received_He_Buffer[local_counter] = 0xA;
				break;
			case 'a':
				_Received_He_Buffer[local_counter] = 0xA;
				break;
				
			case 'B':
				_Received_He_Buffer[local_counter] = 0xB;
				break;
			case 'b':
				_Received_He_Buffer[local_counter] = 0xB;
				break;
				
			case 'C':
				_Received_He_Buffer[local_counter] = 0xC;
				break;
			case 'c':
				_Received_He_Buffer[local_counter] = 0xC;
				break;
				
			case 'D':
				_Received_He_Buffer[local_counter] = 0xD;
				break;
			case 'd':
				_Received_He_Buffer[local_counter] = 0xD;
				break;
				
			case 'E':
				_Received_He_Buffer[local_counter] = 0xE;
				break;
			case 'e':
				_Received_He_Buffer[local_counter] = 0xE;
				break;
				
			case 'F':
				_Received_He_Buffer[local_counter] = 0xF;
				break;
			case 'f':
				_Received_He_Buffer[local_counter] = 0xF;
				break;
				
			default:
				_Received_He_Buffer[local_counter] = str[local_counter]-'0';
				break;
		}
		local_counter++;
	}
}


void	SendSeed(void)
{
	for(u8 i = 0; i<4 ; i++)
	{ 
		randomNum[i] = TIMER0_voidGetTCNT();
		HashSeed[i]  = randomNum[i] ^ 0x35;
		for(u16 a =0; a<1000; a++);
	}
	
	USART_Transmit_String("6701",'\0',5);
	u8 buffer[2];
	for (u8 i =0; i<4; i++)
	{
		decimalToHex(randomNum[i],buffer);
		USART_Transmit_String(buffer,'\0',2);
		LCD_WriteString(buffer);
	}
	LCD_GoTo_xy(1,0);
	for (u8 i =0; i<4; i++)
	{	decimalToHex(HashSeed[i],buffer);
		LCD_WriteString(buffer);
	}
}


u8	CheckSeed(void)
{
	u8 Local_Flag = 0;
	u8 local_buffer[4];
	u8 buffer[3];
	
	_delay_ms(100);
	LCD_GoTo_xy(3,0);
	for(u8 i=0,j=0; i<4 ; i++,j+=2)
	{
		decimalToHex(HashSeed[i],buffer);
		for(u8 a=0;a<2; a++)
		{
			if ( buffer[a] == Received_St_Buffer[4+j+a] )
			{
				GPIO_SetValue_Pin(GPIO_PA6,HIGH);
				Local_Flag = 1;
			}
			else
			{
				GPIO_SetValue_Pin(GPIO_PA6,LOW);
				Local_Flag = 0;
				break;
			}
		}
		if(!Local_Flag)
		{
			break;
		}
	}
	return Local_Flag;
}
