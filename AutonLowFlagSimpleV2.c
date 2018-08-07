#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    gyro,           sensorGyro)
#pragma config(Sensor, I2C_1, leftEncoder ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2, rightEncoder ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           flipper,       tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           left1,         tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port3,           left2,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           left3,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           right1,        tmotorVex393_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port6,           right2,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           right3,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           flywheelTop,   tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           flywheelBottom, tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          liftMotors,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//PROPORTION AND INTEGRAL
//USING GYRO AND ENCODERS
#define abs(X) ((X<0)?-1*X:X)

void driveStraightDistance(int inches, int masterPower);
void powerLeftDrive(int power);
void powerRightDrive(int power);
void resetDriveEncoders();
int inchesToTicks(int inches, float gearRatio);

//Using Proportional control, we will drive forward in a straight line. Proportionality constant needs to be tested and fine tuned
task main()	{
driveStraightDistance(42, 70);
}

int inchesToTicks(int inches, float gearRatio)	{
	int tick = ((inches/(2*3.141528*2))*627.2)* gearRatio;
	return tick;
}

void driveStraightDistance(int inches, int masterPower)	{
	int tickGoal = inchesToTicks(inches, 0.5);
  //int totalTicks = 0;//This will count up the total encoder ticks despite the fact that the encoders are constantly reset.

  int error;
  float driveKp = 0.2; //follow testing procedure to fine tune
  float driveKi = 0.02; // fine tune
  float driveProportion;
  int driveIntegralRaw;
  float driveIntegral;

  int angleError;
  float angleKp = 0.2; //needs to be fine tuned
  float angleKi = 0.02; //needs to be fine tuned
	float angleProportion;
  int angleIntegralRaw;
  float angleIntegral;

  //float integralActiveZone = inchesToTicks(3, 0.5);//zone before target where proportionality constant starts to fail and cannot move the robot. use testing procedure to start integral active zone slightly before the kp starts to fail

  resetDriveEncoders(); //reset encoders
  SensorValue[gyro] = 90; //reset gyro
	powerLeftDrive(masterPower);//start drive motors
  powerRightDrive(masterPower);


  while(SensorValue[leftEncoder] < inchesToTicks(inches - 0.5, 0.5))	{ //comparing encoder value to 0.5 inch before the final target tick value
  	//first adjust master side based on target distance
  	error = tickGoal - SensorValue[leftEncoder];
  	driveProportion = error * driveKp;
		driveIntegralRaw += error;
  	/*if(error < integralActiveZone)	{ // Unless the robot is near the target, the integral will not start accumulating. Used to control integral variable
  		integralRaw += error;
  	}
  	else	{
  		integralRaw = 0;
  	}*/
  	driveIntegral = driveKi * driveIntegralRaw;
  	if(driveIntegral > 28)	{	//hard limit on integral value to prevent from blowing up. making sure motor speed doesnot exceed this value in the integral active zone. This value also has to be tested
  		driveIntegral = 28;
  	}

  	masterPower = driveProportion + driveIntegral; //adding proportion, integral, and later derivative
    powerLeftDrive(masterPower);
    powerRightDrive(masterPower);

    //next adjust slave side based on gyro data
		angleError = 90 - SensorValue[gyro];
		angleProportion = error * angleKp;
		angleIntegralRaw += error;
		angleIntegral = angleIntegralRaw * angleKi;

		powerRightDrive(masterPower - angleProportion - angleIntegral);
  }
  powerLeftDrive(0); // Stop the loop once the encoders have counted up the correct number of encoder ticks.
  powerRightDrive(0);
}

void powerLeftDrive(int power) {
   motor[left1] = power;
   motor[left2] = power * -1;
   motor[left3] = power;
}

void powerRightDrive(int power) {
   motor[right1] = power * -1;
   motor[right2] = power ;
   motor[right3] = power * -1;
}
void resetDriveEncoders()	{
	nMotorEncoder[leftEncoder] = 0;
  nMotorEncoder[rightEncoder] = 0;
}
