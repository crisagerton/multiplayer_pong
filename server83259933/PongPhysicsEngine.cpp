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
	// Account for offsets for paddle sizes and ball size

	double currentXMovement = movementSpeed * cos(ballAngle * PI / 180);
	double currentYMovement = movementSpeed * sin(ballAngle * PI / 180);

	double ballMaxCoordX = ballCoordinates.first + 20;
	double ballMaxCoordY = ballCoordinates.second + 20;
	// A Paddle
	if (ballCoordinates.first == paddleCoordinates.first && ballCoordinates.second == paddleCoordinates.second) {
		if (currentXMovement > 0) {
			ballAngle -= 90;
		}
		else {
			ballAngle += 90;
		}
	}

	// Top Wall
	if (ballCoordinates.second <= screenYMin) {
		if (currentXMovement > 0) {
			ballAngle += 90;
		}
		else {
			ballAngle -= 90;
		}
	}

	// Bottom Wall
	if (ballMaxCoordY >= screenYMax) {
		if (currentXMovement > 0) {
			ballAngle -= 90;
		}
		else {
			ballAngle += 90;
		}
	}

	// Left Wall
	if (ballCoordinates.first <= screenXMin) {
		if (currentYMovement > 0) {
			ballAngle -= 90;
		}
		else {
			ballAngle += 90;
		}
	}

	// Right Wall
	if (ballMaxCoordX >= screenXMax) {
		if (currentYMovement > 0) {
			ballAngle += 90;
		}
		else {
			ballAngle -= 90;
		}
	}
	if (ballAngle > 360) {
		ballAngle -= 360;
	}
	if (ballAngle < -360) {
		ballAngle += 360;
	}
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