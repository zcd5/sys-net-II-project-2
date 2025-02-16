/*
File name: httpClient.cpp
Description: Client that can send requests and messages to the Server
Authors: Zahria Davis, Jun Nguyen
COP4635
*/
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
// funciton to return a get request
string getrequest(string filetogo){
string thegetrequest;
// simple get request
    thegetrequest = "GET /" +filetogo+" HTTP/1.1\r\n";
    return thegetrequest;
}

int main(int argc, char *argv[]){
string serverAddress;
string filename;
//Checks if users enters the correct number of arguments
   if(argc <3){
    cout << "Insert Address(127.0.0.1): " << endl;
    getline(cin,serverAddress);
    //default empty
    filename = "";
   }
   else{
    serverAddress = argv[1];
    filename = argv[2];
   }

 
    
int tcp_client_socket;
char databuffer[3000];
struct sockaddr_in tcp_server_address;


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
close(tcp_client_socket);


string input;
string sentmessage;
// sending page if specified during run command
memset(databuffer, '\0', sizeof(databuffer));
if (filename.length()!=0){
    tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0);
    connection = connect(tcp_client_socket, (struct sockaddr *)&tcp_server_address, sizeof(tcp_server_address)); 
    send(tcp_client_socket,sentmessage.c_str(),sentmessage.length(),0);
    recv(tcp_client_socket, &databuffer, sizeof(databuffer), 0);
    cout << endl << "Recieved: \n" << databuffer;
}
while(true){
    // creates new socket and connects it
    tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0);
    connection = connect(tcp_client_socket, (struct sockaddr *)&tcp_server_address, sizeof(tcp_server_address)); 
    // clears databuffer
memset(databuffer, '\0', sizeof(databuffer));
    if(connection < 0){
        perror("Error connecting to the server!");
        exit(1);
    }  
    // gets user command
    cout << "\nexit (ends program)\ngoto (goes to a page)\nmessage (sends a direct message to)\nInput Command:";
    getline(cin,input);

    if(input.find("goto")!=string::npos){
        cout << "\ngoto selected,where would you like to go to?\nOptions:index.html,testPresence.html,img.jpg\nexample: \"index.html\" will take you to the index.html page";
        cout <<"\nInput: ";
        getline(cin,input);
        sentmessage=getrequest(input);
        send(tcp_client_socket,sentmessage.c_str(),sentmessage.length(),0);
        recv(tcp_client_socket, &databuffer, sizeof(databuffer), 0);
        cout << endl << "Recieved: \n" << databuffer;
    }
    else if(input.find("message")!=string::npos){
        cout << "\nmessage to send:";
        getline(cin,input);
        sentmessage = "message " + input;
        send(tcp_client_socket,sentmessage.c_str(),sentmessage.length(),0);
        recv(tcp_client_socket, &databuffer, sizeof(databuffer), 0);
        cout << endl << "Recieved: \n" << databuffer;
    }
    else if (input.find("exit")!=string::npos){
        cout << "\nprogram called to exit";
        close(tcp_client_socket);
        return 0;
    }
    else {
        cout << "command not found";
    }
    // deletes socket
    close(tcp_client_socket);
}

close(tcp_client_socket);

return 0;
}