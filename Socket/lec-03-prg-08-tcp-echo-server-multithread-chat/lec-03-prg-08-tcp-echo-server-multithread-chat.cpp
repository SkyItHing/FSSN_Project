#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <winSock2.h>
#include <process.h>
#include <vector>
#pragma comment(lib, "ws2_32")

using namespace std;
#define PORT 65456
#define BufferSize 1024
#define Cli_MAX 5
vector<SOCKET> Cli_List;

void RecvThread(void* p)
{
	SOCKET ClientSocket = (SOCKET)p;
	while (1)
	{
		char RecvData[BufferSize] = {};
		if (recv(ClientSocket, RecvData, sizeof(RecvData), 0) <= 0)
			break;
		for (int i = 0; i < Cli_List.size(); i++)
			send(Cli_List[i], RecvData, strlen(RecvData), 0);
		cout << "echoed: " << RecvData << endl;
	}

	vector<SOCKET>::iterator iter = Cli_List.begin();
	for (int i = 0; i < Cli_List.size(); i++)
	{
		if (Cli_List[i] == ClientSocket)
		{
			Cli_List.erase(iter);
			break;
		}
		iter++;
	}
	closesocket(ClientSocket);
}

int main()
{
	cout << "> echo-server is activated" << endl;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket;
	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ServerSocketAddr = {};
	ServerSocketAddr.sin_family = AF_INET;
	ServerSocketAddr.sin_port = htons(PORT);
	ServerSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(ServerSocket, (SOCKADDR*)&ServerSocketAddr, sizeof(ServerSocketAddr));
	listen(ServerSocket, Cli_MAX);

	SOCKADDR_IN ClientAddr = {};
	int ClientSize = sizeof(ClientAddr);

	while (1)
	{
		SOCKET ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &(ClientSize));
		Cli_List.push_back(ClientSocket);
		cout << "> Client connected IP address = " << inet_ntoa(ClientAddr.sin_addr) << " with Port Number " << ntohs(ClientAddr.sin_port) << endl;
		_beginthread(RecvThread, NULL, (void*)ClientSocket);
	}

	closesocket(ServerSocket);
	WSACleanup();

	cout << "> echo-server is de-activated" << endl;

	return 0;
}