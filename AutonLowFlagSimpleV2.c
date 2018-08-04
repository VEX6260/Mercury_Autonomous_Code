#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           flipper,       tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           left1,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           left2,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           left3,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           right1,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           right2,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           right3,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           flywheelTop,   tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           flywheelBottom, tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          liftMotors,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//PROPORTION AND INTEGRAL

#define abs(X) ((X<0)?-1*X:X)

void driveStraightDistance(int inches, int masterPower, float gearRatio);
void powerLeftDrive(int power);
void powerRightDrive(int power);
void resetDriveEncoders();

//Using Proportional control, we will drive forward in a straight line. Proportionality constant needs to be tested and fine tuned
task main()	{
driveStraightDistance(42, 70, 0.5);
}

void driveStraightDistance(int inches, int masterPower, float gearRatio)	{
	int tickGoal = ((inches/(2*3.141528*2))*627.2)* gearRatio;
  int totalTicks = 0;//This will count up the total encoder ticks despite the fact that the encoders are constantly reset.

  int error;
  float driveKp = 0.2; //follow testing procedure to fine tune
  float driveKi = 0.02; // fine tune

  float proportion;
  int integralRaw;
  float integral;

 	bool timerBool = true; //timer based on error to stop oscillation

  resetDriveEncoders(); //reset encoders
	powerLeftDrive(masterPower);//start drive motors
  powerRightDrive(masterPower);
  //Monitor 'totalTicks', instead of the values of the encoders which are constantly reset.
  wait1Msec(40);
  while(timerBool)	{
  	totalTicks += (SensorValue[leftEncoder]+SensorValue[rightEncoder])/2;
  	error = tickGoal - totalTicks;
  	proportion = error * driveKp;

  	integralRaw += error;
  	integral = driveKi * integralRaw;

  	masterPower = proportion; //adding proportion, integral, and derivative

    powerLeftDrive(masterPower);
    powerRightDrive(masterPower);

		resetDriveEncoders();
    wait1Msec(40);

    if(error < 50)	{ //edit the condition later based on experimental results
    	timerBool = false;
    }

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
   motor[left1] = power * -1;
   motor[left2] = power ;
   motor[left3] = power * -1;
}
void resetDriveEncoders()	{
	SensorValue[leftEncoder] = 0;
  SensorValue[rightEncoder] = 0;
}
