/*
 * TeleopCommands.h
 *
 *  Created on: Mar 20, 2017
 *      Author: CurtisMason
 */

#ifndef SRC_TELEOPCOMMANDS_TELEOPCOMMANDS_H_
#define SRC_TELEOPCOMMANDS_TELEOPCOMMANDS_H_
#include "WPILib.h"
#include "Values.h"
class TeleopCommands{
	public:
		bool SpeedControllerA;
		bool SpeedControllerB;
		bool SpeedControllerC;
		float SpeedFlow;
		Joystick* pSticks[2];
	TeleopCommands(Joystick* sticks[2], float SpeedConstant);
	~TeleopCommands();
	void PositiveIncrement();
	void NegativeIncrement();
};




#endif /* SRC_TELEOPCOMMANDS_TELEOPCOMMANDS_H_ */
