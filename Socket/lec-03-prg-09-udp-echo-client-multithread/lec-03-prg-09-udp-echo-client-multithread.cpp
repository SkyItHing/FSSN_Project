#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <winsock2.h> // ���� ��� ���� 
#include <windows.h> 

#pragma comment (lib,"ws2_32.lib") // ���� ���̺귯�� ��ũ
#define BUFFER_SIZE 1024 // ���� ������

using namespace std;

void main(void)
{
    WSADATA wsaData; // ���� ������ ����ü
    SOCKET ClientSocket; // ���� ����
    SOCKADDR_IN ToServer; // ������ ������ �ּ����� ����ü
    SOCKADDR_IN FromServer; // �������� �޴� �ּ����� ����ü

    int Send_Size;
    int FromServer_Size;
    int Recv_Size;
    ULONG   ServerPort = 61557; // ���� ��Ʈ��ȣ

    double dd = 100.12;
    char Buffer[BUFFER_SIZE] = {};
    sprintf_s(Buffer, "STOP,%.2lf\n", dd);
    if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR)
    {
        cout << "WinSock �ʱ�ȭ�κп��� ���� �߻� " << endl;
        WSACleanup();
        exit(0);
    }

    memset(&ToServer, 0, sizeof(ToServer));
    memset(&FromServer, 0, sizeof(FromServer));

    ToServer.sin_family = AF_INET;
    ToServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    ToServer.sin_port = htons(ServerPort); // ��Ʈ��ȣ

    ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//

    if (ClientSocket == INVALID_SOCKET)
    {
        cout << "������ �����Ҽ� �����ϴ�." << endl;
        closesocket(ClientSocket);
        WSACleanup();
        exit(0);
    }

    while (1)
    {
        //- ��Ŷ�۽�
        Send_Size = sendto(ClientSocket, Buffer, BUFFER_SIZE, 0,
            (struct sockaddr*)&ToServer, sizeof(ToServer));
        printf("%s\n", Buffer);
        // ��Ŷ�۽Ž� ����ó��
        if (Send_Size != BUFFER_SIZE)
        {
            cout << "sendto() error!" << endl;
            exit(0);
        }
    }
    FromServer_Size = sizeof(FromServer);
    memset(Buffer, 0, BUFFER_SIZE);

    // ��Ŷ����
    Recv_Size = recvfrom(ClientSocket, Buffer, BUFFER_SIZE, 0,
        (struct sockaddr*)&FromServer, &FromServer_Size);

    // ��Ŷ���Ž� ����ó��
    if (Recv_Size < 0)
    {
        cout << "recvfrom() error!" << endl;
        exit(0);
    }

    // ������ ��Ŷ���� IP / Data ���
    cout << "Recv from " << inet_ntoa(FromServer.sin_addr) << endl;
    cout << "Data " << Buffer << endl;

    closesocket(ClientSocket); //���� �ݱ�
    WSACleanup();
}