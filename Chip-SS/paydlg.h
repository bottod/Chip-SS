#ifndef PAYDLG_H
#define PAYDLG_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>


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
    ~PayDlg();

private:
    Ui::PayDlg *ui;
    double sumPay;//需要付款的总价
    int *Chip_kind;//购买清单
    QString *NameOfChips;//芯片名字
    QLabel *m_label[13];//购买清单label方式显示
};

#endif // PAYDLG_H
