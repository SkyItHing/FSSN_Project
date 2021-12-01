#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h> 
#pragma comment (lib,"ws2_32.lib")

#define HOST "192.168.56.1"
#define PORT 65456
#define BufferSize 1024
using namespace std;

int main()
{
    cout << "> echo-client is activated" << endl;

    WSADATA wsaData;
    SOCKET ClientSocket;
    SOCKADDR_IN ServerSocketAddr;
    SOCKADDR_IN ClientAddr;

    if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR)
    {
        WSACleanup();
        exit(0);
    }

    memset(&ServerSocketAddr, 0, sizeof(ServerSocketAddr));
    memset(&ClientAddr, 0, sizeof(ClientAddr));

    ServerSocketAddr.sin_family = AF_INET;
    ServerSocketAddr.sin_addr.s_addr = inet_addr(HOST);
    ServerSocketAddr.sin_port = htons(PORT);

    ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//

    if (ClientSocket == INVALID_SOCKET)
    {
        closesocket(ClientSocket);
        WSACleanup();
        exit(0);
    }

    char sendMsg[BufferSize] = {};
    char RecvData[BufferSize] = {};
    int FromServer_Size = sizeof(ClientAddr);
    while (1)
    {
        cout << "> ";
        cin >> sendMsg;
        sendto(ClientSocket, sendMsg, BufferSize, 0, (struct sockaddr*)&ServerSocketAddr, sizeof(ServerSocketAddr));
        recvfrom(ClientSocket, RecvData, BufferSize, 0, (struct sockaddr*)&ClientAddr, &FromServer_Size);
        cout << RecvData << endl;
        if (strcmp(sendMsg, "quit") == 0)
            break;
    }
    closesocket(ClientSocket);
    WSACleanup();

    cout << "> echo-client is de-activated" << endl;

    return 0;
}