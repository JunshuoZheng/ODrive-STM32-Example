#include "task_odrive.h"
#include "main.h"

extern Axis AXIS0; // Declare Axis Struct Instance


void odrive_task(void const *pvParameters)
{
	
	//Init Axis Id and CAN Instance
  AXIS0.AXIS_ID = 0x010;

  HAL_Delay(100);

  //Example Code
  Set_Controller_Modes(AXIS0, VELOCITY_CONTROL, PASSTHROUGH);
  Set_Axis_Requested_State(AXIS0, CLOSED_LOOP_CONTROL);
  Set_Input_Vel(AXIS0, 2, 0);
  HAL_Delay(1000);
  Set_Input_Vel(AXIS0, 0, 0);
	
}

