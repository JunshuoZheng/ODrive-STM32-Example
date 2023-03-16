#ifndef BSP_CAN_H
#define BSP_CAN_H
#include "struct_typedef.h"
#include "CAN_defines.h"
#include "bsp_GPIO.h"


#define Transmit_Mailbox_Empty	CAN_IER_TMEIE
#define Fifo0_Message_Pending	CAN_IER_FMPIE0
#define Fifo1_Message_Pending	CAN_IER_FMPIE1
#define Fifo0_Full				CAN_IER_FFIE0
#define Fifo1_Full				CAN_IER_FFIE1
#define Fifo0_Overflow			CAN_IER_FOVIE0
#define Fifo1_Overflow			CAN_IER_FOVIE1

typedef struct CAN_RX_Typedef
{
	int message_timestamp;
	int data_length;
	int data[8];
	int filter_index;
	int frame_type;
	int id_type;
	int32_t ID;
}CAN_RX_Typedef;


typedef struct CAN_TX_Typedef
{
	int id_type;
	int frame_type;
	int send_timestamp;
	int32_t ID;
	int data_length;
	int data[8];
}CAN_TX_Typedef;


void CAN_Filter_Init(void);

void CAN_Send_Packet(CAN_HandleTypeDef *init, CAN_TX_Typedef *tx);

void CAN_Get_Packet(CAN_HandleTypeDef *init, CAN_RX_Typedef *rx);


#endif

