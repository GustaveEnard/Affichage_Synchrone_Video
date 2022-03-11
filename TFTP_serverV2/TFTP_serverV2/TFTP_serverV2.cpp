/**
	TFTP Server
	Atliko: Vilius Paulauskas [3 kursas, IV grupe, MIF PS]
	Uzduotis: TFTP
**/


#include "main.h"
#include "tftp_server.h"
using namespace std;

int main() {

	int port = 5555;
	const char* tftpRoot = "C:\\Users\\genard\\Desktop\\BDDreception\\";

	cout << "Starting TFTP server on port " << port << endl;
	
#ifdef DEBUG
	TFTPServer server(port, tftpRoot);
#else
	TFTPServer server(port, tftpRoot);
#endif

	cout << "TFTP Server was shut down" << endl;

	return 1;

}

