//using namespace std;

#include "server.h"
#include <iostream>

Server::Server(int portAddress, long maxMemorySize, 
		int timeOutDurationInSec){
	port = portAddress;
	serverTerminationRequested = false;
	maxMemSize = maxMemorySize;
	timeOutSec = timeOutDurationInSec;
	lastHeartBit = 0;
}
bool Server::waitForConnection(){
	if(listen(server,10)!=0){
		closesocket(server);
		WSACleanup();
		return false;
	}
	sockaddr_in from;
	int fromlen=sizeof(from);
	// Wait for connection here
	std::cout<< "Waiting for connection ("<<"Port: "<<port<<") ... ";
	do{
		client=accept(server,(struct sockaddr*)&from,&fromlen);
	}while(client == INVALID_SOCKET);
	std::cout<<"Connected"<<std::endl;
	lastHeartBit = GetTickCount();
	
	return true;
}
bool Server::initialize(){
	// Initialize Winsock
	int wsaret=WSAStartup(0x101,&wsaData);
	if(wsaret!=NO_ERROR)
		return false;
	 // Create a SOCKET for connecting to server
	server=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(server==INVALID_SOCKET){
		WSACleanup();
		return false;
	}
	// Setup our socket address structure
	local.sin_family=AF_INET; //Address family
	local.sin_addr.s_addr=INADDR_ANY; //Wild card IP address
	local.sin_port=htons((u_short)port); //port to use

	// We are going to create a non-blocking socket
	u_long mode = 1;
	int ret = ioctlsocket(server,FIONBIO,&mode);
	if(ret!=0){
		WSACleanup();
		return false;
	}	
	// Attempt to connect to server
	if(bind(server,(sockaddr*)&local,sizeof(local))!=0){
		closesocket(server);
		WSACleanup();
		return false;
	}	
	return true;
}
void Server::startMainLoop(std::string (*callback)(std::vector<char>&)){
	std::vector<char> charBuff;
	unsigned long count2Sec = 0;
	std::string result = "";
	// Main loop for communication and heartbit monitoring
			while(true){
				result = "";
				// If data arrived ....
				if(checkRecvAny(charBuff)){
					// ... process it ... 
					//std::cout<<std::string(charBuff.begin(),charBuff.end())<<std::endl;
					result = callback(charBuff);
				}else{
					// If no data for a long time (3 sec)
					if ((howLongToDie()<timeOutSec) && (GetTickCount() - count2Sec)/1000.0 > 1.){	//0.25 new value, originally 1
						count2Sec = GetTickCount();
						sendStr("ok");	// Send shock pulse
						continue;
					}
					// Monitor heartbit
					//std::cout<<"How long to die? "<< howLongToDie()<<std::endl;
					// if dead, close server
					if(isDead()){
						closeServer();
						break;	// break free the loop
					}
				}
				if(!result.empty()){
					sendStr(result);
				}
				Sleep(100);
			}
}
bool Server::sendStr(std::string anyText){
	if (send(client, anyText.c_str(), anyText.size(), 0) == -1)             
		return false;
	return true;
}
bool Server::receiveStr(std::string &Data){
	//TODO: Implement it!!
	return false;
}
bool Server::checkRecvAny(std::vector<char> &data){
	std::vector<char> internalReceiveBuffer;
	int buffSize = 10485760; // 10MB buffer size
	char* buffer = new char[buffSize];
	int bytesReceived = 0;
	std::string strVersionBuffer;
	data.clear();

	std::memset(buffer,0,buffSize);	// reset buff
	// The loop continues grabbing the data until memory explodes
	// or the reception finishes. For small received data,
	// it will finish immedietely
	while(((bytesReceived = recv(client,buffer,buffSize,0))>0) 
		&& (internalReceiveBuffer.size()<maxMemSize)){
			// String version of the data just received
			strVersionBuffer = std::string(buffer,buffer+bytesReceived);
		// Debug
		std::cout<<"Received Bytes = "
			<<bytesReceived<<" : Stored Bytes = "<<internalReceiveBuffer.size()<<std::endl;
		//std::cout<<std::string(buffer,buffer+bytesReceived)<<std::endl;

		// If it is just an acknowledgement, update heartbit and break free
		if (strVersionBuffer.compare("ok")==0){
			lastHeartBit = GetTickCount();
			break;
		}else{
			// Otherwise, accumulate the data in internalBuffer
			for (int i = 0; i < bytesReceived;i++)
				internalReceiveBuffer.push_back(buffer[i]);
			// Record the time of last arrival
			lastHeartBit = GetTickCount();
		}
		Sleep(25);
	}
	if(internalReceiveBuffer.size() > 0){
		// Copy in the return data type
		data.clear();
		for (int i = 0; i < internalReceiveBuffer.size(); i++)
			data.push_back(internalReceiveBuffer[i]);

		// Debug
		std::cout<<data.size()<<std::endl;
	}
	delete buffer;
	return (data.size() != 0);
}
void Server::closeServer(){
	closesocket(client);
	closesocket(server);
	WSACleanup();
	std::cout<<"Connection Closed (Port: "<<port<<")"<<std::endl;
}
