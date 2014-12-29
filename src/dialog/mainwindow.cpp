#include "mainwindow.h"
#include<QtGui>
MainWindow::MainWindow(QWidget *parent)
    : BaseDialog(parent)
{

    setFixedSize(850, 680);
    setWindowIcon(QIcon(":/logo/images/tool"));
    //stateDialog
    stateDialog = new AisleTestdialog(this);
    stateDialog->setWindowModality(Qt::ApplicationModal);
    stateTimer = new QTimer(this);
    connect(stateTimer,SIGNAL(timeout()),
            this,SLOT(stateTimeroutSlot()));
    isGetSys = false;
    isGetHuodao = false;

    //请求超时定时器
    pcRequestTimer = new QTimer(this);
    connect(pcRequestTimer,SIGNAL(timeout()),this,SLOT(pcRequestTimeoutSlot()));


    statkedWidget = new QStackedWidget();
    //串口线程
    serialThread = new SerialThread;
    serialThreadConnect();

    //1页 数据打印
    printDialog = new PrintDialog();
    printDialogConnect();
    //2页 系统参数
    sysParaDialog = new SysParaDialog();
    sysParaDialogConnect();
    //3页 aisleConfigDialog
    aisleConfigDialog = new AisleConfigDialog();
    aisleConfigDialogConnect();
    //4页 aisleMannagementDialog
    aisleManagementDialog = new AisleManagementDialog();
    aisleManagementDialogConnect();
    //5串口助手
    serialAssistantDialog = new SerialAssistantDialog();
    serialAssistantDialogConnect();
    //6盒饭柜界面
    hefanDialog = new HefanDialog(this);
    hefanDialogConnect();



    //状态界面
    statusDialog = new StatusDialog(this);
    statusDialogConnect();

    //dateDialog
    dateDialogConnect();
    //主菜单
    mainMenu = new MainMenu();
    abountUsDialog = new AbountUsDialog(this);
    settingDialog = new SettingDialog(this);
    SettingDialogConnect();





    mainMenuInit();
    titleWidget = new TitleWidget();
    connect(titleWidget,SIGNAL(showMainMenuWidget()),
            this,SLOT(showMainMenuWidget()));
    connect(titleWidget,SIGNAL(closeWidegt()),
            this,SLOT(close()));
    connect(titleWidget,SIGNAL(showMinWidget()),
            this,SLOT(showMinimized()));
    connect(titleWidget,SIGNAL(turnPage(int)),
            this,SLOT(turnPage(int)));


    
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::white));
    statkedWidget->setPalette(palette);
    statkedWidget->setAutoFillBackground(true);
    statkedWidget->addWidget(printDialog);
    statkedWidget->addWidget(sysParaDialog);
    statkedWidget->addWidget(aisleConfigDialog);
    statkedWidget->addWidget(aisleManagementDialog);
    statkedWidget->addWidget(serialAssistantDialog);
    statkedWidget->addWidget(hefanDialog);

    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->addWidget(statkedWidget);
    centerLayout->setSpacing(0);
    centerLayout->setContentsMargins(0, 0, 0, 0);


    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(titleWidget);
    mainLayout->addLayout(centerLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(SHADOW_WIDTH, SHADOW_WIDTH,
                                   SHADOW_WIDTH, SHADOW_WIDTH);
    setLayout(mainLayout);

    titleWidget->turnPage("0");
    setConnectState(false);


}

MainWindow::~MainWindow()
{
    //serialAssistantDialog->deleteLater();
}

void MainWindow::translateLanguage()
{

}


void MainWindow::paintEvent(QPaintEvent *event)
{
    BaseDialog::paintEvent(event);

    QImage image;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    QRect rect = QRect(SHADOW_WIDTH,SHADOW_WIDTH,
                       this->rect().width() - 2* SHADOW_WIDTH
                       ,this->rect().height() - 2 * SHADOW_WIDTH);

    QDir dir("./images");
    //qDebug()<<"image.load="<<dir.absolutePath()<<"./images/back_ground";
    if(image.load("./images/back_ground"))//加载背景图片
    {
        painter.drawImage(rect,image);
    }
    else
    {
        painter.setBrush(Qt::blue);
        painter.drawRoundedRect(rect,2,2);
    }


}

void MainWindow::closeEvent(QCloseEvent *event)
{
    BaseDialog::closeEvent(event);
    serialThread->stopSerialThread();
    serialThread->wait();
    qDebug()<<"mainWindow close..";
}

//解决最小化恢复后 界面不刷新的原因
void MainWindow::changeEvent(QEvent *event)
{
    //qDebug()<<"changeEvent->type()"<<event->type();
    if(event->type() == QEvent::WindowStateChange)
    {
       // this->update();
        this->repaint();
    }
    else if(event->type() == QEvent::ActivationChange)
    {
    }
    event->accept();
}

/**********************************************************************************************
 *槽函数
 *
 **********************************************************************************************/
void MainWindow::turnPage(int curIndex)
{
    qDebug()<<curIndex;
    if(curIndex == 0)
    {
       statkedWidget->setCurrentWidget(printDialog);

    }
    else if(curIndex == 1)
    {
         statkedWidget->setCurrentWidget(sysParaDialog);
    }
    else if(curIndex == 2)
    {
        statkedWidget->setCurrentWidget(aisleConfigDialog);
        vmcUpdateHuodaoPageSLot(false);

    }
    else if(curIndex == 3)
    {
        statkedWidget->setCurrentWidget(aisleManagementDialog);
        vmcUpdateHuodaoPageSLot(false);
    }

    else if(curIndex == 4)
        statkedWidget->setCurrentWidget(serialAssistantDialog);
    else if(curIndex == 5)
        statkedWidget->setCurrentWidget(hefanDialog);
}


void MainWindow::vmcCabinetNoChangedSlot(int no)
{
    if(getAisleCabinet() != (no + 1) )
    {
        qDebug()<<"cabinetNo changed %d"<<no + 1;
        setAisleCabinet(no + 1);
        aisleConfigDialog->cabinetCurindexChanged(no);
        aisleManagementDialog->cabinetCurindexChanged(no);
        vmcUpdateHuodaoPageSLot(true);
    }     
}


void MainWindow::vmcUpdateHuodaoPageSLot(const bool flag)
{
    if(flag || !this->isGetHuodao)
        aisleConfigDialog->pcGetAisleReq();
}

void MainWindow::vmcUpdateSystemPageSLot(const bool flag)
{
    if(flag || !this->isGetSys)
        sysParaDialog->vmcConnect();
}


void MainWindow::showMainMenuWidget()
{
    QPoint p = rect().topRight();
    p.setX(p.x() - 90);
    p.setY(p.y() + 25);
    mainMenu->exec(this->mapToGlobal(p));//
}


void MainWindow::showAbountUs()
{
    //this->abountUsDialog->show();
    //this->abountUsDialog->setModal(true);
    QPoint p = rect().center();
    p.setX(p.x() - this->abountUsDialog->rect().width() / 2);
    p.setY(p.y() - this->abountUsDialog->rect().height() / 2);
    this->abountUsDialog->move(this->mapToGlobal(p));
    this->abountUsDialog->exec();
}

void MainWindow::showSettingDialog()
{
    QPoint p = rect().center();
    p.setX(p.x() - this->settingDialog->rect().width() / 2);
    p.setY(p.y() - this->settingDialog->rect().height() / 2);
    this->settingDialog->move(this->mapToGlobal(p));
    this->settingDialog->show();
    if(this->getConnectState())
    {
        QByteArray arr;
        QDataStream in(&arr,QIODevice::WriteOnly);
        in<<(quint8)0x00;
        this->pcRequestSendSlot(DEBUG_REQ,arr);
       // settingDialog->setEnabled(true);
    }
}

void MainWindow::showVMCStatusSlot()
{
    QPoint p = rect().center();
    p.setX(p.x() - this->settingDialog->rect().width() / 2);
    p.setY(p.y() - this->settingDialog->rect().height() / 2);
    this->statusDialog->move(this->mapToGlobal(p));
    this->statusDialog->show();
    this->statusDialog->timerStatusReqStart();
    if(this->getConnectState())
    {
        QByteArray arr;
        QDataStream in(&arr,QIODevice::WriteOnly);
        in<<(quint8)0x00;
        this->pcRequestSendSlot(STATUS_REQ,arr);
    }
}



void MainWindow::languageSelectSlot(LANGUAGE language)
{
    //qDebug()<<"languageSelectSlot "<<language;
    if(this->language == language)
    {
       // qDebug()<<"selecet language is same...."<<this->language;
        return;
    }
    QString language_qm,language_qt_qm;
    if(language == VMC_ZH)
    {
        language_qm = QString(":/language/vmctool_ch.qm");
        language_qt_qm = QString(":/language/qt_zh_CN.qm");
    }
    else if(language == VMC_EN)
    {
        language_qm = QString(":/language/vmctool_en.qm");
    }
    this->language = language;
    QSettings *write = new QSettings("config.ini",QSettings::IniFormat);

    write->setValue("Language/Language",this->language);
    delete write;

    this->translator->load(language_qm);
    this->translator_qt->load(language_qt_qm);

    //进行界面翻译
    this->translateLanguage();
    this->titleWidget->translateLanguage();
    this->abountUsDialog->translateLanguage();
    this->dateDialog->translateLanguage();
    this->mainMenu->translateLanguage();
    this->settingDialog->translateLanguage();
    this->printDialog->translateLanguage();
    this->aisleConfigDialog->translateLanguage();
    this->sysParaDialog->translateLanguage();
    this->aisleManagementDialog->translate();
    this->serialAssistantDialog->translate();
}

void MainWindow::setLanguage(LANGUAGE language)
{

    this->language = language;
    mainMenu->setLanguage(language);
}

/*******************************************************************************
*
*serialthread 槽函数
*
************************************************************************************/
void MainWindow::serialConnectSlot()
{
    isGetSys = false;
    isGetHuodao = false;
    setConnectState(true);
    printDialog->vmcConnect();
    sysParaDialog->vmcConnect();
    aisleManagementDialog->vmcConnect();
    aisleConfigDialog->vmcConnect();
    settingDialog->setConnect(true);
    hefanDialog->setConnect(true);
    statusDialog->vmcConnect(true);
    emit vmcConnectSignal();
}

void MainWindow::serialDisconnectSlot()
{
    stateDialogShowSlot(true,tr("Disconnected vending machine!"));
    setConnectState(false);
    pcRequestTimer->stop();    
    printDialog->vmcDisconnect();
    sysParaDialog->vmcDisconnect();
    aisleManagementDialog->vmcDisconnect();
    aisleConfigDialog->vmcDisconnect();
    settingDialog->setConnect(false);
    statusDialog->vmcConnect(false);
    hefanDialog->setConnect(false);
    emit vmcDisconnectSignal();                         
}






/*******************************************************************************
*
*子界面 槽函数
*
************************************************************************************/
//serial request
void MainWindow::pcRequestSendSlot(quint8 type, QByteArray arr)
{

    QDialog *dialog = qobject_cast<QDialog *>(sender());
    if(!getConnectState())
    {
        stateDialogShowSlot(true,tr("Not connected to the VMC!"));
        return;
    }
    if(serialThread->isPCRequest())
    {
        qDebug()<<QString("Other request send to VMC....");
        return;
    }
    if(dialog == aisleConfigDialog)
    {
        pcSender = AISLECONFIGDIALOG;
    }
    else if(dialog == aisleManagementDialog)
    {
        pcSender = AISLEMANAGEMENTDIALOG;
    }
    else if(dialog == hefanDialog)
        pcSender = HEFAN_DIALOG;
    else
        pcSender = OTHERDIALOG;

    qDebug()<<"pcRequestSend pcSender="<<pcSender;
    serialThread->updatePcCmdType(type);
    serialThread->updatePcSendArr(arr);
    serialThread->setPcRequestFlag(true);

    if(type == HUODAO_REQ && ((quint8)arr.at(0) == TEST_HUODAO_REQ))
    {
        pcRequestTimer->start(PC_REQUEST_TIMEOUT_CHUHUO);//测试货道100秒超时
    }
    else
        pcRequestTimer->start(PC_REQUEST_TIMEOUT);//10秒超时
}

//print
void MainWindow::printDialogOpenSlot(bool checked)
{

    //qDebug()<<"open2...";
    if(checked)
    {
        if(!serialThread->init())
        {
            QMessageBox::warning(this,tr("COM"),
                                 tr("Error in open COM port:")
                                 + serialThread->getPortName(),
                                 QMessageBox::Yes);
            printDialog->comOpenOrClose(2);
            return ;
        }
        printDialog->comOpenOrClose(1);
        serialThread->startSerialThread();
        printDialog->openComInfo(serialThread->getPortName());
        printDialog->labelStatusSetText(serialThread->getOpenPortInfo());
        serialTimer->start(1000);
    }
    else
    {
        //如果连接上 则要断开连接
        if(getConnectState())
            serialDisconnectSlot();

        printDialog->comOpenOrClose(0);
        serialThread->stopSerialThread();
        printDialog->labelStatusSetText("");
        serialTimer->stop();


    }
}

void MainWindow::showCabinetSlot(bool showCabinet)
{
    aisleConfigDialog->showCabinet(showCabinet);
    aisleManagementDialog->showCabinet(showCabinet);
}


void MainWindow::serialTimeoutSlot()
{
    serialThread->setTime();
}


//==========================================================================================
//串口线程 协议包上报槽
//==========================================================================================

void MainWindow::serialRptSlot(const quint8 type, const QByteArray &arr)
{
   if(type != VMC_POLL)
        pcRequestTimer->stop();
 #if 1
   switch(type)
   {
       case TRADE_RPT:
            serialPrintSlot(arr);
            break;
       case CLEAR_RPT:
            serialSumClearSlot(arr);
            break;
       case SYSPARA_RPT:
            serialSystemParaSlot(arr);
            break;
       case HUODAO_RPT:
            if(pcSender == HEFAN_DIALOG)
                hefanDialog->huodaoRptSlot(arr);
            else
                serialHuoDaoSlot(arr);
            break;
       case FACTORRECOVER_RPT:
            serialFactorySlot(arr);
            break;
       case DATETIME_RPT:
            serialDateSlot(arr);
            break;
       case STATUS_RPT:
            serialStatusSlot(arr);
            break;
       case DEBUG_RPT:
            serialDebugSlot(arr);
            break;
       case FACTOR_SET_RPT:
            sysParaDialog->setFactorySlot(arr);
            break;
       default:
         break;
   }
  #endif
}

//交易记录上报
void MainWindow::serialPrintSlot(const QByteArray &arr)
{
    QByteArray recvArr = arr;
    quint8 type = (quint8)recvArr.at(0);
    recvArr.remove(0,1);//remove the type byte
    if(type == 0x00)//totale trade
    {
        printDialog->recvPrintTotalInfo(recvArr);
    }
    else if(type == 0x01)//trade
    {
        printDialog->recvPrintTradeInfo(recvArr);
    }

}

void MainWindow::serialSumClearSlot(const QByteArray &arr)
{
    if(arr.isEmpty())
        return;
    QByteArray recvArr = arr;
    quint8 type = (quint8)recvArr.at(0);
    recvArr.remove(0,1);
    if(type == 0x00)
        printDialog->recvSumClear(recvArr);
}


void MainWindow::serialSystemParaSlot(const QByteArray &arr)
{
    QByteArray recvArr = arr;
    if(arr.isEmpty())
       return;
    quint8 type = (quint8)recvArr.at(0);
    recvArr.remove(0,1);
    if(type == 0x00)//get systemppara
    {
        isGetSys = true;
        sysParaDialog->recvSystemParaSlot(recvArr);

    }
    else if(type == 0x01)//set sys
    {
        qDebug()<<"serialSystemParaSetSlot"<<(quint8)recvArr.at(0);
        bool flag;
        if((quint8)recvArr.at(0) == 0x01)
            flag = true;
        else
            flag = false;
        sysParaDialog->SystemParaSetResult(flag);
    }
    else
    {
        QMessageBox::information(this,tr("System"),
                                 tr("failure!"),
                                 QMessageBox::Yes);
        return;
    }
    //获取参数后  开始选择获取货道
    if(!isGetHuodao)
    {
        if(this->statkedWidget->currentWidget() == aisleManagementDialog
                || this->statkedWidget->currentWidget() == aisleConfigDialog)
        {
            aisleConfigDialog->pcGetAisleReq();
        }
    }

}


void MainWindow::serialHuoDaoSlot(const QByteArray &arr)
{
    quint8 type,cabinetNo;//货道请求方式
    QByteArray recvArr = arr;
    type = (quint8)recvArr.at(0);
    cabinetNo = (quint8)recvArr.at(1);//获取柜号
    recvArr.remove(0,1);
    switch(type)
    {
        case GET_HUODAO_REQ:
            serialAisleGetSlot(recvArr);
            break;
        case SET_HUODAO_REQ:
            serialAisleSetSlot(recvArr);
            break;
        case INIT_HUODAO_REQ:
            serialAisleInitSlot(recvArr);
            break;
        case WRITE_HUODAO_REQ:
           serialAisleWRSlot(recvArr);
            break;
        case ONOFF_HUODAO_REQ:
            serialAisleCloseSlot(recvArr);
            break;
        case ADD_HUODAO_REQ:
            serialAddGoodsSlot(recvArr);
            break;
        case TEST_HUODAO_REQ:
            serialAisleTestSlot(recvArr);
            break;
        case GET_HUODAO_SELECT_REQ:
            serialGetSelectSlot(recvArr);
            break;
        case SET_HUODAO_SELECT_REQ:
            serialSetSelectSlot(recvArr);
            break;
        default:
            stateDialogShowSlot(true,tr("An unknown error!"));
            break;
    }
}

void MainWindow::serialDateSlot(const QByteArray &arr)
{
    QByteArray recvArr = arr;
    quint8 type = (quint8)recvArr.at(0);
    recvArr.remove(0,1);
    if(arr.size() < 1)
    {
      stateDialogShowSlot(true,tr("err"));
      return;
    }
    if(type == 0x00)//set date
    {
        sysParaDialog->dateSetSlot(recvArr);
    }
    else if(type == 0x01)//get date
    {

    }
    else
    {
        stateDialogShowSlot(true,tr("An unknown error"));
    }
}


void MainWindow::serialFactorySlot(const QByteArray &arr)
{ 
    sysParaDialog->factorySetSlot(arr);
}



void MainWindow::serialDebugSlot(const QByteArray &arr)
{
    QByteArray recvArr = arr;
    quint8 type = (quint8)recvArr.at(0);
    recvArr.remove(0,1);
    if(type == 0 || type == 1)
    {
        settingDialog->updateDebug(recvArr);
    }  
    else if(type > 1 && type <= 10)
    {
        if(recvArr.size() < 1)
        {
            return;
        }
        bool ckecked = ((quint8)recvArr.at(0) == 1) ? true:false;
        settingDialog->updateDebug(type,ckecked);
    }
}

void MainWindow::serialStatusSlot(const QByteArray &arr)
{
    this->statusDialog->pcStatusRpt(arr);
}



void MainWindow::serialAisleGetSlot(const QByteArray &arr)
{
    isGetHuodao = true;
    switch (pcSender)
    {
    case ONEKEYCONDIALOG:
        aisleConfigDialog->pcGetAisleRpt(arr);
        //aisleDialog->aisleGet(arr);
        break;
    case AISLECONFIGDIALOG:
        aisleConfigDialog->pcGetAisleRpt(arr);
        break;
    case ONEKEYCON:

        //aisleConfigDialog->upAisleDialog();;
    default:
       //aisleDialog->aisleGet(arr);
        break;
    }
}

void MainWindow::serialAisleInitSlot(const QByteArray &arr)
{

    switch (pcSender)
    {
        case ONEKEYCONDIALOG:
          //  oneKeyConfig->aisleInit(arr);
        break;

        case AISLECONFIGDIALOG:
            aisleConfigDialog->pcInitAisleRpt(arr);
        break;
        default:
           // aisleDialog->aisleInit(arr);
        break;
    }


}
void MainWindow::serialAisleSetSlot(const QByteArray &arr)
{
    if(pcSender == ONEKEYCONDIALOG)
        ;//oneKeyConfig->aisleSet(arr);
    else if(pcSender == AISLECONFIGDIALOG)
        aisleConfigDialog->pcConAisleRpt(arr);
    else if(pcSender == AISLEMANAGEMENTDIALOG)
        aisleManagementDialog->pcConGoodsRpt(arr);
    else
     pcSender = pcSender;//   aisleDialog->aisleSet(arr);
}
void MainWindow::serialAisleWRSlot(const QByteArray &arr)
{
    if(pcSender == ONEKEYCONDIALOG)
        ;// oneKeyConfig->aisleWrite(arr);
    else if(pcSender == AISLECONFIGDIALOG)
        aisleConfigDialog->pcWriteRpt(arr);
    else if(pcSender == AISLEMANAGEMENTDIALOG)
        aisleManagementDialog->pcWriteRpt(arr);
    else
      pcSender = pcSender;//  aisleDialog->aisleWrite(arr);


}

void MainWindow::serialAisleCloseSlot(const QByteArray &arr)
{
    if(pcSender == ONEKEYCONDIALOG)
    {
        pcSender = pcSender;
    }
    else if(pcSender == AISLECONFIGDIALOG)
        aisleConfigDialog->pcCloseAisleRpt(arr);
    else
    {
       pcSender = pcSender;
    }
}

void MainWindow::serialAisleTestSlot(const QByteArray &arr)
{
   // aisleDialog->aisleTestBack(arr);
    aisleManagementDialog->pcTestAisleRpt(arr);
}

void MainWindow::serialAddGoodsSlot(const QByteArray &arr)
{
    qDebug()<<"AddGoodsSetSlot="<<pcSender;
    if(pcSender == AISLEMANAGEMENTDIALOG)
    {
        aisleManagementDialog->pcAddGoodsRpt(arr);
        aisleConfigDialog->pcGetAisleReq();
    }
    else
        pcSender = pcSender;
}


void MainWindow::serialGetSelectSlot(const QByteArray &arr)
{
    if(pcSender == AISLECONFIGDIALOG)
    {
        aisleConfigDialog->pcGetSelectBoardRpt(arr);
    }
    else if(pcSender == AISLEMANAGEMENTDIALOG)
    {
        aisleManagementDialog->pcConSelectBoardRpt(arr);
    }

}

void MainWindow::serialSetSelectSlot(const QByteArray &arr)
{
    if(pcSender == AISLECONFIGDIALOG)
        aisleConfigDialog->pcConSelectBoardRpt(arr);
    else if(pcSender == AISLEMANAGEMENTDIALOG)
        aisleManagementDialog->pcConSelectBoardRpt(arr);
}




void MainWindow::systemChangedSlot(const quint8 type,QByteArray arr)
{

    if(type == SysParaDialog::CABINET_SUB_OPEN)
    {
        bool isOpenSlave = (bool)arr.at(0);
        setVMCSystemPara(VMC_SYSTEM_SLAVE_CABINET,isOpenSlave);
        showCabinetSlot(isOpenSlave);

    }
    else if(type == SysParaDialog::HEFAN_ENABLED)
    {
        if((bool)arr.at(0))//开启盒饭柜
        {
            titleWidget->showPage(6);
        }
        else
        {
            titleWidget->hidePage(6);
        }
    }
    else if(type == SysParaDialog::VMC_SCALE)
    {
        setVMCSystemPara(VMC_SYSTEM_SCALE,arr.at(0));
    }
    else
        this->aisleManagementDialog->initGoodsInfo();
}

void MainWindow::stateTimeroutSlot()
{
    stateTimer->stop();
    stateDialog->setText("");
    stateDialog->hide();
}

void MainWindow::stateDialogShowSlot(bool flag,const QString &text,quint32 msec)
{
    QPoint p = rect().center();
    p.setX(p.x() -  100);
    p.setY(p.y() - this->stateDialog->rect().height() / 2);
    this->stateDialog->move(this->mapToGlobal(p));
    stateDialog->setText(text);
    stateDialog->show();
    if(flag)
    {
        stateTimer->start(msec);
    }
}

void MainWindow::stateDialogShowSlot(bool flag,const QString &text)
{
    QPoint p = rect().center();
    p.setX(p.x() -  100);
    p.setY(p.y() - this->stateDialog->rect().height() / 2);
    this->stateDialog->move(this->mapToGlobal(p));
    stateDialog->setText(text);
    stateDialog->show();
    if(flag)
    {
        stateTimer->start(1000);
    }
    else
    {
       stateTimer->start(50000);
    }
}

void MainWindow::updateTable(quint8 levelNo)
{
    this->aisleManagementDialog->upGoodsDialog(levelNo);
}

void MainWindow::pcRequestTimeoutSlot()
{
    pcRequestTimer->stop();
    aisleConfigDialog->pcRequestTimeout();
    aisleManagementDialog->requestTimeoutSlot();
    this->stateDialogShowSlot(true,tr("PC request timeout!"));

}

/*******************************************************************************
*
*子线程serialThread 初始化函数
*
************************************************************************************/
void MainWindow::serialThreadConnect()
{
    serialTimer = new QTimer(this);
    connect(serialTimer,SIGNAL(timeout()),
            this,SLOT(serialTimeoutSlot()));

    connect(serialThread,SIGNAL(serialConnectSignal()),
            this,SLOT(serialConnectSlot()),Qt::QueuedConnection);
    connect(serialThread,SIGNAL(serialDisconnectSignal()),
            this,SLOT(serialDisconnectSlot()),Qt::QueuedConnection);
    connect(serialThread,SIGNAL(serialRptSignal(quint8,QByteArray)),
            this,SLOT(serialRptSlot(quint8,QByteArray)),Qt::QueuedConnection);

}
/*******************************************************************************
*
*子界面 初始化函数
*
************************************************************************************/
void MainWindow::printDialogConnect()
{
    connect(printDialog,SIGNAL(printDialogOpenSignal(bool)),
            this,SLOT(printDialogOpenSlot(bool)));
    connect(printDialog,SIGNAL(printRequestSignal(quint8,QByteArray)),
            this,SLOT(pcRequestSendSlot(quint8,QByteArray)));
}

void MainWindow::sysParaDialogConnect()
{
    connect(sysParaDialog,SIGNAL(systemParaRequestSignal(quint8,QByteArray)),
            this,SLOT(pcRequestSendSlot(quint8,QByteArray)));

    connect(sysParaDialog,SIGNAL(resultSignal(bool,QString)),
            this,SLOT(stateDialogShowSlot(bool,QString)));
    connect(sysParaDialog,SIGNAL(systemChangedSignal(quint8,QByteArray)),
            this,SLOT(systemChangedSlot(quint8,QByteArray)));
}


void MainWindow::aisleConfigDialogConnect()
{
    connect(aisleConfigDialog,SIGNAL(aisleRequestSignal(quint8,QByteArray)),
            this,SLOT(pcRequestSendSlot(quint8,QByteArray)));
    connect(aisleConfigDialog,SIGNAL(getAisleLevelNo(quint8)),
            this,SLOT(updateTable(quint8)));
    connect(aisleConfigDialog,SIGNAL(processTextSignal(bool,QString)),
            this,SLOT(stateDialogShowSlot(bool,QString)));
    connect(aisleConfigDialog,SIGNAL(vmcCabinetNoChangedSignal(int)),
            this,SLOT(vmcCabinetNoChangedSlot(int)));

}

void MainWindow::aisleManagementDialogConnect()
{
    connect(aisleManagementDialog,SIGNAL(goodsRequestSignal(quint8,QByteArray)),
            this,SLOT(pcRequestSendSlot(quint8,QByteArray)));
    connect(aisleManagementDialog,SIGNAL(getGoodsInfoSignal()),
            this->aisleConfigDialog,SLOT(pcGetAisleReq()));
    connect(aisleManagementDialog,SIGNAL(processTextSignal(bool,QString)),
            this,SLOT(stateDialogShowSlot(bool,QString)));
    connect(aisleManagementDialog,SIGNAL(vmcCabinetNoChangedSignal(int)),
            this,SLOT(vmcCabinetNoChangedSlot(int)));
}

void MainWindow::serialAssistantDialogConnect()
{
   // connect(serialAssistantDialog,SIGNAL(closeSignal()),
   //         this,SLOT(serialAssRecoverSlot()));

  //  connect(serialAssistantDialog,SIGNAL(independSignal()),
   //         this,SLOT(serialAssIndependSlot()));
}

void MainWindow::SettingDialogConnect()
{
    connect(settingDialog,SIGNAL(setRequestSignal(quint8,QByteArray)),
            this,SLOT(pcRequestSendSlot(quint8,QByteArray)));
}

void MainWindow::dateDialogConnect()
{
    dateDialog = new DateDialog(this);

    connect(dateDialog,SIGNAL(dateTimeSetSiganl(quint8,QByteArray)),
            this,SLOT(pcRequestSendSlot(quint8,QByteArray)));
    connect(dateDialog,SIGNAL(resultSignal(bool,QString)),
            this,SLOT(stateDialogShowSlot(bool,QString)));
}


void MainWindow::statusDialogConnect()
{
    connect(statusDialog,SIGNAL(pcStatusReq(quint8,QByteArray)),
            this,SLOT(pcRequestSendSlot(quint8,QByteArray)));
}

void MainWindow::hefanDialogConnect()
{
    connect(hefanDialog,SIGNAL(pcRequestSignal(quint8,QByteArray)),
            this,SLOT(pcRequestSendSlot(quint8,QByteArray)));

    connect(hefanDialog,SIGNAL(processTextSignal(bool,QString)),
            this,SLOT(stateDialogShowSlot(bool,QString)));
}


/******************************************************************************************
 *
 * 主菜单初始化函数
 * *****************************************************************************************/
void MainWindow::mainMenuInit()
{
    connect(mainMenu,SIGNAL(showAbountUs()),this,
            SLOT(showAbountUs()));
    connect(mainMenu,SIGNAL(languageSignal(LANGUAGE)),
            this,SLOT(languageSelectSlot(LANGUAGE)));
    connect(mainMenu,SIGNAL(showSetting()),this,
            SLOT(showSettingDialog()));
    connect(mainMenu,SIGNAL(showVMCStatus()),this,
            SLOT(showVMCStatusSlot()));
}
