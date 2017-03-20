/*
 * 5ft 5.3 from middle position to gear peg
And 7 ft 9.3 from side positions
*/
#include "Auton.h"
Auton::Auton(ADXRS450_Gyro* Gyro, Encoder* dbEncoders[2], Spark* Motors[4], Spark* Shooter)
{
	pGyro = Gyro;
	pdbEncoders[2] = dbEncoders[2];
	pMotors[4] = Motors[4];
	pShooter = Shooter;
}
//Probably Unnecessary
void Auton::EncoderReset()
{
	pdbEncoders[LEFT]->Reset();
	pdbEncoders[RIGHT]->Reset();
}
void Auton::GyroReset()
{
	pGyro->Reset();
}
//Probably Unnecessary
void Auton::MiddlePeg()
{
	EncoderReset();
	GyroReset();

	for(int x = 0; x<4; x++)
			pMotors[x]->Set(1.0);

	while(pdbEncoders[0]->GetDistance()/3 <= 12*5+5.3){}

	for(int x = 0; x<4; x++)
			pMotors[x]->Set(0.0);
}

void Auton::LeftPeg()
{

	for(int x = 0; x<4; x++)
			pMotors[x]->Set(1.0);

	while(pdbEncoders[0]->GetDistance()/3 <= 12*5+5.3){}

	for(int x = 0; x<2; x++)
				pMotors[x]->Set(0.45);
	for(int x = 2; x<4; x++)
					pMotors[x]->Set(-0.45);
	//CHECK WHICH DIRECTION THIS IS ORIENTED TOWARDS
	while (fabs(pGyro->GetAngle()) < 60){}

	for(int x = 0; x<4; x++)
				pMotors[x]->Set(1.0);

	while(pdbEncoders[0]->GetDistance()/3 <= 60){}

	for(int x = 0; x<4; x++)
					pMotors[x]->Set(0.0);
}

void Auton::RightPeg()
{

	for(int x = 0; x<4; x++)
			pMotors[x]->Set(1.0);

	while(pdbEncoders[0]->GetDistance()/3 <= 12*5+5.3){}

	for(int x = 0; x<2; x++)
				pMotors[x]->Set(-0.45);
	for(int x = 2; x<4; x++)
					pMotors[x]->Set(0.45);
	//CHECK WHICH DIRECTION THIS IS ORIENTED TOWARDS
	while (fabs(pGyro->GetAngle()) < 60){}

	for(int x = 0; x<4; x++)
				pMotors[x]->Set(1.0);

	while(pdbEncoders[0]->GetDistance()/3 <= 60){}

	for(int x = 0; x<4; x++)
					pMotors[x]->Set(0.0);
}
void Auton::Default(){
	//The Moses Classic
	for(int x = 0; x<4; x++)
				pMotors[x]->Set(1.0);
}
