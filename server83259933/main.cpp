#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <random>
#include <chrono>
#include "websocket.h"
#include "PongPhysicsEngine.h"
#define INTERVAL_MS 10

using namespace std;
using namespace std::chrono;

webSocket server;
//random_device device;
//default_random_engine engine{ device() };
//uniform_int_distribution<int> distribution{ 0, 360 };
PongPhysicsEngine physics = PongPhysicsEngine(70, 94, 177, 494, 577);
int interval_clocks = CLOCKS_PER_SEC * INTERVAL_MS / 1000;

//Latency variables
vector<pair<int, string>> latencyQueueSent; //stores timestamp (int) and message to be processed (string)
vector<tuple<int, int, string>> latencyQueueReceived; //stores timestamp (int), clientID (int) and message to be processed (string)
int latency = 500;
int minLatency = 0;
int maxLatency = 600;
int incrementBy = 1;
string latencyType; //"fixed", "random", or "incremental"

random_device device;
default_random_engine engine{ device() };
uniform_int_distribution<int> distribution{ minLatency, maxLatency };

int getLatency() {
	if (latencyType == "fixed") {
		return latency;
	}
	else if (latencyType == "random") {
		latency = distribution(device);
		return latency;
	}
	else if (latencyType == "incremental"){
		if (latency < maxLatency && server.getGameRoomMap()[1].size() >= 4) {
			latency += incrementBy;
			return latency;
		}
		return latency;
	}

	return 0;
}


/* called when a client connects */
void openHandler(int clientID) {
	ostringstream os;
	os << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() 
		<< " ug" << clientID ;
	server.wsSend(clientID, os.str()); ///get username from client

	vector<int> clientIDs = server.getClientIDsWithSamePortAs(clientID);
	for (int i = 0; i < clientIDs.size(); i++) {
		if (clientIDs[i] != clientID) {
			ostringstream os;
			os << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()
				<< " u";
			os << clientIDs[i] << " " << server.getClientUsername(clientIDs[i]);
			server.wsSend(clientID, os.str());
		}
	}
}

/* called when a client disconnects */
void closeHandler(int clientID) {
	ostringstream os;
	os << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() 
		<< " c Stranger " << clientID << " has left.";
	ostringstream resetUsername;
	resetUsername << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()
		<< " u" << clientID << " No Player";

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


void addMovementMessageToLatencyQueue(int clientID) {
	ostringstream pc; ///paddle coordinates
	std::pair<double, double> padCoords;

	pc << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

	switch (clientID) {
	case 0:
		padCoords = physics.getPaddleCoordinates(0);
		pc << " p t " << padCoords.first << " " << padCoords.second;
		break;
	case 1:
		padCoords = physics.getPaddleCoordinates(1);
		pc << " p b " << padCoords.first << " " << padCoords.second;
		break;
	case 2:
		padCoords = physics.getPaddleCoordinates(2);
		pc << " p l " << padCoords.first << " " << padCoords.second;
		break;
	case 3:
		padCoords = physics.getPaddleCoordinates(3);
		pc << " p r " << padCoords.first << " " << padCoords.second;
		break;
	}
	latencyQueueSent.push_back(make_pair(
		duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() + getLatency(), 
		pc.str()));
}

void processMessageFromClient(string message, int clientID) {
	if (message.substr(0, 1) == "p") {
		int changeXorYby = 0;
		if (message == "paddle move +") { changeXorYby = 2; }
		if (message == "paddle move -") { changeXorYby = -2; }
		///otherwise it's "paddle stop" so changeXorYby = 0;
		physics.movePaddle(clientID, changeXorYby, 5); //call with clientID
		addMovementMessageToLatencyQueue(clientID);
	}

	//add usernames to storage
	if (message.substr(0, 1) == "u") {
		server.setClientUsername(clientID, message.substr(2));
		vector<int> clientIDs = server.getClientIDs();
		for (int i = 0; i < clientIDs.size(); i++) { //sending everything to client via encoded string messages
			ostringstream os;
			os << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()
				<< " u" << clientID << " " << message.substr(2);
			//cout << os.str() << endl;
			server.wsSend(clientIDs[i], os.str());
		}
	}
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message) {
	latencyQueueReceived.push_back(make_tuple(
		duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() + getLatency(),
		clientID, message));

	/*if (message.substr(0, 1) == "p") {
		int changeXorYby = 0;
		if (message == "paddle move +") { changeXorYby = 2; }
		if (message == "paddle move -") { changeXorYby = -2; }
		///otherwise it's "paddle stop" so changeXorYby = 0;
		physics.movePaddle(clientID, changeXorYby, 5); //call with clientID
		addMovementMessageToLatencyQueue(clientID);
	}

	//add usernames to storage
	if (message.substr(0,1) == "u") {
		server.setClientUsername(clientID, message.substr(2));
		vector<int> clientIDs = server.getClientIDs();
		for (int i = 0; i < clientIDs.size(); i++) { //sending everything to client via encoded string messages
			ostringstream os;
			os << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()
				<<" u" << clientID << " " << message.substr(2);
			//cout << os.str() << endl;
			server.wsSend(clientIDs[i], os.str());
		}
	}*/
}


/* called once per select() loop */
void periodicHandler() {
	static clock_t next = clock() + interval_clocks;
	clock_t current = clock();

	int currenttime = physics.timer;
	physics.timer++;

	if (current >= next) {
		//physics.timer = 0;
		int systemTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		ostringstream os;
		ostringstream score;
		score << systemTime << " s " << physics.getPlayerScore(0) << " " << physics.getPlayerScore(1) << " " <<
			physics.getPlayerScore(2) << " " << physics.getPlayerScore(3);
		latencyQueueSent.push_back(make_pair(systemTime + getLatency(), score.str()));

		if (server.getGameRoomMap()[1].size() >= 2) { //TODO: Change back to 4
			physics.moveBall(4);
		}
		ostringstream ballCoordinates;
		std::pair<double, double> ballCoords = physics.getBallCoordinates();
		ballCoordinates << systemTime << " b " << ballCoords.first << " " << ballCoords.second;
		latencyQueueSent.push_back(make_pair(systemTime + getLatency(), ballCoordinates.str()));

		vector<int> clientIDs = server.getClientIDs();
		for (int i = 0; i < latencyQueueSent.size(); i++) {
			if (latencyQueueSent[i].first <= systemTime) {
				for (int j = 0; j < clientIDs.size(); j++) {
					server.wsSend(clientIDs[j], latencyQueueSent[i].second);
				}

				latencyQueueSent.erase(latencyQueueSent.begin() + i);
			}
		}
		for (int i = 0; i < latencyQueueReceived.size(); i++) {
			if (get<0>(latencyQueueReceived[i]) <= systemTime) {
				for (int j = 0; j < clientIDs.size(); j++) {
					processMessageFromClient(get<2>(latencyQueueReceived[i]), get<1>(latencyQueueReceived[i]));
				}
				latencyQueueReceived.erase(latencyQueueReceived.begin() + i);
			}
		}

		next = clock() + interval_clocks;
	}
}

int main(int argc, char *argv[]) {
	/* Setting latency: 
	uncomment one and comment the others
	to test different types of latency*/
	//latencyType = "fixed";
	latencyType = "random";
	//latencyType = "incremental"; latency = minLatency;

	/* set ports */
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
