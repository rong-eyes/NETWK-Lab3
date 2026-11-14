#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib") 
#else
    #include <sys/socket.h>
    #include <netinet/in.h>

#endif

int main(int argc, char *argv[]) {
    // initialize Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    char server_name[64] = "Server of Yreana & Samira";
    srand(time(0)); 

    // create the TCP welcoming socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }
    printf("Server socket created successfully.\n");

    // configure server address and port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6885);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // bind socket to the port
    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        printf("Bind failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("Server successfully bound to Port 6885.\n");

    // listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        printf("Listen failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // structs matching client.c
    struct client_infoTag {
        char name[64];
        int num;
    } client_info;

    struct server_infoTag {
        char name[64];
        int num;
    } server_info;

    // keep accepting clients until an out-of-range integer is received
    while (1) {
        printf("\nServer is listening for incoming connections...\n");

        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            printf("Failed to accept client.\n");
            continue;
        }
        printf("Client connected.\n");

        // receive data from client
        int received = recv(client_socket, (char *)&client_info, sizeof(client_info), 0);
        if (received <= 0) {
            printf("Failed to receive data from client.\n");
            closesocket(client_socket);
            continue;
        }

        // check if client integer is out of range
        if (client_info.num < 1 || client_info.num > 100) {
            printf("Received out-of-range number (%d). Shutting down server...\n", client_info.num);
            closesocket(client_socket);
            break; // exit loop to close server_socket and terminate
        }

        // display names
        printf("\nClient name: %s\n", client_info.name);
        printf("Server name: %s\n", server_name);

        // pick server number, compute sum, and display
        int server_num = (rand() % 100) + 1;
        int sum = client_info.num + server_num;

        printf("Client number: %d\n", client_info.num);
        printf("Server number: %d\n", server_num);
        printf("Sum: %d\n", sum);

        // prepare and send reply to client
        strncpy(server_info.name, server_name, sizeof(server_info.name) - 1);
        server_info.name[sizeof(server_info.name) - 1] = '\0';
        server_info.num = server_num;

        int send_success = send(client_socket, (char *)&server_info, sizeof(server_info), 0);
        if (send_success < 0) {
            printf("Failed to send server information.\n");
        } else {
            printf("Server information sent to client.\n");
        }

        // close this client's connection before waiting for the next client
        closesocket(client_socket);
        printf("Client connection closed.\n");
    }

    // release main server socket and clean up Winsock
    closesocket(server_socket);
    WSACleanup();
    printf("Server shut down cleanly.\n");

    system("pause");
    return 0;
}
