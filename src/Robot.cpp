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
#include "DoubleSolenoid.h"
using namespace frc;
using namespace std;
class Robot: public frc::SampleRobot
{
	ADXRS450_Gyro* Gyro;
	Encoder* dbEncoders[2];
	Spark* Motors[4];
	Spark* Shooter;
	Joystick* stick[2]; // only joystick
	Joystick* gamepad;
	Spark* Winch;
	Spark* Intake;
	DoubleSolenoid* Pistons[3];
	frc::SendableChooser<std::string> chooser;

public:

	Robot() {
		//REVIEW GYRO AND ENCODER PORTS
		Gyro = new ADXRS450_Gyro(SPI::Port::kOnboardCS0);
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
		Winch = new Spark(7);
		Intake = new Spark(8);
		Pistons[0] = new DoubleSolenoid(6,1);
		Pistons[1] = new DoubleSolenoid(5,1);
		Pistons[2] = new DoubleSolenoid(4,2);
	}
	void Default(){
		//The Moses Classic
		for(int x = 0; x<4; x++)
					Motors[x]->Set(1.0);
	}
	void EncoderReset()
		{
			dbEncoders[LEFT]->Reset();
			dbEncoders[RIGHT]->Reset();
		}
		void GyroReset()
		{
			Gyro->Reset();
		}
	void LeftPeg(){
		for(int x = 0; x<4; x++)
				Motors[x]->Set(1.0);

		while(dbEncoders[0]->GetDistance()/3 <= 12*5+5.3){}

		for(int x = 0; x<2; x++)
					Motors[x]->Set(0.45);
		for(int x = 2; x<4; x++)
						Motors[x]->Set(-0.45);
		//CHECK WHICH DIRECTION THIS IS ORIENTED TOWARDS
		while (fabs(Gyro->GetAngle()) < 60){}

		for(int x = 0; x<4; x++)
					Motors[x]->Set(1.0);

		while(dbEncoders[0]->GetDistance()/3 <= 60){}

		for(int x = 0; x<4; x++)
						Motors[x]->Set(0.0);
	}
	void RightPeg(){

		for(int x = 0; x<4; x++)
				Motors[x]->Set(1.0);

		while(dbEncoders[0]->GetDistance()/3 <= 12*5+5.3){}

		for(int x = 0; x<2; x++)
					Motors[x]->Set(-0.45);
		for(int x = 2; x<4; x++)
						Motors[x]->Set(0.45);
		//CHECK WHICH DIRECTION THIS IS ORIENTED TOWARDS
		while (fabs(Gyro->GetAngle()) < 60){}

		for(int x = 0; x<4; x++)
					Motors[x]->Set(1.0);

		while(dbEncoders[0]->GetDistance()/3 <= 60){}

		for(int x = 0; x<4; x++)
						Motors[x]->Set(0.0);
	}
	void MiddlePeg()
	{
		EncoderReset();
		GyroReset();

		for(int x = 0; x<4; x++)
				Motors[x]->Set(1.0);

		while(dbEncoders[0]->GetDistance()/3 <= 12*5+5.3){}

		for(int x = 0; x<4; x++)
				Motors[x]->Set(0.0);
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
		if(autoSelected == autoNameLeftPeg){this->LeftPeg();}
		else if(autoSelected == autoNameRightPeg){this->RightPeg();}
		else if(autoSelected == autoNameMiddlePeg){this->MiddlePeg();}
		else{this->Default();}

		//Kill it with fire
	}

	void OperatorControl() override
	{

		while (IsOperatorControl() && IsEnabled()) {
			Motors[0]->Set(stick[LEFT]->GetY()*SpeedConstant);
			Motors[1]->Set(stick[LEFT]->GetY()*SpeedConstant);
			Motors[2]->Set(stick[RIGHT]->GetY()*SpeedConstant);
			Motors[3]->Set(stick[RIGHT]->GetY()*SpeedConstant);
			if(stick[LEFT]->GetRawButton(0)&&stick[RIGHT]->GetRawButton(0))
			{
				SpeedConstant = 0.45;
			}
			else
			{
				SpeedConstant = 1.0;
			}
			//Shooter Button
			if(gamepad->GetRawButton(A))//A
			{
				Shooter->Set(1.0);
			}
			else
			{
				Shooter->Set(0.0);
			}
			if(gamepad->GetRawButton(4))
			{
				Intake->Set(1.0);
			}
			else
			{
				Intake->Set(0.0);
			}
			if(gamepad->GetRawButton(B))
			{
				Winch->Set(1.0);
			}
			else
			{
				Winch->Set(0.0);
			}
			if(gamepad->GetRawButton(X))
			{
				Pistons[0]->Set(DoubleSolenoid::kForward);
				Pistons[1]->Set(DoubleSolenoid::kForward);
			}
			else
			{
				Pistons[0]->Set(DoubleSolenoid::kReverse);
				Pistons[1]->Set(DoubleSolenoid::kReverse);
			}
			frc::Wait(0.005);
		}

	}
};

START_ROBOT_CLASS(Robot)
