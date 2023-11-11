/************************************************************************/
#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H
/************************************************************************/
/*______________________________________________________________________*/
#define INPUT 0
#define OUTPUT 1
/*______________________________________________________________________*/

/*______________________________________________________________________*/
#define CONCATE(b7, b6, b5, b4, b3, b2, b1, b0)                                \
(0b##b7##b6##b5##b4##b3##b2##b1##b0)

#define HELPER(b7, b6, b5, b4, b3, b2, b1, b0)                                 \
CONCATE(b7, b6, b5, b4, b3, b2, b1, b0)
/*______________________________________________________________________*/

/*______________________________________________________________________*/
#define A0 OUTPUT
#define A1 OUTPUT
#define A2 OUTPUT
#define A3 OUTPUT
#define A4 OUTPUT
#define A5 OUTPUT
#define A6 OUTPUT
#define A7 OUTPUT

#define PORTA_DIRECTION HELPER(A7, A6, A5, A4, A3, A2, A1, A0)
/*______________________________________________________________________*/
/*______________________________________________________________________*/
#define B0 OUTPUT
#define B1 OUTPUT
#define B2 OUTPUT
#define B3 OUTPUT
#define B4 OUTPUT
#define B5 OUTPUT
#define B6 INPUT
#define B7 OUTPUT

#define PORTB_DIRECTION HELPER(B7, B6, B5, B4, B3, B2, B1, B0)
/*______________________________________________________________________*/
/*______________________________________________________________________*/
#define C0 INPUT
#define C1 INPUT
#define C2 OUTPUT
#define C3 OUTPUT
#define C4 OUTPUT
#define C5 OUTPUT
#define C6 INPUT
#define C7 INPUT

#define PORTC_DIRECTION HELPER(C7, C6, C5, C4, C3, C2, C1, C0)
/*______________________________________________________________________*/
/*______________________________________________________________________*/
#define D0 INPUT
#define D1 OUTPUT
#define D2 INPUT
#define D3 INPUT
#define D4 INPUT
#define D5 INPUT
#define D6 INPUT
#define D7 INPUT

#define PORTD_DIRECTION HELPER(D7, D6, D5, D4, D3, D2, D1, D0)
/*______________________________________________________________________*/

/************************************************************************/
#endif