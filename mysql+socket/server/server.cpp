#include"server.h"
#include<time.h>
#include<ctime>

Server::Server() 
{
	init();

	db = new mysql_manager();
	db->ConnectDatabase("127.0.0.1", "root", "test", "test", 0);
}
Server::Server(string s_h, string s_u, string s_p, string s_d, int p):
	sql_host(s_h),sql_user(s_u),sql_passwd(s_p),sql_database(s_d),port(p)
{

}
Server::~Server()
{
	delete db;
}

bool Server::acceptClient()
{

	if (-1 == sockfd)
	{
		return false;
	}

	int csockfd = accept(sockfd, NULL, NULL);


	if (-1 == csockfd)
	{
		return false;
	}
	else
	{
		cout << "���ӳɹ�" << endl;
	}

	user_info lf;
	//��ʼ��
	memset(&lf, 0, sizeof(user_info));
	//������Ϣ
	char buffer[1024];
	size_t size = recv(csockfd,buffer, sizeof(buffer), 0);
	memcpy(&lf, buffer, sizeof(user_info));
	cout << "���յ����û���Ϊ" << lf.user_name << endl;

	if (-1 == size || size == 0)
	{
		closesocket(csockfd);
		return false;
	}
	if (1 == lf.flag)
	{
		string* res=db->QueryDatabase_search(lf.user_name);
		cout << "��ʼ��ѯ���ݿ�" << endl;
		int login_flag = 0;
		if (res == NULL)
		{
			login_flag = 0;
			size = send(csockfd, (char*)&login_flag, sizeof(int), 0);
			closesocket(csockfd);
			return false;
		}
		else
		{
			if (!strcmp(lf.user_name, res[1].c_str()) && !strcmp(lf.passwd, res[2].c_str()))
			{
				//������ȷ��¼�Ľ��
				login_flag = 1;
				//��½�ɹ����ر�־λ1
				size = send(csockfd, (char*)&login_flag, sizeof(int), 0);
				cout << "ͨ���������֤" << endl;


				//����һ���߳�
			}
			else
			{
				login_flag = 0;
				//?????????????????
				size = send(csockfd, (char*)&login_flag, sizeof(int), 0);
				closesocket(csockfd);
				return false;
			}
		}
	}
	else
	{
		//�û�ע�Ჿ��
		cout << "user_name" << lf.user_name << endl << "passwd:" << lf.passwd << endl;


		//��ȡ�û�id
		char account[10] = { 0 };
		ifstream in("auto_account.dat");
		in.read(account, sizeof(account));
		in.close();
		cout << account << endl;

		send(csockfd, account, sizeof(account),0);


		//�����û���idΪע��ʱ�䣬����Ψһ
		/*time_t nowtime;
		struct tm* now;
		time(&nowtime);
		now = localtime(&nowtime);
		string user_id = ""+now->tm_year + now->tm_mon + now->tm_mday + now->tm_hour + now->tm_min;*/
		string user_id(account);
		if (db->QueryDatabase_add(user_id, lf.user_name, lf.passwd))
		{
			cout << "ע��ɹ�" << endl;
			int ac = atoi(account);
			ac++;
			string str;
			stringstream ss;

			ss << ac;
			ss >> str;

			ofstream out("auto_account.dat");
			out.write(str.c_str(), strlen(str.c_str()));

			if (!out)
			{
				cout << "write failed" << endl;

			}
		}
		else
		{
			cout << "ע��ʧ��,�û����Ѿ�����" << endl;
		}
		cout << "useridΪ��" << user_id << endl;

		closesocket(csockfd);
	}
	return true;
}
void Server::closeServer()
{
	closesocket(sockfd);
	//�ر������
	WSACleanup();
}



//��ʼ��������������״̬
void Server::init()
{

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		//���������Ϣ
		cout << "Failed to load socket library" << endl;
		system("pause");
		return;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(LISTEN_PORT);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	//�������ڻỰ��socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (-1 == sockfd)
	{
		closesocket(sockfd);
		sockfd = -1;

		return;
	}

	int ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

	if (ret == -1)
	{
		closesocket(sockfd);
		sockfd = -1;
		return;
	}
	else
	{
		cout << "bind success" << endl;
	}

	ret = listen(sockfd, 1024);

	if (ret == -1)
	{
		closesocket(sockfd);
		return;
	}
	else
	{
		cout << "listen success" << endl;
	}
}



