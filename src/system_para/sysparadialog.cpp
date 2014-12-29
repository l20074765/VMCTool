#include "sysparadialog.h"
#include "ui_sysparadialog.h"
#include <QtGui>
#include <QTextCodec>
#include <QDebug>
SysParaDialog::SysParaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SysParaDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    initLayout();
    connect(ui->recover_pushButton,SIGNAL(clicked()),
            this,SLOT(on_recover_pushButton_clicked()));
    dateDialog = new DateDialog(this);
    connect(dateDialog,SIGNAL(dateTimeSetSiganl(quint8,QByteArray)),
            this,SIGNAL(systemParaRequestSignal(quint8,QByteArray)));

    connect(ui->pushButtonFactoryReset,SIGNAL(clicked()),
            this,SLOT(pushButtonFactoryResetClicked()));

    connect(ui->subBinOpen_comboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(subBinOpenSlot(int)));

    connect(ui->userSelectKeyBoard_comboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(userSelectKeyBoardSlot(int)));

    connect(ui->pcEnable_comboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(pcEnableSlot(int)));

    //单层最大货道数更改
    connect(ui->channelType_comboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(channelTypeSLot(int)));

    connect(ui->subBinChannelType_comboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(channelTypeSLot(int)));

    //盒饭柜
    connect(ui->LittleCabinet_comboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(hefanOpenSlot(int)));
    connect(ui->comboBox_Scale,SIGNAL(currentIndexChanged(int)),this,SLOT(vmcScaleSlot(int)));




    subBinOpenSlot(0);

    vmcDisconnect();
    superMode(true);

    setFontAndColor();
    translateLanguage();
    QFont font = this->font();
    font.setPointSize(10);
    ui->labelLanguage->setFont(font);
    ui->language_comboBox->setFont(font);



    ui->pushButtonFactoryReset->setPicName(":/button/images/button");
    ui->pushButtonSetDate->setPicName(":/button/images/button");
    ui->recover_pushButton->setPicName(":/button/images/button");
    ui->save_pushButton->setPicName(":/button/images/button");
    ui->defaultSet_pushButton->setPicName(":/button/images/button");

    QList<QLineEdit *> lineEditList;
    lineEditList<<ui->billValue_lineEdit<<ui->billValue_lineEdit_2
                  <<ui->billValue_lineEdit_3<<ui->billValue_lineEdit_4<<ui->billValue_lineEdit_5
                    <<ui->billValue_lineEdit_6<<ui->billValue_lineEdit_7<<ui->billValue_lineEdit_8
                      <<ui->coinValue_lineEdit<<ui->coinValue_lineEdit_2
                        <<ui->coinValue_lineEdit_3<<ui->coinValue_lineEdit_4<<ui->coinValue_lineEdit_5
                          <<ui->coinValue_lineEdit_6<<ui->coinValue_lineEdit_7<<ui->coinValue_lineEdit_8
                            <<ui->hopperValue_lineEdit<<ui->hopperValue_lineEdit_2<<ui->hopperValue_lineEdit_3
                           <<ui->lineEdit_BillEnableValue;


  //  QRegExp regExp("[0-9]{1,8}[.]{0,1}[0-9]{0,2}");
 //   QValidator *validator = new QRegExpValidator(regExp, this);
 //   ui->MaxValue_lineEdit->setValidator(validator);
    foreach(QLineEdit *lineEdit,lineEditList)
    {
 //       lineEdit->setValidator(validator);
        connect(lineEdit,SIGNAL(editingFinished()),
                this,SLOT(lineEditChangedSlot()));
    }

    connect(ui->MaxValue_lineEdit,SIGNAL(editingFinished()),
            this,SLOT(lineEditChangedSlot()));

}

SysParaDialog::~SysParaDialog()
{
    delete ui;
}





void SysParaDialog::translateLanguage()
{
    ui->retranslateUi(this);
    //button
    ui->pushButtonFactoryReset->setText(tr("Factory"));
    ui->pushButtonSetDate->setText(tr("SetDate"));
    ui->recover_pushButton->setText(tr("GetSystem"));
    ui->save_pushButton->setText(tr("SetSystem"));
    ui->defaultSet_pushButton->setText(tr("OneKeyCon"));

    this->resize(this->size());

    //Language:
    ui->labelLanguage->setToolTip(tr("LCD display language type..."));
    ui->logo_label->setToolTip(tr("Whether to show the logo after start the VMC..."));
    ui->billValidatorType_label->setToolTip(tr("The bill validator machine type..."));
    ui->coinAcceptorType_label->setToolTip(tr("The coin receiver machine type..."));
    ui->coinChangerType_label->setToolTip(tr("The coin change machine type..."));
    ui->gOCIsOpen_label->setToolTip(tr("Dispensing confirm sensors..."));
    ui->saleTime_label->setToolTip(tr("After a timeout to return money..."));
    ui->cunhuoKou_label->setToolTip(tr("Whether to open the manual add goods   "));

    ui->MaxValue_label->setToolTip(tr("Single purchase, bill receives the maximum amount   "));
    ui->channel_label->setToolTip(tr("Shipping methods: spring, elevator   "));
    ui->subchannel_label->setToolTip(tr("Shipping methods: spring, elevator   "));

    ui->channelType_label->setToolTip(tr("Maximum number of goods in monolayer   "));
    ui->subBinChannelType_label->setToolTip(tr("Maximum number of goods in monolayer   "));
    ui->userSelectKeyBoard_label->setToolTip(tr("Whether to open the select button board goods   "));

    ui->MaxValue_lineEdit->setToolTip(tr("Monetary unit : cents      "));


}




void SysParaDialog::setFontAndColor()
{
    //label
    QFont font = this->font();
    QPalette palette = this->palette();
    font.setBold(true);
    font.setPointSize(14);
    palette.setColor(QPalette::WindowText,QColor(Qt::blue));

}


/***********************************************************************************************************
 * 配置参数按钮槽
 ************************************************************************************************************/
void SysParaDialog::on_save_pushButton_clicked()
{
    int ret;
    quint8 aisleType,temp8,subAisleType;
    quint32 tempAmount;
    ret = QMessageBox::information(this,tr("Save"),
                             tr("Are you sure to Set the system parameters?"),
                             QMessageBox::Yes,QMessageBox::No);

    if(ret == QMessageBox::No)
        return;

    QByteArray arr;
    bool ok;
    QDataStream in(&arr,QIODevice::WriteOnly);

    in<<(quint8)0x01;//类型 type = 1设置参数
    in<<(quint8)ui->language_comboBox->currentIndex();//语言版本

    in<<(quint32)ui->VMCSN_lineEdit_2->text().toULong(&ok,16);//售货机型号

    //主控板型号
    in<<(quint16)0x0114;

    switch (ui->VMCType_comboBox->currentIndex())//售货机类型
    {
        case 0://饮料
            temp8 = VMC_DRINK;
            break;
        case 1://食品
            temp8 = VMC_FOOD;
            break;
        case 2:
            temp8 = VMC_YITUO;
            break;
        default:
            temp8 = VMC_OTHER;
            break;
    }
    in<<temp8;

    //纸币器 硬币器 找零器 读卡器
    in <<(quint8)ui->billValidatorType_comboBox->currentIndex()
        <<(quint8)ui->coinAcceptorType_comboBox->currentIndex()
        <<(quint8)ui->coinChangerType_comboBox_2->currentIndex()      
        <<(quint8)ui->comboBoxReadCardType->currentIndex();


    //纸币器通道面值 + 硬币器通道面值 + 找零器通道面值
    QList <QLineEdit *> channelList;
    channelList <<ui->billValue_lineEdit<<ui->billValue_lineEdit_2
                <<ui->billValue_lineEdit_3<<ui->billValue_lineEdit_4
                <<ui->billValue_lineEdit_5<<ui->billValue_lineEdit_6
                <<ui->billValue_lineEdit_7<<ui->billValue_lineEdit_8

                <<ui->coinValue_lineEdit<<ui->coinValue_lineEdit_2
                <<ui->coinValue_lineEdit_3<<ui->coinValue_lineEdit_4
                <<ui->coinValue_lineEdit_5<<ui->coinValue_lineEdit_6
                <<ui->coinValue_lineEdit_7<<ui->coinValue_lineEdit_8

                <<ui->hopperValue_lineEdit<<ui->hopperValue_lineEdit_2
                <<ui->hopperValue_lineEdit_3;

    for(int i = 0; i < channelList.count(); i++)
    {
       QString tempStr =  channelList.at(i)->text();
       if(tempStr.indexOf(".") < 0)
           tempStr += "00";
       else
       {
           QString str = QString("%1")
                   .arg(tempStr.split(".").at(1),-2,QLatin1Char('0'));
           tempStr = tempStr.split(".").at(0) + str;
       }
       quint32 channelValue = priceFormat(tempStr).toULong(&ok,10);
       temp8 = encodAmount(amountToScale(channelValue));
       in<< temp8;
    }

    //售卖时间
    temp8 = ui->saleTime_comboBox->currentIndex();   
    temp8 = (temp8 == 9 ? 255 : temp8 * 30);
    in <<temp8;

    //纸币最大上限
    QString maxCountStr = ui->MaxValue_lineEdit->text();
    if(maxCountStr.indexOf(".") < 0)
        maxCountStr += "00";
    else
    {
        QString str = QString("%1")
                .arg(maxCountStr.split(".").at(1),-2,QLatin1Char('0'));
        maxCountStr = maxCountStr.split(".").at(0) + str;
    }

    tempAmount = amountToScale((quint32)(priceFormat(maxCountStr).toULong(&ok,10)));
    in<<(quint32)tempAmount;

    //货到类型
    temp8 = ui->channel_comboBox->currentIndex();
    aisleType = (quint8)ui->channelType_comboBox->currentIndex();
    temp8 = (temp8 << 4) | (aisleType & 0x0F);
    in<<temp8;



    //福柜类型
    temp8 = ui->subchannel_comboBox->currentIndex();
    subAisleType = (quint8)ui->subBinChannelType_comboBox->currentIndex();
    temp8 = (temp8 << 4) | (subAisleType & 0x0F);
    in<<temp8;

    in <<(quint8)ui->pcEnable_comboBox->currentIndex()
        <<(quint8)ui->decimalNum_comboBox->currentIndex();

    //other config
    //B3
    temp8 = 0;
    quint8 threeSelectBoard = ui->userSelectKeyBoard_comboBox->currentIndex();
    threeSelectBoard = (threeSelectBoard == 1) ? 1:0;
    temp8 = (((quint8)ui->LittleCabinet_comboBox->currentIndex() & 0x01) << 0) |
            (((quint8)threeSelectBoard & 0x01) << 1)   |
            (((quint8)ui->comboBoxCVS_MANADD->currentIndex() & 0x01) << 2);

    in<<temp8;

    //B2
    temp8 = 0;
    temp8 = (((quint8)ui->comboBoxCompressorOpen->currentIndex() & 0x01) << 1) |
            (((quint8)ui->comboBoxLightOpen->currentIndex() & 0x01) << 3)      |
            (((quint8)ui->comboBoxTempControl->currentIndex() & 0x01) << 5)    |

            (((quint8)ui->comboBoxSlaveCompressorOpen->currentIndex() & 0x01) << 0) |
            (((quint8)ui->comboBoxSlaveLightOpen->currentIndex() & 0x01) << 2)      |
            (((quint8)ui->comboBoxSlaveTempControl->currentIndex() & 0x01) << 4)
            ;
    in << temp8;

    //B1
    temp8 = 0;
    temp8 = ((quint8)ui->cunhuoKou_comboBox->currentIndex() & 0x01) |
            (((quint8)ui->gOCIsOpen_comboBox->currentIndex() & 0x01) << 1) |
            (((quint8)ui->transEscape_comboBox->currentIndex() & 0x01) << 2) |
            (((quint8)ui->transMultiMode_comboBox->currentIndex() & 0x01) << 3) |
            (((quint8)ui->logo_comboBox->currentIndex() & 0x01) << 4) |
            (((quint8)ui->subBinOpen_comboBox->currentIndex()&0x01) << 6);


    if(ui->userSelectKeyBoard_comboBox->currentIndex())
       temp8 |=  (0x01 << 5);
    else
       temp8 &=  ~(0x01 << 5);


    in<<temp8;

    //logo 字段
    quint8 logoByte = 0;
    if(ui->logo_comboBox->currentIndex() > 0)
    {
        bool ok;
        QSettings *configRW = new QSettings("./config.ini",QSettings::IniFormat);
        logoByte = configRW->value("SYS/logo").toInt(&ok);
        if(!ok)
            logoByte = 1;
        delete configRW;
    }


    in<<logoByte;
    qDebug()<<"logoByte:"<<logoByte;
    //新增 金额比率
    temp8 = (quint8)(ui->comboBox_Scale->currentIndex());
    in<<temp8;


    //纸币循环斗
    QString cycValue = ui->lineEdit_billRecvValue->text();
    if(cycValue.indexOf(".") < 0)
        cycValue += "00";
    else
    {
        QString str = QString("%1")
                .arg(cycValue.split(".").at(1),-2,QLatin1Char('0'));
        cycValue = cycValue.split(".").at(0) + str;
    }
    tempAmount = amountToScale((quint32)(priceFormat(cycValue).toULong(&ok,10)));

    in<<encodAmount(tempAmount);


    //纸币开启上限
    QString billEnableValueStr = ui->lineEdit_BillEnableValue->text();
    if(billEnableValueStr.indexOf(".") < 0)
        billEnableValueStr += "00";
    else
    {
        QString str = QString("%1")
                .arg(billEnableValueStr.split(".").at(1),-2,QLatin1Char('0'));
        billEnableValueStr = billEnableValueStr.split(".").at(0) + str;
    }

    tempAmount = amountToScale((quint32)(priceFormat(billEnableValueStr).toULong(&ok,10)));
    in<<(quint32)tempAmount;


    emit resultSignal(false,processText(SYSSETTING));
    emit systemParaRequestSignal(SYSPARA_REQ,arr);

}
void SysParaDialog::on_recover_pushButton_clicked()
{
    QByteArray arr;

    emit resultSignal(false,processText(SYSGETTING));
    QDataStream in(&arr,QIODevice::WriteOnly);
    in << (quint8)0x00;
    emit systemParaRequestSignal(SYSPARA_REQ,arr);


}


void SysParaDialog::recvSystemParaSlot(QByteArray arr)
{
    quint8 index = 0,aisleType,subAisleType,temp8;
    bool ok; 
    //比例

    temp8 = (quint8)arr.at(44);
    qDebug()<<"system scale:"<<temp8;
    setVMCSystemPara(VMC_SYSTEM_SCALE,temp8);

    //语言版本
    ui->language_comboBox->setCurrentIndex((quint8)arr.at(index++));
    //售货机型号
    ui->VMCSN_lineEdit_2->setText(QString("%1").arg(arr.mid(index,4).toHex().toUInt(&ok,10)));
    index += 4;
    //主控板版本号
    quint8 version1 =  (quint8)arr.at(index++);
    quint8 version2 =  (quint8)arr.at(index++);
    //ui->ARMSN_lineEdit_2->setText(QString("%1%2").arg(version1).arg(version2)); //HEX

    if(!(version1 == 0x01 && version2 == 0x16))//ver v1.1.6
    {
       setVMCSystemPara(VMC_SYSTEM_SCALE,0);
    }

    //售货机类型
    temp8 = (quint8)arr.at(index++);
    if(temp8 == VMC_DRINK)//drink
       ui->VMCType_comboBox->setCurrentIndex(0);
    else if(temp8 == VMC_FOOD)
       ui->VMCType_comboBox->setCurrentIndex(1);
    else if(temp8 == VMC_YITUO)
       ui->VMCType_comboBox->setCurrentIndex(2);
    else
       ui->VMCType_comboBox->setCurrentIndex(3);
    //纸币器
    ui->billValidatorType_comboBox->setCurrentIndex((quint8)arr.at(index++));

    //硬币器
    ui->coinAcceptorType_comboBox->setCurrentIndex((quint8)arr.at(index++));
    //找零器
    ui->coinChangerType_comboBox_2->setCurrentIndex((quint8)arr.at(index++));
    //读卡器类型
    ui->comboBoxReadCardType->setCurrentIndex((quint8)arr.at(index++));

    //纸币器通道面值 + 硬币器通道面值 + 找零器通道面值
    QList <QLineEdit *> channelList;
    channelList <<ui->billValue_lineEdit<<ui->billValue_lineEdit_2
                <<ui->billValue_lineEdit_3<<ui->billValue_lineEdit_4
                <<ui->billValue_lineEdit_5<<ui->billValue_lineEdit_6
                <<ui->billValue_lineEdit_7<<ui->billValue_lineEdit_8

                <<ui->coinValue_lineEdit<<ui->coinValue_lineEdit_2
                <<ui->coinValue_lineEdit_3<<ui->coinValue_lineEdit_4
                <<ui->coinValue_lineEdit_5<<ui->coinValue_lineEdit_6
                <<ui->coinValue_lineEdit_7<<ui->coinValue_lineEdit_8

                <<ui->hopperValue_lineEdit<<ui->hopperValue_lineEdit_2
                <<ui->hopperValue_lineEdit_3;

    for(int i = 0; i < channelList.count(); i++)
    {
       quint32 value = analysisAmount((quint8)arr.at(index++));
       value = amountFromScale(value);

       QString str = QString("%1.%2").arg(value / 100).arg(value % 100,2,10,QLatin1Char('0'));
       channelList.at(i)->setText(str);
    }

    //超时退币
    temp8 = (quint8)arr.at(index++);
    temp8 = (temp8 == 255 ? 9 : temp8/30);
    ui->saleTime_comboBox->setCurrentIndex(temp8);

    //投纸币上限
    quint32 maxValue = arr.mid(index,4).toHex().toULong(&ok,16);
    maxValue = amountFromScale(maxValue);
    ui->MaxValue_lineEdit->setText(QString("%1.%2").arg(maxValue / 100).arg(maxValue % 100,2,10,QLatin1Char('0')));
    index += 4;

    //主柜货到类型
    aisleType = (quint8)arr.at(index++);
    temp8 = (aisleType >> 4) & 0x0F;
    ui->channel_comboBox->setCurrentIndex(temp8);//1弹簧 2 升降机

    temp8 = aisleType & 0x0F;
    ui->channelType_comboBox->setCurrentIndex(temp8);//8 or 10

    //副柜货到类型
    subAisleType = (quint8)arr.at(index++);
    temp8 = (subAisleType >> 4) & 0x0F;
    ui->subchannel_comboBox->setCurrentIndex(temp8);//1弹簧 2 升降机
    temp8 = subAisleType & 0x0F;
    ui->subBinChannelType_comboBox->setCurrentIndex(temp8);

    //PC通信类型
    ui->pcEnable_comboBox->setCurrentIndex((quint8)arr.at(index++));
    //小数点位数
    ui->decimalNum_comboBox->setCurrentIndex((quint8)arr.at(index++));

    bool openFlag;

    //配置表B3
    temp8 = (quint8)arr.at(index++);//保留
    openFlag = temp8 & 0x01;//盒饭柜开启
    ui->LittleCabinet_comboBox->setCurrentIndex(openFlag);
    bool paipaiSelectFlag = (temp8 >> 1) & 0x01;//拍拍
    openFlag = (temp8 >> 2) & 0x01;//盒饭柜手动补货
    ui->comboBoxCVS_MANADD->setCurrentIndex(openFlag);


    // 配置表 B2主柜
    temp8 = (quint8)arr.at(index++);
    openFlag = (temp8 >> 1) & 0x01;
    ui->comboBoxCompressorOpen->setCurrentIndex(openFlag);
    openFlag = (temp8 >> 3) & 0x01;
    ui->comboBoxLightOpen->setCurrentIndex(openFlag);   
    openFlag = (temp8 >> 5) & 0x01;
    ui->comboBoxTempControl->setCurrentIndex(openFlag);

    openFlag = (temp8 >> 0) & 0x01;
    ui->comboBoxSlaveCompressorOpen->setCurrentIndex(openFlag);
    openFlag = (temp8 >> 2) & 0x01;
    ui->comboBoxSlaveLightOpen->setCurrentIndex(openFlag);
    openFlag = (temp8 >> 4) & 0x01;
    ui->comboBoxSlaveTempControl->setCurrentIndex(openFlag);

    //配置表B1
    temp8 = (quint8)arr.at(index++);
    openFlag = temp8 & 0x01;
    ui->cunhuoKou_comboBox->setCurrentIndex(openFlag);
    openFlag = (temp8 >> 1) & 0x01;
    ui->gOCIsOpen_comboBox->setCurrentIndex(openFlag);
    openFlag = (temp8 >> 2) & 0x01;
    ui->transEscape_comboBox->setCurrentIndex(openFlag);
    openFlag = (temp8 >> 3) & 0x01;
    ui->transMultiMode_comboBox->setCurrentIndex(openFlag);
    openFlag = (temp8 >> 4) & 0x01;
    ui->logo_comboBox->setCurrentIndex(openFlag);
    openFlag = (temp8 >> 5) & 0x01;
    if(openFlag)
    {
        if(paipaiSelectFlag)
            ui->userSelectKeyBoard_comboBox->setCurrentIndex(1);
        else
            ui->userSelectKeyBoard_comboBox->setCurrentIndex(2);
    }
    else
        ui->userSelectKeyBoard_comboBox->setCurrentIndex(0);
    openFlag = (temp8 >> 6) & 0x01;
    ui->subBinOpen_comboBox->setCurrentIndex(openFlag);

    temp8 = (quint8)arr.at(index++);//logo
    temp8 = (quint8)arr.at(index++);//scale
    ui->comboBox_Scale->setCurrentIndex(temp8);

    //纸币循环斗面值
    temp8 = (quint8)arr.at(index++);//scale
    quint32 cycvalue = analysisAmount(temp8);
    cycvalue = amountFromScale(cycvalue);
    ui->lineEdit_billRecvValue->setText(QString("%1.%2").arg(cycvalue / 100)
                                        .arg(cycvalue % 100,2,10,QLatin1Char('0')));



    quint32 billEnableVlaue = arr.mid(index,4).toHex().toULong(&ok,16);
    billEnableVlaue = amountFromScale(billEnableVlaue);
    ui->lineEdit_BillEnableValue->setText(QString("%1.%2").arg(billEnableVlaue / 100)
                                          .arg(billEnableVlaue % 100,2,10,QLatin1Char('0')));

    QString message;
    if(arr.size() < 5)
        message = processText(SYSGETFAIL);
    else
        message = processText(SYSGETSUC);
    emit resultSignal(true,message);

}

void SysParaDialog::on_defaultSet_pushButton_clicked()
{
    QString fileName;
    quint8 temp8;
    fileName =  QFileDialog::getOpenFileName(this,tr("Config File"),"./Conf","*.ini");
    if(fileName.isEmpty())
        return;

    QSettings *read = new QSettings(fileName,QSettings::IniFormat);
    bool ok;
    ui->language_comboBox->setCurrentIndex((quint8)read->value("SYSPARA/Language").toInt(&ok));
    ui->coinAcceptorType_comboBox->setCurrentIndex(read->value("SYSPARA/CoinAcceptorType").toInt(&ok));
    ui->coinChangerType_comboBox_2->setCurrentIndex(read->value("SYSPARA/CoinChangerType").toInt(&ok));
    ui->billValidatorType_comboBox->setCurrentIndex(read->value("SYSPARA/BillValidatorType").toInt(&ok));
    ui->comboBoxReadCardType->setCurrentIndex(read->value("SYSPARA/CashlessDeviceType").toInt(&ok));
    ui->decimalNum_comboBox->setCurrentIndex(read->value("SYSPARA/DecimalNum").toInt(&ok));

    //读取logo 值
    int logoType = read->value("SYSPARA/Logo").toInt(&ok);
    if(logoType > 0)
    {
        ui->logo_comboBox->setCurrentIndex(1);
        QSettings *configRW = new QSettings("./config.ini",QSettings::IniFormat);
        configRW->setValue("SYS/logo",logoType);
        delete configRW;
    }
    else
        ui->logo_comboBox->setCurrentIndex(0);

    ui->userSelectKeyBoard_comboBox->setCurrentIndex(read->value("SYSPARA/UserSelectKeyBoard").toInt(&ok));

    temp8 = read->value("SYSPARA/ChannelType").toInt(&ok);
    ui->channelType_comboBox->setCurrentIndex(temp8);
    ui->subBinOpen_comboBox->setCurrentIndex(read->value("SYSPARA/SubBinOpen").toInt(&ok));
    temp8 = read->value("SYSPARA/SubBinChannelType").toInt(&ok);

    ui->subBinChannelType_comboBox->setCurrentIndex(temp8);
    ui->gOCIsOpen_comboBox->setCurrentIndex(read->value("SYSPARA/GOCIsOpen").toInt(&ok));

    temp8 = read->value("SYSPARA/SaleTime").toInt(&ok);
    temp8 = (temp8 == 255 ? 9 : temp8/30);
    ui->saleTime_comboBox->setCurrentIndex(temp8);

    ui->pcEnable_comboBox->setCurrentIndex(read->value("SYSPARA/PcEnable").toInt(&ok));
    ui->cunhuoKou_comboBox->setCurrentIndex(read->value("SYSPARA/CunhuoKou").toInt(&ok));
    ui->transMultiMode_comboBox->setCurrentIndex(read->value("SYSPARA/TransMultiMode").toInt(&ok));
    ui->transEscape_comboBox->setCurrentIndex(read->value("SYSPARA/TransEscape").toInt(&ok));

    //货道还是弹簧
    temp8 = read->value("SYSPARA/Channel").toInt(&ok);
    ui->channel_comboBox->setCurrentIndex(temp8);

    temp8 = read->value("SYSPARA/SubChannel").toInt(&ok);
    ui->subchannel_comboBox->setCurrentIndex(temp8);

    //ui->VMCDebug_comboBox->setCurrentIndex(read->value("SYSPARA/TraceFlag").toInt(&ok));
    ui->comboBoxCompressorOpen->setCurrentIndex(read->value("SYSPARA/CompressorOpen").toInt(&ok));
    ui->comboBoxLightOpen->setCurrentIndex(read->value("SYSPARA/LightOpen").toInt(&ok));


    ui->MaxValue_lineEdit->setText(read->value("SYSPARA/MaxValue").toString());
    ui->billValue_lineEdit->setText(read->value("BillValue/BillValue1").toString());
    ui->billValue_lineEdit_2->setText(read->value("BillValue/BillValue2").toString());
    ui->billValue_lineEdit_3->setText(read->value("BillValue/BillValue3").toString());
    ui->billValue_lineEdit_4->setText(read->value("BillValue/BillValue4").toString());
    ui->billValue_lineEdit_5->setText(read->value("BillValue/BillValue5").toString());
    ui->billValue_lineEdit_6->setText(read->value("BillValue/BillValue6").toString());
    ui->billValue_lineEdit_7->setText(read->value("BillValue/BillValue7").toString());
    ui->billValue_lineEdit_8->setText(read->value("BillValue/BillValue8").toString());

    ui->coinValue_lineEdit->setText(read->value("CoinValue/CoinValue1").toString());
    ui->coinValue_lineEdit_2->setText(read->value("CoinValue/CoinValue2").toString());
    ui->coinValue_lineEdit_3->setText(read->value("CoinValue/CoinValue3").toString());
    ui->coinValue_lineEdit_4->setText(read->value("CoinValue/CoinValue4").toString());
    ui->coinValue_lineEdit_5->setText(read->value("CoinValue/CoinValue5").toString());
    ui->coinValue_lineEdit_6->setText(read->value("CoinValue/CoinValue6").toString());
    ui->coinValue_lineEdit_7->setText(read->value("CoinValue/CoinValue7").toString());
    ui->coinValue_lineEdit_8->setText(read->value("CoinValue/CoinValue8").toString());

    ui->hopperValue_lineEdit->setText(read->value("HopperValue/HopperValue1").toString());
    ui->hopperValue_lineEdit_2->setText(read->value("HopperValue/HopperValue2").toString());
    ui->hopperValue_lineEdit_3->setText(read->value("HopperValue/HopperValue3").toString());


    ui->VMCSN_lineEdit_2->setText(read->value("SYSPARA/VMCSN").toString());
    //ui->ARMSN_lineEdit_2->setText(read->value("SYSPARA/ARMSN").toString());

    temp8 = read->value("SYSPARA/VMCType").toInt(&ok);
    if(temp8 == VMC_DRINK)//drink
       ui->VMCType_comboBox->setCurrentIndex(0);
    else if(temp8 == VMC_FOOD)
       ui->VMCType_comboBox->setCurrentIndex(1);
    else if(temp8 == VMC_YITUO)
       ui->VMCType_comboBox->setCurrentIndex(2);
    else
       ui->VMCType_comboBox->setCurrentIndex(3);


    temp8 = read->value("SYSPARA/boxCabinet").toInt(&ok);
    ui->LittleCabinet_comboBox->setCurrentIndex(temp8);

    temp8 = read->value("SYSPARA/boxManAdd").toInt(&ok);
    ui->comboBoxCVS_MANADD->setCurrentIndex(temp8);

    ui->comboBox_Scale->setCurrentIndex(read->value("SYSPARA/Scale").toInt(&ok));

    ui->lineEdit_billRecvValue->setText(read->value("SYSPARA/Billcycle").toString());

    ui->lineEdit_BillEnableValue->setText(read->value("SYSPARA/BillEnableValue").toString());


    delete read;
   on_save_pushButton_clicked();
}
void SysParaDialog::pcRequestFail()
{
    //ui->status_label->setText(tr("Send Fail"));
}

void SysParaDialog::SystemParaSetResult(bool flag)
{
    //qDebug()<<"SystemParaSetResult"<<flag;
    QString rstStr;
    if(flag)
        rstStr = processText(SYSSETSUC);

    else
        rstStr = processText(SYSSETFAIL);


    emit resultSignal(true,rstStr);
}

void SysParaDialog::dateSetSlot(QByteArray arr)
{    
    QString rstStr;
    if((quint8)arr.at(0) == 0x01)
    {
        rstStr = processText(DATESETSUC);// ui->status_label->setText(tr("Set DateTime suc"));
    }
    else
    {
        rstStr = processText(DATESETFAIL);//ui->status_label->setText(tr("Set DateTime fail"));
    }
    emit resultSignal(true,rstStr);

}

void SysParaDialog::factorySetSlot(QByteArray arr)
{
    QString rstStr;
    if((quint8)arr.at(0) == 0x01)
    {
        rstStr = processText(FACTORYSUC);
       // ui->status_label->setText(tr("Restore the factory successfully"));
    }
    else
    {
        rstStr = processText(FACTORYFAIL);
       // ui->status_label->setText(tr("Failure to restore factory Settings"));
    }
    emit resultSignal(true,rstStr);
}

void SysParaDialog::setFactorySlot(QByteArray arr)
{
    QString rstStr;
    if((quint8)arr.at(0) == 0x01)
    {
        rstStr = processText(FACTORYSUC);
       // ui->status_label->setText(tr("Restore the factory successfully"));
    }
    else
    {
        rstStr = processText(FACTORYFAIL);
       // ui->status_label->setText(tr("Failure to restore factory Settings"));
    }
    emit resultSignal(true,rstStr);
}


void SysParaDialog::on_pushButtonSetDate_clicked()
{

    dateDialog->setDateTime();
    dateDialog->show();
    dateDialog->setModal(true);
}
void SysParaDialog::pushButtonFactoryResetClicked()
{
    //请选择恢复出厂设置 还是设置恢复出厂


    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Reset"));
    msgBox.setText( tr("1.Restore the factory Settings?") + "\n" +
                    tr("2.Set to restore the factory?"));
    msgBox.addButton(tr("Restore"),QMessageBox::ActionRole);
    msgBox.addButton(tr("Set"),QMessageBox::ActionRole);
    msgBox.addButton(tr("Cancel"),QMessageBox::ActionRole);
    int select = msgBox.exec();
    qDebug()<<"pushButtonFactoryResetClicked:"<<select;
    if(select == 0)
    {
        int ret;
        ret = QMessageBox::information(this,tr("Reset"),
                                       tr("Are you sure you want to restore the factory Settings?")
                                       + "\n" + tr("All the configuration will be lost!"),
                                       QMessageBox::Yes,QMessageBox::No);
        if(ret == QMessageBox::No)
        {
             return;
        }
        QByteArray arr;
        emit resultSignal(false,processText(FACTORYSETTING));
        emit systemParaRequestSignal(FACTORRECOVER_REQ,arr);
    }
    else if(select == 1)
    {
        int ret;
        ret = QMessageBox::information(this,tr("Reset"),
                                       tr("Are you sure you want to do?"),
                                       QMessageBox::Yes,QMessageBox::No);
        if(ret == QMessageBox::No)
        {
             return;
        }
        QByteArray arr;
        emit resultSignal(false,processText(FACTORYSETTING));
        emit systemParaRequestSignal(FACTOR_SET_REQ,arr);
    }


}
void SysParaDialog::vmcConnect()
{
    ui->recover_pushButton->setEnabled(true);
    ui->defaultSet_pushButton->setEnabled(true);
    ui->save_pushButton->setEnabled(true);
    ui->pushButtonSetDate->setEnabled(true);
    ui->pushButtonFactoryReset->setEnabled(true);
    on_recover_pushButton_clicked();
}
void SysParaDialog::vmcDisconnect()
{
    ui->recover_pushButton->setEnabled(false);
    ui->defaultSet_pushButton->setEnabled(false);
    ui->save_pushButton->setEnabled(false);
    ui->pushButtonSetDate->setEnabled(false);
    ui->pushButtonFactoryReset->setEnabled(false);
}

void SysParaDialog::superMode(bool flag)
{
    if(flag)
    {
        ui->VMCSN_label_2->show();
        ui->VMCSN_lineEdit_2->show();

        ui->VMCType_comboBox->show();
        ui->VMCType_label_2->show();

        //ui->ARMSN_label_2->show();
        //ui->ARMSN_lineEdit_2->show();

        ui->comboBoxReadCardType->show();
        ui->cashlessDeviceType_label->show();


    }
    else
    {
        ui->VMCSN_label_2->hide();
        ui->VMCSN_lineEdit_2->hide();

        ui->VMCType_comboBox->hide();
        ui->VMCType_label_2->hide();

        //ui->ARMSN_label_2->hide();
        //ui->ARMSN_lineEdit_2->hide();

        ui->comboBoxReadCardType->hide();
        ui->cashlessDeviceType_label->hide();


    }
}



void SysParaDialog::userSelectKeyBoardSlot(int index)
{
    if(index)
    {
        //开启选货按键板
        setVMCSystemPara(VMC_SYSTEM_SELECTBOARD,1);
    }
    else
    {
        //关闭
        setVMCSystemPara(VMC_SYSTEM_SELECTBOARD,0);
    }

    emit systemChangedSignal(SELECT_BOARD,NULL);
}

void SysParaDialog::pcEnableSlot(int index)
{
    if(index)//开启工控机
    {
       setVMCSystemPara(VMC_SYSTEM_PCTYPE,1);
    }
    else
    {
       setVMCSystemPara(VMC_SYSTEM_PCTYPE,0);
    }
    emit systemChangedSignal(PC_ENABLED,NULL);
}


void SysParaDialog::subBinOpenSlot(int index)
{
    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);
    if(index == 1)//open
    {
        ui->groupBox_slaveCabinet->show();
        in<<(quint8)0x01;
    }
    else
    {
        ui->groupBox_slaveCabinet->hide();
        in<<(quint8)0x00;
    }  
    emit systemChangedSignal(CABINET_SUB_OPEN,arr);

}





void SysParaDialog::hefanOpenSlot(int index)
{
    if(index == 0)
    {
        ui->comboBoxCVS_MANADD->hide();
        ui->Label_CVS_MANADD->hide();
    }
    else
    {
       ui->comboBoxCVS_MANADD->show();
       ui->Label_CVS_MANADD->show();
    }


    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);
    in<<(quint8)index;
    emit systemChangedSignal(HEFAN_ENABLED,arr);
}


void SysParaDialog::vmcScaleSlot(int index)
{
    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);
    in<<(quint8)index;
    emit systemChangedSignal(VMC_SCALE,arr);
}


void SysParaDialog::channelTypeSLot(int index)
{
    QComboBox *box = qobject_cast<QComboBox *>(this->sender());
    if(box == ui->channelType_comboBox)
    {
        setAisleType(1,index);
    }
    else if(box == ui->subBinChannelType_comboBox)
    {
        setAisleType(2,index);
    }

    qDebug()<<"channelTypeSLot:"<<index;

}


QString SysParaDialog::priceFormat(const QString &str)
{
    QString tempStr = str;
    if(tempStr.indexOf(".") > 0)
    {  
        return  tempStr.split(".").at(0);

    }
    return tempStr;
}


quint8 SysParaDialog::encodAmount(quint32 amount)
{

    unsigned char i = 0,value;
    if(amount == 0)
        return 0;
    while(!(amount % 10))
    {
        amount = amount / 10;
        i++;
    }
    switch(amount)
    {
        case 1:
            value = 1;
            break;
        case 2:
            value = 2;
            break;
        case 5:
            value = 5;
            break;
        default:
            value = 0;
            break;
    }
    if(value)
    {
        value = (i << 4) | (value & 0x0f);
        return value;
    }
    else
        return 0;
}


quint32 SysParaDialog::analysisAmount(quint8 data)
{

    unsigned int amount;
    unsigned char uint;
    if(data == 0)
        return 0;
    uint =  data >> 4;
    amount = data & 0x0f;
    while(uint)
    {
        amount = amount * 10;
        uint--;
    }
    return amount;
}


QString SysParaDialog::processText(PTEXT pText)
{
    QString text;
    switch(pText)
    {
        case SYSGET:

            break;
        case SYSGETTING:
            text = tr("Getting the system parameters...");
            break;
        case SYSGETSUC:
            text = tr("Succeeded to get system parameters!");
            break;
        case SYSGETFAIL:
            text = tr("Failed to get system parameters!");
            break;
        case SYSSETTING:
            text = tr("Saving the system parameters...");
            break;
        case SYSSETSUC:
            text = tr("Succeeded to set system parameters!");
            break;
        case SYSSETFAIL:
            text = tr("Failed to set system parameters!");
            break;
        case DATESETTING:
            text = tr("Setting DateTime ...");
            break;
        case DATESETSUC:
            text = tr("Succeeded to set dateTime!");
            break;
        case DATESETFAIL:
            text = tr("Failed to set dateTime!");
            break;
        case FACTORYSETTING:
            text = tr("Restoring the factory Settings...");
            break;
        case FACTORYSUC:
            text = tr("Succeeded to Restore the factory Settings!");
            break;
        case FACTORYFAIL:
            text = tr("Failed to Restore the factory Settings!");
            break;
        default:
            text =tr("err!");
            break;
    }

    return text;
}


void SysParaDialog::initLayout()
{
    ui->groupBoxSys->setObjectName("sys");
    ui->groupBoxBill->setObjectName("sys");
    ui->groupBoxCoin->setObjectName("sys");
    ui->groupBoxHopper->setObjectName("sys");
    ui->groupBox_cabinet->setObjectName("sys");
    ui->groupBox_slaveCabinet->setObjectName("sys");


    QFont font = this->font();
    font.setPointSize(10);
    font.setBold(true);
    ui->groupBoxSys->setFont(font);
    ui->groupBoxBill->setFont(font);
    ui->groupBoxCoin->setFont(font);
    ui->groupBoxHopper->setFont(font);
    ui->groupBox_cabinet->setFont(font);
    ui->groupBox_slaveCabinet->setFont(font);

    QPalette palette;
    palette.setColor(QPalette::WindowText,QColor(Qt::blue));
    font.setPointSize(12);
    ui->VMCSN_label_2->setPalette(palette);
    ui->VMCSN_label_2->setFont(font);
    font.setPointSize(10);
    ui->VMCSN_lineEdit_2->setFont(font);


}


void SysParaDialog::lineEditChangedSlot()
{
    qDebug()<<"pressed"<<this->sender();

    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(this->sender());
    if(lineEdit == NULL)
    {
        qDebug()<<"lineEditChangedSlot:warning..object is NULL";
        return;
    }

    QString str = lineEdit->text();
    QRegExp regExp("[0-9]{1,8}[.]{0,1}[0-9]{0,2}");
    if(!regExp.exactMatch(str) || str.isEmpty())
    {
        lineEdit->setText("0.00");
    }
    else
    {
        if(str.indexOf(".") < 0)
        {
            str += ".00";
        }
        else
        {
            QString tempStr = QString("%1")
                    .arg(str.split(".").at(1),-2,QLatin1Char('0'));
            str = str.split(".").at(0) + "." + tempStr;
        }
        lineEdit->setText(str);
    }




}


