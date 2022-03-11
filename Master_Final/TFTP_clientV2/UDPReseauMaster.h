#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
// je transfère les.h du main vers le UDPSender.h
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <list>
#include "tinyxml.h"

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")


#define TAILLE_BUFFER 1024
#define CHEMIN_PHYSIQUE "ConfigReseau.xml"  // Chemin physique où ce situt le fichier de configutation ex : "C:\\Users\\Gustave\\Desktop\\ConfigReseau.xml"
#define TIMEOUT 1

class User {

public:
	std::string ip, hostname, username, os, nbvideo;
};


class UDPReseauMaster{

private:

	//Commun
	int iResult = 0;
	WSADATA wsaData;

	//SenderUDP
	SOCKET SendSocket = INVALID_SOCKET;
	sockaddr_in SendAddr;
	unsigned short PortSend;

	std::string receiverAdress;

	char SendBuf[TAILLE_BUFFER];
	int SendBufLen = TAILLE_BUFFER;

	//ReceiverUDP
	SOCKET RecvSocket = INVALID_SOCKET;
	sockaddr_in RecvAddr;
	unsigned short PortRecv;
	
	char RecvBuf[TAILLE_BUFFER];
	int RecvBufLen = TAILLE_BUFFER;
	
	sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);

	//Config Présente
	std::list<User> config;
	int nb_User = 0;
	User un_User;
	std::string derniere_date;

public:

	UDPReseauMaster(int portSend, std::string receiverAdress, int portRecv);
	void ecouter();
	void envoyer(std::string message);
	void effacerUneConfigEnregistrée(std::string list);
	void effacerToutesLesConfig();
	void effacerConfigPresente();
	std::string getConfigPresente();
	std::string getDerniereDate();
	std::string getUneConfigEnregistrée(std::string list);
	int enregistrerConfigPresente();
	void creerConfig(const char* dt);
	void recuperer_AdrIP(std::list<std::string>* ip);

	~UDPReseauMaster();
};

const std::string currentDateTime();