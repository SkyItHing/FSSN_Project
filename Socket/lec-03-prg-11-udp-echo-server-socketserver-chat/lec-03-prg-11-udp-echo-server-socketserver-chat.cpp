#define _WINSOCK_DEPRECATED_NO_WARNINGS
using namespace std;
#include <iostream>
#include <stdio.h>
#include <winSock2.h>
#include <thread>
#include <windows.h> 
#include <process.h>
#include <vector>
#pragma comment(lib, "ws2_32")

#define PORT 65456
#define BufferSize 1024
vector<SOCKADDR_IN> Cli_List;

int main()
{
    cout << "> echo-server is activated" << endl;

    WSADATA wsaData;
    SOCKET ServerSocket;
    SOCKADDR_IN ServerSocketAddr;
    SOCKADDR_IN ClientAddr;

    int ClientAddr_Size;
    int Recv_Size;

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

        if (RecvData[0] == '#' || strcmp(RecvData, "quit") == 0)
        {
            if (strcmp(RecvData, "#REG") == 0)
            {
                cout << "> client registered" << inet_ntoa(ClientAddr.sin_addr)
                    << " with Port Number " << ntohs(ClientAddr.sin_port) << endl;
                Cli_List.push_back(ClientAddr);
                cout << "> recevied: " << RecvData << endl;
            }
            else if (strcmp(RecvData, "#DEREG") == 0 || strcmp(RecvData, "quit") == 0)
            {
                vector<SOCKADDR_IN>::iterator iter = Cli_List.begin();
                for (int i = 0; i < Cli_List.size(); i++)
                {
                    if (Cli_List[i].sin_port == ClientAddr.sin_port)
                    {
                        Cli_List.erase(iter);
                        break;
                    }
                    iter++;
                }

                cout << "> client de-registered " << inet_ntoa(ClientAddr.sin_addr)
                    << " with Port Number " << ntohs(ClientAddr.sin_port) << endl;
            }
        }
        else
        {
            bool YN = false;
            vector<SOCKADDR_IN>::iterator iter = Cli_List.begin();
            for (int i = 0; i < Cli_List.size(); i++)
            {
                if (Cli_List[i].sin_port == ClientAddr.sin_port)
                {
                    YN = true; // true면 같은 게 있다.
                    break;
                }
                iter++;
            }
            if (Cli_List.size() == 0)
                cout << "> no clients to echo" << endl;

            else if(YN == false)
                cout << "> ignores a message from un-registered client" << endl;
            else
                cout << "> received ('" << RecvData << "') and echoed to '" << Cli_List.size() << " clients'" << endl;
        }
        sendto(ServerSocket, RecvData, Recv_Size, 0, (struct sockaddr*)&ClientAddr, sizeof(ClientAddr));
    }
    closesocket(ServerSocket);
    WSACleanup();

    cout << "> echo-server is de-activated" << endl;

    return 0;
}