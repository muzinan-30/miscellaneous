#pragma once
#ifndef LOGIN_H_
#define LOGIN_H_

#include"connectServer.h"


struct logininfo
{
	char user_name[11];
	//----md5----
	char pwd[33];

	int flag;
};

class Login
{
private:
	int sockfd=-1;
	connectServer cs;
public:
	bool loginTo();
};
#endif // !LOGIN_H_
