#include "mysql.h"

MySql::MySql()
{
	sock = new MYSQL;
	mysql_init(sock);
}

MySql::~MySql()
{
	if(sock)
	{
		delete sock;
		sock = NULL;
	}
}

bool MySql::connect(string host,string username,string password,string db)
{
	if(!mysql_real_connect(sock,(const char*)host.c_str(),(const char*)username.c_str(),(const char*)password.c_str(),(const char*)db.c_str(),0,nullptr,CLIENT_MULTI_STATEMENTS))
		return false;
	return true;
}

void MySql::destroy()
{
	mysql_close(sock);
}

bool MySql::query(string sql,list<string>& res,int count)
{
	if(mysql_query(sock,(const char*)sql.c_str()))
		return false;
	results = mysql_store_result(sock);

	if(NULL == results)
		return false;
	while(record = mysql_fetch_row(results))
	{
		for(int i=0;i<count;i++)
		{
			res.push_back(record[i]);
		}   
	}   
	mysql_free_result(results);
	return true;
}

bool MySql::update(string sql)
{
	if(!(const char*)sql.c_str())
		return false;
	do
	{
		results = mysql_store_result(sock);
		mysql_free_result(results);
	}while(!mysql_next_result(sock));

	if(mysql_query(sock,(const char*)sql.c_str()))
	{
		return false;
	}
	return true;
}

