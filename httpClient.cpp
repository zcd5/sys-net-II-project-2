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
using namespace std;


int main(){

int tcp_client_socket;
struct sockaddr_in tcp_server_address;


//Creating the Socket
tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0);
if(tcp_client_socket < 0){
    perror("Error creating socket");
    exit(1);
}

tcp_server_address.sin_family = AF_INET;
tcp_server_address.sin_port = htons(60002); 
tcp_server_address.sin_addr.s_addr = INADDR_ANY;

int connection = connect(tcp_client_socket, (struct sockaddr *)&tcp_server_address, sizeof(tcp_server_address)); 
if(connection < 0){
    perror("Error connecting to the server!");
    exit(1);
}

char tcp_server_response[256];
recv(tcp_client_socket, &tcp_server_response, sizeof(tcp_server_response), 0);

printf("\n\n Server says: %s \n", tcp_server_response);

close(tcp_client_socket);


    return 0;
}