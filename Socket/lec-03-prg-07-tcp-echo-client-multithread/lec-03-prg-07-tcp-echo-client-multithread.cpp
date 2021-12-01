#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>
#pragma comment(lib, "ws2_32")

#define HOST "192.168.56.1"
#define PORT 65456
#define BufferSize 1024
using namespace std;

void RecvThread(void* p)
{
	SOCKET ClientSocket = (SOCKET)p;
	while (1)
	{
		char RecvData[BufferSize] = {};
		if (recv(ClientSocket, RecvData, sizeof(RecvData), 0) <= 0)
			break;
		cout << "> received: : " << RecvData << endl;
	}
}

int main()
{
	cout << "> echo-client is activated" << endl;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ClientAddr = {};
	ClientAddr.sin_family = AF_INET;
	ClientAddr.sin_port = htons(PORT);
	ClientAddr.sin_addr.s_addr = inet_addr(HOST);

	connect(ClientSocket, (SOCKADDR*)&ClientAddr, sizeof(ClientAddr));

	_beginthread(RecvThread, 0, (void*)ClientSocket);

	while (1)
	{
		char sendMsg[BufferSize] = {};
		cout << "> ";
		cin >> sendMsg;
		send(ClientSocket, sendMsg, strlen(sendMsg), 0);
		if (strcmp(sendMsg, "quit") == 0)
			break;
	}

	closesocket(ClientSocket);
	WSACleanup();

	cout << "> echo-client is de-activated" << endl;

	return 0;
}