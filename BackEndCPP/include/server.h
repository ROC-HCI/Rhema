#ifndef SERVER_H
#define SERVER_H


#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <Windows.h>

// Coded by Md. Iftekhar Tanveer (go2chayan@gmail.com)
// ***************************************************
//
// A TCP server for windows with heartbeat detection.
// No response for more than a certain amount of
// time (30 sec by default) indicates lost connection.
// Maximum data chunk can be arbitrarily varied.
// The server will check the heartbit. The responsibility of
// client is just to respond (by sending anything) if it listens "ok" from
// the server.
// The receiver should not send multiple things together (e.g. "ok" and
// main data)
class Server{ 
private:
	SOCKET server;
	WSADATA wsaData;
	sockaddr_in local;
	SOCKET client;
	int port;
	bool serverTerminationRequested;
	int timeOutSec;
	long maxMemSize;
	unsigned long lastHeartBit;
public:

	// Creates a new server. By default, maximum allowed data
	// size (to be received) is 1 GB but it can be changed
	Server(int portAddress, long maxMemorySize = 1073741824, 
		int timeOutDurationInSec = 30);
	// Attempts to create a non-blocking socket
	bool initialize();
	// Waits for client request. If succeed
	// returns zero. Otherwise, it returns
	// a status indicating in which step it
	// failed. See code for status codes.
	bool waitForConnection();
	// A blocking main loop with callback for data arrival.
	// The callback function must accept a std::vector<char>
	// as argument
	void startMainLoop(std::string (*callback)(std::vector<char>&));
	//void startMainLoop();
	// Sends a string of arbitrary length
	bool sendStr(std::string anyText);
	// Checks if any data is arrived
	// returns the data by receivedData argument
	bool checkRecvAny(std::vector<char> &receivedData);
	// Converts the output of checkRecvAny to String
	bool receiveStr(std::string &Data);
	inline bool isClosingRequested(){return serverTerminationRequested;};
	inline int howLongToDie(){return (timeOutSec - (GetTickCount() - lastHeartBit)/1000.0);};
	// Check if timeout or not
	inline bool isDead(){return howLongToDie() <= 0;};
	// Closes a server
	void closeServer();
};
#endif