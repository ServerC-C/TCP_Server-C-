#include<iostream>
#include<WS2tcpip.h>

#pragma comment(lib, "WS2_32.lib")
using namespace std;

void main() {

    //initiaize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);


    int wsOk = WSAStartup(ver,&wsData);


    if (wsOk != 0) {
        cerr << "Can't Initialize winsock ! Quiting" << endl;
        return;
    }
    

    //creat a socket
    SOCKET lisining = socket(AF_INET, SOCK_STREAM, 0);
    if (lisining == INVALID_SOCKET) {
        cerr << "Can't Creat a Socket! Quiting" << endl;
        return;
    }

    //bind the ip adrees and port to socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;//could also used inet pton...
    bind(lisining,(sockaddr*)& hint, sizeof(hint));



    //tell winsock the socket is for lisining 
    listen(lisining, SOMAXCONN);


    //wait for a connection
    sockaddr_in client;
    int clientsize = sizeof(client);
    SOCKET clientSocket = accept(lisining, (sockaddr*)&client, &clientsize);


    char host[NI_MAXHOST];
    char service[NI_MAXHOST];


    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXHOST);

    memset(host, 0, NI_MAXHOST);


    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        cout << host << "connected on port " << service << endl;
    }
    else{
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << "connected on port " <<
            ntohs(client.sin_port) << endl;
    }

    //close lisining socket
    closesocket(lisining);


    //while loop: accept and echo message back to client
    char buff[4096];
    while (true) {
        ZeroMemory(buff, 4096);

        //wait for cilent to sent data
        int byteReceived = recv(clientSocket, buff, 4096, 0);
        if (byteReceived == SOCKET_ERROR) {
            cerr << "Error is recv().Quiting " << endl;
            break;
        }
        if (byteReceived == 0) {
            cout << "client disconnected" << endl;
            break;
        }
        //echo message back to client
        send(clientSocket, buff, byteReceived + 1,0);

        

    }
    

    
    //close the socket
    closesocket(clientSocket);


    //shutdown winsock
    WSACleanup();
}