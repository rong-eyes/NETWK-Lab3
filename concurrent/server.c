#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib") // Links the Winsock library for MSVC
#else
    #include <sys/socket.h>
    #include <netinet/in.h>

#endif

//STRUCTS TO BE USED LATER
struct server_infoTag {
    char name[64];
    int num;
};

struct client_infoTag {
    char name[64];
    int num;
};

void * handle_connection(void *p_client){
	char server_name[256] = "Server of OA";
	char connect_confirm[256] = "Client has reached the server!";
	int client = *((int *)p_client);
	free(p_client);
		
	struct client_infoTag client_info;
    
    int received = recv(client,(char *)&client_info,sizeof(client_info), 0);
    
    if(received <= 0){
    	printf("Failed to receive.\n");
    	#ifdef _WIN32
        closesocket(client);
	#else
        close(client);
	#endif
	
		return NULL;
	}
	
	//Displaying client info
	printf("\nClient name: %s\n", client_info.name);

    printf("Server name: %s\n", server_name);
	
	//pickiing  number & displying
	srand(time(0));
	int server_num = (rand() % 100) + 1;
	
	int sum = client_info.num + server_num;
	
	printf("Client number: %d\n", client_info.num);
    printf("Server number: %d\n", server_num);
    printf("Sum: %d\n", sum);
	
	//prepping the server message
	struct server_infoTag server_info;
	
	strcpy(server_info.name, server_name);
	server_info.num = server_num;
	
	//sending message
	int send_success = send(client, (char *)&server_info, sizeof(server_info), 0);
	if (send_success < 0) {
        printf("Failed to send server information.\n");
    } else {
        printf("Server information sent to client.\n");
    }

	//closing sockwts
	#ifdef _WIN32
	    closesocket(client);
	#else
	    close(client);
	#endif

    printf("Thread shut down.\n");
}

int main(int argc, char *argv[]){
	
	//WSA startup
	#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
	#endif
	
	//setting up socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket < 0){
		printf("Socket Creation failed.\n");
		return 1;
	}
	
	//setting up sever ddress
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2814);	
	server_addr.sin_addr.s_addr = INADDR_ANY; //accepts any local connect
	
	//binding thee socket to the specifid port and IP
	if(bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0 ){
		printf("Bind Failed.\n");
		#ifdef _WIN32
        closesocket(server_socket);
        WSACleanup();
	#else
        close(server_socket);
	#endif

        return 1;	
	}
	
	printf("Server successfully bound to: Port 6885\n\n");
	
	//listen
	listen(server_socket, 5);
	
	while(true){
		printf(" Server is listening...");	//wait and accept
			//holding client socket
		int client_socket = accept(server_socket, NULL, NULL); //NULL because we ae conneecting fom local machine, not fom elsewhere
	
		if(client_socket < 0){
			printf("Failed to accept Client.\n");
	
			#ifdef _WIN32
	        	closesocket(server_socket);
	        	WSACleanup();
			#else
	        	close(server_socket);
			#endif
	
	        return 1;
		}else{
			printf("Client connected.\n");
		}
		pthread_t count;
		int *p_client = malloc(sizeof(int));
		*p_client = client_socket;
		pthread_create(&count, NULL, handle_connection,p_client);
		pthread_detach(count);
	}

	
	system("pause");
	return 0;
}
