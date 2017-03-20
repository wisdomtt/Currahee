#include "TeleopCommands.h"
TeleopCommands::TeleopCommands(Joystick* sticks[2], float SpeedConstant)
{
	SpeedControllerA = false;
	SpeedControllerB = false;
	SpeedControllerC = false;
	pSticks[LEFT] = sticks[LEFT];
	pSticks[RIGHT] = sticks[RIGHT];
	SpeedFlow = SpeedConstant;
}
void TeleopCommands::PositiveIncrement()
{
	if(pSticks[RIGHT]->GetRawButton(0))
	{
		SpeedControllerA = true;
	}
	if(!pSticks[RIGHT]->GetRawButton(0)&&SpeedControllerA){
		if(SpeedFlow <=0.9){
			SpeedFlow+=0.1;
		}
		SpeedControllerA = false;
	}
}
void TeleopCommands::NegativeIncrement()
{
	if(pSticks[LEFT]->GetRawButton(0))
	{
		SpeedControllerB = true;
	}
	if(!pSticks[LEFT]->GetRawButton(0)&&SpeedControllerB){
		if(SpeedFlow >0.1){
			SpeedFlow-=0.1;
		}
		SpeedControllerB = false;
	}
}
