#include <iostream>
#include <string.h>
#include "mysql.h"
#include <string>
using namespace std;

int main(void) {
	MySql * mysql = new MySql();
	if(! mysql->connect("localhost","root","199874","testDb")) {
		cout<<" connect mysql testDb err"<<endl;
	} else {
		cout<<" connect mysql success"<<endl;
	}
	string sql = "insert into testTable values('hello','world')";
	mysql->update(sql);
	sql = "select * from testTable";
	list<string> res;
	mysql->query(sql,res);
	cout<<res.size()<<endl;
}
