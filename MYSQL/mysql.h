#ifndef MYSQL_H
#define MYSQL_H

#include <mysql/mysql.h>
#include <list>
#include <iostream>
#include <string.h>
using namespace std;

class MySql {
	private:
		MYSQL* sock;
		MYSQL_RES* results;
		MYSQL_ROW record;
	public:
		MySql();
		~MySql();
	public:
		/**
		 * 连接数据库
		 * @param 服务器ip
		 * @param 用户名
		 * @param 密码
		 * @param 数据库
		*/
		bool connect(string host,string username,string password,string db);
		/**
		 * 功能：销毁数据库实例
		*/
		void destroy();
		/**
		 * 对数据库进行查询操作
		 * @param 数据库操作语句，后续直接对其封装，隐藏sql语句 todo
		 * @param 查询结果集
		 * @param 要查询的数据条数，不传只查询一条
		*/
		bool query(string sql,list<string>& res,int count = 1);
		/**
		 * 对数据库进行添加、更新、删除操作
		 * @param 数据库操作语句
		*/
		bool update(string sql);
};

#endif
