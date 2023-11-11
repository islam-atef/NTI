/*
 * UART_program.c
 *
 * Created: 10/29/2023 10:58:08 AM
 *  Author: Dell
 */ 
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"

#include "UART_config.h"
#include "UART_interface.h"
#include "UART_private.h"

/****************************************************************/
static void (* UART_RX_COMP_CALLBACK )(void) = NULL;
static void (* UART_TX_COMP_CALLBACK )(void) = NULL;
static void (* UART_UDR_EMPTY_CALLBACK )(void) = NULL;
/****************************************************************/
/* Description    :  This function used to initialize USART     */
/*					 Inputs : void 								*/
/*					 return : void		 						*/
/****************************************************************/
void USART_Init( )
{
	// 1- select the Double Speed Mode.
	(*UCSRA)	= 0x00;
	(*UCSRA)	|=	(_U2X_ <<U2X);
	// 2- select to access {UCSRC} Register.
	(*UCSRC)	= 0x80;  
	// select the Operating mode.
	(*UCSRC)	|= (UART_MODE << UMSEL);  
	// select the parity mode.
	(*UCSRC)	|= (PARITY_MODE << UPM0);  
	// select the number of stop bits.
	(*UCSRC)	|= (STOP_BITS_NUM << USBS);  
	// select the Data Bits number.
	if (DATA_BITS_NUM != _9_BIT_)
	{
		(*UCSRC)	|= (DATA_BITS_NUM << UCSZ0);  
	} 
	else
	{
		SET_BIT((*UCSRC),UCSZ0);
		SET_BIT((*UCSRC),UCSZ1);
		SET_BIT((*UCSRB),UCSZ2);
	}
	// select the Operation mode.
	(*UCSRB)	= 0x00;
	switch(UART_OPERATE)
	{
		case UART_TX:
			SET_BIT((*UCSRB),TXEN);
			break;
		case UART_RX:
			SET_BIT((*UCSRB),RXEN);
			break;
		case UART_TX_RX:
			SET_BIT((*UCSRB),TXEN);
			SET_BIT((*UCSRB),RXEN);
			break;
		default:
			break;
	}
	// Baud Rate Setting.
	(*UBRRL)	=	BAUD_RATE;  
}


/****************************************************************/
/* Description    : This function used to Send Data				*/
/*																*/
/*					Inputs : u8 data				  			*/
/*					Return : void		 					    */
/****************************************************************/
void USART_Transmit( u8 data )
{
	// wait until the DATA register empty flag being set.
	while(!GET_BIT((*UCSRA),UDRE));
	(void)(*UDR);
	(*UDR) = (u8)data;
}


/****************************************************************/
/* Description    : This function used to Transmit String		*/
/*																*/
/*					Inputs : void					  			*/
/*					Return : u8			 					    */
/****************************************************************/
void USART_Transmit_String(u8 *buffer , u8 end_char , u8 size )
{
	volatile u8 counter = 0;
	// wait until the DATA register Not empty flag being set.
	while((buffer[counter] != end_char) && (counter < size))
	{
		USART_Transmit(buffer[counter]);
		counter++;
	}
}


/****************************************************************/
/* Description    : This function used to Receive Data			*/
/*																*/
/*					Inputs : void					  			*/
/*					Return : u8			 					    */
/****************************************************************/
u8 USART_Receive( )
{
	while (!(*UCSRA & (1 << RXC)));
	// Return received data
	return *UDR;
}


/****************************************************************/
/* Description    : This function used to Receive String		*/
/*																*/
/*					Inputs : void					  			*/
/*					Return : u8			 					    */
/****************************************************************/
void USART_Receive_String(u8* receivedString , u16 maxLength  )
{
	//volatile u8  counter = 0;
	//// wait until the DATA register Not empty flag being set.
	////while(!GET_BIT((*UCSRA),RXC));
	//while (counter < size)
	//{
	//	if (*UDR == end_char)
	//	{
	//		break;
	//	}
	//	*Received_Buffer =  USART_Receive();
	//	Received_Buffer++;
	//	counter++;
	//}
	//*Received_Buffer = '\0';
	
	u16 i = 0;
	u8 receivedChar;
	while (i < maxLength - 1) {
		// Receive a character
		receivedChar = USART_Receive();
		// Break the loop if the termination character is received
		if (receivedChar == '\r' || receivedChar == '\n') {
			break;
		}
		// Store the received character in the string
		receivedString[i] = receivedChar;
		// Increment the index
		i++;
	}
	// Null-terminate the received string
	receivedString[i] = '\0';
}

/****************************************************************/
/* Description    : This function used to Enable Interrupts		*/
/*																*/
/*					Inputs : void					  			*/
/*					Return : u8			 					    */
/****************************************************************/
void USART_Interrupt_Enable(INTE_TYPES Inte_Type)
{
	SET_BIT((*UCSRB),Inte_Type);
}

/****************************************************************/
/* Description    : This function used to Disable Interrupts	*/
/*																*/
/*					Inputs : void					  			*/
/*					Return : u8			 					    */
/****************************************************************/
void USART_Interrupt_Disable(INTE_TYPES Inte_Type)
{
	CLR_BIT((*UCSRB),Inte_Type);
}

/****************************************************************/
/* Description    : This function used to Set callback function */
/*								 for each Interrupts			*/
/*																*/
/*					Inputs : void					  			*/
/*					Return : u8			 					    */
/****************************************************************/
void USART_InteSetCallback(INTE_TYPES Inte_Type, void (*ptr)(void))
{
	switch(Inte_Type)
	{
		case RC_COMPLETE_INT:
			UART_RX_COMP_CALLBACK = ptr;
			break;
		case TC_COMPLETE_INT:
			UART_TX_COMP_CALLBACK = ptr;
			break;
		case DATA_REG_EMPTY :
			UART_UDR_EMPTY_CALLBACK = ptr;
			break;
		default:
			break;
	};
}




void __vector_13(void) __attribute__((signal, used, externally_visible)); // to avoid optimization (TIMER0 OVF ISR)
void __vector_13(void)
{
	UART_RX_COMP_CALLBACK();
}

void __vector_14(void) __attribute__((signal, used, externally_visible)); // to avoid optimization (TIMER0 OVF ISR)
void __vector_14(void)
{
	UART_TX_COMP_CALLBACK();
}

void __vector_15(void) __attribute__((signal, used, externally_visible)); // to avoid optimization (TIMER0 OVF ISR)
void __vector_15(void)
{
	UART_UDR_EMPTY_CALLBACK();
}