#include "aisleconfigdialog.h"
#include "ui_aisleconfigdialog.h"
#include <QtGui>

//#define TestHuodao


AisleConfigDialog::AisleConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AisleConfigDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->pushButtonReadAisle->setPicName(":/button/images/button");
    ui->pushButtonConfigAisle->setPicName(":/button/images/button");
    ui->pushButtonOneKeyConfig->setPicName(":/button/images/button");
    isAllBinConfig = false;
    groupW = 66;
    groupH = 62;
    aisleW = 70;
    aisleH = 62;
    initAisleButton(1);
    initAisleButton(2);
    connect(ui->pushButtonOneKeyConfig,SIGNAL(clicked()),
            this,SLOT(pcOneKeyConReq()));
    connect(ui->pushButtonConfigAisle,SIGNAL(clicked()),
            this,SLOT(pcConAisleReq()));
    connect(ui->pushButtonReadAisle,SIGNAL(clicked()),
            this,SLOT(pcGetAisleReq()));

    QFont font = this->font();
    font.setBold(true);
    font.setPointSize(11);
    ui->comboBox->setFont(font);
    this->showCabinet(false);
    ui->comboBox->setObjectName("cabinet");

    ui->comboBox->setCurrentIndex(0);
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),
            SIGNAL(vmcCabinetNoChangedSignal(int)));
    //upAisleDialog();
    setVMCState(false);
    translateLanguage();

#ifdef TestHuodao
    initAisleButton(AisleConfigDialog::SHOW);
#endif
    ui->frame->setFixedSize(groupW + aisleW*10 + 3,aisleH * 8 + 4*8 - 1);

}

AisleConfigDialog::~AisleConfigDialog()
{
    delete ui;
}


void AisleConfigDialog::translateLanguage()
{
    ui->retranslateUi(this);
    ui->pushButtonReadAisle->setText(tr("GetClomun"));
    ui->pushButtonConfigAisle->setText(tr("SetColumn"));
    ui->pushButtonOneKeyConfig->setText(tr("OneKeyCon"));

    int i = 0;
    foreach(AisleConfigButtonDialog *button,buttonList)
    {       
        button->translateLanguage();
        int x = i % 11;
        int y = i / 11;
       if(x == 0)
            button->setText(QString("%1-%2").arg(tr("Level")).arg(y+1));
        i++;
    }
    ui->labelColumn->setText(tr("V\nM\nC\n\nC\no\nl\nu\nm\nn"));
}


QString AisleConfigDialog::processinfoText(PROCESSTEXT text)
{
    QString textStr;
    switch(text)
    {
        case INITING:

            textStr = QString("%1 %2...").arg(tr("Initializing the column of"))
                                     .arg(ui->comboBox->currentText());
            break;
        case INITFAIL:
            textStr = tr("Failed to initialize the information of the column!");
            break;
        case CONFIGING:
            textStr = tr("Configuring the information of the column...");
            break;
        case CONFIGFAIL:
            textStr = tr("Failed to configure the information of the column!");
            break;
        case SAVING:
            textStr = tr("Saving information of the column...");
            break;
        case GETTING:
            textStr = tr("Getting column information...");
            break;
        case GETFAIL:
            textStr = tr("Failed to get information of the column");
            break;
        case GETSUC:
            textStr = tr("Succeeded to get information of the column");
            break;
        case CONSELECT:
            textStr = tr("Start to set Choose goods number keys");
            break;
        case CONSELECTTING:
            textStr = tr("Setting the Choose goods number keys");
            break;
        case GETTINGSELECT:
            textStr = tr("Getting the Choose goods number keys");
            break;
        case READFAIL:
            textStr = QString("%1 %2...").arg(tr(" Failed to read the file('ini') of "))
                                         .arg(ui->comboBox->currentText());
            break;
        default:
            break;
    }

    return textStr;
}

QString AisleConfigDialog::processinfoText(PROCESSTEXT text,quint8 leveNo)
{
    QString str;
    switch(text)
    {
    case GETTING:
        str = QString("%1%2%3")
                .arg(tr("Getting ")).arg(leveNo).arg(tr(" level column information..."));
        break;
    case CONFIGING:
        str = QString("%1%2%3")
             .arg(tr("Setting ")).arg(leveNo).arg(tr(" level column information..."));
        break;
    default:

        break;
    }
     return str;
}


void AisleConfigDialog::initAisleButton(quint8 type)
{
    AisleConfigButtonDialog *buttonTemp = NULL;
    quint8 i,j,x,y;
    if(type == 1)
    {
        buttonList.clear();
        for(j = 0; j < 8;j++)
        {
            for(i = 0 ;i < 11;i++)
            {
                buttonTemp = new AisleConfigButtonDialog(ui->frame);
                connect(buttonTemp,SIGNAL(openClickedSinal(bool)),
                        this,SLOT(openClickedSlot(bool)));
                connect(buttonTemp,SIGNAL(maxCountChangedSignal(QString)),
                        this,SLOT(maxCountChanged(QString)));
                connect(buttonTemp,SIGNAL(actionTestSignal(bool,AisleConfigButtonDialog)),
                        this,SLOT(actionTestSlot(bool,AisleConfigButtonDialog)));
                buttonTemp->hide();
                buttonTemp->setOpen(false);
                buttonList << buttonTemp;
            }
        }
    }
    else if(type == 2)
    {
        i = 0;
        quint8 aisleNoTemp;
        foreach(buttonTemp,buttonList)
        {
            x = i % 11;
            y = i / 11;
            if(x == 0)
            {
                buttonTemp->setFixedSize(groupW,groupH);
                buttonTemp->setGroup(true);
                buttonTemp->setGroupNo(y+1);
                buttonTemp->setGeometry(x + 1,y*groupH + 1 + y*4,groupW,groupH);
                buttonTemp->setText(QString("%1-%2").arg(tr("Level")).arg(y+1));
                buttonTemp->setOpen(false);
            }
            else
            {
                if(x == 10)
                  aisleNoTemp = (y+1)*10;
                else
                  aisleNoTemp = (y+1)*10 + x;
                buttonTemp->setFixedSize(aisleW,aisleH);
                buttonTemp->setGroup(false);
                buttonTemp->setGroupNo(y+1);
                buttonTemp->setAisleNo(x);
                buttonTemp->setText(QString("%1").arg(aisleNoTemp,2,10,QLatin1Char('0')));
                buttonTemp->setGeometry((x - 1)*aisleW+groupW + 1,y*aisleH + 1 + y*4,aisleW,aisleH);
                buttonTemp->setOpen(false);
                buttonTemp->setMaxCount(10);
            }
            i++;
        }

    }
    else if(type == 3)
    {
        foreach(buttonTemp,buttonList)
        {
           buttonTemp->show();
        }
    }
    else if(type == 4)
    {
        foreach(buttonTemp,buttonList)
        {
           buttonTemp->hide();
        }
    }

    else if(type == 10)
    {

    }
}

//=========================SLOT===============================//
void AisleConfigDialog::openClickedSlot(bool checked)
{
    AisleConfigButtonDialog *buttonTemp = NULL,*button = NULL;
    buttonTemp  = qobject_cast<AisleConfigButtonDialog *>(sender());
    foreach(button,buttonList)
    {
        if(button == buttonTemp)
        {
            if(button->isGroup())//层架
            {
                button->setOpen(checked);
                setLeveButtonOpen(checked,button->getGroupNo());
            }
            else//非层架
            {
            }
            //qDebug()<<"openClickedSlot="<<checked;

            break;
        }
    }
}


void AisleConfigDialog::cabinetCurindexChanged(int no)
{
    ui->comboBox->setCurrentIndex(no);
}

void AisleConfigDialog::maxCountChanged(QString text)
{
    AisleConfigButtonDialog *buttonTemp = NULL,*button = NULL;
    buttonTemp  = qobject_cast<AisleConfigButtonDialog *>(sender());
    foreach(button,buttonList)
    {
        if(button == buttonTemp)
        {
            if(button->isGroup())//层架
            {
                this->setLeveButtonStore(text,button->getGroupNo());
            }
            else//非层架
            {
            }
            break;
        }
    }
}


void AisleConfigDialog::pcConAisleReq()
{
    int ret;
    ret = QMessageBox::information(this,tr("Column"),
                                   tr("Are you sure want to configure the column?"),
                                   QMessageBox::Yes,QMessageBox::No);
    if(ret == QMessageBox::No)
        return;

    quint8 cabinetNo = ui->comboBox->currentIndex() + 1;
    setButtonEnable(false);
    initAisle(getAisleType(cabinetNo));
    readAisleDialog(); //更新货道配置界面信息
    pcInitAisleReq();
}
void AisleConfigDialog::pcGetAisleReq()
{

#ifndef TestHuodao
    quint8 cabinetNo = ui->comboBox->currentIndex() + 1;
    if(getVMCSystemPara(VMC_SYSTEM_SELECTBOARD))
    {
        pcGetSelectBoardReq();
    }
    else
    {
        emit processTextSignal(false,processinfoText(GETTING));
        initAisle(getAisleType(cabinetNo));
        upAisleDialog(0);//add by yoc 2013.11.14
        leveNo = 1;
        emit processTextSignal(false,processinfoText(GETTING,leveNo));
        setButtonEnable(false);
        sendGetAisle();
    }
#endif
}
void AisleConfigDialog::pcInitAisleReq()
{
    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);
    emit processTextSignal(false,processinfoText(INITING));

    in<<(quint8)INIT_HUODAO_REQ;
    in<<(quint8)(ui->comboBox->currentIndex() + 1);
    emit aisleRequestSignal(HUODAO_REQ,arr);
}

void AisleConfigDialog::pcCloseAisleReq()
{
    QByteArray sendArr;
    QDataStream in(&sendArr,QIODevice::WriteOnly);  
    in<<(quint8)ONOFF_HUODAO_REQ;
    in<<(quint8)(ui->comboBox->currentIndex() + 1);
    for(quint8 i = 0;i < 8;i++)
    {
       quint16 temp1 = 0,temp = 0;
       temp |= (leveAisleST[i].isOpen << 15);
       for(quint8 j = 0;j < 10;j++)
       {
           temp1 = leveAisleST[i].aisle[j].isOpen;
           temp |= (temp1 << j);

       }
       in<<temp;
    }
    emit processTextSignal(false,processinfoText(CONFIGING));
   // ui->labelState->setText(processinfoText(CONFIGING));
    emit aisleRequestSignal(HUODAO_REQ,sendArr);
}


void AisleConfigDialog::pcConSelectBoardReq()
{
    QByteArray sendArr;
    QDataStream in(&sendArr,QIODevice::WriteOnly);
    in<<(quint8)SET_HUODAO_SELECT_REQ;
    in<<(quint8)(ui->comboBox->currentIndex() + 1);
    in<<(quint8)0x00;
    for(int i = 0; i < 8;i++)
    {
        for(int j = 0; j < 10;j++)
        {
            in<<(quint8)leveAisleST[i].aisle[j].selectNo;
        }
    }

    emit processTextSignal(false,processinfoText(CONSELECTTING));
    emit aisleRequestSignal(HUODAO_REQ,sendArr);
}

void AisleConfigDialog::pcGetSelectBoardReq()
{
    emit processTextSignal(false,processinfoText(GETTINGSELECT));
    QByteArray sendArr;
    QDataStream in(&sendArr,QIODevice::WriteOnly);
    in<<(quint8)GET_HUODAO_SELECT_REQ;
    in<<(quint8)(ui->comboBox->currentIndex() + 1);
    in<<(quint8)0x00;
    in<<(quint8)0x00;

    emit aisleRequestSignal(HUODAO_REQ,sendArr);
}

//货道一键配置按钮槽
void AisleConfigDialog::pcOneKeyConReq()
{
    QString fileName;
    quint8 cabinetNo = (ui->comboBox->currentIndex() + 1);
    //打开文件阻塞了主线程
    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle(tr("File"));
    fileDialog.setDirectory(QDir::currentPath() + "/Conf");
    fileDialog.setFilter("*.ini");

    if(fileDialog.exec() == QDialog::Accepted)
    {
        fileName =  fileDialog.selectedFiles()[0];
    }
    qDebug()<<"aisleConfig fileName="<<fileName;

    if(fileName.isEmpty())
        return;
    aisleFileName = fileName;
    initAisle(getAisleType(cabinetNo));
    if(!readAisleOneKeyCon(fileName,0))
    {
        emit processTextSignal(true,processinfoText(READFAIL));
        return;
    }

    isAllBinConfig = getVMCSystemPara(VMC_SYSTEM_SLAVE_CABINET);
    ui->comboBox->setCurrentIndex(0);//默认从主柜开始配置
    pcInitAisleReq();

}
//=========================End SLOT===============================//



//=================Serial Back================================//


void AisleConfigDialog::pcCloseAisleRpt(const QByteArray &arr)
{
    //cabinet
    quint8 cabinet = (quint8)arr.at(0);
    cabinet = cabinet;
    quint8 rst = (quint8)arr.at(1);
    if(rst == VMC_OK )
    {
        leveNo = 1;
        emit processTextSignal(false,processinfoText(CONFIGING,leveNo));
        sendConAisleInfo();
    }
    else
    {
        emit processTextSignal(true,processinfoText(CONFIGFAIL));
    }

}
void AisleConfigDialog::pcConAisleRpt(const QByteArray &arr)
{

    quint8 index = 0;
    quint8 cabinet = (quint8)arr.at(index++);
    cabinet = cabinet;
    quint8 type = (quint8)arr.at(index++);
    if(type != 0x01)
        return;
    quint8 rst = (quint8)arr.at(index++);
    if((rst == VMC_OK) )
    {
        if(leveNo == 8)//配置结束 发送写入FLASH命令
        {
            emit processTextSignal(false,processinfoText(SAVING));
            //如果开启了选货按键板则还要配置选货按键板
            if(getVMCSystemPara(VMC_SYSTEM_SELECTBOARD))
            {
                pcConSelectBoardReq();
            }
            else
            {
                QByteArray sendArr;
                QDataStream in(&sendArr,QIODevice::WriteOnly);
                in<<(quint8)WRITE_HUODAO_REQ;
                in<<(quint8)(ui->comboBox->currentIndex() + 1);
                emit aisleRequestSignal(HUODAO_REQ,sendArr);
            }
        }
        else
        {
            leveNo++;
            emit processTextSignal(false,processinfoText(CONFIGING,leveNo));
            sendConAisleInfo();
        }
    }
    else
        emit processTextSignal(true,processinfoText(CONFIGFAIL));

}
void AisleConfigDialog::pcGetAisleRpt(const QByteArray &arr)
{
    quint8 index = 0,leveNum,i,j,maxNum;
    bool ok;
    quint8 cabinet = (quint8)arr.at(index++);

    //解析获取方式为层号
    if((quint8)arr.at(index++) != 0x01)
    {
        qDebug()<<"pcGetAisleRpt type is not right!"<<arr.at(0);
        emit processTextSignal(true,processinfoText(GETFAIL));
        //setStateText(processinfoText(GETFAIL));
        return;
    }
    //解析层号
    leveNum = (quint8)arr.at(index++);
    qDebug()<<"pcGetAisleRpt leveNum="<<leveNum
           <<"leveNo="<<getLeveNo();
    if(leveNo == leveNum)//验证
    {
        i = leveNum - 1;
        //解析层开启标志
        if((quint8)arr.at(index++) == 1)
        {

            leveAisleST[i].isOpen = true;
            maxNum = getAisleType(cabinet);
            for(j = 0;j < maxNum;j++)
            {
                //解析货道编号
                quint8 physicNo = (quint8)arr.at(index++);
                physicNo = physicNo;
                //leveAisleST[i].aisle[j].physicNo =
                leveAisleST[i].aisle[j].logicNo = (quint8)arr.at(index++);
                //qDebug()<<"physicNo"<<leveAisleST[i].aisle[j].physicNo;
                if((quint8)arr.at(index++))
                {
                   leveAisleST[i].aisle[j].isOpen = true;

                   quint32 tempAmount = arr.mid(index,2).toHex().toULong(&ok,16);
                   leveAisleST[i].aisle[j].price = amountFromScale(tempAmount);
                   index += 2;
                   leveAisleST[i].aisle[j].status = (quint8)arr.at(index++);
                   leveAisleST[i].aisle[j].remain = (quint8)arr.at(index++);
                   leveAisleST[i].aisle[j].maxCount = (quint8)arr.at(index++);
                   leveAisleST[i].aisle[j].tradeNum = (quint8)arr.at(index++);
                   leveAisleST[i].aisle[j].id = (quint8)arr.at(index++);

                }
                else
                {
                    leveAisleST[i].aisle[j].isOpen = false;
                    index += 7;
                }
            }
        }
        else
        {
            leveAisleST[i].isOpen = false;           
        }

        if(leveNo == 8)//获取最后一层 成功
        {

            setButtonEnable(true);
            upAisleDialog(leveNo);//add by yoc 2013.11.14
            emit getAisleLevelNo(leveNo);
            emit processTextSignal(true,processinfoText(GETSUC));
        }
        else//继续获取下一层
        {
            if(leveNo == 1)
            {
                quint8 aisleType = getAisleType(cabinet);
                //qDebug()<<"leveNo == 1 aisleType="<<aisleType;
                if(aisleType == 8)
                {
                    ui->frame->setFixedSize(groupW + aisleW*8 + 3,aisleH * 8 + 4*8 - 1);
                }
                else if(aisleType == 10)
                {
                    ui->frame->setFixedSize(groupW + aisleW*10 + 3,aisleH * 8 + 4*8 - 1);
                }
                else
                {
                    emit processTextSignal(true,processinfoText(GETFAIL));
                     //setStateText(processinfoText(GETFAIL));
                     return;
                }
            }
            upAisleDialog(leveNo);//add by yoc 2013.11.14
            emit getAisleLevelNo(leveNo);
            leveNo++;
            emit processTextSignal(false,processinfoText(GETTING,leveNo));
            sendGetAisle();
        }
    }
    else
    {
        // setStateText(processinfoText(GETFAIL));
        emit processTextSignal(true,processinfoText(GETFAIL));
        setButtonEnable(true);
    }
}
void AisleConfigDialog::pcInitAisleRpt(const QByteArray &arr)
{
    //解析柜号
    quint8 cabinet = (quint8)arr.at(0);
    cabinet = cabinet;
    //rst
    quint8 rst = (quint8)arr.at(1);
    if(rst == VMC_OK)
    {
        pcCloseAisleReq();
    }
    else
    {
         emit processTextSignal(true,processinfoText(INITFAIL));
    }

    qDebug()<<"pcInitAisleRpt\n\n\n\n"<<"\n";
}


void AisleConfigDialog::pcConSelectBoardRpt(const QByteArray &arr)
{
    //解析柜号
    quint8 cabinet = (quint8)arr.at(0);
    cabinet = cabinet;
    quint8 type = (quint8)arr.at(1);
    //rst
    quint8 rst = (quint8)arr.at(2);
    if(rst == VMC_OK && type == 0x00 )
    {
        //设置成功 保存配置
        emit processTextSignal(false,processinfoText(SAVING));
        QByteArray sendArr;
        QDataStream in(&sendArr,QIODevice::WriteOnly);
        in<<(quint8)WRITE_HUODAO_REQ;
        in<<(quint8)(ui->comboBox->currentIndex() + 1);
        emit aisleRequestSignal(HUODAO_REQ,sendArr);
    }
    else //配置失败
    {
         emit processTextSignal(true,processinfoText(CONFIGFAIL));
    }
}


void AisleConfigDialog::pcGetSelectBoardRpt(const QByteArray &arr)
{
    qDebug()<<"pcGetSelectBoardRpt size"<<arr.size();
    //解析柜号
    quint8 index = 0;
    quint8 cabinet = (quint8)arr.at(index++);

    quint8 type = (quint8)arr.at(index++);
    //rst
    if(type == 0x00)
    {
       initAisle(getAisleType(cabinet));
       upAisleDialog(0);

       for(int i = 0;i < 8;i++)
       {
           for(int j = 0;j < 10;j++)
           {
               leveAisleST[i].aisle[j].selectNo = (quint8)arr.at(index++);
           }
       }
       emit processTextSignal(false,processinfoText(GETTING));
       leveNo = 1;
       emit processTextSignal(false,processinfoText(GETTING,leveNo));
       setButtonEnable(false);
       sendGetAisle();
    }
    else if(type == 0x01)
    {}
    else //配置失败
    {
        emit processTextSignal(true,processinfoText(GETFAIL));
    }
}

void AisleConfigDialog::pcWriteRpt(const QByteArray &arr)
{
    quint8 cabinet = (quint8)arr.at(0);
    cabinet = cabinet;
    quint8 rst = (quint8)arr.at(1);
    if(rst == VMC_OK)
    {
        qDebug()<<"pcWriteRpt\n\n\n\n";
        if(getVMCSystemPara(VMC_SYSTEM_SLAVE_CABINET) && isAllBinConfig)
        {
           if(!readAisleOneKeyCon(aisleFileName,1))
           {
              emit processTextSignal(true,processinfoText(READFAIL));
              return;
           }
           isAllBinConfig = false;
           setAisleCabinet(2);
           ui->comboBox->setCurrentIndex(1);//配置从柜
           pcInitAisleReq();
        }
        else
        {
            emit processTextSignal(false,processinfoText(GETTING));
            pcGetAisleReq();
        }


    }
    else
    {
        emit processTextSignal(true,processinfoText(CONFIGFAIL));
    }

    isAllBinConfig = false;

}






void AisleConfigDialog::vmcConnect()
{
    setVMCState(true);

}

void AisleConfigDialog::pcRequestTimeout()
{
    setVMCState(true);
}
void AisleConfigDialog::vmcDisconnect()
{
    setVMCState(false);
    upAisleDialog(0);
}
//=================End Serial Back============================//


void AisleConfigDialog::setLeveButtonOpen(bool flag, quint8 groupNo)
{
    quint8 index;
    QList<AisleConfigButtonDialog *> list;
    list = buttonList;
    index = (groupNo - 1) * 11;
    for(int i = 1;i < 11;i++)
    {
        list.at(i + index)->setOpen(flag);

        list.at(i + index)->setMaxCount(flag *10);
    }
    list.clear();
}



void AisleConfigDialog::setLeveButtonStore(QString text, quint8 groupNo)
{
    quint8 index;
    QList<AisleConfigButtonDialog *> list;
    list = buttonList;
    index = (groupNo - 1) * 11;
    for(int i = 1;i < 11;i++)
    {
        list.at(i + index)->setMaxCount(text);
    }
    list.clear();
}
void AisleConfigDialog::sendGetAisle()
{
    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);
    in<<(quint8)GET_HUODAO_REQ;
    in<<(quint8)(ui->comboBox->currentIndex() + 1);
    in<<(quint8)0x01;
    in<<(quint8)leveNo;
    emit aisleRequestSignal(HUODAO_REQ,arr);
}

void AisleConfigDialog::sendConAisleInfo()
{
   QByteArray arr;
   QDataStream in(&arr,QIODevice::WriteOnly);
   quint16 temp16;
   in<<(quint8)SET_HUODAO_REQ;
   //柜号
   in<<(quint8)(ui->comboBox->currentIndex() + 1);
   //设置方式 level
   in<<(quint8)0x01;
   //leveNo 1byte
   in << (quint8)leveNo;
   for(int j = 0;j < 10;j++)
   {
       //aisle State 1byte
      // qDebug()<<"physicNo="<<leveAisleST[leveNo - 1].aisle[j].physicNo;
       in<<(quint8)leveAisleST[leveNo - 1].aisle[j].physicNo;

       temp16 = amountToScale(leveAisleST[leveNo - 1].aisle[j].price);
        in <<(quint8)(temp16 /256)
           <<(quint8)(temp16 % 256)
           <<(quint8)leveAisleST[leveNo - 1].aisle[j].status
           <<(quint8)leveAisleST[leveNo - 1].aisle[j].remain
           <<(quint8)leveAisleST[leveNo - 1].aisle[j].maxCount
           <<(quint8)leveAisleST[leveNo - 1].aisle[j].id;
   }
   emit aisleRequestSignal(HUODAO_REQ,arr);
}


bool AisleConfigDialog::getAisleindex(quint8 *index_i, quint8 *index_j,quint8 physicNo)
{
    quint8 i ,j;
    quint8 cabinetNo = ui->comboBox->currentIndex() + 1;
    if(physicNo)
    {
       for(i = 0; i <  8;i++)
       {
           for(j = 0; j < getAisleType(cabinetNo);j++)
           {
               if(leveAisleST[i].aisle[j].physicNo == physicNo)
               {
                   *index_i = i;
                   *index_j = j;
                   return true;
               }
           }
       }
    }
    return false;

}

#if 0
void AisleConfigDialog::upAisleDialog()
{

    quint8 leveNum,aisleNo,aisleType;
    AisleConfigButtonDialog *buttonTemp = NULL;
    aisleType = getAisleType();

    qDebug()<<"upAisleDialog aisleType="<<aisleType;
    if(aisleType == 8)
    {
        ui->frame->setFixedSize(groupW + aisleW*8 + 3, aisleH * 8 + 4*8 -1 );
    }
    else if(aisleType == 10)
    {
        ui->frame->setFixedSize(groupW + aisleW*10 + 3,aisleH * 8 + 4*8 - 1);
    }
    else
        return;

    qDebug()<<tr("AisleConfigDialog upAisleDialog1");

    foreach(buttonTemp,buttonList)
    {
        if(buttonTemp->isGroup())
        {
            leveNum = buttonTemp->getGroupNo() - 1;
            buttonTemp->setOpen(leveAisleST[leveNum].isOpen);
        }
        else
        {
            leveNum = buttonTemp->getGroupNo() - 1;
            aisleNo = buttonTemp->getAisleNo() - 1;
            if(leveAisleST[leveNum].isOpen)
            {
                buttonTemp->setOpen(leveAisleST[leveNum].aisle[aisleNo].isOpen);
                buttonTemp->setMaxCount(leveAisleST[leveNum].aisle[aisleNo].maxCount);
            }
            else
                buttonTemp->setOpen(leveAisleST[leveNum].isOpen);
        }
       buttonTemp->show();
    }
}
#endif

#if 1
void AisleConfigDialog::upAisleDialog(quint8 leveNo)
{
    quint8 leveNum,aisleNo;
    AisleConfigButtonDialog *buttonTemp = NULL;


    if(leveNo == 0)
    {
        initAisleButton(4);
    }
    else if(leveNo <= 8)
    {
        for(int i = 0;i < 11;i++)
        {
           buttonTemp = buttonList.at((leveNo - 1) * 11 + i);
           if(buttonTemp->isGroup())
           {
               leveNum = buttonTemp->getGroupNo() - 1;
               buttonTemp->setOpen(leveAisleST[leveNum].isOpen);
           }
           else
           {
               leveNum = buttonTemp->getGroupNo() - 1;
               aisleNo = buttonTemp->getAisleNo() - 1;
               if(leveAisleST[leveNum].isOpen)
               {
                   buttonTemp->setOpen(leveAisleST[leveNum].aisle[aisleNo].isOpen);
                   buttonTemp->setMaxCount(leveAisleST[leveNum].aisle[aisleNo].maxCount);
               }
               else
                   buttonTemp->setOpen(leveAisleST[leveNum].isOpen);
           }
          buttonTemp->show();
        }
    }


}
#endif

void AisleConfigDialog::readAisleDialog()
{
    AisleConfigButtonDialog *button = NULL;
    quint8 i,j;
    quint8 cabinetNo = ui->comboBox->currentIndex() + 1;
    foreach(button,buttonList)
    {
        i = button->getGroupNo() - 1;
        j = button->getAisleNo() - 1;
        if(button->isGroup())
        {
            leveAisleST[i].isOpen = button->getOpen();
        }
        else
        {            
            leveAisleST[i].aisle[j].isOpen = button->getOpen();
            leveAisleST[i].aisle[j].maxCount = button->getMaxCount();
            leveAisleST[i].aisle[j].remain = leveAisleST[i].aisle[j].maxCount;
        }
    }
    quint16 id = 1;
    for(int i = 0;i < 8;i++)
    {
        if(!leveAisleST[i].isOpen)
            continue;
        for(int j = 0;j < getAisleType(cabinetNo);j++)
        {
            if(leveAisleST[i].aisle[j].isOpen)
            {
                leveAisleST[i].aisle[j].id = id++;
                leveAisleST[i].aisle[j].status = 1;
                leveAisleST[i].aisle[j].price = 100;
                leveAisleST[i].aisle[j].selectNo = 0x00;
            }
        }
    }
}


//OneKey read
bool AisleConfigDialog::readAisleOneKeyCon(const QString &fileName,const quint8 cabinet)
{
    bool ok;
    quint8 leveOpen,aisleOpen,logicNo;
    QStringList leveOpenList,idList,priceList,maxCountList,selectNoList;
    QSettings *read = new QSettings(fileName,QSettings::IniFormat);

    QString cabinetStr = (cabinet == 0) ? "": "1";
    QString tempStr = QString("AisleType%1/type").arg(cabinetStr);
    quint8 aisleType = read->value(tempStr).toString().toInt(&ok,10);

    qDebug()<<"readAisleConfig fileName="<<fileName<<"aisleType="<<aisleType;
   //leve 寻址
   for(int i = 0;i < 8;i++)
   {
       leveOpenList = read->value(QString("AisleOpen%1/leve%2").arg(cabinetStr).arg(i + 1)).toString().split(" ");
       idList = read->value(QString("ID%1/leve%2").arg(cabinetStr).arg(i + 1)).toString().split(" ");
       priceList = read->value(QString("Price%1/leve%2").arg(cabinetStr).arg(i + 1)).toString().split(" ");
       maxCountList = read->value(QString("MaxCount%1/leve%2").arg(cabinetStr).arg(i + 1)).toString().split(" ");
       selectNoList =  read->value(QString("SelectNO%1/leve%2").arg(cabinetStr).arg(i + 1)).toString().split(" ");
       qDebug()<<"leveOpenList="<<leveOpenList;
       qDebug()<<"idList="<<idList;
       qDebug()<<"priceList="<<priceList;
       qDebug()<<"maxCountList="<<maxCountList;
       if(leveOpenList.size() != 11   ||   idList.size() != 10        ||
          priceList.size() != 10 ||   maxCountList.size() != 10       ||
          (getVMCSystemPara(VMC_SYSTEM_SELECTBOARD) && selectNoList.size() != 10))
       {
           qDebug()<<"read Config file is not right!";
           qDebug()<<selectNoList.size();
           return false;
       }

       leveOpen = (quint8)leveOpenList.at(0).split("-").at(1).toInt(&ok,10);
       qDebug()<<"leveOpen="<<leveOpen;
       leveAisleST[i].isOpen = leveOpen;
       if(!leveOpen)
            continue;
       for(int j = 0; j < aisleType;j++)
       {
           if(j >= 9)
               logicNo = (i + 1) * 10;
           else
               logicNo = (i + 1) * 10 + j + 1;
           logicNo = logicNo;
           aisleOpen = (quint8)leveOpenList.at(j + 1).split("-").at(1).toInt(&ok,10);
           qDebug()<<"aisleOpen="<<aisleOpen;
           leveAisleST[i].aisle[j].isOpen = aisleOpen;
           if(!aisleOpen)
               continue;

           //货道属性
           quint16 temp2 = (quint16)idList.at(j).split("-").at(1).toInt(&ok,16);
           leveAisleST[i].aisle[j].id = temp2;
           quint8 temp1 = (quint8)maxCountList.at(j).split("-").at(1).toInt(&ok,10);
           leveAisleST[i].aisle[j].maxCount = temp1;
           leveAisleST[i].aisle[j].remain = temp1;

          //temp1 = (quint8)read->value(QString("%1/%2").arg(logicNo).arg("State")).toString().toInt(&ok,10);
           leveAisleST[i].aisle[j].status = 1;

           temp2 = (quint16)priceList.at(j).split("-").at(1).toULong(&ok,10);
           leveAisleST[i].aisle[j].price = temp2;
           //新增选货按键板
           temp1 = (quint8)selectNoList.at(j).split("-").at(1).toInt(&ok,10);
           leveAisleST[i].aisle[j].selectNo = temp1;
       }

   }
   delete read;


   return true;
}

void AisleConfigDialog::setVMCState(const bool flag)
{
    setButtonEnable(flag);
}
void AisleConfigDialog::setButtonEnable(const bool flag)
{
    ui->pushButtonConfigAisle->setEnabled(flag);
    ui->pushButtonOneKeyConfig->setEnabled(flag);
    ui->pushButtonReadAisle->setEnabled(flag);
}

void AisleConfigDialog::showCabinet(const bool isShow)
{
    if(isShow)
    {
        ui->comboBox->show();

    }
    else
    {
        ui->comboBox->setCurrentIndex(0);
        ui->comboBox->hide();   
    }
}



void AisleConfigDialog::actionTestSlot(const bool allTest, const AisleConfigButtonDialog &button)
{

    AisleConfigButtonDialog *buttonTemp = (AisleConfigButtonDialog *)&button;
    qDebug()<<"AisleConfigDialog::actionTestSlot"<<allTest<<buttonTemp;
    if(buttonList.indexOf(buttonTemp) < 0)
    {
        qDebug()<<"buttonList.contains(button) is false"<<buttonList.contains(buttonTemp);
        return;
    }
    if(allTest)//全货道测试
    {

    }
    else
    {
       if(buttonTemp->isGroup())//测试层
       {}
       else//测试单个货道
       {

       }
    }
}
//void AisleConfigDialog::paintEvent(QPaintEvent *event)
//{
//    AisleConfigDialog::paintEvent(event);
//    qDebug()<<"AisleConfigDialog::paintEvent";
//}
