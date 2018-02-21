#include "PongPhysicsEngine.h"
#include <cmath>
#include <iostream>

PongPhysicsEngine::PongPhysicsEngine(double angle, double xMin, double yMin, double xMax, double yMax) {
	ballCoordinates = std::make_pair(285, 360);

	ballAngle = angle;

	// 0 is top, 1 is bot, 2 is left, 3 is right
	paddleCoordinates.push_back(std::make_pair(245, yMin));
	paddleCoordinates.push_back(std::make_pair(245, yMax-10));
	paddleCoordinates.push_back(std::make_pair(xMin, 340));
	paddleCoordinates.push_back(std::make_pair(xMax-10, 340));

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

	if ((ballCoordinates.second <= paddleCoordinates[0].second + 10) &&
		(ballMaxCoordX >= paddleCoordinates[0].first) && (ballCoordinates.first <= paddleCoordinates[0].first + 100) && currentYMovement < 0) {
		// Top Paddle
		playerScores[0] += 1;
		ballAngle = 360 - ballAngle;
	} else if ((ballMaxCoordY >= paddleCoordinates[1].second) &&
		(ballMaxCoordX >= paddleCoordinates[1].first) && (ballCoordinates.first <= paddleCoordinates[1].first + 100) && currentYMovement > 0) {
		// Bot Paddle
		playerScores[1] += 1;
		ballAngle = 360 - ballAngle;
	} else if ((ballCoordinates.second <= paddleCoordinates[2].second + 100) &&
		(ballMaxCoordY >= paddleCoordinates[2].second) && (ballCoordinates.first <= paddleCoordinates[2].first + 10) && currentXMovement < 0) {
		// Left Paddle
		playerScores[2] += 1;
		ballAngle = 180 - ballAngle;
	} else if ((ballCoordinates.second <= paddleCoordinates[3].second + 100) &&
		(ballMaxCoordY >= paddleCoordinates[3].second) && (ballMaxCoordX >= paddleCoordinates[3].first) && currentXMovement > 0) {
		// Right Paddle
		playerScores[3] += 1;
		ballAngle = 180 - ballAngle;
	} else if (ballCoordinates.second <= screenYMin && currentYMovement < 0) {
		// Top Wall
		ballAngle = 360 - ballAngle;
	}  else if (ballMaxCoordY >= screenYMax && currentYMovement > 0) {
		// Bot Wall
		ballAngle = 360 - ballAngle;
	} else if (ballCoordinates.first <= screenXMin && currentXMovement < 0) {
		// Left Wall
		ballAngle = 180 - ballAngle;
	} else if (ballMaxCoordX >= screenXMax && currentXMovement > 0) {
		// Right Wall
		ballAngle = 180 - ballAngle;
	}

	ballCoordinates.first += movementSpeed * cos(ballAngle * PI / 180);
	ballCoordinates.second += movementSpeed * sin(ballAngle * PI / 180);
}


void PongPhysicsEngine::movePaddle(int i, int dir, double movementSpeed) {
	// Depends which paddle this is
	if (i == 0 || i == 1) { //top or bottom
		int newX = paddleCoordinates[i].first + (dir * movementSpeed);
		if (newX > screenXMin && newX < (screenXMax - 90)) {
			paddleCoordinates[i].first += dir * movementSpeed;
		}
	}
	else if (i == 2 || i == 3) { //left or right
		int newY = paddleCoordinates[i].second + (dir * movementSpeed);
		if (newY > screenYMin && newY < (screenYMax - 90)) {
			paddleCoordinates[i].second += dir * movementSpeed;
		}
	}
}


std::pair<double, double> PongPhysicsEngine::getPaddleCoordinates(int i) {
	return paddleCoordinates[i];
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

	// 0 is top, 1 is bot, 2 is left, 3 is right
	paddleCoordinates.push_back(std::make_pair(245, yMin));
	paddleCoordinates.push_back(std::make_pair(245, yMax - 10));
	paddleCoordinates.push_back(std::make_pair(xMin, 340));
	paddleCoordinates.push_back(std::make_pair(xMax - 10, 340));

	screenXMin = xMin;
	screenYMin = yMin;
	screenXMax = xMax;
	screenYMax = yMax;

	paddleLastTouched = NULL;
	playerScores[0] = 0;
	playerScores[1] = 0;
	playerScores[2] = 0;
	playerScores[3] = 0;
}