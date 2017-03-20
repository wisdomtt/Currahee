#ifndef SRC_AUTONS_AUTON_H_
#define SRC_AUTONS_AUTON_H_
#include "WPILib.h"
#include "Math.h"
#include "Values.h"
using namespace frc;
class Auton{
	public:
		ADXRS450_Gyro* pGyro;
		Encoder* pdbEncoders[2];
		Spark* pMotors[4];
		Spark* pShooter;
	Auton(ADXRS450_Gyro* Gyro, Encoder* dbEncoders[2], Spark* Motors[4], Spark* Shooter);
	~Auton();
	void MiddlePeg();
	void LeftPeg();
	void RightPeg();
	void Default();
	void EncoderReset();
	void GyroReset();

};

#endif
