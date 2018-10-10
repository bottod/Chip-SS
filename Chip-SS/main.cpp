#include "chipsell.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChipSell w;
    w.setWindowFlags(w.windowFlags()&~Qt::WindowStaysOnTopHint);//取消窗口最前显示
    w.resize(900,450);
    w.show();

    return a.exec();
}
