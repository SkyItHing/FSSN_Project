#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <thread>
#include <windows.h> 
#include <process.h>
#pragma comment (lib,"ws2_32.lib")

#define PORT 65456
#define BufferSize 1024

using namespace std;

int main()
{
    cout << "> echo-server is activated" << endl;

    WSADATA wsaData;
    SOCKET ServerSocket;
    SOCKADDR_IN ServerSocketAddr;
    SOCKADDR_IN ClientAddr;

    int ClientAddr_Size;
    int Recv_Size;
    int Send_Size;

    char RecvData[BufferSize];

    if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR)
        WSACleanup();

    memset(&ServerSocketAddr, 0, sizeof(ServerSocketAddr));
    memset(&ClientAddr, 0, sizeof(ClientAddr));
    memset(RecvData, 0, BufferSize);

    ServerSocketAddr.sin_family = AF_INET;
    ServerSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ServerSocketAddr.sin_port = htons(PORT);


    ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ServerSocket == INVALID_SOCKET)
    {
        closesocket(ServerSocket);
        WSACleanup();
        exit(0);
    }

    if (bind(ServerSocket, (struct sockaddr*)&ServerSocketAddr,
        sizeof(ServerSocketAddr)) == SOCKET_ERROR)
    {
        closesocket(ServerSocket);
        WSACleanup();
        exit(0);
    }

    while (1)
    {
        ClientAddr_Size = sizeof(ClientAddr);
        Recv_Size = recvfrom(ServerSocket, RecvData, BufferSize, 0, (struct sockaddr*)&ClientAddr, &ClientAddr_Size);

        if (Recv_Size < 0)
            exit(0);

        cout << "Client : " << inet_ntoa(ClientAddr.sin_addr) << " with Port Number " << ntohs(ClientAddr.sin_port) << endl;
        cout << "> recevied: " << RecvData << endl;

        Send_Size = sendto(ServerSocket, RecvData, Recv_Size, 0, (struct sockaddr*)&ClientAddr, sizeof(ClientAddr));
        if (Send_Size != Recv_Size)
            exit(0);
    }
    closesocket(ServerSocket);
    WSACleanup();

    cout << "> echo-server is de-activated" << endl;

    return 0;
}