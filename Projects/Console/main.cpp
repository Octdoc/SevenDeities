#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<winsock2.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

//server
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
	int addrlen = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);
	if (newConnection == 0)
	{
		std::cout << "failed to accept the client's connection" << std::endl;
	}
	else
	{
		std::cout << "client connected" << std::endl;
		while (true)
		{
			int msgLen;
			char buffer[128]{};
			recv(newConnection, (char*)&msgLen, 4, 0);
			recv(newConnection, buffer, msgLen, 0);
			std::cout << buffer << std::endl;
			/*if (buffer[0] == '2' && buffer[1] == '1' && buffer[2] == '3')
			{
				msgLen = 3;
				send(newConnection, (char*)&msgLen, 4, 0);
				send(newConnection, "end", msgLen, 0);
			}*/
		}
	}

	system("pause");
	return 0;
}