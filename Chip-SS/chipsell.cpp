#include "chipsell.h"
#include "ui_chipsell.h"

#include <QDebug> //测试用

ChipSell::ChipSell(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChipSell)
{
    isConnected_NET = false;//最先初始化为false
    isFirstLoginSys = true;//最先初始化为true

    No_Net_DLG.setWindowFlags(No_Net_DLG.windowFlags() | Qt::WindowStaysOnTopHint);//设置无网络窗口在最前显示避免闪屏出现主窗口
    No_Net_DLG.showFullScreen();//全屏显示没有网络连结
    No_Net_DLG.setWindowIcon(QIcon(":/Ico/WinIco"));
    No_Net_DLG.setStyleSheet("border-image:url(:/BG_Image/NoNet)");

    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/Ico/WinIco"));

    Other_Init();
    Init_Chips();
    Layout_Func();
    ConnectToSQL_Server();

    //初始化(网络监测)定时器（800毫秒timeout 快速响应） 该定时器始终贯穿整个程序
    //初始化(定时更新)定时器（5000~20000毫秒timeout 缓慢响应避免服务器卡顿（伪DDOS 只需要保持连接在就行）） 该定时器始终贯穿整个程序
    //-----------------------------------------------------------------
        timer_net = new QTimer(this);
        connect(timer_net,&QTimer::timeout,this,&ChipSell::timer_net_func);
        timer_net->start(800);
        timer_SQL_db = new QTimer(this);
        connect(timer_SQL_db,&QTimer::timeout,this,&ChipSell::timer_SQL_db_func);
        timer_SQL_db->start(30000);
    //------------------------------------------------------------------

 }

//定时器timeout函数
void ChipSell::timer_SQL_db_func()
{
    if(isConnected_NET)
    {
        //如果检测到网络连结则尝试去连接数据库更新
        ConnectToSQL_Server();
    }
    else
    {/*否则什么都不做*/}
}

//定时器timeout函数
void ChipSell::timer_net_func()
{
      if(isConnectedToNetwork())
      {
          //如果检测到网络连结则关闭No_Net_DLG
          No_Net_DLG.close();
          if(isFirstLoginSys)//如果是断网后第一次连上网络 则立马执行一次查询数据库
          {
            ConnectToSQL_Server();
            isFirstLoginSys=false;
          }
      }
      else
      {
          //如果没检测到网络连结则全屏显示No_Net_DLG
          No_Net_DLG.showFullScreen();
          isFirstLoginSys=true;
      }
}

//初始化其他的变量
void ChipSell::Other_Init()
{
    Sum_Chips=0;//芯片总价默认为0；
    all_Prices=new QLabel(this);
    all_Prices->setAlignment(Qt::AlignBottom);
    all_Prices->setText("您需要支付:(元)");
    all_Prices->setStyleSheet("QLabel{font-size:20px;font-weight:bold;color:red}");
    all_Prices->setMinimumSize(120,50);
    all_Prices->setMaximumSize(160,50);
    Unit_Price_LCD = new QLCDNumber(this);
    Unit_Price_LCD->setMinimumSize(160,100);
    Unit_Price_LCD->setMaximumSize(200,120);
    OK_Btn = new QPushButton(this);
    OK_Btn->setMinimumSize(100,90);
    OK_Btn->setMaximumSize(140,80);
    OK_Btn->setText("确认支付");
    connect(OK_Btn,&QPushButton::pressed,this,&ChipSell::Ok_Btn_Pressed);
    OK_Btn->setStyleSheet("QPushButton{color:#000000;font-size:25px;font-weight:bold;background-color:green;border:4px groove gray;border-radius:30px;padding:4px 8px;\
                            background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(0,180,0,75))}\
                            QPushButton:Pressed{color:#000000;font-size:25px;font-weight:bold;background-color:green;border:4px groove gray;border-radius:30px;padding:4px 8px;\
                                                background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(180,0,0,75))};");
    for(int i=0;i<12;i++)
    {
        ChipsName[i]="";
        Chip_Kind_Num[i]=0;//所有芯片数量的初始化 为0
        Chip_All_Num[i]=0;//所有芯片库存的初始化 为0
        m_Unit_Prices[i]=0;//所有芯片单价的初始化 为0
        label_Unit_Prices[i]=new QLabel(this);//单价标签
        label_Unit_Prices[i]->setStyleSheet("QLabel{font-size:20px;font-weight:bold;color:#000000}");
        label_Unit_Prices[i]->setAlignment(Qt::AlignBottom);//单价靠下显示
        vbox[i]=new QVBoxLayout;//垂直
        hbox[i]=new QHBoxLayout;//水平
        label_Unit_num[i]=new QLabel(this);//数量显示
        label_Unit_num[i]->setStyleSheet("QLabel{font-size:20px;font-weight:bold;color:#000000}");
        label_Unit_num[i]->setMinimumSize(30,30);
        label_Unit_num[i]->setMaximumSize(40,70);
        m_Chips_add[i]=new QPushButton(this);//加按钮
        m_Chips_add[i]->setStyleSheet("QPushButton{border-image:url(:/res/Add_Normal)}QPushButton:pressed{border-image:url(:/res/Add_Pressed)}");
        m_Chips_add[i]->setMinimumSize(40,40);
        m_Chips_add[i]->setMaximumSize(70,70);
        connect(m_Chips_add[i],&QPushButton::pressed,this,&ChipSell::Btn_add_pressed);
        m_Chips_del[i]=new QPushButton(this);//减按钮
        m_Chips_del[i]->setStyleSheet("QPushButton{border-image:url(:/res/Del_Normal)}QPushButton:pressed{border-image:url(:/res/Del_Pressed)}");
        m_Chips_del[i]->setMinimumSize(40,40);
        m_Chips_del[i]->setMaximumSize(70,70);
        connect(m_Chips_del[i],&QPushButton::pressed,this,&ChipSell::Btn_del_pressed);
//数量label上显示 X0
        label_Unit_num[i]->setText("X "+QString::number(Chip_Kind_Num[i], 10));
    }
}

ChipSell::~ChipSell()
{
    delete ui;
}

//自行布局
void ChipSell::Layout_Func()
{
    //手动布局 窗口自适应
    //QMainWindow的布局与其他窗口布局不同 其他的直接布局并设置就行了 QMainWindow需要先创建一个QWidget实例 并将该实例设置为centralWidget
    QGridLayout *Chips_Layout = new QGridLayout;
    QWidget *widget_cen=new QWidget;
    this->setCentralWidget(widget_cen);

    //每块芯片模块的布局
    for(int i=0;i<12;i++)
    {
        hbox[i]->addWidget(m_Chips_add[i]);
        hbox[i]->addWidget(label_Unit_num[i]);
        hbox[i]->addWidget(m_Chips_del[i]);
        vbox[i]->addWidget(label_Unit_Prices[i]);
        vbox[i]->addWidget(m_Chips[i]);
        vbox[i]->addLayout(hbox[i]);
    }
    Chips_Layout->addLayout(vbox[0],0,0);
    Chips_Layout->addLayout(vbox[1],0,1);
    Chips_Layout->addLayout(vbox[2],0,2);
    Chips_Layout->addLayout(vbox[3],0,3);
    Chips_Layout->addLayout(vbox[4],1,0);
    Chips_Layout->addLayout(vbox[5],1,1);
    Chips_Layout->addLayout(vbox[6],1,2);
    Chips_Layout->addLayout(vbox[7],1,3);
    Chips_Layout->addLayout(vbox[8],2,0);
    Chips_Layout->addLayout(vbox[9],2,1);
    Chips_Layout->addLayout(vbox[10],2,2);
    Chips_Layout->addLayout(vbox[11],2,3);

    QVBoxLayout *Pay_Layout = new QVBoxLayout;
    Pay_Layout->addWidget(all_Prices);
    Pay_Layout->addWidget(Unit_Price_LCD);
    Pay_Layout->addWidget(OK_Btn);
    QHBoxLayout *HBox = new QHBoxLayout;
    HBox->addLayout(Chips_Layout);
    HBox->addLayout(Pay_Layout);
    widget_cen->setLayout(HBox);
}

//芯片数量加1 事件
void ChipSell::Btn_add_pressed()
{
    int Temp_kind=0;//保存是哪个种类的芯片个数加一
    QPushButton *BTN=(QPushButton*)sender();//信号槽机制里获取发送者信息
    for(int i=0;i<12;i++)
    {
        if(BTN==m_Chips_add[i])//获取到发送者与实例按钮比较
        {
            Temp_kind=i;
            break;//已找到就不继续向下找 减少消耗
        }
    }
    //增加判断条件 默认不能超过5个 后加上不能超过库存
    if(Chip_All_Num[Temp_kind]>Chip_Kind_Num[Temp_kind]&&Chip_Kind_Num[Temp_kind]<5)
    {
        Chip_Kind_Num[Temp_kind]++;
        label_Unit_num[Temp_kind]->setText("X "+QString::number(Chip_Kind_Num[Temp_kind], 10));
        Sum_Chips+=m_Unit_Prices[Temp_kind];//总价加一个该芯片的价格
        if(Sum_Chips<0.00001)//这里需要加判断 莫名其妙出现一个超小的数导致bug
            Sum_Chips=0;
        Unit_Price_LCD->display(Sum_Chips);//LCD模块显示
    }
}

//芯片数量减1  事件
void ChipSell::Btn_del_pressed()
{
    int Temp_kind=0;//保存是哪个种类的芯片个数加一
    QPushButton *BTN=(QPushButton*)sender();//信号槽机制里获取发送者信息
    for(int i=0;i<12;i++)
    {
        if(BTN==m_Chips_del[i])//获取到发送者与实例按钮比较
        {
            Temp_kind=i;
            break;//已找到就不继续向下找 减少消耗
        }
    }
    if(Chip_Kind_Num[Temp_kind]>0)
    {
        Chip_Kind_Num[Temp_kind]--;
        label_Unit_num[Temp_kind]->setText("X "+QString::number(Chip_Kind_Num[Temp_kind], 10));
        Sum_Chips-=m_Unit_Prices[Temp_kind];//总价减一个该芯片的价格
        if(Sum_Chips<0.00001)//这里需要加判断 莫名其妙出现一个超小的数导致bug
            Sum_Chips=0;
        Unit_Price_LCD->display(Sum_Chips);//LCD模块显示
    }
}

//初始化所有的按钮并且设置ui(后决定采用QSS) 绑定信号槽pressed
void ChipSell::Init_Chips()
{
    for(int num=0;num<12;num++)
    {
        m_Chips[num] = new QPushButton(this);//需要添加一个父窗口
        m_Chips[num]->setFixedSize(QPixmap(":/res/IC_NORMAL").size());
        m_Chips[num]->setStyleSheet("QPushButton{border-image:url(:/res/IC_NORMAL);text-align:center;font-size:40px;font-weight:bold;color:#ffffff}");
        m_Chips[num]->setMinimumSize(90,40);
        m_Chips[num]->setDisabled(true);//禁止点击按钮
    }
}

//检测网络判断是否联网 后续将继续加上通过socket判断的方法（ping一个固定IP）
bool ChipSell::isConnectedToNetwork()
{
//************************************************************************
/*遍历所有网卡信息判断有无符合要求的网卡信息 同样无法判断虚拟网卡 有虚拟网卡会造成误判
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool isConnected = false;
    for (int i = 0; i < ifaces.count(); i++)
    {
        QNetworkInterface iface = ifaces.at(i);
        if ( iface.flags().testFlag(QNetworkInterface::IsUp)
             && iface.flags().testFlag(QNetworkInterface::IsRunning)
             && !iface.flags().testFlag(QNetworkInterface::IsLoopBack)
              )
        {
            //环回网卡 需要判断
            for (int j=0; j<iface.addressEntries().count(); j++)
            {
                if (isConnected == false)
                    isConnected = true;
            }
        }

    }
    return isConnected;*/
//************************************************************************

//----------------------------------------------------------------
//简易判断检测网卡信息 不保证可以上网 同样无法判断虚拟网卡
    QNetworkConfigurationManager mgr;
    isConnected_NET = mgr.isOnline();
    return mgr.isOnline();
//----------------------------------------------------------------
}

//连接到服务器的mysql数据库
typedef QSqlDatabase (*MyConnectSQL_Ptr)(QString IPname,QString dbname,QString username,QString passwdname);
void ChipSell::ConnectToSQL_Server()
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
                query.exec("select * from Chips_Price");
                query.first();
                for(int i=0;i<12;i++)
                {
                    ChipsName[i]=query.record().fieldName(i);
                    m_Chips[i]->setText(ChipsName[i]);//更新芯片名字
                    m_Unit_Prices[i]=query.value(i).toDouble();//由于数据库中只有一行数据 可以直接采用该query不需要更改
                    label_Unit_Prices[i]->setText("单价:"+QString::number(m_Unit_Prices[i],10,3));
                }
                query.exec("select * from Chips_Num");
                query.first();
                for(int i=0;i<12;i++)
                {
                    Chip_All_Num[i]=query.value(i).toDouble();//由于数据库中只有一行数据 可以直接采用该query不需要更改
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

//确认付款按钮点击事件
typedef PayDlg* (*PayDlgShow_Ptr)(double Sum,int* KindChip,QString *NameChips);
void ChipSell::Ok_Btn_Pressed()
{
    QLibrary PayLib("ChipsCheck");
    if(PayLib.load())
    {
        PayDlgShow_Ptr showPayDlg = (PayDlgShow_Ptr)PayLib.resolve("ShowPayDlg");
        if(showPayDlg)
        {
            PayDlg *m_PayDlg_1 = showPayDlg(Sum_Chips,Chip_Kind_Num,ChipsName);
            m_PayDlg_1->show();
            Reload();//重新初始化
        }
    }
    else
    {
        //全屏
        QMessageBox DLLMessage(QMessageBox::NoIcon,"DLL Missing","<font color='red'>ChipsCheck</font> <strong>Not Found</strong>",QMessageBox::Yes|QMessageBox::No);
        DLLMessage.setWindowIcon(QIcon(":/Ico/WinIco"));
        DLLMessage.setIconPixmap(QPixmap(":/Ico/WinIco"));
        DLLMessage.setButtonText(QMessageBox::Yes,QString("任然继续"));
        DLLMessage.setButtonText(QMessageBox::No,QString("终止程序"));
        if(DLLMessage.exec() == QMessageBox::No)
            exit(-1);//如果没有'登录的DLL' 并且按了退出或关闭则退出
        else{/*什么都不做 继续执行*/}
    }
}

//重新初始化一些数字量
void ChipSell::Reload()
{
    for(int i=0;i<12;i++)
    {
       Chip_Kind_Num[i] = 0;//购买数量重置
       label_Unit_num[i]->setText("X "+QString::number(Chip_Kind_Num[i], 10));
    }
    Sum_Chips = 0;
    Unit_Price_LCD->display(Sum_Chips);//LCD模块显示
}

//背景绘制
void ChipSell::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPixmap(rect(),QPixmap(":/res/BG"));
}
