#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

#define HOST "192.168.56.1"
#define PORT 65456
#define BufferSize 1024
using namespace std;

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

	if (ClientSocket != INVALID_SOCKET)
	{
		try
		{
			if (connect(ClientSocket, (SOCKADDR*)&ClientAddr, sizeof(ClientAddr)) == SOCKET_ERROR)
			{
				cout << "> connect() failed and program terminated" << endl;
				closesocket(ClientSocket);
			}
		}
		catch (exception&)
		{
			cout << "> connect() failed by exception" << endl;
			closesocket(ClientSocket);
		}
	}

	while (1)
	{
		char sendMsg[BufferSize] = {};
		cout << "> ";
		cin >> sendMsg;
		send(ClientSocket, sendMsg, strlen(sendMsg), 0);

		char recvData[BufferSize] = {};
		recv(ClientSocket, recvData, BufferSize, 0);
		cout << "> received: : " << recvData << endl;

		if (strcmp(sendMsg, "quit") == 0)
			break;
	}

	closesocket(ClientSocket);
	WSACleanup();

	cout << "> echo-client is de-activated" << endl;

	return 0;
}