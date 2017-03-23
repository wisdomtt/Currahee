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
#include "CV/GripPipeline.h"
#include <thread>
#include <CameraServer.h>
#include <IterativeRobot.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>

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
	int Iteration = 1;
	frc::SendableChooser<std::string> chooser;

public:

	Robot() {
		//REVIEW GYRO AND ENCODER PORTS
		Gyro = new ADXRS450_Gyro(SPI::Port::kOnboardCS0);
		dbEncoders[LEFT] = new Encoder(0,1);
		dbEncoders[RIGHT] = new Encoder(2,3);
		Motors[0] = new Spark(0);
		Motors[1] = new Spark(1);
		Motors[2] = new Spark(2);
		Motors[3] = new Spark(3);
		Shooter = new Spark(4);
		stick[LEFT] = new Joystick(0);
		stick[RIGHT] = new Joystick(1);
		gamepad = new Joystick(2);
		Winch = new Spark(7);
		Intake = new Spark(8);
		Pistons[0] = new DoubleSolenoid(6,0);
		Pistons[1] = new DoubleSolenoid(5,1);
		Pistons[2] = new DoubleSolenoid(4,2);
	}
	static void VisionThread()
	{
		// Get the USB camera from CameraServer
		cs::UsbCamera camera = CameraServer::GetInstance()->StartAutomaticCapture();
		// Set the resolution
		camera.SetResolution(640, 480);
		// Get a CvSink. This will capture Mats from the Camera
		cs::CvSink cvSink = CameraServer::GetInstance()->GetVideo();
		// Mats are very memory expensive. Lets reuse this Mat.
		cv::Mat mat;
		cvSink.GrabFrame(mat);
		grip::GripPipeline pipeline;
		pipeline.Process(mat);
		cv::Rect r1 = cv::boundingRect(pipeline.GetFilterContoursOutput()[0]);
		cv::Rect r2 = cv::boundingRect(pipeline.GetFilterContoursOutput()[1]);
		double centerX1 = r1.x + (r1.width / 2);
		double centerX2 = r2.x + (r2.width / 2);
		double turn = ((centerX1 + centerX2) / 2) - (640 / 2);
	}
	void Default(){
		Motors[0]->Set(1.0);
		Motors[1]->Set(1.0);
		Motors[2]->Set(1.0);
		Motors[3]->Set(1.0);
	}
	void Debug()
	{
		cout << "Iteration: " << endl << Iteration << endl;
		cout << "Left Encoder Inches:" << endl << dbEncoders[LEFT]->GetDistance()/3 << endl;
		cout << "Right Encoder Inches:" << endl << dbEncoders[RIGHT]->GetDistance()/3 << endl;
		cout << "Gyro Output:" << endl << Gyro->GetAngle() << endl;
		Iteration++;
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

		Motors[0]->Set(1.0);
		Motors[1]->Set(1.0);
		Motors[2]->Set(1.0);
		Motors[3]->Set(1.0);

		while(dbEncoders[0]->GetDistance()/3 <= 12*5+5.3){}

		Motors[0]->Set(0.45);
		Motors[1]->Set(0.45);

		Motors[2]->Set(-0.45);
		Motors[3]->Set(-0.45);
		//CHECK WHICH DIRECTION THIS IS ORIENTED TOWARDS
		while (fabs(Gyro->GetAngle()) < 60){}

		Motors[0]->Set(1.0);
		Motors[1]->Set(1.0);
		Motors[2]->Set(1.0);
		Motors[3]->Set(1.0);
		//If the wheels aren't turning, then encoders won't register
		Wait(2.0);
		//while(dbEncoders[0]->GetDistance()/3 <= 60){}

		Motors[0]->Set(0.0);
		Motors[1]->Set(0.0);
		Motors[2]->Set(0.0);
		Motors[3]->Set(0.0);

		Pistons[0]->Set(DoubleSolenoid::kForward);
		Pistons[0]->Set(DoubleSolenoid::kForward);
	}
	void RightPeg(){

		Motors[0]->Set(1.0);
		Motors[1]->Set(1.0);
		Motors[2]->Set(1.0);
		Motors[3]->Set(1.0);

		while(dbEncoders[0]->GetDistance()/3 <= 12*5+5.3){}

		Motors[0]->Set(-0.45);
		Motors[1]->Set(-0.45);

		Motors[2]->Set(0.45);
		Motors[3]->Set(0.45);
		//CHECK WHICH DIRECTION THIS IS ORIENTED TOWARDS
		while (fabs(Gyro->GetAngle()) < 60){}

		Motors[0]->Set(1.0);
		Motors[1]->Set(1.0);
		Motors[2]->Set(1.0);
		Motors[3]->Set(1.0);

		//If the wheels aren't turning, then encoders won't register
		Wait(2.0);
		//while(dbEncoders[0]->GetDistance()/3 <= 60){}

		Motors[0]->Set(0.0);
		Motors[1]->Set(0.0);
		Motors[2]->Set(0.0);
		Motors[3]->Set(0.0);

		Pistons[0]->Set(DoubleSolenoid::kForward);
		Pistons[1]->Set(DoubleSolenoid::kForward);

	}
	void MiddlePeg()
	{
		EncoderReset();
		GyroReset();
		Motors[0]->Set(1.0);
		Motors[1]->Set(1.0);
		Motors[2]->Set(1.0);
		Motors[3]->Set(1.0);

		while(dbEncoders[0]->GetDistance()/3 <= 12*5+5.3){}
		//ensure that the gear is on the peg
		Wait(0.5);
		Motors[0]->Set(0.0);
		Motors[1]->Set(0.0);
		Motors[2]->Set(0.0);
		Motors[3]->Set(0.0);
		Pistons[0]->Set(DoubleSolenoid::kForward);
		Pistons[1]->Set(DoubleSolenoid::kForward);
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

		thread visionThread(VisionThread);
		visionThread.detach();
	}

	void Autonomous()
	{
		while(IsAutonomous()&&IsEnabled())
		{
			auto autoSelected = chooser.GetSelected();
			if(autoSelected == autoNameLeftPeg){this->LeftPeg();}
			else if(autoSelected == autoNameRightPeg){this->RightPeg();}
			else if(autoSelected == autoNameMiddlePeg){this->MiddlePeg();}
			else{this->Default();}
		}
	}

	void OperatorControl() override
	{

		while (IsOperatorControl() && IsEnabled()) {
			Motors[0]->Set(-stick[LEFT]->GetY()*SpeedConstant);
			Motors[1]->Set(-stick[LEFT]->GetY()*SpeedConstant);
			Motors[2]->Set(-stick[RIGHT]->GetY()*SpeedConstant);
			Motors[3]->Set(-stick[RIGHT]->GetY()*SpeedConstant);
			if(stick[LEFT]->GetRawButton(1)&&stick[RIGHT]->GetRawButton(1))
			{
				SpeedConstant = 0.45;
			}
			else
			{
				SpeedConstant = 1.0;
			}
			//Shooter Button
			if(gamepad->GetRawButton(ABUTTON))//A
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
			if(gamepad->GetRawButton(BBUTTON))
			{
				Winch->Set(1.0);
			}
			else
			{
				Winch->Set(0.0);
			}
			if(gamepad->GetRawButton(XBUTTON))
			{
				Pistons[2]->Set(DoubleSolenoid::kForward);
				Pistons[1]->Set(DoubleSolenoid::kForward);
			}
			else
			{
				Pistons[2]->Set(DoubleSolenoid::kReverse);
				Pistons[1]->Set(DoubleSolenoid::kReverse);
			}
			if(gamepad->GetRawButton(5))
			{
				ButtonRelease = true;
			}
			else if(!gamepad->GetRawButton(5)&&ButtonRelease)
			{
				this->Debug();
				ButtonRelease = false;
			}
			if(gamepad->GetRawButton(6))
			{
				ButtonRelease2 = true;
			}
			else if(!gamepad->GetRawButton(6)&&ButtonRelease2)
			{
				this->EncoderReset();
				this->GyroReset();
				ButtonRelease2 = false;
			}
			frc::Wait(0.005);
		}

	}
};

START_ROBOT_CLASS(Robot)
