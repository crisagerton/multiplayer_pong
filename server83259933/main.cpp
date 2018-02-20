#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <random>
#include "websocket.h"
#include "PongPhysicsEngine.h"

using namespace std;

webSocket server;
//random_device device;
//default_random_engine engine{ device() };
//uniform_int_distribution<int> distribution{ 0, 360 };
PongPhysicsEngine physics = PongPhysicsEngine(70, 94, 174, 494, 574);

/* called when a client connects */
void openHandler(int clientID) {
	/*ostringstream os;
	os << "Stranger " << clientID << " has joined.";

	vector<int> clientIDs = server.getClientIDsWithSamePortAs(clientID);
	for (int i = 0; i < clientIDs.size(); i++) {
		if (clientIDs[i] != clientID)
			server.wsSend(clientIDs[i], os.str());
	}*/
	server.wsSend(clientID, "Welcome!");
	server.wsSend(clientID, "u get"); ///get username from client

	vector<int> clientIDs = server.getClientIDsWithSamePortAs(clientID);
	for (int i = 0; i < clientIDs.size(); i++) {
		if (clientIDs[i] != clientID) {
			ostringstream os;
			os << "u";
			os << clientIDs[i] << " " << server.getClientUsername(clientIDs[i]);
			server.wsSend(clientID, os.str());
		}
	}
}

/* called when a client disconnects */
void closeHandler(int clientID) {
	ostringstream os;
	os << "c Stranger " << clientID << " has left.";
	ostringstream resetUsername;
	resetUsername << "u" << clientID << " No Player";

	vector<int> clientIDs = server.getClientIDsWithSamePortAs(clientID);
	for (int i = 0; i < clientIDs.size(); i++) {
		if (clientIDs[i] != clientID) {
			server.wsSend(clientIDs[i], os.str());
			server.wsSend(clientIDs[i], resetUsername.str());
		}
	}
	physics.resetTo(70, 94, 174, 494, 574);
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message) {
	//server.wsSend(clientID, message);
	int changeXorYby = 0;
	if (message == "10") { changeXorYby = 2; }
	if (message == "-10") { changeXorYby = -2; }
	physics.movePaddle(clientID, changeXorYby, 5); //call with clientID

	//add usernames to storage
	if (message.substr(0,1) == "u") {
		server.setClientUsername(clientID, message.substr(2));
		vector<int> clientIDs = server.getClientIDs();
		for (int i = 0; i < clientIDs.size(); i++) { //sending everything to client via encoded string messages
			ostringstream os;
			os << "u" << clientID << " " << message.substr(2);
			cout << os.str() << endl;
			server.wsSend(clientIDs[i], os.str());
		}
	}
}

/* called once per select() loop */
void periodicHandler() {
	static time_t next = time(NULL) + 1;
	time_t current = time(NULL);

	int currenttime = physics.timer;
	physics.timer++;

	if (current >= next) {
		next = time(NULL) + 1;
	}

	if (currenttime == 5) {
		physics.timer = 0;
		ostringstream os;
		std::pair<double, double> paddleCoords = physics.getPaddleCoordinates(0);
		ostringstream score;
		score << "s " << physics.getPlayerScore(0);

		if (server.getGameRoomMap()[1].size() >= 4) {
			physics.moveBall(8);
		}
		ostringstream ballCoordinates;
		std::pair<double, double> ballCoords = physics.getBallCoordinates();
		ballCoordinates << "b " << ballCoords.first << " " << ballCoords.second;

		ostringstream pc1;
		std::pair<double, double> padCoords = physics.getPaddleCoordinates(0);
		pc1 << "p t " << padCoords.first << " " << padCoords.second;
		ostringstream pc2;
		padCoords = physics.getPaddleCoordinates(1);
		pc2 << "p l " << padCoords.first << " " << padCoords.second;
		ostringstream pc3;
		padCoords = physics.getPaddleCoordinates(2);
		pc3 << "p r " << padCoords.first << " " << padCoords.second;
		ostringstream pc4;
		padCoords = physics.getPaddleCoordinates(3);
		pc4 << "p b " << padCoords.first << " " << padCoords.second;

		vector<int> clientIDs = server.getClientIDs();
		for (int i = 0; i < clientIDs.size(); i++) { //sending everything to client via encoded string messages
			server.wsSend(clientIDs[i], score.str());
			server.wsSend(clientIDs[i], ballCoordinates.str());
			server.wsSend(clientIDs[i], pc1.str()); //top paddle coordinates
			server.wsSend(clientIDs[i], pc2.str()); //left paddle coordinates
			server.wsSend(clientIDs[i], pc3.str()); //right paddle coordinates
			server.wsSend(clientIDs[i], pc4.str()); //bottom paddle coordinates
		}
	}
}

int main(int argc, char *argv[]) {

	//int ports[4] = { 8000,8001,8002,8003 };
	int ports[1] = { 8000 };
	
	/* set event handler */
	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(messageHandler);
	server.setPeriodicHandler(periodicHandler);

	/* start the chatroom server, listen to ip '127.0.0.1' and all ports in ports[4] */
	server.startServers(ports);

	return 1;
}
