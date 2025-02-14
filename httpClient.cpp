#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h>
#include <unistd.h> 
#include <cstring>
#include <arpa/inet.h>
using namespace std;


int main(int argc, char *argv[]){

//Checks if users enters the correct number of arguments
   if(argc != 3){
    cerr << "Usage: ./httpClient <address> <file name>" << endl;
    exit(1);
   }
 
    
int tcp_client_socket;
struct sockaddr_in tcp_server_address;
string serverAddress = argv[1];
string filename = argv[2];

//Creating the Socket
tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0);
if(tcp_client_socket < 0){
    perror("Error creating socket");
    exit(1);
}

//sets the server's address to the user's input
tcp_server_address.sin_family = AF_INET;
tcp_server_address.sin_port = htons(60002); 
tcp_server_address.sin_addr.s_addr = inet_addr(serverAddress.c_str());


//Connects to the server
int connection = connect(tcp_client_socket, (struct sockaddr *)&tcp_server_address, sizeof(tcp_server_address)); 
if(connection < 0){
    perror("Error connecting to the server!");
    exit(1);
}

//Receives connection establishment message from the server
char tcp_server_response[256];
recv(tcp_client_socket, &tcp_server_response, sizeof(tcp_server_response), 0);

cout << "Server:" << tcp_server_response << endl;

/*TODO: add infinite loop for sending requests*/


close(tcp_client_socket);

return 0;
}