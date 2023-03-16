#include "bsp_can.h"
#include "main.h"


extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;


void CAN_Filter_Init(void)
{

    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = 0x0000;
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 0;
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
	
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
		HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

	
//    HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);
//    HAL_CAN_Start(&hcan2);
//    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);

}


void CAN_Send_Packet(CAN_HandleTypeDef *init, CAN_TX_Typedef *tx)
{
	init -> Instance -> sTxMailBox[0].TDHR &= ~0xFFFFFFFF;
	init -> Instance -> sTxMailBox[0].TDLR &= ~0xFFFFFFFF;
	init -> Instance -> sTxMailBox[0].TDTR &= ~0xFFFFFFFF;
	init -> Instance -> sTxMailBox[0].TIR  &= ~0xFFFFFFFF;
//---------------------------------------------------------------------------------------------------------------------
	switch (tx->id_type)
	{
		case CAN_ID_Standard:
		{
			init -> Instance -> sTxMailBox[0].TIR  = tx->ID << 21;
			init -> Instance -> sTxMailBox[0].TIR  &= ~1 << 2;
		}
			break;
		case CAN_ID_Extended:
		{
			init -> Instance -> sTxMailBox[0].TIR  = tx->ID << 3;
			init -> Instance -> sTxMailBox[0].TIR  |= 1 << 2;
		}
			break;
	}
//---------------------------------------------------------------------------------------------------------------------
	switch (tx->frame_type)
	{
		case CAN_Frame_Data:
		{
			init -> Instance -> sTxMailBox[0].TIR  &= ~(1 << 1);
		}
			break;
		case CAN_Frame_Remote:
		{
			init -> Instance -> sTxMailBox[0].TIR  |= (1 << 1);
		}
			break;
	}
//---------------------------------------------------------------------------------------------------------------------
	init -> Instance -> sTxMailBox[0].TDTR = tx->data_length;
	init -> Instance -> sTxMailBox[0].TDTR &= ~CAN_TDT0R_TGT;
	init -> Instance -> sTxMailBox[0].TDHR = tx->data[7] << 24 | tx->data[6] << 16 | tx->data[5] << 8 | tx->data[4] << 0;
	init -> Instance -> sTxMailBox[0].TDLR = tx->data[3] << 24 | tx->data[2] << 16 | tx->data[1] << 8 | tx->data[0] << 0;
	init -> Instance -> sTxMailBox[0].TIR  |= (1 << 0);
	while(init -> Instance -> sTxMailBox[0].TIR & (1 << 0)){}

}

void CAN_Get_Packet(CAN_HandleTypeDef *init, CAN_RX_Typedef *rx)
{


	int frame_type = 0;
	int id_type = 0;



	id_type =  (CAN1 -> sFIFOMailBox[0].RIR & CAN_RI0R_IDE_Msk) >> CAN_RI0R_IDE_Pos ;
	frame_type = (CAN1 -> sFIFOMailBox[0].RIR & CAN_RI0R_RTR_Msk) >> CAN_RI0R_RTR_Pos ;

	if(id_type)
	{
		//Extended ID
		rx->id_type = CAN_ID_Extended;
		rx->ID = (init -> Instance -> sFIFOMailBox[0].RIR & CAN_RI0R_EXID_Msk) >> CAN_RI0R_EXID_Pos;
	}
	else
	{
		//Standard ID
		rx->id_type = CAN_ID_Standard;
		rx->ID = (init -> Instance -> sFIFOMailBox[0].RIR & CAN_RI0R_STID_Msk) >> CAN_RI0R_STID_Pos;
	}

	if(frame_type)
	{
		//RTR Frame
		rx->frame_type = CAN_Frame_Remote;
		rx->data_length = (init -> Instance -> sFIFOMailBox[0].RDTR & CAN_RDT0R_DLC_Msk) >> CAN_RDT0R_DLC_Pos;
		init -> Instance -> RF0R |= CAN_RF0R_RFOM0;
		while((init -> Instance -> RF0R & CAN_RF0R_RFOM0)){}
	}
	else
	{
		//Data Frame
		rx->frame_type = CAN_Frame_Data;
		rx->data_length = (init -> Instance -> sFIFOMailBox[0].RDTR & CAN_RDT0R_DLC_Msk) >> CAN_RDT0R_DLC_Pos;
		for(int i = 0; i < rx->data_length; i++)
		{
			if(i < 4)
			{
				rx->data[i] =  (init -> Instance -> sFIFOMailBox[0].RDLR & ( 0xFF << (8*i))) >> (8*i);
			}
			else
			{
				rx->data[i] =  (init -> Instance -> sFIFOMailBox[0].RDHR & ( 0xFF << (8*(i-4)))) >> (8*(i-4));
			}
		}

		init -> Instance -> RF0R |= CAN_RF0R_RFOM0;	}


}

