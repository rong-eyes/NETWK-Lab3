#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

int main(int argc, char *argv[]){	

	//need to do before startup according to Beej Guide
	//****STARTUP***
	{
		WSADATA wsaData;
		
		if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {	//checks if the library has initialized
			fprintf(stderr, "WSAStartup failed.\n");
			exit(1);
		}
		
		if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2){ //checks the version of Winsock
			fprintf(stderr,"Version 2.2 of Winsock not available.\n");
	        WSACleanup(); //function to cll when done using the Winsok library
	        exit(2);
		}
		
		printf("Winsock initialized successfully!\n");
	}
	
	//***STRUCT DECLARATIONS***
	{
		//addrinfo -> defines how we'll talk to the socket
		struct addrinfo {
			int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
    		int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
		    int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
		    int              ai_protocol;  // use 0 for "any"
		    size_t           ai_addrlen;   // size of ai_addr in bytes
		    struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
		    char            *ai_canonname; // full canonical hostname
		    struct addrinfo *ai_next;      // linked list, next node
		};
		
		//sockaddr -> holds the address info
		struct sockaddr{
			unsigned short sa_family;	//address family, AF_INET or AF_INET6 (IPv4 or IPv6)
			char		   sa_data[14]; //contains deest address and port number of the socket
		};
		
		//sockaddr_in -> for ease of reference of the socket addreess (for IPv4)
		struct sockaddr_in {
    	short int          sin_family;  // should be AF_INET
    	unsigned short int sin_port;    // Port number, must be in Network Byte Order using htons()
   		struct in_addr     sin_addr;    // Internet address
    	unsigned char      sin_zero[8]; // Same size as struct sockaddr, should be set to 0 w memset()
		};
		
		
	}
	
	 WSACleanup(); 
	 system("pause"); 
	return 0;
}

