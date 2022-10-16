#include"login.h"

#include "pch.h"
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <aes.h>
#include <filters.h>
#include <modes.h>

#include <sha.h>
#include <crc.h>
#include <files.h>
#include <md5.h>
#include <hex.h>

#pragma comment(lib, "cryptlib.lib")

using namespace std;
using namespace CryptoPP;

#include<iostream>

using namespace std;


// �����ļ��� SHA256 ֵ
string CalSHA256_ByFile(char* pszFileName)
{
	string value;
	SHA256 sha256;
	FileSource(pszFileName, true, new HashFilter(sha256, new HexEncoder(new StringSink(value))));
	return value;
}


bool Login::loginTo()
{
	cout << "���ӷ���" << endl;
	if (sockfd != -1)
	{
		cs.closeSocket(sockfd);
	}

	sockfd = cs.connectTo("127.0.0.1",6666);

	if (-1 == sockfd)
		return false;
	else
	{
		cout << "���ӷ���˳ɹ�" << endl;
	}

	logininfo lf;
	cout << "ע�ᣨ0���򣨵�¼1��" << endl;
	cin>>lf.flag;
	

	string dst;


	char user_name[11] = {0};
	char pwd[33] = {0};


	cout << "�����û���" << endl;
	cin >> user_name;
	cout << "��������" << endl;
	cin >> pwd;

	cout << "username:" << user_name << endl;
	// ��������MD5ֵ��ʹ��
	MD5 md5;
	StringSource(pwd, true, new HashFilter(md5, new HexEncoder(new StringSink(dst))));
	cout << "�����ַ���MD5: " << dst << endl;


	strcpy(pwd, dst.c_str());
	cout << pwd;
	//�����û�����
	/*strcpy(lf.user_name, user_name);
	strcpy(lf.pwd, dst.c_str());*/

	memcpy(lf.user_name, user_name, sizeof(user_name));
	memcpy(lf.pwd, pwd, sizeof(pwd));


	size_t size = send(sockfd, (char*)&lf, sizeof(logininfo), 0);
	if (size == -1)
	{
		cout << "�������ݽ�����֤ʧ��" << endl;
		return false;
	}

	int flag = 0;
	size = recv(sockfd,(char*) &flag, sizeof(int), 0);

	if (1 == flag)
	{
		cout << "login_success" << endl;

	}
	else
	{
		cout << "login_failed" << endl;
		//return false;
	}

	system("pause");
}


int main()
{
	Login log;
	log.loginTo();
}