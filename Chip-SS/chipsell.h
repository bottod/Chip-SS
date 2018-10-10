#ifndef CHIPSELL_H
#define CHIPSELL_H

#include <QMainWindow>
#include <QPushButton>
#include <QAction>
#include <QPixmap>
#include <QObject>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QtNetwork>
#include <qnetworkconfigmanager.h>
#include <QtSql>
#include <QMessageBox>
#include <QTimer>
#include <QLCDNumber>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include "paydlg.h"

namespace Ui {
class ChipSell;
}

class ChipSell : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChipSell(QWidget *parent = 0);
    void Init_Chips();//初始化所有的按钮 并绑定ui改变事件
    void Layout_Func();//自行布局
    bool isConnectedToNetwork();//检测网络判断是否联网 后续将继续加上通过socket判断的方法（ping一个固定IP）
    void ConnectToSQL_Server();//连接到服务器的mysql数据库
    void timer_net_func();//判断网络连接的定时器的绑定函数
    void timer_SQL_db_func();//定时更新数据库中信息的定时器的绑定函数
    void Other_Init();//初始化其他的变量
    void Btn_add_pressed();//选中芯片数量加一
    void Btn_del_pressed();//选中芯片数量减一
    void paintEvent(QPaintEvent *e);
    void Ok_Btn_Pressed();//确认付款按钮点击事件
    void Reload();//买完物品后或者付款失败都reload一下
    ~ChipSell();

private:
    //--1水平布局 --2垂直布局
    Ui::ChipSell *ui;
    QDialog No_Net_DLG;//没有网络连结时显示的页面
    QPushButton *m_Chips[12];//十二种芯片  （这里重新改需求了 不想改太多继续沿用button 但是不能点击 ）    --2
    QLabel *label_Unit_Prices[12];//十二种芯片的单价的标签             --2
    QLabel *label_Unit_num[12];//十二种芯片每个购买个数的标签          --1
    QPushButton *m_Chips_add[12];//十二种芯片数量加1            --1
    QPushButton *m_Chips_del[12];//十二种芯片数量减一           --1
    QVBoxLayout *vbox[12];//                                       --2
    QHBoxLayout *hbox[12];//                                 --1

    QLabel *all_Prices;//总价标签
    QPushButton *OK_Btn;//确认付款按钮
    int Chip_Kind_Num[12];//每种芯片要买的数量
    QString ChipsName[12];//12种芯片的名字
    int Chip_All_Num[12];//每种芯片在数据库里的存量
    double m_Unit_Prices[12];//十二种芯片的单价
    QLCDNumber *Unit_Price_LCD;//显示单价的LCD模块
    QTimer *timer_net;//判断网络连接的定时器
    QTimer *timer_SQL_db;//定时更新数据库中信息并且防止与数据库的连接在长时间不响应后的断开连接的定时器
    bool isConnected_NET;//是否连上网络 默认false
    bool isFirstLoginSys;//是否是断网后的第一次连上网络 默认true
    double Sum_Chips;//所有的芯片的总价
};

#endif // CHIPSELL_H
