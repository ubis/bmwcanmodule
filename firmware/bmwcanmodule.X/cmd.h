/* 
 * File:   cmd.h  
 * Author: Jokubas Maciulaitis (ubis)
 * Date:   2022 02 16 
 */

#ifndef __CMD_H__
#define	__CMD_H__

// Wake message
#define CMD_WAKE_MESSAGE_ID         0x130
#define CMD_WAKE_MESSAGE_MASK       0x0F
#define CMD_WAKE_MESSAGE_ON         0x05
#define CMD_WAKE_MESSAGE_OFF        0x00

// PDC event & button message
#define CMD_PDC_EVT_MESSAGE_ID      0x39F
#define CMD_PDC_EVT_MESSAGE_ON      0xF9
#define CMD_PDC_EVT_MESSAGE_OFF     0xFB

#define CMD_PDC_BTN_MESSAGE_ID      0x30A
#define CMD_PDC_BTN_MESSAGE_PRESS   0xFD
#define CMD_PDC_BTN_EMSSAGE_RELEASE 0xFC

#endif	// __CMD_H__