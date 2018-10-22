#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<winsock2.h>
#include <iostream>
#include <vector>

#pragma comment(lib,"ws2_32.lib")

template <typename T>
void SendSocketFlipBytes(SOCKET& s, T& t)
{
	std::vector<char> data;
	char* b = (char*)&t;
	for (int i = sizeof(t) - 1; i >= 0; i--)
		data.push_back(b[i]);
	send(s, data.data(), data.size(), 0);
}
void SendSocketFlipBytes(SOCKET& s, void* d, size_t size)
{
	std::vector<char> data;
	char* b = (char*)d;
	for (int i = size - 1; i >= 0; i--)
		data.push_back(b[i]);
	send(s, data.data(), data.size(), 0);
}
void RecvSocketFlipBytes(SOCKET& s, void* d, size_t size)
{
	std::vector<char> data;
	data.resize(size);
	char* b = (char*)d;
	recv(s, data.data(), size, 0);
	for (int i = size - 1; i >= 0; i--)
		b[i] = data[size - 1 - i];
}


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

	std::wstring msg = L"Can I please send you this: 213?";
	int msgLen = msg.length() * sizeof(wchar_t);
	int fullLen = msgLen + 2 * sizeof(int);

	SendSocketFlipBytes(connection, &fullLen, sizeof(int));
	SendSocketFlipBytes(connection, &msgLen, sizeof(int));
	for (wchar_t ch : msg)
		SendSocketFlipBytes(connection, &ch, sizeof(ch));
	wchar_t ch;
	RecvSocketFlipBytes(connection, &fullLen, sizeof(fullLen));
	RecvSocketFlipBytes(connection, &fullLen, sizeof(fullLen));
	std::wcout << L"Incoming: " << fullLen << std::endl;
	for (int i = 0; i < fullLen / 2; i++)
	{
		RecvSocketFlipBytes(connection, &ch, sizeof(ch));
		std::wcout << ch;
	}


	std::cout << std::endl;

	system("pause");
	return 0;
}