#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <winsock2.h> // ���� ��� ���� 
#include <windows.h> 
#pragma comment (lib,"ws2_32.lib") // ���� ���̺귯�� ��ũ
#define BUFFER_SIZE 1024 // ���� ������

using namespace std;

void main(void)
{
    WSADATA wsaData; // ���� ������ ����ü.(WSAStartup() ����Ҳ�!)
    SOCKET ServerSocket; // ���� ����
    SOCKADDR_IN ServerInfo; // ���� �ּ����� ����ü
    SOCKADDR_IN FromClient; // Ŭ���̾�Ʈ���� �޴� �ּ����� ����ü

    int FromClient_Size; // Ŭ���̾�Ʈ�κ��� �޴� �޽��� ũ��
    int Recv_Size; // �޴� ������
    int Send_Size; // ������ ������
    int Count;

    char Buffer[BUFFER_SIZE];
    short ServerPort = 61557; // ���� ��Ʈ��ȣ

    //WSAStartup�� WS2_32.DLL�� �ʱ�ȭ �ϴ� �Լ�
    if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) // WSAStartup �������� ���� �߻��ϸ�
    {
        cout << "WinSock �ʱ�ȭ�κп��� ���� �߻� " << endl;
        WSACleanup(); // WS2_32.DLL�� ��� ����
    }

    // memset : ����ڰ� ���� �޸� ũ�⸸ŭ �޸� ������ Ư���� ������ ä��
    memset(&ServerInfo, 0, sizeof(ServerInfo)); // 0���� �ʱ�ȭ
    memset(&FromClient, 0, sizeof(FromClient));
    memset(Buffer, 0, BUFFER_SIZE);

    ServerInfo.sin_family = AF_INET; // IPv4 �ּ�ü�� ��� 
    ServerInfo.sin_addr.s_addr = inet_addr("127.0.0.1"); // ������ IP. �� ȥ�ڳ��̿�..
    ServerInfo.sin_port = htons(ServerPort); // ��Ʈ��ȣ

    // ���� ����
    ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // udp�� ���� ����. SOCK_DGRAM : UDP ���
    if (ServerSocket == INVALID_SOCKET) // ���� �߻���
    {
        cout << "������ �����Ҽ� �����ϴ�." << endl;
        closesocket(ServerSocket);
        WSACleanup();
        exit(0);
    }

    // bind() - ���� ���� Ŭ���̾�Ʈ�� ���� welcome ����
    // ���޸� �Ҳ��� �ʿ����
    if (bind(ServerSocket, (struct sockaddr*)&ServerInfo, sizeof(ServerInfo)) == SOCKET_ERROR)
    {
        cout << "���ε带 �� �� �����ϴ�." << endl;
        closesocket(ServerSocket);
        WSACleanup();
        exit(0);
    }

    while (1)
    {
        FromClient_Size = sizeof(FromClient);
        // recvfrom : UDP��� �񿬰���.  ��Ŷ����
        Recv_Size = recvfrom(ServerSocket, Buffer, BUFFER_SIZE, 0,
            (struct sockaddr*)&FromClient, &FromClient_Size);

        if (Recv_Size < 0)
        {
            cout << "recvfrom() error!" << endl;
            exit(0);
        }

        cout << "��Ŷ���ŵ�! ��Ŷ�� ���� Ŭ���̾�Ʈ�� " << inet_ntoa(FromClient.sin_addr) << endl;
        cout << "��Ŷ�� �����ʹ� " << Buffer << endl;

        //  ��Ŷ�۽�
        Send_Size = sendto(ServerSocket, Buffer, Recv_Size, 0,
            (struct sockaddr*)&FromClient, sizeof(FromClient));
        if (Send_Size != Recv_Size)
        {
            cout << "sendto() error!" << endl;
            exit(0);
        }
    }
    closesocket(ServerSocket); // ������ �ݽ��ϴ�.
    WSACleanup();
}