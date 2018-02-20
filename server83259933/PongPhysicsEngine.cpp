#include "PongPhysicsEngine.h"
#include <cmath>

PongPhysicsEngine::PongPhysicsEngine(double angle, double xMin, double yMin, double xMax, double yMax) {
	ballCoordinates = std::make_pair(285, 360);

	ballAngle = angle;

	topPaddleCoordinates = std::make_pair(245, yMin);
	botPaddleCoordinates = std::make_pair(245, yMax);
	leftPaddleCoordinates = std::make_pair(xMin, 245);
	rightPaddleCoordinates = std::make_pair(xMax, 245);

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

	// Top Paddle
	if ((ballCoordinates.second <= topPaddleCoordinates.second + 10) &&
		(ballMaxCoordX >= topPaddleCoordinates.first) && ballCoordinates.first <= topPaddleCoordinates.first + 100 && currentYMovement < 0){
		playerScores[0] += 1;
		ballAngle = 360 - ballAngle;
	}

	// Bot Paddle
	if ((ballCoordinates.second >= botPaddleCoordinates.second) &&
		(ballMaxCoordX >= botPaddleCoordinates.first) && ballCoordinates.first <= botPaddleCoordinates.first + 100 && currentYMovement > 0) {
		playerScores[1] += 1;
		ballAngle = 360 - ballAngle;
	}

	// Left Paddle
	if ((ballCoordinates.second <= leftPaddleCoordinates.second + 100) &&
		(ballCoordinates.second >= leftPaddleCoordinates.second) && (ballCoordinates.first >= leftPaddleCoordinates.first + 10) && currentXMovement > 0) {
		playerScores[2] += 1;
		ballAngle = 360 - ballAngle;
	}

	// Right Paddle
	if ((ballCoordinates.second <= rightPaddleCoordinates.second + 100) &&
		(ballCoordinates.second >= rightPaddleCoordinates.second) && (ballCoordinates.first <= rightPaddleCoordinates.first) && currentXMovement < 0) {
		playerScores[3] += 1;
		ballAngle = 360 - ballAngle;
	}

	// Top Wall
	else if (ballCoordinates.second <= screenYMin && currentYMovement < 0) {
		ballAngle = 360 - ballAngle;
	}

	// Bottom Wall
	if (ballMaxCoordY >= screenYMax && currentYMovement > 0) {
		ballAngle = 360 - ballAngle;
	}

	// Left Wall
	if (ballCoordinates.first <= screenXMin && currentXMovement < 0) {
		ballAngle = 180 - ballAngle;
	}

	// Right Wall
	if (ballMaxCoordX >= screenXMax && currentXMovement > 0) {
		ballAngle = 180 - ballAngle;
	}

	ballCoordinates.first += movementSpeed * cos(ballAngle * PI / 180);
	ballCoordinates.second += movementSpeed * sin(ballAngle * PI / 180);
}

void PongPhysicsEngine::movePaddle(int dir, double movementSpeed) {
	// Depends which paddle this is
	int newX = topPaddleCoordinates.first + (dir * movementSpeed);
	if (newX > screenXMin && newX < (screenXMax-90)) {
		topPaddleCoordinates.first += dir * movementSpeed;
	}
}

std::pair<double, double> PongPhysicsEngine::getPaddleCoordinates() {
	return topPaddleCoordinates;
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

void PongPhysicsEngine::resetTo(double angle, double xMin, double yMin, double xMax, double yMax) {
	ballCoordinates = std::make_pair(285, 360);

	ballAngle = angle;

	topPaddleCoordinates = std::make_pair(245, yMin);

	screenXMin = xMin;
	screenYMin = yMin;
	screenXMax = xMax;
	screenYMax = yMax;

	paddleLastTouched = NULL;
}