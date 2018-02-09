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

	double paddleMaxCoordX = paddleCoordinates.first + 100;
	double paddleMaxCoordY = paddleCoordinates.second + 10;

	double ballMaxCoordX = ballCoordinates.first + 20;
	double ballMaxCoordY = ballCoordinates.second + 20;

	// A Paddle
	if ((ballCoordinates.second < paddleMaxCoordY) && 
		(ballMaxCoordX > paddleCoordinates.first) && ballCoordinates.first <= paddleMaxCoordX && currentYMovement < 0){
		/*
		if (currentXMovement > 0) {
			ballAngle += 90;
		}
		else {
			ballAngle -= 90;
		}
		*/
		playerScores[0] += 1;
		ballAngle = 360 - ballAngle;
	}

	// Top Wall
	else if (ballCoordinates.second <= screenYMin && currentYMovement < 0) {
		/*
		if (currentXMovement > 0) {
			ballAngle += 90;
		}
		else {
			ballAngle -= 90;
		}
		*/
		ballAngle = 360 - ballAngle;
	}

	// Bottom Wall
	if (ballMaxCoordY >= screenYMax && currentYMovement > 0) {
		/*
		if (currentXMovement > 0) {
			ballAngle -= 90;
		}
		else {
			ballAngle += 90;
		}
		*/
		ballAngle = 360 - ballAngle;
	}

	// Left Wall
	if (ballCoordinates.first <= screenXMin && currentXMovement < 0) {
		/*
		if (currentYMovement > 0) {
			ballAngle -= 90;
		}
		else {
			ballAngle += 90;
		}
		*/
		ballAngle = 180 - ballAngle;
	}

	// Right Wall
	if (ballMaxCoordX >= screenXMax && currentXMovement > 0) {
		/*
		if (currentYMovement > 0) {
			ballAngle += 90;
		}
		else {
			ballAngle -= 90;
		}
		*/
		ballAngle = 180 - ballAngle;
	}

	ballCoordinates.first += movementSpeed * cos(ballAngle * PI / 180);
	ballCoordinates.second += movementSpeed * sin(ballAngle * PI / 180);
}

void PongPhysicsEngine::movePaddle(int dir, double movementSpeed) {
	// Depends which paddle this is
	int newX = paddleCoordinates.first + (dir * movementSpeed);
	if (newX > screenXMin && newX < (screenXMax-90)) {
		paddleCoordinates.first += dir * movementSpeed;
	}
	////try bouncing the ball thing only once
}

std::pair<double, double> PongPhysicsEngine::getPaddleCoordinates() {
	return paddleCoordinates;
}

std::pair<double, double> PongPhysicsEngine::getBallCoordinates() {
	return ballCoordinates;
}

int PongPhysicsEngine::getPlayerScore(int playerNum) {
	if (playerNum >= 0 && playerNum < 4) {
		return playerScores[playerNum];
	}
	return 0;
}