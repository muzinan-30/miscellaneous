#include<Windows.h>
#include<winsock.h>
#include<iostream>

#include<string>
#include<mysql.h>

#include <fstream>

using namespace std;

//#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")

const string crete_table = "";
//sqlstr = "create table if not exists user_info";
//sqlstr += "(";
//sqlstr +=
//	" user_id  int unsigned not null auto_increment primary key comment 'unique user id', ";
//sqlstr +=
//	"user_name varchar(100) character set gb2312 collate gb2312_chinese_ci null comment 'Name of user',";
//sqlstr +=
//	"user_second_name int unsigned not null default 0 comment 'the summation of using time' ";
//sqlstr +=
//	")";
//�����û���Ϣ

class mysql_manager
{

private:
	MYSQL mysql;//���ݿ�����
	string sqlstr;//sql���

public:
	bool QueryDatabase_add(string user_id, string user_name, string user_gender);
	//��ѯ
	bool QueryDatabase_delete(string user_id);
	string* QueryDatabase_search(string user_id);
	bool ConnectDatabase(string hostname, string username, string passwd, string database, unsigned int port);
	bool CloseDatabase();
	bool Control_log(string filename,string content);
};


bool mysql_manager::ConnectDatabase(string hostname = "localhost", string username = "root", string passwd = "test", string database = "test", unsigned int port = 0)
{
	mysql_init(&mysql);; // Initialise the instance
	//��ʽ��������
	//��ʼ�����ݿ�
	//if (0 == mysql_library_init(0, NULL, NULL))
	//{
	//	cout << "���ݿ��ʼ���ɹ�" << endl;
	//}
	//else
	//{
	//	cout << "���ݿ��ʼ���ɹ�" << endl;
	//}
	//��ʼ�����ݿ�

	//�����ַ�����
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");

	//��ʼ�����ݽṹ
	if (!mysql_real_connect(&mysql, hostname.c_str(), username.c_str(), passwd.c_str(), database.c_str(), port, NULL, 0))
	{
		cout << "���ݿ�����ʧ��" << mysql_error(&mysql) << endl;
		printf("����ԭ�� %s\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		cout << "���ݿ����ӳɹ�" << endl;
		return true;
	}
}
//�������

bool mysql_manager::QueryDatabase_add(string user_id, string user_name, string user_pwd)
{
	//��������
	sqlstr = "insert into user(user_id,user_name,pwd) values('" +
		user_id + "\','"
		+ user_name + "\','"
		+ user_pwd+ "\'"
		+ ")";
	cout << sqlstr;
	if (0 == mysql_query(&mysql, sqlstr.c_str()))
	{

		cout << "ִ�гɹ�" << endl;
		return true;
	}
	else
	{
		cout << "ִ��ʧ��" << endl;
		printf("����ԭ�� %s\n", mysql_error(&mysql));
		Control_log("sqllog.txt", mysql_error(&mysql));
		return false;
	}
}
//ɾ���û���Ϣ
bool mysql_manager::QueryDatabase_delete(string user_id)
{
	//��������
	sqlstr = "delete from user where user_name=" + user_id;
	cout << sqlstr;
	if (0 == mysql_query(&mysql, sqlstr.c_str()))
	{

		cout << "ִ�гɹ�" << endl;
		return true;
	}
	else
	{
		cout << "ִ��ʧ��" << endl;
		printf("����ԭ�� %s\n", mysql_error(&mysql));
		Control_log("sqllog.txt", mysql_error(&mysql));

		return false;
	}
}
//��ѯ�û�����Ϣ
string*  mysql_manager::QueryDatabase_search(string user_id)
{
	sqlstr = "select * from user where user_name='" + user_id+"'";
	MYSQL_RES* result = NULL;
	if (0 == mysql_query(&mysql, sqlstr.c_str())) {
		cout << "mysql_query() select data succeed" << endl;

		//һ����ȡ�����ݼ� ����һ�������Ľ�����ϸ��ͻ���
		result = mysql_store_result(&mysql);
		//ȡ�ò���ӡ����
		//mysql_num_fields() ����һ����������ص��е�������
		int rowcount = mysql_num_rows(result);
		cout << "row count: " << rowcount << endl;

		//ȡ�ò���ӡ���ֶε�����
		unsigned int fieldcount = mysql_num_fields(result);
		MYSQL_FIELD* field = NULL;
		for (unsigned int i = 0; i < fieldcount; i++) {
			//mysql_fetch_field_direct () ����һ�����ֶε����ͣ�����һ���ֶα�š�
			field = mysql_fetch_field_direct(result, i);
			cout << field->name << "\t\t";
		}
		cout << endl;


		//������
		
		string* res = new string[3];
		//��ӡ����
		//mysql_fetch_row() �ӽ��������ȡ����һ�С�
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		while (NULL != row) {
			for (int i = 0; i < fieldcount; i++) {
				cout << row[i] << "\t\t";
				res[i] = row[i];
			}
			cout << endl;
			row = mysql_fetch_row(result);
		}

		//mysql_free_result(result);
		return res;
	}
	else {

		cout << "mysql_query() select data failed" << endl;
		Control_log("sqllog.txt", mysql_error(&mysql));
		mysql_close(&mysql);
		mysql_free_result(result);
		return NULL;
	}
}

bool mysql_manager::CloseDatabase()
{
	mysql_close(&mysql);
	mysql_server_end();
	cout << "���ݿ�رճɹ�" << endl;
	return true;
}

bool mysql_manager::Control_log(string filename,string content)
{
	ofstream inFile;

	try {
		inFile.open(filename, ios::in | ios::out | ios::app);
		if (!inFile.is_open()) { cout << " open failed" << endl; }

		inFile << content;
		inFile << '\n';
		throw "nice error";
	}
	catch (const char* msg)
	{
		cerr << msg << endl;
	}

		inFile.close();
	

	return true;
}
