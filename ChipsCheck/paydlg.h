#ifndef PAYDLG_H
#define PAYDLG_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QtSql>
#include <QSqlDatabase>
#include <QKeyEvent>
#include <QMessageBox>


namespace Ui {
class PayDlg;
}

class PayDlg : public QMainWindow
{
    Q_OBJECT

public:
    explicit PayDlg(double Sum,int *KindChip,QString *ChipNames,QWidget *parent = 0);
    void InitLabel();//初始化标签
    void LayOut_Func();//自己布局
    void paintEvent(QPaintEvent *e);
    void checkTime_func();//判断时间函数
    void Back_Pressed();//返回按钮按下
    void keyPressEvent(QKeyEvent *event);
    void DefaultSell();//默认卖出
    void SerialPort();//串口信息发送
    ~PayDlg();

private:
    Ui::PayDlg *ui;
    double sumPay;//需要付款的总价
    int Chip_kind[12];//购买清单
    QString *NameOfChips;//芯片名字
    QLabel *m_label[13];//购买清单label方式显示
    QPushButton *m_Btn_Cancel;//返回按钮
    QLabel *TipLabel;//提示刷卡标签
    QLabel *PayLabel;//提示支付情况
    QTimer *checkTime;//判断时间
    QString CardNum;//学生卡号
    QString Serial_Info_Send;//应该发送的串口信息
    double money;
};

#endif // PAYDLG_H
