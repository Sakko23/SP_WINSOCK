// Cli.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <iostream>
#include <WinSock2.h>
using namespace std;

SOCKET Connection;

char q[256];
char ans[1];

void ClientThread() {
	char buffer[256];
	char id[256];
	int bytes_recv;
	while (true)
	{	
		bytes_recv = 0;
		bytes_recv = recv(Connection, buffer, sizeof(buffer)-1, 0);
		buffer[bytes_recv] = 0;
		cout << buffer << endl;
	}
}
int main()
{	
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock startup failed.", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	
	SOCKADDR_IN addr;
	int addrlen = sizeof(addr); //req for accept call
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local broad
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, addrlen) != 0) //unable to connect
	{
		MessageBoxA(NULL, "Failed to connect", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	cout << "Connected to server!" << endl;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);

	char buffer[256];

	while (true) {
		cin.getline(buffer, sizeof(buffer));
		send(Connection, buffer, sizeof(buffer), NULL);
		Sleep(10);

	}
	
	/*
	while (true)
	{
		cin.getline(buffer, sizeof(buffer));
		send(Connection, buffer, sizeof(buffer), NULL);
		Sleep(10);
	}
	*/
	system("pause");
	return 0;
}

/*
void ClientThread() {

for (int j = 0; j < 3; j++) {
for (int i = 0; i < 5; i++) {
recv(Connection, q, sizeof(q), NULL);
cout << q << endl;
}
cout << "rigt your answer";
cin.getline(ans, 1);
send(Connection, ans, sizeof(ans), NULL);

}
}
*/