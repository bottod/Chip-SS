#include "qtsqldll.h"


QtSQLDLL::QtSQLDLL()
{
}

QSqlDatabase MyQtSQLConnect(QString IPname,QString dbname,QString username,QString passwdname)
{
    QSqlDatabase    dbconn;
    dbconn = QSqlDatabase::addDatabase("QMYSQL","SQL_user_info");//连接名称为MySQL_user_info
    dbconn.setHostName(IPname);//主机名字
    dbconn.setDatabaseName(dbname);//数据库名字
    if(dbconn.open(username, passwdname)/*用户名、密码，成功返回1*/)
       dbconn = QSqlDatabase::database("SQL_user_info");
    return dbconn;
}
