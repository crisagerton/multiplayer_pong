#include "PongPhysicsEngine.h"
#include <cmath>

PongPhysicsEngine::PongPhysicsEngine(double angle, double xMin, double yMin, double xMax, double yMax) {
	ballCoordinates = std::make_pair(285, 360);

	ballAngle = angle;

	paddleCoordinates = std::make_pair(245, yMin);

	screenXMin = xMin;
	screenYMin = yMin;
	screenXMax = xMax;
	screenYMax = yMax;

	paddleLastTouched = NULL;
}

void PongPhysicsEngine::moveBall(double movementSpeed) {
	//if (ballCoordinates.first > xMax || )

	ballCoordinates.first += movementSpeed * cos(ballAngle * PI / 180);
	ballCoordinates.second += movementSpeed * sin(ballAngle * PI / 180);
}

void PongPhysicsEngine::movePaddle(int dir, double movementSpeed) {
	// Depends which paddle this is
	int newX = paddleCoordinates.first + (dir * movementSpeed);
	if (newX > screenXMin && newX < screenXMax) {
		paddleCoordinates.first += dir * movementSpeed;
	}
}

std::pair<double, double> PongPhysicsEngine::getPaddleCoordinates() {
	return paddleCoordinates;
}

std::pair<double, double> PongPhysicsEngine::getBallCoordinates() {
	return ballCoordinates;
}