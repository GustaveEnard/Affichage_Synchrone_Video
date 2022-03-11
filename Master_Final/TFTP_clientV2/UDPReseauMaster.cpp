#include "UDPReseauMaster.h"


//Retourne la date 
const std::string currentDateTime() {
	
	const char* months[] = {
		"Janvier", "Février", "Mars", "Avril", "Mai", "Juin", "Juillet",
		"Août", "Septembre", "Octobre", "Novembre", "Décembre"
	};
	const char* days[] = {
		"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"
	};

	time_t timestamp = time(NULL);
	struct tm* now = localtime(&timestamp);

	std::string espace = " ";
	std::string date = days[now->tm_wday] + espace + std::to_string(now->tm_mday) + espace + months[now->tm_mon] + espace + std::to_string(now->tm_year + 1900)
		+ espace + std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min)+ ":" + std::to_string(now->tm_sec);

	return date;
}

UDPReseauMaster::UDPReseauMaster(int portSend, std::string receiverAdress, int portRecv) : PortSend(portSend), receiverAdress(receiverAdress), PortRecv(portRecv)
{

	//Commun
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {

		wprintf(L"WSAStartup failed with error: %d\n", iResult);
	}

	//SenderUDP
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSocket == INVALID_SOCKET) {

		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
	}
	SendAddr.sin_family = AF_INET;
	SendAddr.sin_port = htons(PortSend);
	SendAddr.sin_addr.s_addr = inet_addr(this->receiverAdress.c_str());


	//ReceiverUDP
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (RecvSocket == INVALID_SOCKET) {

		wprintf(L"socket failed with error %d\n", WSAGetLastError());
	}
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(PortRecv);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	iResult = bind(RecvSocket, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
	if (iResult != 0) {

		wprintf(L"bind failed with error %d\n", WSAGetLastError());
	}
}


void UDPReseauMaster::ecouter() {
	
	int resultat = 0;
	//Variable stockant les informations des utilisateurs
	char hostname[255];
	char username[255];
	char os[255];
	char nbvideo[255];
	
	while (1){

		//Mise en place du TIMEOUT
		fd_set select_fds;                
		struct timeval timeout;           

		FD_ZERO(&select_fds);             
		FD_SET(RecvSocket, &select_fds);           
											 
		timeout.tv_sec = TIMEOUT;         
		timeout.tv_usec = 0;

		if (select(32, &select_fds, NULL, NULL, &timeout) == 0){

			break;
		}
		else{

			resultat = recvfrom(RecvSocket, RecvBuf, RecvBufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);

			//Remplissage des variables
			int i = 0;
			int j = 0;
			while (RecvBuf[i] != ';') {

				hostname[j] = RecvBuf[i];
				i++;
				j++;
			}
			hostname[j] = '\0';

			j = 0;
			i++;
			while (RecvBuf[i] != ';') {

				username[j] = RecvBuf[i];
				i++;
				j++;
			}
			username[j] = '\0';

			j = 0;
			i++;
			while (RecvBuf[i] != ';') {

				os[j] = RecvBuf[i];
				i++;
				j++;
			}
			os[j] = '\0';

			j = 0;
			i++;
			while (RecvBuf[i] != ';') {

				nbvideo[j] = RecvBuf[i];
				i++;
				j++;
			}
			nbvideo[j] = '\0';

			//Implémentation liste contenant la configuration réseaux
			un_User.ip = inet_ntoa(SenderAddr.sin_addr);
			un_User.hostname = hostname;
			un_User.username = username;
			un_User.os = os;
			un_User.nbvideo = nbvideo;
			config.push_back(un_User);
			un_User.ip = "";
			un_User.hostname = "";
			un_User.username = "";
			un_User.os = "";
			un_User.nbvideo = "";

			nb_User++;
		}
	}
}



void UDPReseauMaster::envoyer(std::string message) {

	int i = 0;
	for (auto c : message) {
		SendBuf[i] = c;
		i++;
	}
	SendBuf[i] = '\0';

	iResult = sendto(SendSocket,
		SendBuf, SendBufLen, 0, (SOCKADDR*)&SendAddr, sizeof(SendAddr));
}

void UDPReseauMaster::effacerUneConfigEnregistrée(std::string list) {

	TiXmlDocument doc(CHEMIN_PHYSIQUE);
	if (!doc.LoadFile()) {
		std::cerr << "erreur lors du chargement" << std::endl;
		std::cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;

	}

	bool trouve = false;
	TiXmlHandle hdl(&doc);
	TiXmlElement* f = doc.FirstChildElement();
	TiXmlElement* elem = hdl.FirstChildElement().FirstChildElement().Element();

	while (elem && !trouve) {
		if (std::string(elem->Attribute("date")) == list) {
			trouve = true;
			break;
		}
		elem = elem->NextSiblingElement();
	}

	if (!trouve)
		std::cerr << "user inexistant" << std::endl;
	else {

		f->RemoveChild(elem);
		doc.SaveFile(CHEMIN_PHYSIQUE);
	}
}

void UDPReseauMaster::effacerToutesLesConfig() {

	TiXmlDocument doc(CHEMIN_PHYSIQUE);
	if (!doc.LoadFile()) {
		std::cerr << "erreur lors du chargement" << std::endl;
		std::cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;

	}

	TiXmlHandle hdl(&doc);
	TiXmlElement* elem = hdl.FirstChildElement().Element();

	doc.RemoveChild(elem);

	TiXmlElement le_nouveau("ConfigReseau");
	doc.InsertEndChild(le_nouveau);
	doc.SaveFile(CHEMIN_PHYSIQUE);
}

void UDPReseauMaster::effacerConfigPresente() {

	config.clear();
}

std::string UDPReseauMaster::getConfigPresente() {

	std::string configtoreturn = "\n";
	
	for (auto i = config.begin(); i != config.end(); i++) {

		configtoreturn +="Adresse : " + i->ip + " , Hostname : " + i->hostname + " , Username : " + i->username + " , OS : " + i->os + " , Nombre de videos maximum : " + i->nbvideo + "\n";
	}

	return configtoreturn;
}

std::string UDPReseauMaster::getDerniereDate() {

	return derniere_date;
}

std::string UDPReseauMaster::getUneConfigEnregistrée(std::string list) {

	std::string config = "\n";

	TiXmlDocument doc(CHEMIN_PHYSIQUE);
	if (!doc.LoadFile()) {
		std::cerr << "erreur lors du chargement" << std::endl;
		std::cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;

	}

	bool trouve = false;
	TiXmlHandle hdl(&doc);
	TiXmlElement* f = doc.FirstChildElement();
	TiXmlElement* elem = hdl.FirstChildElement().FirstChildElement().Element();
	TiXmlElement* elem2 = NULL;

	while (elem && !trouve) {

		if (std::string(elem->Attribute("date")) == list) {

			trouve = true;
			break;
		}

		elem = elem->NextSiblingElement();
	}

	if (!trouve) {

		std::cerr << "user inexistant" << std::endl;
	}
	else {

		elem2 = elem->FirstChildElement();
	}

	std::string adr = "Adresse : ";
	std::string hst = " , Hostname : ";
	std::string usr = " , Username : ";
	std::string stros = " , OS : ";
	std::string vdmax = " , Nombre de videos maximum : ";
	if (elem2 != NULL) {

		while (elem2) {

			config += adr + elem2->Attribute("ip") + hst + elem2->Attribute("hostname") +
				usr + elem2->Attribute("username") + stros + elem2->Attribute("os")
				+ vdmax + elem2->Attribute("nbvideo") + "\n";
			elem2 = elem2->NextSiblingElement();
		}
	}

	doc.SaveFile(CHEMIN_PHYSIQUE);
	return config;
}

int UDPReseauMaster::enregistrerConfigPresente() {

	if (nb_User == 0) {
		std::cerr << "aucun user present dans la liste" << std::endl;
		return 1;
	}
	else {
		std::list<User>::iterator i;
		int erreur = 0;
		for (i = config.begin(); i != config.end(); i++) {
			if (i->ip == "" || i->nbvideo == "") {
				erreur++;
			}
		}
		if (erreur != 0) {
			std::cerr << erreur << " informations necessaire manquantes" << std::endl;
			return 2;
		}
		else {

			TiXmlDocument doc(CHEMIN_PHYSIQUE);
			if (!doc.LoadFile()) {
				std::cerr << "erreur lors du chargement" << std::endl;
				std::cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;

			}

			TiXmlHandle hdl(&doc);
			TiXmlElement* elem = hdl.FirstChildElement().FirstChildElement().Element();
			TiXmlElement* f = doc.FirstChildElement();

			derniere_date = currentDateTime();
			const char* dt = derniere_date.c_str();

			creerConfig(dt);


			//Parcourir la liste d'utilisateur
			std::list<User>::iterator i;
			for (i = config.begin(); i != config.end(); i++) {

				TiXmlDocument doc(CHEMIN_PHYSIQUE);
				if (!doc.LoadFile()) {
					std::cerr << "erreur lors du chargement" << std::endl;
					std::cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;
					break;
				}
				//Remplir les champs

				char* ip1 = (char*)i->ip.c_str();

				if (i->hostname == "") {
					i->hostname = "non renseignee";
				}
				char* hostname1 = (char*)i->hostname.c_str();

				if (i->username == "") {
					i->username = "non renseignee";
				}
				char* username1 = (char*)i->username.c_str();

				if (i->os == "") {
					i->os = "non renseignee";
				}
				char* os1 = (char*)i->os.c_str();
				char* nbvideo1 = (char*)i->nbvideo.c_str();

				bool trouve = false;
				TiXmlHandle hdl(&doc);
				TiXmlElement* elem = hdl.FirstChildElement().FirstChildElement().Element();

				while (elem && !trouve) {
					if (std::string(elem->Attribute("date")) == dt) {
						trouve = true;
						break;
					}
					elem = elem->NextSiblingElement();
				}

				if (!trouve) {
					std::cerr << "user inexistant" << std::endl;
				}
				else {

					//Ajout d'une config
					TiXmlElement le_nouveau("user");
					le_nouveau.SetAttribute("ip", ip1);
					le_nouveau.SetAttribute("hostname", hostname1);
					le_nouveau.SetAttribute("username", username1);
					le_nouveau.SetAttribute("os", os1);
					le_nouveau.SetAttribute("nbvideo", nbvideo1);

					elem->InsertEndChild(le_nouveau);
					doc.SaveFile(CHEMIN_PHYSIQUE); // enregistrement des modifications
					
				}

			}
		}
	}
	return 0;
}

void UDPReseauMaster::creerConfig(const char* dt) {
	TiXmlDocument doc(CHEMIN_PHYSIQUE);
	if (!doc.LoadFile()) {
		std::cerr << "erreur lors du chargement" << std::endl;
		std::cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;

	}

	TiXmlElement* f = doc.FirstChildElement();

	TiXmlElement le_nouveau("Config");
	le_nouveau.SetAttribute("date", dt);
	f->InsertEndChild(le_nouveau);
	doc.SaveFile(CHEMIN_PHYSIQUE);

}

UDPReseauMaster::~UDPReseauMaster()
{
	//SenderUDP
	iResult = closesocket(SendSocket);
	if (iResult == SOCKET_ERROR) {

		wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
	}

	//ReceiverUDP
	iResult = closesocket(RecvSocket);
	if (iResult == SOCKET_ERROR) {

		wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
	}

	//Commun
	WSACleanup();
}

void UDPReseauMaster::recuperer_AdrIP(std::list<std::string>* ip) {
	for (auto i = config.begin(); i != config.end(); i++) {

		ip->push_back(i->ip);
	}
}