#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"
#include "PongPhysicsEngine.h"

using namespace std;

webSocket server;
PongPhysicsEngine physics = PongPhysicsEngine(90, -750, -750, 750, 750);

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
}

/* called when a client disconnects */
void closeHandler(int clientID) {
	ostringstream os;
	os << "Stranger " << clientID << " has left.";

	vector<int> clientIDs = server.getClientIDsWithSamePortAs(clientID);
	for (int i = 0; i < clientIDs.size(); i++) {
		if (clientIDs[i] != clientID)
			server.wsSend(clientIDs[i], os.str());
	}
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message) {
	server.wsSend(clientID, message);
	int y = 0;
	if (message == "10") { y = 10; }
	if (message == "-10") { y = -10; }
	physics.movePaddle(y, 5);
}

/* called once per select() loop */
void periodicHandler() {
	static time_t next = time(NULL) + 10;
	time_t current = time(NULL);
	if (current >= next) {
		ostringstream os;
		/*string timestring = ctime(&current);
		timestring = timestring.substr(0, timestring.size() - 1);
		os << timestring;*/
		os << "periodicHandler...";

		std::pair<double, double> paddleCoords = physics.getPaddleCoordinates();
		os << paddleCoords.first << " " << paddleCoords.second;
		os << " " << current << " " << next;

		ostringstream score;
		score << "s 100";

		
		vector<int> clientIDs = server.getClientIDs();
		for (int i = 0; i < clientIDs.size(); i++) {
			server.wsSend(clientIDs[i], os.str());
			server.wsSend(clientIDs[i], score.str());
		}
		next = time(NULL) + 10;


	}
}

int main(int argc, char *argv[]) {
	int ports[4] = { 8000,8001,8002,8003 };

	/* set event handler */
	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(messageHandler);
	server.setPeriodicHandler(periodicHandler);

	/* start the chatroom server, listen to ip '127.0.0.1' and all ports in ports[4] */
	server.startServers(ports);

	return 1;
}