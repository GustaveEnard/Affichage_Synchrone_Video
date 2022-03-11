#ifndef TFTPCLIENT
#define TFTPCLIENT

#include "tftp_packet.h"
#include "main.h"
#include <list>
#include "main.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)
using namespace std;

#define COMMAND_GET 0
#define COMMAND_PUT 1

#define TFTP_CLIENT_SERVER_TIMEOUT 2000

#define TFTP_CLIENT_ERROR_TIMEOUT 0
#define TFTP_CLIENT_ERROR_SELECT 1
#define TFTP_CLIENT_ERROR_CONNECTION_CLOSED 2
#define TFTP_CLIENT_ERROR_RECEIVE 3
#define TFTP_CLIENT_ERROR_NO_ERROR 4
#define TFTP_CLIENT_ERROR_PACKET_UNEXPECTED 5

class TFTPClient {

private:

	const char* server_ip;
	int	server_port;

	//- kliento socketo descriptorius
	int socket_descriptor;

	//- socket'o endpoint'u strukturos
	struct sockaddr_in client_address;
	int connection;

	TFTP_Packet received_packet;

protected:

	int sendBuffer(char* buffer);
	int sendPacket(TFTP_Packet* packet);

public:

	TFTPClient(const char* ip, int port);
	~TFTPClient();

	void setPort(int port);
	int connectToServer();
	bool getFile(const char* filename, const char* destination);
	bool sendFile(const char* filename, const char* destination);

	int waitForPacket(TFTP_Packet* packet, int timeout_ms = TFTP_CLIENT_SERVER_TIMEOUT);
	bool waitForPacketACK(int packet_number, int timeout_ms = TFTP_CLIENT_SERVER_TIMEOUT);
	int waitForPacketData(int packet_number, int timeout_ms = TFTP_CLIENT_SERVER_TIMEOUT);

	void errorReceived(TFTP_Packet* packet);

	int tftpSend(int Port, const char* Source, const char* Destination, std::list<std::string>* ip);

};

class ETFTPSocketCreate : public std::exception {
	virtual const char* what() const throw() {
		return "Unable to create a socket";
	}
};

class ETFTPSocketInitialize : public std::exception {
	virtual const char* what() const throw() {
		return "Unable to find socket library";
	}
};

void DEBUGMSG(const char*);

#endif