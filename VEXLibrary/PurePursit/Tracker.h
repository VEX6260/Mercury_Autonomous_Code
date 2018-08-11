
//Add system to get left and right inches
//Add system to get over all inches (average of both sides, should return zero for dPosition)

float deltaY;
float prevY;
float globalX;
float globalY;
void updateSystem();
float getLiftInches();
float getRightInches();
float getRelativeYInches();
float getHeading();
float getGlobalX();
float getGlobalY();


float getLeftInches () {
	//Fill In
}

float getRightInches () {
	//Fill IN
}

float getRelativeYInches () {
	return (getRightInches() + getLeftInches())/2;
}

float getHeading () {
	//Fill In
}

float getGlobalX() {
	return globalX;
}

float getGlobalY() {
	return globalY;
}

void updateSystem () {
	deltaY = getRelativeYInches() - prevY;
	float heading = getHeading();
	// globalX += Sin of the heading * the deltaY
	// globalY += cos of heading * the deltaY
	prevY = getRelativeYInches();
}
