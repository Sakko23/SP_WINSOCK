// Serv.cpp: определяет точку входа для консольного приложения.

#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <iostream>
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <fstream>
#include <string>
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
using namespace std;

int clientCounter = 0;
SOCKET Clients[100];


char mess[256];


void ClientHandlerThread(int index) {
	fstream qfile;
	string str;
	const char *ar;
	int score = 0;
	qfile.open("questions.txt");
	char buffer[256];
	char answer[256];
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			int bytes_sent;
			getline(qfile, str);
			ar = str.c_str();
			cout << ar << endl;
			bytes_sent = send(Clients[index], ar, sizeof(ar), NULL);
		}
		recv(Clients[index], answer, sizeof(answer), NULL);
		cout << "received answer: " << answer << endl;
		getline(qfile, str);
		if (answer == str) {
			score++;
		}
	}
	
	char* charVariable = (char*)(&score);
	send(Clients[index], "YOU SCORED: ", 12, NULL);
	send(Clients[index], charVariable, sizeof(charVariable), NULL);

}
	/*
const char *ar = str.c_str();
cout << ar << endl;
*/

int main()
{
	

	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock startup failed.", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr;
	int addrlen = sizeof(addr); //required in accept()
	unsigned address;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local broad
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET; 

	SOCKET sock = socket(AF_INET, SOCK_STREAM, NULL); //create
	bind(sock, (SOCKADDR*)&addr, sizeof(addr)); //bind
	listen(sock, SOMAXCONN); //max connections in queue



	SOCKET newClient;
	for (int i = 0; i < 100; i++) {
		newClient = accept(sock, (SOCKADDR*)&addr, &addrlen);

		if (newClient == 0) {
			cout << "Failed to accept the clien connection" << endl;
		}

		//if client is connected succesfully
		
		else {														
			cout << "Player "<< i+1 <<" is connected" << endl;
			Clients[i] = newClient;
			clientCounter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL);
		}
	}
	system("pause");

	return 0;
}


/*
void ClientHandlerThread(int index) {
char buffer[256];
char id[256];
while (true) {
recv(Clients[index], buffer, sizeof(buffer), NULL);
for (int i = 0; i < clientCounter; i++) {
if (i == index) {
continue;
}

char id = index;
send(Clients[i], buffer, sizeof(buffer), NULL);
//send(Clients[i], index, sizeof(index), NULL);
}
}
}
*/