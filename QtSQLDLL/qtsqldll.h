#ifndef QTSQLDLL_H
#define QTSQLDLL_H

#include "qtsqldll_global.h"
#include "QString"
#include "QtSql"

class QTSQLDLLSHARED_EXPORT QtSQLDLL
{

public:
    QtSQLDLL();
};

extern "C" QTSQLDLLSHARED_EXPORT  QSqlDatabase MyQtSQLConnect(QString IPname,QString dbname,QString username,QString passwdname);

#endif // QTSQLDLL_H
