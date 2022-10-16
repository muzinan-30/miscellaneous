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
//增加用户信息

class mysql_manager
{

private:
	MYSQL mysql;//数据库连接
	string sqlstr;//sql语句

public:
	bool QueryDatabase_add(string user_id, string user_name, string user_gender);
	//查询
	bool QueryDatabase_delete(string user_id);
	string* QueryDatabase_search(string user_id);
	bool ConnectDatabase(string hostname, string username, string passwd, string database, unsigned int port);
	bool CloseDatabase();
	bool Control_log(string filename,string content);
};


bool mysql_manager::ConnectDatabase(string hostname = "localhost", string username = "root", string passwd = "test", string database = "test", unsigned int port = 0)
{
	mysql_init(&mysql);; // Initialise the instance
	//方式二：？？
	//初始化数据库
	//if (0 == mysql_library_init(0, NULL, NULL))
	//{
	//	cout << "数据库初始化成功" << endl;
	//}
	//else
	//{
	//	cout << "数据库初始化成功" << endl;
	//}
	//初始化数据库

	//设置字符编码
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");

	//初始化数据结构
	if (!mysql_real_connect(&mysql, hostname.c_str(), username.c_str(), passwd.c_str(), database.c_str(), port, NULL, 0))
	{
		cout << "数据库连接失败" << mysql_error(&mysql) << endl;
		printf("错误原因： %s\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		cout << "数据库连接成功" << endl;
		return true;
	}
}
//建表语句

bool mysql_manager::QueryDatabase_add(string user_id, string user_name, string user_pwd)
{
	//插入数据
	sqlstr = "insert into user(user_id,user_name,pwd) values('" +
		user_id + "\','"
		+ user_name + "\','"
		+ user_pwd+ "\'"
		+ ")";
	cout << sqlstr;
	if (0 == mysql_query(&mysql, sqlstr.c_str()))
	{

		cout << "执行成功" << endl;
		return true;
	}
	else
	{
		cout << "执行失败" << endl;
		printf("错误原因： %s\n", mysql_error(&mysql));
		Control_log("sqllog.txt", mysql_error(&mysql));
		return false;
	}
}
//删除用户信息
bool mysql_manager::QueryDatabase_delete(string user_id)
{
	//插入数据
	sqlstr = "delete from user where user_name=" + user_id;
	cout << sqlstr;
	if (0 == mysql_query(&mysql, sqlstr.c_str()))
	{

		cout << "执行成功" << endl;
		return true;
	}
	else
	{
		cout << "执行失败" << endl;
		printf("错误原因： %s\n", mysql_error(&mysql));
		Control_log("sqllog.txt", mysql_error(&mysql));

		return false;
	}
}
//查询用户的信息
string*  mysql_manager::QueryDatabase_search(string user_id)
{
	sqlstr = "select * from user where user_name='" + user_id+"'";
	MYSQL_RES* result = NULL;
	if (0 == mysql_query(&mysql, sqlstr.c_str())) {
		cout << "mysql_query() select data succeed" << endl;

		//一次性取得数据集 检索一个完整的结果集合给客户。
		result = mysql_store_result(&mysql);
		//取得并打印行数
		//mysql_num_fields() 返回一个结果集合重的列的数量。
		int rowcount = mysql_num_rows(result);
		cout << "row count: " << rowcount << endl;

		//取得并打印各字段的名称
		unsigned int fieldcount = mysql_num_fields(result);
		MYSQL_FIELD* field = NULL;
		for (unsigned int i = 0; i < fieldcount; i++) {
			//mysql_fetch_field_direct () 返回一个表字段的类型，给出一个字段编号。
			field = mysql_fetch_field_direct(result, i);
			cout << field->name << "\t\t";
		}
		cout << endl;


		//保存结果
		
		string* res = new string[3];
		//打印各行
		//mysql_fetch_row() 从结果集合中取得下一行。
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
	cout << "数据库关闭成功" << endl;
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
