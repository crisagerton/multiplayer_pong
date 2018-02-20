#ifndef PONGPHYSICSENGINE_H
#define PONGPHYSICSENGINE_H
#include <utility>;
#include <vector>;

class PongPhysicsEngine {
public:
	PongPhysicsEngine(double angle, double xMin, double yMin, double xMax, double yMax);

	void moveBall(double movementSpeed);

	void movePaddle(int i, int dir, double movementSpeed);
	std::pair<double, double> getPaddleCoordinates(int i);
	std::pair<double, double> getBallCoordinates();
	int getPlayerScore(int playerNum);
	void resetTo(double angle, double xMin, double yMin, double xMax, double yMax);

	int timer = 0;

private:
	std::pair<double, double> ballCoordinates;

	std::vector<std::pair<double, double>> paddleCoordinates;

	double ballAngle;

	double screenXMin;
	double screenYMin;
	double screenXMax;
	double screenYMax;

	unsigned int paddleLastTouched;
	unsigned int playerScores[4] = { 0,0,0,0 };

	const double PI = 3.141592653589793238463;
};

#endif