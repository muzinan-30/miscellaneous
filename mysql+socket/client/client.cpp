#include<winsock2.h> //�����ͷ�ļ�
#pragma comment(lib,"ws2_32.lib")//���ļ�
#include<iostream>
#include <fstream>
#include<time.h>

#define  _WINSOCK_DEPRECATED_NO_WARNINGS  1


using namespace std;
int main()
{
	//��ʼ��DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);


	unsigned int clientfd;


	if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "socket created failed.\n" << endl;
	};

	struct sockaddr_in servaddr;

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(6666);
	servaddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");


	if (connect(clientfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		std::cout << "connect error\n";
	}


	cout << "connecting\n" << endl;

	

	//�ر��׽���
	closesocket(clientfd);

	//��ֹʹ�� DLL
	WSACleanup();
}