#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	
	//WSA startup
	#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
	#endif
	
	//creating a socket
	int net_socket;
	net_socket = socket(AF_INET, SOCK_STREAM, 0); //IPv4, Socket Stream, TCP
	
	//check creaation status
	if(net_socket < 0){
		printf("Socket Creation failed.\n");
		return 1;
	}
	
	//setting up the ddress of the server it will connect to
	struct sockaddr_in server_addr;
	
	memset(&server_addr, 0, sizeof(server_addr)); //must be set as per Beej's guide
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(6885);	//the port it will connect to
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	
	//Accepting integer from user
	int client_num = 0;
	char client_name[64] = "Client of John Doe";
	printf("Enter an integer from 1 to 100: ");
	scanf("%d", &client_num);
		
	if(client_num < 1 || client_num > 100) {			//if int is out of range close the socket
		printf("Number out of range...\n Closing socket...\n");
		closesocket(net_socket);
		printf("Socket closed.\n");
		
		//peventing errors
			#ifdef _WIN32
		    closesocket(net_socket);
		    WSACleanup();
		#else
		    close(net_socket);
		#endif
		
		    return 1;
	}
	
	//initiating and checking connection to server
	int connect_success = connect(net_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)); //returns an integer indicating connection success
	
	if(connect_success == -1){
		printf("There was an error connecting to the server.\n\n");
		#ifdef _WIN32
        printf("Error code: %d\n", WSAGetLastError());
		#else
        perror("connect");
		#endif
	}else{
		printf("Connection successful!\n\n");
		
		//constructing message
		struct client_infoTag{
			char name[64]; //declaration of client_name: char client_name[64] = "Client of Hal Jordan";
			int num;
		} client_info;
		
		strncpy(client_info.name, client_name, sizeof(client_info.name) -1);
		client_info.name[sizeof(client_info.name) - 1] = '\0';
		client_info.num = client_num;
		
		//sending to the server
		int send_success = send(net_socket, (char *)&client_info, sizeof(client_info), 0);
		if(send_success == SOCKET_ERROR){
			printf("Failed to send message.\n");
		}
		
		//receiving message from server
		char server_mssg[256];
		
		int received = recv(net_socket, server_mssg, sizeof(server_mssg) -1, 0);
		if(received > 0){
			server_mssg[received] = '\0';
			printf("Server Response: \n\n %s \n", server_mssg);
		}else{
			printf("Failed to receive server response.\n");
		}
		
		//we close the connection once its done
		#ifdef _WIN32
    	closesocket(net_socket);
	    WSACleanup();
	#else
	    close(net_socket);
	#endif
	}

	system("pause"); 
	return 0;
}
