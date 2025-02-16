/*
File name: httpServer.cpp
Description: Server that services request from a browser client
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
using namespace std;


char tcp_server_message[256] = " Hello, you have successfully connected to the TCP Server \n";

//function for serving files
string serve(string filename){

    ifstream file(filename.c_str());
    stringstream buffer;
    buffer << file.rdbuf();
    string htmlPage = buffer.str();
    string servecommand = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + htmlPage;
    return servecommand;
}


int main(){


struct sockaddr_in tcp_server_address;
int tcp_server_socket;
int tcp_client_socket;



//Creation of socket
tcp_server_socket = socket(AF_INET, SOCK_STREAM, 0);
if(tcp_server_socket == 0){
    perror("Socket creation failed");
    exit(1);
}

cout << "Server started" << endl;


//Setting the socket's address
tcp_server_address.sin_family = AF_INET;
tcp_server_address.sin_addr.s_addr = INADDR_ANY;
tcp_server_address.sin_port = htons(60002);

//Binding the socket
if(bind(tcp_server_socket, (struct sockaddr *) &tcp_server_address, sizeof(tcp_server_address)) < 0){
    perror("Binding failed");
    exit(1);
}

cout << "Server ready" << endl;

cout << "Server is listening on 127.0.0.1:60002" << endl;

//listening for connections
if(listen(tcp_server_socket, 5) < 0){
    perror("listen");
    exit(1);
}



tcp_client_socket = accept(tcp_server_socket, NULL, NULL);
cout << "Client has connected!" << endl;

// inital index.html website
string response = serve("index.html");

//send connection establishment message to the client
send(tcp_client_socket, tcp_server_message, sizeof(tcp_server_message), 0);
close(tcp_client_socket);

char databuffer[1500];
// loop opens, responds, and closes client sockets
while(true){
    
tcp_client_socket = accept(tcp_server_socket, NULL, NULL);
//check if it was accepted if not, keep looping till an accept arrives
if(tcp_client_socket < 0){
    continue;
}

// clears databuffer
memset(databuffer, '\0', sizeof(databuffer));
// recieve data from client
recv(tcp_client_socket, &databuffer, sizeof(databuffer),0);

cout << databuffer << endl;

// responds to requests
     // home page
if(strncmp(databuffer,"GET / ", 6)==0){
        cout << "Server is listening for client requests..." << endl;
        cout << "serving homepage(index.html)";
        response = serve("index.html");
        send(tcp_client_socket, response.c_str(), response.length(), 0); 
    }
else if(strncmp(databuffer,"GET /index.html", 15)==0){

        cout << "Fetching file" << endl;
        response = serve("index.html");
        send(tcp_client_socket, response.c_str(), response.length(), 0); 
    }
else if(strncmp(databuffer,"GET /testPresence.html", 22)==0){

        cout << "Fetching file" << endl;
        response = serve("testPresence.html");
        send(tcp_client_socket, response.c_str(), response.length(), 0); 
        cout << "File has been fetched" << endl;
    
    }
else if (strncmp(databuffer,"GET /img.jpg", 12)==0){
        cout << "Fetching image" << endl;
        // open image file and set to the eof 
        ifstream f("img.jpg", ios::ate);
        if(!f.is_open()) perror ("file not found");

        // get position of the end of the image file
        streampos size = f.tellg();
        char* image = new char [size];
        //start at the beginning of the image file and read unitl the end
        f.seekg (0, ios::beg);
        f.read (image, size);
        f.close();

        //Send the image to the client/browser
        string headers = "HTTP/1.0 200 OK\r\nContent-type: image/jpeg\r\n\r\n";
        send (tcp_client_socket, headers.data(), headers.length(), 0);      
        send (tcp_client_socket, image, size, 0);
        cout << "Image has been fetched" << endl;
    }
else if(strncmp(databuffer,"message", 7)==0){
    cout << "\nmessage recived: ";
    cout << databuffer << endl;
    //sending info to client saying info was recived
    response = "Server- this message was recived: " + string(databuffer);
    send(tcp_client_socket, response.c_str(), response.length(), 0); 
}
        // any other requests get a 404 response, i made a Small 404.html
else{
        response = serve("404.html");
        send(tcp_client_socket, response.c_str(), response.length(), 0); 
    
    }



close(tcp_client_socket);
}





//close the socket
close(tcp_server_socket);

return 0;
}