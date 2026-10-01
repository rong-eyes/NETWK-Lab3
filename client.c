#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib") // Links the Winsock library for MSVC
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

int main(int argc, char *argv[]){	
	
	//creating a socket
	int net_socket;
	net_socket = socket(AF_INET, SOCK_STREAM, 0); //IPv4, Socket Stream, TCP
	
	//setting up the ddress of the server it will connect to
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(6885);	//the port it will connect to
	server_addr.sin_addr.s_addr = INADDR_ANY; //shortcut if saying we'ree connecting IP 0.0.0---
	
	//initiating and checking connection to server
	int connect_success = connect(net_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)); //returns an integer indicating connection success
	
	if(connect_success == -1){
		printf("There was an error connecting to the server.\n\n");
	}else
		printf("Connection successful!\n\n");
		
	//receiving message from server
	char server_mssg[256];
	recv(net_socket, &server_mssg, sizeof(server_mssg), 0);
	
	//display message
	printf("Server Response: \n %s", server_mssg);
	
	//we close the connection once its done
	closesocket(net_socket);
	
	WSACleanup(); 	//must close
	system("pause"); 
	return 0;
}
