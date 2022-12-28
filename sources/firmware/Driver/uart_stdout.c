/*
 *-----------------------------------------------------------------------------
 * The confidential and proprietary information contained in this file may
 * only be used by a person authorised under and to the extent permitted
 * by a subsisting licensing agreement from ARM Limited.
 *
 *            (C) COPYRIGHT 2010-2015  ARM Limited or its affiliates.
 *                ALL RIGHTS RESERVED
 *
 * This entire notice must be reproduced on all copies of this file
 * and copies of this file may only be made by a person if such person is
 * permitted to do so under the terms of a subsisting license agreement
 * from ARM Limited.
 *
 *      SVN Information
 *
 *      Checked In          : $Date: 2013-04-10 15:14:20 +0100 (Wed, 10 Apr 2013) $
 *
 *      Revision            : $Revision: 243501 $
 *
 *      Release Information : Cortex-M0 DesignStart-r1p0-00rel0
 *-----------------------------------------------------------------------------
 */

 /*

 UART functions for retargetting

 */
#include "CMSDK_CM3.h"

#define UARTCLK 18432000

void UartStdOutInit(void);

void UartStdOutInit(void)
{
	unsigned int temp;
	unsigned int divider;
	unsigned int remainder;
	unsigned int fraction;

	// UART SETTING
	temp = 16 * 115200;			// BaudRate;
	divider = UARTCLK / temp;
	remainder = UARTCLK % temp;
	temp = (8 * remainder) / 115200;
	fraction = (temp >> 1) + (temp & 1);

	*UART_CR	    = 0x0;
	*UART_IMSC  	= 0x0;  		// Interrupt mask set/clear
	*UART_IBRD  	= 0x0;  		// Integer baud rate divisor
	*UART_FBRD  	= 0x0;  		// Fractional baud rate divisor
	*UART_LCR_H 	= 0x0;  		// Line control
	*UART_CR    	= 0x0;  		// Control
	*UART_IFLS  	= 0x0;  		// Interrupt FIFO level select

	*UART_IBRD	    = divider;
	*UART_FBRD 	    = fraction;
	*UART_LCR_H     = 0x0;
	*UART_LCR_H     = (3<<5) | (1<<4);
	*UART_CR	    = 0x380;		    // (bit9:RXE), (bit8:TXE) 
	*UART_IFLS 	    = ((0x0<<3)|0x4);	// INT Level : Rx fifo (1/8), TX fifo (1/2)
								//                     (1/8)*32 = 4byte rx, interrupt is generated
	*UART_CR 	    = 0x381;

	CMSDK_GPIO1->ALTFUNCSET = (1<<5);
	CMSDK_GPIO1->ALTFUNCSET = (1<<5);
	return;
}
// Output a character
unsigned char UartPutc(unsigned char my_ch)
{
	while (((*UART_FR & 0x20)==0x20)); // Wait if Transmit Holding register is full
	*UART_DR = my_ch; // write to transmit holding register
	return (my_ch);
}
// Get a character
unsigned char UartGetc(void)
{
  while (((*UART_FR & 0x10)==0x10)); // Wait if Receive Holding register is empty

  return (*UART_DR);
}

void UartEndSimulation(void)
{
  UartPutc((char) 0x4); // End of simulation
  while(1);
}

