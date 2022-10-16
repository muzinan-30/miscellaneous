#pragma once
#ifndef SERVER_H
#define SERVER_H
#include<winsock2.h> //网络库头文件
#pragma comment(lib,"ws2_32.lib")//库文件
//#include <unistd.h>
#include <sys/types.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
#include "user.h"
#include "mysql_manage.hpp"
//#include "userlist.h"
//#include "thread.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdlib.h>


#define LISTEN_PORT 6666
using namespace std;

class Server
{
public:
	Server();
	Server(string s_h, string s_u, string s_p, string s_d, int p);
	~Server();

	bool acceptClient();
	void closeServer();
private:
	struct sockaddr_in addr;
	int sockfd;
	//UserList* list;
	mysql_manager* db;
	WSADATA wsaData;


	string sql_host;
	string sql_user;
	string sql_passwd;
	string sql_database;
	int port;

	void init();//fail:sockfd is -1 success: sockfd
};

#endif