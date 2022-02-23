/* 
 * File:   conf.h  
 * Author: Jokubas Maciulaitis (ubis)
 * Date:   2022 02 16 
 */

#ifndef __CONF_H__
#define	__CONF_H__

#define DEBUG_MODE

// CAN RS pin configuration
#define CAN_RS_PIN        RB5
#define CAN_RS_PIN_BIT    (1 << 5)
#define CAN_RS_PIN_PORT   TRISB

// PDC/HDC led output configuration
#define HDC_LED_PIN       RC4
#define HDC_LED_PIN_BIT   (1 << 4)
#define HDC_LED_PIN_PORT  TRISC

#define PDC_LED_PIN       RC5
#define PDC_LED_PIN_BIT   (1 << 5)
#define PDC_LED_PIN_PORT  TRISC

// ADC input button configuration
#define BTN_ADC_CHANNEL   1

// ADC button values
#define PDC_ADC_MIN_VALUE 1000
#define PDC_ADC_MAX_VALUE 4000

#define HDC_ADC_MIN_VALUE 5000
#define HDC_ADC_MAX_VALUE 10000

#define CAM_ADC_MIN_VALUE 11000
#define CAM_ADC_MAX_VALUE 18000

#endif	// __CONF_H__