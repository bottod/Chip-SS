#include "paydlg.h"
#include "ui_paydlg.h"
#include <QDebug>
#include <QThread>

PayDlg::PayDlg(double Sum,int *KindChip,QString *ChipNames,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PayDlg)
{
    ui->setupUi(this);
    Serial_Info_Send = "";
    CardNum = "";
    money = 0;
    sumPay = Sum;
    //由于KindChip在打开该窗口后会重置（和主程序享有相同的内存位置） 所以不能像ChipNames一样简单的附一个地址 而要重新开辟一块地址
    for(int i=0;i<12;i++)
    {
        int x=KindChip[i];
        Chip_kind[i]=x;
    }
    NameOfChips = ChipNames;

    InitLabel();
    LayOut_Func();

    this->setFocus();
    checkTime = new QTimer(this);
    connect(checkTime,&QTimer::timeout,this,&PayDlg::checkTime_func);
    checkTime->start(45000);
}

//时间判断函数
void PayDlg::checkTime_func()
{
    this->close();
}

PayDlg::~PayDlg()
{
    delete ui;
}

//初始化标签
void PayDlg::InitLabel()
{
    for(int i=0;i<12;i++)
    {
        m_label[i] = new QLabel(this);
        m_label[i]->setStyleSheet("QLabel{font-size:20px;}");
        m_label[i]->setText("您购买了 <b><font color='red'>"+NameOfChips[i]+"</font></b> X<b>"+QString::number(Chip_kind[i],10)+"</b>个");
        m_label[i]->setAlignment(Qt::AlignRight);
        if(Chip_kind[i]==0)
            m_label[i]->hide();
    }
    m_label[12] = new QLabel(this);
    m_label[12]->setStyleSheet("QLabel{font-size:30px;font-weight:bold;color:#000000}");
    m_label[12]->setAlignment(Qt::AlignRight);
    m_label[12]->setText("总价格:<font color='red'>"+QString::number(sumPay,10,3)+"</font>");
    m_Btn_Cancel = new QPushButton(this);
    m_Btn_Cancel->setStyleSheet("QPushButton{border-image:url(:/res/Back)};");
    connect(m_Btn_Cancel,&QPushButton::pressed,this,&PayDlg::Back_Pressed);
    m_Btn_Cancel->setMinimumSize(100,90);
    m_Btn_Cancel->setMaximumSize(150,140);

    PayLabel = new QLabel(this);
    PayLabel->setStyleSheet("QLabel{font-size:38px;font-weight:bold;color:#000000;}");
    PayLabel->setText("<font color='red'>等待付款</font>");
    PayLabel->adjustSize();
    //让QLabel能够自动判断并换行显示：
    PayLabel->setGeometry(QRect(328, 240, 329, 27*4));  //四倍行距
    PayLabel->setWordWrap(true);
    PayLabel->setAlignment(Qt::AlignCenter);

    TipLabel = new QLabel(this);
    TipLabel->setStyleSheet("QLabel{font-size:38px;font-weight:bold;color:#000000;}");
    TipLabel->setText("请在确认后直接刷卡支付 <font color='red'>(支付等待时间约为40秒！)</font>");
    TipLabel->adjustSize();
    //让QLabel能够自动判断并换行显示：
    TipLabel->setGeometry(QRect(328, 240, 329, 27*4));  //四倍行距
    TipLabel->setWordWrap(true);
    TipLabel->setAlignment(Qt::AlignCenter);
}

//自定义布局
void PayDlg::LayOut_Func()
{
    QWidget *widget_cen=new QWidget;
    this->setCentralWidget(widget_cen);
    QVBoxLayout *vbox = new QVBoxLayout;
    for(int i=0;i<13;i++)
    {
        vbox->addWidget(m_label[i]);
    }
    QVBoxLayout *cancel_tip_vbox = new QVBoxLayout;
    cancel_tip_vbox->addWidget(m_Btn_Cancel);
    cancel_tip_vbox->addWidget(TipLabel);
    cancel_tip_vbox->addWidget(PayLabel);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addLayout(cancel_tip_vbox);
    hbox->addLayout(vbox);
    widget_cen->setLayout(hbox);
}

//返回按钮按下
void PayDlg::Back_Pressed()
{
    this->close();
}

//背景绘制
void PayDlg::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPixmap(rect(),QPixmap(":/res/BG"));
}

//默认卖出事件
typedef QSqlDatabase (*MyConnectSQL_Ptr)(QString IPname,QString dbname,QString username,QString passwdname);
void PayDlg::DefaultSell()
{
    QLibrary SQLLib("QtSQLDLL");
    if(SQLLib.load())
    {
        MyConnectSQL_Ptr MyConnect = (MyConnectSQL_Ptr)SQLLib.resolve("MyQtSQLConnect");
        if(MyConnect)
        {
            QSqlDatabase dbconn;
            dbconn = MyConnect("101.132.70.232","user_info","XFLM","147258liu");//固定所有信息
            if(dbconn.isOpen())
            {
                QSqlQuery query = (QSqlQuery)dbconn;//选择数据库语句执行的数据库
                QString command = "select * from Chips_User where cardnum='"+CardNum+"';";
                query.exec(command.toStdString().c_str());
                query.first();
                money = query.value(2).toDouble();
                if(sumPay<=money)
                {
                    PayLabel->setText("<font color='green'>付款成功</font>");
                    QString QStr_user_query = "UPDATE Chips_User SET money='"+QString::number(money-sumPay,10,3)+"' "+"where cardnum='"+CardNum+"';";
                    query.exec(QStr_user_query.toStdString().c_str());
                    for(int i=0;i<12;i++)
                    {
                       if(Chip_kind[i])
                        {
                            Serial_Info_Send = Serial_Info_Send+QString::number(i,10)+","+QString::number(Chip_kind[i],10)+";";
                            query.exec("select * from Chips_Num");
                            query.first();
                            int modify_num = query.value(i).toInt();
                            modify_num -= Chip_kind[i];
                            QString QStr_query = "UPDATE Chips_Num SET "+NameOfChips[i]+"='"+QString::number(modify_num,10)+"' "+"where "+NameOfChips[i]+"='"+QString::number(query.value(i).toInt(),10)+"';";
                            query.exec(QStr_query.toStdString().c_str());
                        }
                    }
                }
                else
                {
                    PayLabel->setText("付款失败 err:1");
                }
            }
            else
            {
                //占位 暂时没想好怎么做
            }
        }
    }
    else
    {
        //全屏
        QMessageBox DLLMessage(QMessageBox::NoIcon,"DLL Missing","<font color='red'>QtSQLDLL</font> <strong>Not Found</strong>",QMessageBox::Yes|QMessageBox::No);
        DLLMessage.setWindowIcon(QIcon(":/Ico/WinIco"));
        DLLMessage.setIconPixmap(QPixmap(":/Ico/WinIco"));
        DLLMessage.setButtonText(QMessageBox::Yes,QString("任然继续"));
        DLLMessage.setButtonText(QMessageBox::No,QString("终止程序"));
        if(DLLMessage.exec() == QMessageBox::No)
            exit(-1);//如果没有'登录的DLL' 并且按了退出或关闭则退出
        else{/*什么都不做 继续执行*/}
    }
}

//串口信息发送
void PayDlg::SerialPort()
{
    qDebug()<<Serial_Info_Send;
}

//键盘鼠标响应
void PayDlg::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
    if(event->modifiers()==Qt::ControlModifier)//这里是判断是否有修饰的按键，例如control，shift等,此处以ctrl为例
    {
        if(event->key()==Qt::Key_Return||event->key()==Qt::Key_Enter)//判断键值，此处以enter键为例
        {
            //do something
        }
    }
    else
    {
        switch(event->key())
        {
        //当焦点在主窗口上而非窗口的其他控件上时才进行响应刷卡
        case Qt::Key_0:
            CardNum = CardNum + "0";
            break;
        case Qt::Key_1:
            CardNum = CardNum + "1";
            break;
        case Qt::Key_2:
            CardNum = CardNum + "2";
            break;
        case Qt::Key_3:
            CardNum = CardNum + "3";
            break;
        case Qt::Key_4:
            CardNum = CardNum + "4";
            break;
        case Qt::Key_5:
            CardNum = CardNum + "5";
            break;
        case Qt::Key_6:
            CardNum = CardNum + "6";
            break;
        case Qt::Key_7:
            CardNum = CardNum + "7";
            break;
        case Qt::Key_8:
            CardNum = CardNum + "8";
            break;
        case Qt::Key_9:
            CardNum = CardNum + "9";
            break;
            //按下enter键后传递cardnum然后清空cardnum
            //键盘上的两个enter消息都要能响应
        case Qt::Key_Enter://小键盘区
        case Qt::Key_Return://字母区
        {
            DefaultSell();//位置不能换
            SerialPort();
            //----------等待一段时间（2s）并且保持程序主界面响应------------
            QEventLoop eventloop;
            QTimer::singleShot(2000,&eventloop,SLOT(quit()));
            eventloop.exec();
            //------------------------------------------
            this->close();
        }
            break;
            //按下esc键时焦点回到主窗口 可刷卡查询
        case Qt::Key_Escape:
            this->setFocus();
            break;
        default:
            QMainWindow::keyPressEvent(event);
        }
    }

}
