#include "task_odrive.h"
#include "main.h"

extern Axis odrive_axis[4]; 

void odrive_task(void const *pvParameters)
{
	
	//Init Axis Id and CAN Instance
  odrive_axis[0].AXIS_ID = CAN_ODRIVE_M1_ID;
  odrive_axis[1].AXIS_ID = CAN_ODRIVE_M2_ID;

  HAL_Delay(100);

  //Example Code
  Set_Controller_Modes(odrive_axis[0], VELOCITY_CONTROL, PASSTHROUGH);
  Set_Axis_Requested_State(odrive_axis[0], CLOSED_LOOP_CONTROL);
  Set_Input_Vel(odrive_axis[0], 2, 0);
  HAL_Delay(1000);
  Set_Input_Vel(odrive_axis[0], 0, 0);
	
	
	Set_Controller_Modes(odrive_axis[1], VELOCITY_CONTROL, PASSTHROUGH);
  Set_Axis_Requested_State(odrive_axis[0], CLOSED_LOOP_CONTROL);
  Set_Input_Vel(odrive_axis[1], 2, 0);
  HAL_Delay(1000);
  Set_Input_Vel(odrive_axis[1], 0, 0);
}

