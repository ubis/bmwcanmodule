/**
  CAN Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    can.h

  @Summary
    This is the generated header file for the ECAN driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs driver for CAN.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18LF26K80
        Driver Version    :  2.12.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB 	          :  MPLAB X 5.45
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef ECAN_H
#define ECAN_H

/**
  Section: Included Files
*/

#include <stdbool.h>
#include <stdint.h>

/**

Global Defines  

*/
typedef union {

    struct {
        uint8_t idType;
        uint32_t id;
        uint8_t dlc;
        uint8_t data0;
        uint8_t data1;
        uint8_t data2;
        uint8_t data3;
        uint8_t data4;
        uint8_t data5;
        uint8_t data6;
        uint8_t data7;
    } frame;
    uint8_t array[14];
} uCAN_MSG;

/**
 Defines
*/

#define dSTANDARD_CAN_MSG_ID_2_0B 1
#define dEXTENDED_CAN_MSG_ID_2_0B 2

// For backwards-compatibility with pre-v1.79 users
#define ECAN_ISR_ECAN_WAKI ECAN_WAKI_ISR
#define ECAN_ISR_ECAN_RXB1I ECAN_RXB1I_ISR
#define ECAN_ISR_ECAN_RXB0I ECAN_RXB0I_ISR
#define ECAN_ISR_ECAN_ERRI ECAN_ERRI_ISR
#define ECAN_ISR_ECAN_IRXI ECAN_IRXI_ISR

/**
  Section: CAN APIs
*/

/**
    @Summary
        Initializes the ECAN module. 

    @Description
        This routine sets all the set parameters to the ECAN module.

    @Preconditions 
        None

    @Param
        None
  
    @Returns
        None
 
    @Example
        <code>
        ECAN_Initialize();  
        </code>                                                                      
*/
void ECAN_Initialize(void);
/**
                                                                           
    @Summary
        CAN_sleep

    @Description
        Puts the CAN module to sleep

    @Param
        None 

    @Returns
        None   

    @Example
        <code>
        CAN_Initialize();  
        </code> 
                                                                           
*/

void CAN_sleep(void);

/**
    @Summary
        CAN_transmit

    @Description
        Transmits out sCAN_MSG

    @Param
        Pointer to a sCAN_MSG

    @Returns
        True or False if message was loaded to transmit buffer 

    @Example
        <code>
        uCAN_MSG txMessage;
        CAN_transmit(&txMessage);  
        </code>                                                                             
*/
uint8_t CAN_transmit(uCAN_MSG *tempCanMsg);


/**

	@Summary
		CAN_receive

	@Description
		Receives CAN messages

	@Param
		Pointer to a sCAN_MSG

	@Returns
		True or False for a new message 
  
	@Example
		<code>
		uCAN_MSG rxMessage;
		CAN_receive(&rxMessage);  
		</code> 
                                                                             
*/
uint8_t CAN_receive(uCAN_MSG *tempCanMsg);

/**
 
	@Summary
		CAN_messagesInBuffer
 
	@Description
		Checks to see how many messages are in the buffer
	
	@Param
		None

	@Returns
		Returns total number of messages in the buffers

	@Example
		<code>
		uint8_t nrMsg;
		nrMsg = CAN_messagesInBuffer();  
		</code>                                                                            
*/
uint8_t CAN_messagesInBuffer(void);

/**

	@Summary
		CAN_isBusOff

	@Description
		Checks to see if module is busoff

	@Param
		None

	@Returns
		True if module is in Busoff, False is if it is not

	@Example
		<code>
		uint8_t busOff;
		busOff = CAN_isBusOff();  
		</code> 
                                                  
*/

uint8_t CAN_isBusOff(void);

/**

	@Summary
		CAN_isRXErrorPassive

	@Description
		Checks to see if module is RX Error Passive
        
	@Param
		None
 
	@Returns
 		True if module is in RX Error Passive, False is if it is not                                                    

 	@Example
		<code>
		uint8_t errRxPasive;
		errRxPasive = CAN_isRXErrorPassive();
		</code>     
                                                    
 */
 
uint8_t CAN_isRXErrorPassive(void);

/**

	@Summary
		CAN_isTXErrorPassive

	@Description
		Checks to see if module is TX Error Passive

	@Param
		None

	@Returns
		True if module is in TX Error Passive, False is if it is not

	@Example
		<code>
		uint8_t errTxPasive;
		errTxPasive = CAN_isTXErrorPassive();  
		</code>       

*/

uint8_t CAN_isTXErrorPassive(void);

/**
    @Summary
        ECAN_SetRXB0InterruptHandler

    @Description
        Sets the ECAN Receive buffer 0 interrupt handler

    @Param
        Address of the callback routine

    @Returns
        None

    @Example
        <code>
        volatile bool customRXB0Flag = false;

        void CustomRXB0InterruptHandler(void)
        {
            customRXB0Flag = true;
            // ...
        }

        void main(void)
        {
            // ...
            ECAN_SetRXB0InterruptHandler(CustomRXB0InterruptHandler);

            while (1)
            {
                if (customRXB0Flag) {
                    customRXB0Flag = false;
                    // ...
                }
            }
        }
        </code>       
*/
void ECAN_SetRXB0InterruptHandler(void (*handler)(void));

/**
    @Summary
        ECAN_RXB0I_ISR

    @Description
        Implements the ECAN Receive buffer 0 interrupt service routine

    @Param
        None

    @Returns
        None
*/
void ECAN_RXB0I_ISR(void);

/**
    @Summary
        ECAN_SetRXB1InterruptHandler

    @Description
        Sets the ECAN Receive buffer 1 interrupt handler

    @Param
        Address of the callback routine

    @Returns
        None

    @Example
        <code>
        volatile bool customRXB1Flag = false;

        void CustomRXB1InterruptHandler(void)
        {
            customRXB1Flag = true;
            // ...
        }

        void main(void)
        {
            // ...
            ECAN_SetRXB1InterruptHandler(CustomRXB1InterruptHandler);

            while (1)
            {
                if (customRXB1Flag) {
                    customRXB1Flag = false;
                    // ...
                }
            }
        }
        </code>       
*/
void ECAN_SetRXB1InterruptHandler(void (*handler)(void));

/**
    @Summary
        ECAN_RXB1I_ISR

    @Description
        Implements the ECAN Receive buffer 1 interrupt service routine

    @Param
        None

    @Returns
        None
*/
void ECAN_RXB1I_ISR(void);

/**
    @Summary
        ECAN_SetRXB0OverflowHandler

    @Description
        Sets the ECAN Receive buffer 0 overflow interrupt handler

    @Param
        Address of the callback routine

    @Returns
        None

    @Example
        <code>
        volatile bool customRXB0OverflowFlag = false;

        void CustomRXB0OverflowHandler(void)
        {
            customRXB0OverflowFlag = true;
            // ...
        }

        void main(void)
        {
            // ...
            ECAN_SetRXB0OverflowHandler(CustomRXB0OverflowHandler);

            while (1)
            {
                if (customRXB0OverflowFlag) {
                    customRXB0OverflowFlag = false;
                    // ...
                }
            }
        }
        </code>       
*/
void ECAN_SetRXB0OverflowHandler(void (*handler)(void));

/**
    @Summary
        ECAN_SetRXB1OverflowHandler

    @Description
        Sets the ECAN Receive buffer 1 overflow interrupt handler

    @Param
        Address of the callback routine

    @Returns
        None

    @Example
        <code>
        volatile bool customRXB1OverflowFlag = false;

        void CustomRXB1OverflowHandler(void)
        {
            customRXB1OverflowFlag = true;
            // ...
        }

        void main(void)
        {
            // ...
            ECAN_SetRXB1OverflowHandler(CustomRXB1OverflowHandler);

            while (1)
            {
                if (customRXB1OverflowFlag) {
                    customRXB1OverflowFlag = false;
                    // ...
                }
            }
        }
        </code>       
*/
void ECAN_SetRXB1OverflowHandler(void (*handler)(void));

/**
    @Summary
        ECAN_SetBusOffHandler

    @Description
        Sets the ECAN Bus off interrupt handler

    @Param
        Address of the callback routine

    @Returns
        None

    @Example
        <code>
        volatile bool customBusOffFlag = false;

        void CustomBusOffHandler(void)
        {
            customBusOffFlag = true;
            // ...
        }

        void main(void)
        {
            // ...
            ECAN_SetBusOffHandler(CustomBusOffHandler);

            while (1)
            {
                if (customBusOffFlag) {
                    customBusOffFlag = false;
                    // ...
                }
            }
        }
        </code>       
*/
void ECAN_SetBusOffHandler(void (*handler)(void));

/**
    @Summary
        ECAN_SetTXPassiveHandler

    @Description
        Sets the ECAN TX passive interrupt handler

    @Param
        Address of the callback routine

    @Returns
        None

    @Example
        <code>
        volatile bool customTXPassiveFlag = false;

        void CustomTXPassiveHandler(void)
        {
            customTXPassiveFlag = true;
            // ...
        }

        void main(void)
        {
            // ...
            ECAN_SetTXPassiveHandler(CustomTXPassiveHandler);

            while (1)
            {
                if (customTXPassiveFlag) {
                    customTXPassiveFlag = false;
                    // ...
                }
            }
        }
        </code>       
*/
void ECAN_SetTXPassiveHandler(void (*handler)(void));

/**
    @Summary
        ECAN_SetRXPassiveHandler

    @Description
        Sets the ECAN RX passive interrupt handler

    @Param
        Address of the callback routine

    @Returns
        None

    @Example
        <code>
        volatile bool customRXPassiveFlag = false;

        void CustomRXPassiveHandler(void)
        {
            customRXPassiveFlag = true;
            // ...
        }

        void main(void)
        {
            // ...
            ECAN_SetRXPassiveHandler(CustomRXPassiveHandler);

            while (1)
            {
                if (customRXPassiveFlag) {
                    customRXPassiveFlag = false;
                    // ...
                }
            }
        }
        </code>       
*/
void ECAN_SetRXPassiveHandler(void (*handler)(void));

/**
    @Summary
        ECAN_SetTXWarningHandler

    @Description
        Sets the ECAN TX warning interrupt handler

    @Param
        Address of the callback routine

    @Returns
        None

    @Example
        <code>
        volatile bool customTXWarningFlag = false;

        void CustomTXWarningHandler(void)
        {
            customTXWarningFlag = true;
            // ...
        }

        void main(void)
        {
            // ...
            ECAN_SetTXWarningHandler(CustomTXWarningHandler);

            while (1)
            {
                if (customTXWarningFlag) {
                    customTXWarningFlag = false;
                    // ...
                }
            }
        }
        </code>       
*/
void ECAN_SetTXWarningHandler(void (*handler)(void));

/**
    @Summary
        ECAN_SetRXWarningHandler

    @Description
        Sets the ECAN RX warning interrupt handler

    @Param
        Address of the callback routine

    @Returns
        None

    @Example
        <code>
        volatile bool customRXWarningFlag = false;

        void CustomRXWarningHandler(void)
        {
            customRXWarningFlag = true;
            // ...
        }

        void main(void)
        {
            // ...
            ECAN_SetRXWarningHandler(CustomRXWarningHandler);

            while (1)
            {
                if (customRXWarningFlag) {
                    customRXWarningFlag = false;
                    // ...
                }
            }
        }
        </code>       
*/
void ECAN_SetRXWarningHandler(void (*handler)(void));

/**
    @Summary
        ECAN_ERRI_ISR

    @Description
        Implements the ECAN Module error interrupt service routine

    @Param
        None

    @Returns
        None
*/
void ECAN_ERRI_ISR(void);

/**
    @Summary
        ECAN_SetWakeUpInterruptHandler

    @Description
        Sets the ECAN Wake up interrupt handler

    @Param
        Address of the callback routine

    @Returns
        None

    @Example
        <code>
        volatile bool customWakeUpFlag = false;

        void CustomWakeUpInterruptHandler(void)
        {
            customWakeUpFlag = true;
            // ...
        }

        void main(void)
        {
            // ...
            ECAN_SetWakeUpInterruptHandler(CustomWakeUpInterruptHandler);

            while (1)
            {
                if (customWakeUpFlag) {
                    customWakeUpFlag = false;
                    // ...
                }
            }
        }
        </code>       
*/
void ECAN_SetWakeUpInterruptHandler(void (*handler)(void));

/**
    @Summary
        ECAN_WAKI_ISR

    @Description
        Implements the ECAN Wake up interrupt service routine

    @Param
        None

    @Returns
        None
*/
void ECAN_WAKI_ISR(void);

/**
    @Summary
        ECAN_SetMessageErrorInterruptHandler

    @Description
        Sets the ECAN Message error interrupt handler

    @Param
        Address of the callback routine

    @Returns
        None

    @Example
        <code>
        volatile bool customMessageErrorFlag = false;

        void CustomMessageErrorInterruptHandler(void)
        {
            customMessageErrorFlag = true;
            // ...
        }

        void main(void)
        {
            // ...
            ECAN_SetMessageErrorInterruptHandler(CustomMessageErrorInterruptHandler);

            while (1)
            {
                if (customMessageErrorFlag) {
                    customMessageErrorFlag = false;
                    // ...
                }
            }
        }
        </code>       
*/
void ECAN_SetMessageErrorInterruptHandler(void (*handler)(void));

/**
    @Summary
        ECAN_IRXI_ISR

    @Description
        Implements the ECAN Message error interrupt service routine

    @Param
        None

    @Returns
        None
*/
void ECAN_IRXI_ISR(void);


#endif // ECAN_H
