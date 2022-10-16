
#include"connectserver.h"

connectServer::connectServer()
{
	//初始化DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}
connectServer::~connectServer()
{

}
int connectServer::connectTo(string ip_addr, short port)
{
	//初始化socket addr
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6666);
	addr.sin_addr.S_un.S_addr = inet_addr(ip_addr.c_str());


	//创建socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		return -1;
	}

	//connect
	int res = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));


	if (-1 == res)
	{
		closesocket(res);
		return -1;
	}

	cout << "链接成功" << endl;
	return sockfd;
}
void connectServer::closeSocket(int sockfd)
{
	//	关闭套接字
	closesocket(sockfd);

	//关闭网络库
	WSACleanup();
	cout << "关闭网络库" << endl;
}