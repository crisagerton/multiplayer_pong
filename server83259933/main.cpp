#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <random>
#include <queue>
#include "websocket.h"
#include "PongPhysicsEngine.h"
#define INTERVAL_MS 10

using namespace std;

webSocket server;
//random_device device;
//default_random_engine engine{ device() };
//uniform_int_distribution<int> distribution{ 0, 360 };
PongPhysicsEngine physics = PongPhysicsEngine(70, 94, 177, 494, 577);
int interval_clocks = CLOCKS_PER_SEC * INTERVAL_MS / 1000;
queue<pair<int,string>> messageQueue;

/* called when a client connects */
void openHandler(int clientID) {
	/*ostringstream os;
	os << "Stranger " << clientID << " has joined.";

	vector<int> clientIDs = server.getClientIDsWithSamePortAs(clientID);
	for (int i = 0; i < clientIDs.size(); i++) {
		if (clientIDs[i] != clientID)
			server.wsSend(clientIDs[i], os.str());
	}*/
	//server.wsSend(clientID, "Welcome!");
	ostringstream os;
	os << time(0) << " u get";
	server.wsSend(clientID, os.str()); ///get username from client
	//messageQueue.push(std::make_pair(1, os.str()));

	vector<int> clientIDs = server.getClientIDsWithSamePortAs(clientID);
	for (int i = 0; i < clientIDs.size(); i++) {
		if (clientIDs[i] != clientID) {
			ostringstream os;
			os << time(0) << " u";
			os << clientIDs[i] << " " << server.getClientUsername(clientIDs[i]);
			server.wsSend(clientID, os.str());
			//messageQueue.push(std::make_pair(1, os.str()));
		}
	}
}

/* called when a client disconnects */
void closeHandler(int clientID) {
	ostringstream os;
	os << time(0) << " c Stranger " << clientID << " has left.";
	ostringstream resetUsername;
	resetUsername << time(0) << " u" << clientID << " No Player";

	cout << "Client " << clientID << " has left the server" << endl;
	cout << "Remaining clients: ";
	vector<int> clientIDs = server.getClientIDsWithSamePortAs(clientID);
	for (int i = 0; i < clientIDs.size(); i++) {
		if (clientIDs[i] != clientID) {
			cout << clientIDs[i] << ", ";
			server.wsSend(clientIDs[i], os.str());
			server.wsSend(clientIDs[i], resetUsername.str());
		}
	}
	cout << endl;

	physics.resetTo(70, 94, 177, 494, 577);
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
			os << time(0)  <<" u" << clientID << " " << message.substr(2);
			//cout << os.str() << endl;
			server.wsSend(clientIDs[i], os.str());
		}
	}
}

/* called once per select() loop */
void periodicHandler() {
	static clock_t next = clock() + interval_clocks;
	clock_t current = clock();

	int currenttime = physics.timer;
	physics.timer++;

	if (current >= next) {
		//physics.timer = 0;
		ostringstream os;
		ostringstream score;
		score << time(0) << " s " << physics.getPlayerScore(0) << " " << physics.getPlayerScore(1) << " " <<
			physics.getPlayerScore(2) << " " << physics.getPlayerScore(3);

		if (server.getGameRoomMap()[1].size() >= 4) {
			physics.moveBall(4);
		}
		ostringstream ballCoordinates;
		std::pair<double, double> ballCoords = physics.getBallCoordinates();
		ballCoordinates << time(0) << " b " << ballCoords.first << " " << ballCoords.second;

		ostringstream pc1;
		std::pair<double, double> padCoords = physics.getPaddleCoordinates(0);
		pc1 << time(0) << " p t " << padCoords.first << " " << padCoords.second;
		ostringstream pc2;
		padCoords = physics.getPaddleCoordinates(2);
		pc2 << time(0) << " p l " << padCoords.first << " " << padCoords.second;
		ostringstream pc3;
		padCoords = physics.getPaddleCoordinates(3);
		pc3 << time(0) <<" p r " << padCoords.first << " " << padCoords.second;
		ostringstream pc4;
		padCoords = physics.getPaddleCoordinates(1);
		pc4 << time(0) <<" p b " << padCoords.first << " " << padCoords.second;


		//Sending all of this to the client
		//cout << "test " << current << endl;
		vector<int> clientIDs = server.getClientIDs();
		for (int i = 0; i < clientIDs.size(); i++) { //sending everything to client via encoded string messages
													 //Moving ball
			server.wsSend(clientIDs[i], ballCoordinates.str());

			//Setting scores
			server.wsSend(clientIDs[i], score.str());

			//Changing paddle positions
			server.wsSend(clientIDs[i], pc1.str()); //top paddle coordinates
			server.wsSend(clientIDs[i], pc2.str()); //left paddle coordinates
			server.wsSend(clientIDs[i], pc3.str()); //right paddle coordinates
			server.wsSend(clientIDs[i], pc4.str()); //bottom paddle coordinates
		}

		next = clock() + interval_clocks;
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
