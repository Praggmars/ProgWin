#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<winsock2.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

//client
int main()
{
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBox(NULL, L"Winsock startup failed", L"Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET connection = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(connection, (SOCKADDR*)&addr, sizeofaddr) != 0)
	{
		MessageBox(NULL, L"failed to connect", L"Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	std::cout << "Connected" << std::endl;

	std::wstring msg = L"213";
	int msgLen = msg.length() * sizeof(wchar_t);
	int fullLen = msgLen + 2 * sizeof(int);
	send(connection, (char*)&fullLen, sizeof(int), 0);
	send(connection, (char*)&msgLen, sizeof(int), 0);
	send(connection, (char*)msg.data(), msgLen, 0);
	char buffer[1000] = {};
	recv(connection, buffer, 3, 0);

	std::cout << buffer << std::endl;

	system("pause");
	return 0;
}