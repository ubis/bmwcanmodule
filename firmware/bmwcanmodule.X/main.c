#include "mcc_generated_files/mcc.h"
#include <string.h>
#include "cmd.h"
#include "conf.h"
#include "version.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) sizeof(arr) / sizeof(arr[0])
#endif // ARRAY_SIZE

enum btn_id {
    BTN_PDC,
    BTN_HDC,
    BTN_CAM,
};

typedef struct {
    int min;
    int max;
    char *label;
    bool pressed;
} btn_info_t;

static volatile bool g_CanRxMsg;
static volatile bool g_ADCDone;
static volatile uint64_t g_Millis;
static uint64_t g_SleepCnt;
static bool g_WakeFlag;

static btn_info_t g_btn[] = {
    [BTN_PDC] = { PDC_ADC_MIN_VALUE, PDC_ADC_MAX_VALUE, "PDC", false },
    [BTN_HDC] = { HDC_ADC_MIN_VALUE, HDC_ADC_MAX_VALUE, "HDC", false },
    [BTN_CAM] = { CAM_ADC_MIN_VALUE, CAM_ADC_MAX_VALUE, "CAM", false },
};

static void CanRxIRQHandler(void) {
    g_CanRxMsg = true;
}

static void Timer0IRQHandler(void) {
    g_Millis++;
}

static void ADCIRQHandler(void) {
    g_ADCDone = true;
}

static void CanFilterSetupId(int type) {
    printf("CAN ");

    if (type == dSTANDARD_CAN_MSG_ID_2_0B) {
        RXF0SIDL &= ~(1 << 3);
        printf("standard");
    } else if (type == dEXTENDED_CAN_MSG_ID_2_0B) {
        RXF0SIDL |= (1 << 3);
        printf("extended");
    }
    
    printf(" ID messages enabled\r\n");
}

static void IsWakeCommand(uCAN_MSG *msg) {
    uint32_t id = msg->frame.id;
    uint8_t len = msg->frame.dlc;
    
    if (id != CMD_WAKE_MESSAGE_ID) {
        return;
    }
    
    if (!len) {
        return;
    }
    
    // wake  0x45 or 0x05
    // sleep 0x40 or 0x00   
    uint8_t cmd = msg->frame.data0 & CMD_WAKE_MESSAGE_MASK;
    g_WakeFlag = (cmd == CMD_WAKE_MESSAGE_ON);
}

static void IsPDCCommand(uCAN_MSG *msg) {
    uint32_t id = msg->frame.id;
    uint8_t len = msg->frame.dlc;
    
    if (id != CMD_PDC_EVT_MESSAGE_ID) {
        return;
    }
    
    if (!len) {
        return;
    }
     
    uint8_t cmd = msg->frame.data0;
    
    if (cmd == CMD_PDC_EVT_MESSAGE_ON) {
        // turn on PDC led output
        PDC_LED_PIN = 1;
    } else if (cmd == CMD_PDC_EVT_MESSAGE_OFF) {
        // turn off PDC led output
        PDC_LED_PIN = 0;
    }
}

static void CanHandleRxMessage(void) {
    uCAN_MSG msg = { 0 };

    if (!g_CanRxMsg) {
        return;
    }
    
    g_CanRxMsg = false;
    
    if (!CAN_receive(&msg)) {
        return;
    }
    
    uint32_t id    = msg.frame.id;
    uint8_t idtype = msg.array[0];
    uint8_t len    = msg.array[5];

#ifdef DEBUG_MODE
    printf("CAN message received: 0x%08lX; ", id);
    printf("identifier: %s; ", idtype == 1 ? "standard" : "extended");
    printf("length: %u; ", len);
    printf("data: ");
               
    for (int i = 6; i < 6 + len; i++) {
        printf("%02X", msg.array[i]);
        
        if ((i + 1) < (6 + len)) {
            printf(" ");
        }
    }
    
    printf("\r\n");
#endif // DEBUG_MODE
    
    IsWakeCommand(&msg);
    IsPDCCommand(&msg);
}

static void SendPDCButtonState(bool state) {
    uCAN_MSG msg = { 0 };
    
    msg.frame.id     = CMD_PDC_BTN_MESSAGE_ID;
    msg.frame.idType = dSTANDARD_CAN_MSG_ID_2_0B;
    msg.frame.dlc    = 2;
    msg.frame.data0  = (state) ? CMD_PDC_BTN_MESSAGE_PRESS : CMD_PDC_BTN_EMSSAGE_RELEASE;
    msg.frame.data1  = 0xFF;
       
    CAN_transmit(&msg);
    
    printf("Sent PDC event button [%02X] message\r\n", msg.frame.data0);
}

void ReadButtonState(void) {
    if (!g_ADCDone) {
        return;
    }
    
    g_ADCDone = false;
    ADC_StartConversion(BTN_ADC_CHANNEL);
    
    uint16_t value = ADC_GetConversionResult();
    
    for (int i = 0; i < ARRAY_SIZE(g_btn); i++) {
        if (!(value >= g_btn[i].min && value <= g_btn[i].max)) {
            continue;
        }

        printf("Received ADC value: %u\r\n", value);
            
        if (g_btn[i].pressed) {
            // already pressed, holding...
            return;
        }
            
        g_btn[i].pressed = true;
        printf("%s button was pressed!\r\n", g_btn[i].label);
            
        if (i == BTN_PDC) {
            SendPDCButtonState(g_btn[i].pressed);
        }
            
        // only single buton press should be available
        return;
    }
    
    for (int i = 0; i < ARRAY_SIZE(g_btn); i++) {
        if (!g_btn[i].pressed) {
            continue;
        }
        
        g_btn[i].pressed = false;
        
        printf("Received ADC value: %u\r\n", value);
        printf("%s button was released!\r\n", g_btn[i].label);
        
        if (i == BTN_PDC) {
            SendPDCButtonState(g_btn[i].pressed);
        }
        
        // only single buton press should be available
        return;
    }
}

static void EnterSleepMode(void) {
    printf("Entering sleep mode...\r\n");
    
    // turn off LED outputs
    HDC_LED_PIN = 0;
    PDC_LED_PIN = 0; 
    
    // put CAN transceiver into stand-by mode (RS->HIGH)
    CAN_RS_PIN = 1;
    
    // put CAN into sleep mode
    CAN_sleep();

     // enter sleep mode on Sleep()
    OSCCONbits.IDLEN = 0;
    
    Sleep();

    NOP();

    printf("Wake up\r\n");

    // put CAN transceiver into high-speed mode (RS->LOW)
    CAN_RS_PIN = 0;
}

void main(void) {
    SYSTEM_Initialize();
    
    printf("\r\n");
    printf("BMW CAN Module v%d.%d ", FW_VERSION_MAJOR, FW_VERSION_MINOR);
    printf("(build: %s - %s)\r\n", __TIME__, __DATE__);
    
    // configure CAN-RS PDC & HDC led pins as outputs
    CAN_RS_PIN_PORT &= ~(CAN_RS_PIN_BIT);
    PDC_LED_PIN_PORT &= ~(PDC_LED_PIN_BIT);
    HDC_LED_PIN_PORT &= ~(HDC_LED_PIN_BIT);
    
    // put CAN transceiver into high-speed mode (RS->LOW)
    CAN_RS_PIN = 0;
    
    // turn off LED outputs
    HDC_LED_PIN = 0;
    PDC_LED_PIN = 0; 

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    ECAN_SetRXB0InterruptHandler(CanRxIRQHandler);
    ECAN_SetRXB1InterruptHandler(CanRxIRQHandler);
    TMR0_SetInterruptHandler(Timer0IRQHandler);
    ADC_SetInterruptHandler(ADCIRQHandler);
    
    TMR0_StartTimer();
    
    // configure CAN to accept standard or extended messages
    //CanFilterSetupId(dEXTENDED_CAN_MSG_ID_2_0B);
    CanFilterSetupId(dSTANDARD_CAN_MSG_ID_2_0B);
    
    // disable CAN message filters
    //RXB0CONbits.RXM0 = 1;
    //RXB0CONbits.RXM1 = 1;
    
    // start ADC conversion
    ADC_StartConversion(BTN_ADC_CHANNEL);
            
#ifdef DEBUG_MODE
    uint64_t prev = 0;
#endif // DEBUG_MODE
        
    while (1) {
        CanHandleRxMessage();
        ReadButtonState();

        uint64_t now = g_Millis;

#ifdef DEBUG_MODE
        if (now - prev >= 500) {
            printf("Alive\r\n");
            prev = now;
        }
#endif // DEBUG_MODE

        if (!g_WakeFlag && now - g_SleepCnt >= 5000) {
            g_SleepCnt = now;

            printf("No Keep-Alive message received\r\n");
            EnterSleepMode();
        }
    }
}