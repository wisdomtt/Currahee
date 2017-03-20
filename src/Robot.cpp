#include <iostream>
#include <string>
#include <Joystick.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <RobotDrive.h>
#include <Timer.h>
#include "Spark.h"
#include "ADXRS450_Gyro.h"
#include "Encoder.h"
#include "I2C.h"
#include "Values.h"
#include "Autons/Auton.h"
#include "TeleopCommands/TeleopCommands.h"
using namespace frc;
using namespace std;
class Robot: public frc::SampleRobot
{
	Auton* AutonScheduler;
	ADXRS450_Gyro* Gyro;
	Encoder* dbEncoders[2];
	Spark* Motors[4];
	Spark* Shooter;
	Joystick* stick[2]; // only joystick
	Joystick* gamepad;
	frc::SendableChooser<std::string> chooser;
	float SpeedConstant = 1.0;
	const std::string autoNameDefault = "Default";
	const std::string autoNameLeftPeg = "Left Peg";
	const std::string autoNameRightPeg = "Right Peg";
	const std::string autoNameMiddlePeg = "Middle Peg";
	bool SpeedControlA = false;
	bool SpeedControlB = false;
	bool SpeedControlC = false;



public:
	Robot() {

		//REVIEW GYRO AND ENCODER PORTS
		Gyro = new ADXRS450_Gyro(SPI::kMXP);
		dbEncoders[LEFT] = new Encoder(1,2);
		dbEncoders[RIGHT] = new Encoder(3,4);
		Motors[0] = new Spark(0);
		Motors[1] = new Spark(1);
		Motors[2] = new Spark(2);
		Motors[3] = new Spark(3);
		Shooter = new Spark(4);
		stick[LEFT] = new Joystick(1);
		stick[RIGHT] = new Joystick(2);
		gamepad = new Joystick(3);
		AutonScheduler = new Auton(Gyro, dbEncoders[2], Motors[4], Shooter);
	}

	void RobotInit()
	{

		//SDDP = Circumference/(Gear Ratio * encoder value)). When getting multiply by 1/3
		dbEncoders[LEFT]->SetDistancePerPulse((2.0*3.14159*3)/(1440*12.75));
		dbEncoders[RIGHT]->SetDistancePerPulse((2.0*3.14159*3)/(1440*12.75));
		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameLeftPeg, autoNameLeftPeg);
		chooser.AddObject(autoNameRightPeg, autoNameRightPeg);
		chooser.AddObject(autoNameMiddlePeg, autoNameMiddlePeg);

		frc::SmartDashboard::PutData("Auto Modes", &chooser);

	}

	void Autonomous()
	{
		auto autoSelected = chooser.GetSelected();
		if(autoSelected == autoNameLeftPeg){AutonScheduler->LeftPeg();}
		else if(autoSelected == autoNameRightPeg){AutonScheduler->RightPeg();}
		else if(autoSelected == autoNameMiddlePeg){AutonScheduler->MiddlePeg();}
		else{AutonScheduler->Default();}

		//Kill it with fire
		delete AutonScheduler;
	}

	void OperatorControl() override
	{

		while (IsOperatorControl() && IsEnabled()) {
			Motors[0]->Set(stick[LEFT]->GetY()*SpeedConstant);
			Motors[1]->Set(stick[LEFT]->GetY()*SpeedConstant);
			Motors[2]->Set(stick[RIGHT]->GetY()*SpeedConstant);
			Motors[3]->Set(stick[RIGHT]->GetY()*SpeedConstant);

			//PROPER INCREMENTAL ALGORITHM UPON TRIGGER RELEASE RIGHT JOYSTICK

			//PROPER INCREMENTAL ALGORITHM UPON TRIGGER RELEASE LEFT JOYSTICK




			//Shooter Button
			if(gamepad->GetRawButton(4))
			{
				Shooter->Set(1.0);
			}
			else
			{
				Shooter->Set(0.0);
			}

			frc::Wait(0.005);
		}

	}
};

START_ROBOT_CLASS(Robot)