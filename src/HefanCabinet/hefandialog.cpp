#include "hefandialog.h"
#include <QtGui>

#include "../aisle/aisletestdialog.h"


HefanDialog::HefanDialog(QWidget *parent)
    :QDialog(parent)
{
    flowFlag = IDLE;
    cabinetNo = 3;
    isAllTest = false;
    isConnected = false;
    isExsit = false;
    pushButtonHefanGET  = new PushButton();
    pushButtonHefanSET = new PushButton();
    pushButtonHefanTest = new PushButton();
    pushButtonHefanTest->setCheckable(true);
    pushButtonHefanAdd = new PushButton();
    pushButtonOneKey = new PushButton();
    pushButtonOneKey->hide();//目前还没有设置
    pushButtonInit = new PushButton();

    comboBoxCabinet = new QComboBox();
    comboBoxCabinet->setObjectName("cabinet");
    comBoxcabinetInit();


    pushButtonOneKey->setPicName(":/button/images/button");
    pushButtonHefanGET->setPicName(":/button/images/button");
    pushButtonHefanSET->setPicName(":/button/images/button");
    pushButtonHefanTest->setPicName(":/button/images/button");
    pushButtonHefanAdd->setPicName(":/button/images/button");
    pushButtonInit->setPicName(":/button/images/button");

    tableWidget = new QTableWidget(this);
    labelTitle = new QLabel();

    QVBoxLayout *layoutVboxTitle = new QVBoxLayout();
    layoutVboxTitle->addWidget(labelTitle);
    layoutVboxTitle->addStretch();
    layoutVboxTitle->setContentsMargins(5,5,5,5);
    layoutVboxTitle->setSpacing(10);

    QVBoxLayout *layoutVbox = new QVBoxLayout();

    layoutVbox->addWidget(pushButtonHefanGET);
    layoutVbox->addWidget(pushButtonHefanSET);
    layoutVbox->addWidget(pushButtonHefanAdd);
    layoutVbox->addWidget(pushButtonHefanTest);
    layoutVbox->addWidget(pushButtonOneKey);
    layoutVbox->addWidget(pushButtonInit);

    layoutVbox->addWidget(comboBoxCabinet,0,Qt::AlignCenter);

    layoutVbox->addStretch();
    layoutVbox->setContentsMargins(5,5,5,5);
    layoutVbox->setSpacing(10);


    QHBoxLayout *layoutHbox = new QHBoxLayout();
    layoutHbox->addLayout(layoutVboxTitle);
    layoutHbox->addWidget(tableWidget);
    layoutHbox->addLayout(layoutVbox);
    layoutHbox->setContentsMargins(5,5,5,5);
    layoutHbox->setSpacing(10);

    this->setLayout(layoutHbox);

    connect(pushButtonHefanGET,SIGNAL(clicked()),
            this,SLOT(pushButtonClicked()));
    connect(pushButtonHefanSET,SIGNAL(clicked()),
            this,SLOT(pushButtonClicked()));
    connect(pushButtonHefanTest,SIGNAL(clicked(bool)),
            this,SLOT(allTestSlot(bool)));
    connect(pushButtonHefanAdd,SIGNAL(clicked()),
            this,SLOT(pushButtonClicked()));
    connect(pushButtonInit,SIGNAL(clicked()),
            this,SLOT(pushButtonClicked()));

    connect(comboBoxCabinet,SIGNAL(currentIndexChanged(int)),this,SLOT(cabinetNoChangedSlot(int)));

    connect(tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(cellChangedSlot(int,int)));

    QFont font = this->font();
    font.setBold(true);
    font.setPointSize(24);
    labelTitle->setFont(font);
    languageTranslation();
    tableInit();

    comboBoxCabinet->setFixedSize(pushButtonHefanAdd->sizeHint() * 1.2);

    //testDialog
    aisleTestdialog = new AisleTestdialog(this);
    aisleTestdialog->hide();
    testTimer = new QTimer(this);
    connect(testTimer,SIGNAL(timeout()),this,SLOT(testTimeroutSLot()));



    signalMapper = new QSignalMapper(this);
    connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(testBoxSlot(QString)));


}

void HefanDialog::languageTranslation()
{
    pushButtonHefanGET->setText(tr("GetBox"));
    pushButtonHefanSET->setText(tr("SetBox"));
    pushButtonHefanTest->setText(tr("TestBox"));
    pushButtonHefanAdd->setText(tr("AddBox"));
    pushButtonOneKey->setText(tr("OnekeyCon"));
    pushButtonInit->setText(tr("Init"));
    labelTitle->setText(tr("V\nC\nS\nm\na\nn\na\ng\ne"));

}


void HefanDialog::tableInit()
{
    QFont headFont = this->font();
    headFont.setPointSize(12);
    headFont.setWeight(50);
    this->setFont(headFont);
    QStringList listHeader;
    listHeader<<tr(" BoxNo ")<<tr(" GoodsPrice ")<<tr(" BoxState ")
            <<tr(" TestBox ");//<<tr(" GoodsID ")

    this->tableWidget->clear();
    this->tableWidget->setColumnCount(listHeader.count());//设置列
    this->tableWidget->setHorizontalHeaderLabels(listHeader);//设置列头
    this->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems); //选中的方式
    this->tableWidget->horizontalHeader()->setVisible(true); //隐藏行表
    this->tableWidget->verticalHeader()->setVisible(false);

    for(int i = 0;i < this->tableWidget->columnCount();i++)
    {
        QTableWidgetItem *item = this->tableWidget->horizontalHeaderItem(i);
        item->setFont(headFont); //设置字体
        item->setBackgroundColor(QColor(0,60,10)); //设置单元格背景颜色
        item->setTextColor(QColor(200,111,30)); //设置文字颜色

    }
    //还可以将行和列的大小设为与内容相匹配
    this->tableWidget->resizeColumnsToContents();
    this->tableWidget->clearContents();
    this->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条



}


void HefanDialog::comBoxcabinetInit()
{
    QFont font = this->font();
    font.setPointSize(12);
    font.setBold(true);
    comboBoxCabinet->setFont(font);
    for(int i = 0;i < 7;i++)
    {
        comboBoxCabinet->insertItem(i,QString("Bin %1").arg(i + 3,2,10,QLatin1Char('0')));
    }
    comboBoxCabinet->setCurrentIndex(0);

}

void HefanDialog::paintEvent(QPaintEvent *event)
{
    event = event;
}


QString HefanDialog::processinfoText(PROCESSTEXT text)
{
    QString textStr;
    bool flag = false;
    switch(text)
    {


        case INITING:
            textStr = tr("Initializing the column...");
            break;
        case INITFAIL:
            textStr = tr("Failed to initialize the information of the column!");
            flag = true;
            break;
        case INITSUC:
            textStr = tr("Succeeded to initialize the information of the column!");
            flag = true;
            break;
        case SETTING:
            textStr = tr("Configuring the information of the column...");
            break;
        case SETFAIL:
            textStr = tr("Failed to configure the information of the column!");
            flag = true;
            break;
        case SAVING:
            textStr = tr("Saving information of the column...");
            break;
        case SAVEEFAIL:
            textStr = tr("Failed to save the column...");
            break;
        case SAVESUC:
            textStr = tr("Succeeded to save the column...");
            break;

        case GETTING:
            textStr = tr("Getting column information...");
            break;
        case GETFAIL:
            textStr = tr("Failed to get information of the column");
            flag = true;
            break;
        case GETSUC:
            textStr = tr("Succeeded to get information of the column");
            flag = true;
            break;
        case SETSUC:
            textStr = tr("Succeeded to set the commodity information!");
            flag = true;
            break;
        case ADDING:
            textStr = tr("Being added goods...");
            break;
        case ADDSUC:
            textStr = tr("Succeeded to add goods!");
            flag = true;
            break;
        case ADDFAIL:
            textStr = tr("Failed to add goods!");
            flag = true;
        break;
        case CABINET_N_A:
            textStr = tr("Cabinet is not exist!");
            flag = true;
            break;
        default:
            textStr = tr("ERR ERR ERR!");
            flag = true;
            break;
    }
    emit processTextSignal(flag,textStr);
    return textStr;
}

//盒饭柜返回结果
void HefanDialog::huodaoRptSlot(const QByteArray &arr)
{
    quint8 type = (quint8)arr.at(0);
    type = type;
    QByteArray recvArr = arr;
    recvArr.remove(0,1);


    qDebug()<<"huodaoRptSlot:"<<this->flowFlag;
    if(this->flowFlag == GETBOX)
    {
        getBoxRpt(recvArr);
    }
    else if(this->flowFlag == SETBOX)
    {
        setBoxRpt(recvArr);
    }
    else if(this->flowFlag == TESTBOX)
    {
        this->pcTestAisleRpt(recvArr);
    }
    else if(this->flowFlag == ADDBOX)
        this->addBoxRpt(recvArr);
    else if(this->flowFlag == INITBOX)
    {
        this->initBoxRpt(recvArr);
    }
    else if(this->flowFlag == SAVEBOX)
    {
        this->saveBoxRpt(recvArr);
    }


}


void HefanDialog::pushButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(this->sender());
    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);

    if(button == pushButtonOneKey)
    {
        //setBoxRequest(0);
    }
    else if(button == pushButtonHefanGET)//获取盒饭柜
    {
        getBoxRequest(1);
    }
    else if(button == pushButtonHefanSET)//同步信息
    {  
        if(!isExsit)
        {
            return;
        }

        int ret;
        ret = QMessageBox::information(this,tr("GoodsInfo"),
                                       textTips(SET),
                                       QMessageBox::Yes,QMessageBox::No);
        if(ret == QMessageBox::No)
            return;
        setBoxRequest(1);
    }
    else if(button == pushButtonHefanTest)
    {
       if(!isExsit)
       {
            return;
       }
       in<<(quint8)TEST_HUODAO_REQ;
       in<<(quint8)0x05;
       in<<(quint8)0x09;

       emit pcRequestSignal(HUODAO_REQ,arr);
    }
    else if(button == pushButtonHefanAdd)
    {
        if(!isExsit)
        {
            return;
        }
        int ret = QMessageBox::information(this,tr("GoodsInfo"),
                                   textTips(ADD_TIP),
                                   QMessageBox::Yes,QMessageBox::No);
        if(ret == QMessageBox::No)
            return;
       in<<(quint8)ADD_HUODAO_REQ;
       in<<(quint8)this->cabinetNo;
       in<<(quint8)0x00;
       in<<(quint8)0x00;
       this->flowFlag = ADDBOX;
       processinfoText(ADDING);
       emit pcRequestSignal(HUODAO_REQ,arr);
    }
    else if(button == pushButtonInit)//初始化
    {
        int ret = QMessageBox::information(this,tr("GoodsInfo"),
                                       textTips(INIT),
                                       QMessageBox::Yes,QMessageBox::No);
        if(ret == QMessageBox::No)
            return;

        in<<(quint8)INIT_HUODAO_REQ;
        in<<(quint8)this->cabinetNo;
        this->flowFlag = INITBOX;
        processinfoText(INITING);
        emit pcRequestSignal(HUODAO_REQ,arr);

    }



}


void HefanDialog::addBoxRpt(QByteArray arr)
{

    qDebug()<<"addBoxRpt:"<<arr.toHex();
    if(arr.size() < 3)
    {
        processinfoText(ADDFAIL);
        return;
    }
    //解析柜号
    quint8 cabinet = (quint8)arr.at(0);
    cabinet = cabinet;
    //rst
    quint8 rst = (quint8)arr.at(2);

    if(rst == VMC_OK)
    {
        processinfoText(ADDSUC);
        getBoxRequest(1);
    }
    else
         processinfoText(ADDFAIL);


}

void HefanDialog::getBoxRequest(quint8 levelNo)
{
    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);
    quint8 cabinet = this->cabinetNo;
    this->levelNo = levelNo;
    this->flowFlag = GETBOX;
    tableWidget->clearContents();

    in<<(quint8)GET_HUODAO_REQ;
    in<<(quint8)cabinet;
    in<<(quint8)0x01;
    in<<(quint8)levelNo;

    processinfoText(GETTING);
    emit pcRequestSignal(HUODAO_REQ,arr);

}


void HefanDialog::getBoxRpt(QByteArray arr)
{
    quint8 index = 0;
    quint8 cabinet = (quint8)arr.at(index++);
    quint8 getType = (quint8)arr.at(index++);
    getType = getType;
    quint8 no = (quint8)arr.at(index++);
    quint32 value = 0;
    bool ok;


    if(!no || no > 5)
    {
        processinfoText(GETFAIL);
        return;
    }

    for(int i = 0;i < 20 ;i++,index += 5)
    {
        quint8 boxNo = (no - 1)* 20 + i + 1;
        value = (quint16)arr.mid(index,2).toHex().toULong(&ok,16);//单价
        value = amountFromScale(value);
        setHefanHuodaoByIndex(cabinet,boxNo,value,HEFAN_HUODAO_PRICE);
        value = (quint8)arr.at(index + 2);//状态  根据状态是否是零 判断 最后一个格子号

        if(no == 1 && i == 0)//first level and first column is not exist
        {
            if(value == 0)
            {
                processinfoText(CABINET_N_A);
                isExsit = false;
                return ;
            }
            else
                isExsit = true;
        }
        else
        {
            if(value == 0)
            {
                setHefanHuodaoByIndex(cabinet,boxNo -1,boxNo - 1,HEFAN_BOX_NUMS);
                qDebug()<<"The last box,max Box Num="<<boxNo;
                this->levelNo = 5;
                break;
            }
        }

        setHefanHuodaoByIndex(cabinet,boxNo,value,HEFAN_HUODAO_STATE);

        value = (quint8)arr.at(index + 3);//销售量
        setHefanHuodaoByIndex(cabinet,boxNo,value,HEFAN_HUODAO_TRADE_NUM);

        value = (quint8)arr.at(index + 4);//ID
        setHefanHuodaoByIndex(cabinet,boxNo,value,HEFAN_HUODAO_ID);
    }


    qDebug()<<"getBoxRpt:"<<levelNo;


    if(this->levelNo >= 5)
    {
        processinfoText(GETSUC);
        tableGoodsUpdate();
    }
    else
    {
       getBoxRequest(levelNo + 1);
    }

}

void HefanDialog::setBoxRequest(quint8 levelNo)
{

    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);

    this->levelNo = levelNo;

    quint8 cabinet = this->cabinetNo;
    if(this->levelNo == 0)//初始化
    {
        in<<(quint8)INIT_HUODAO_REQ;
        in<<(quint8)cabinet;
        processinfoText(INITING);
    }
    else if(this->levelNo > 5)//保存
    {
        this->flowFlag = SAVEBOX;
        in<<(quint8)WRITE_HUODAO_REQ;
        in<<(quint8)cabinet;
        processinfoText(SAVING);

    }
    else
    {


        this->flowFlag = SETBOX;
        in<<(quint8)SET_HUODAO_REQ;
        in<<(quint8)cabinet;
        in<<(quint8)0x01;
        in<<(quint8)this->levelNo;//1level
        for(int i = 0;i < 20;i++)
        {
            quint8 boxNo = (this->levelNo - 1) * 20 + i + 1;
            quint32 value = getHefanHuodaoByIndex(cabinet,boxNo,HEFAN_HUODAO_PRICE);
            value = amountToScale(value);
            in<<(quint16)(value);

            value = getHefanHuodaoByIndex(cabinet,boxNo,HEFAN_HUODAO_STATE);
            in<<(quint8)value;

            value = getHefanHuodaoByIndex(cabinet,boxNo,HEFAN_HUODAO_TRADE_NUM);
            in<<(quint8)value;

            value = getHefanHuodaoByIndex(cabinet,boxNo,HEFAN_HUODAO_ID);
            in<<(quint8)value;
        }
        processinfoText(SETTING);
    }

    emit pcRequestSignal(HUODAO_REQ,arr);
}


void HefanDialog::setBoxRpt(QByteArray arr)
{
    arr = arr;
    if(this->levelNo > 5) //保存结果
    {
        processinfoText(SETSUC);
    }
    else
    {
        setBoxRequest(this->levelNo + 1);
    }
}


void HefanDialog::saveBoxRpt(QByteArray arr)
{
    //解析柜号
    quint8 cabinet = (quint8)arr.at(0);
    cabinet = cabinet;
    //rst
    quint8 rst = (quint8)arr.at(1);
    if(rst == VMC_OK)
    {
        getBoxRequest(1);
        processinfoText(SETSUC);
    }
    else
    {
        processinfoText(SETFAIL);
    }


}

void HefanDialog::initBoxRpt(QByteArray arr)
{
    //解析柜号
    quint8 cabinet = (quint8)arr.at(0);
    cabinet = cabinet;
    //rst
    quint8 rst = (quint8)arr.at(1);
    qDebug()<<"initBoxRpt:"<<rst<<arr.toHex();
    if(rst == VMC_OK)
    {
        processinfoText(INITSUC);
        getBoxRequest(1);
    }
    else
    {
        processinfoText(INITFAIL);
    }

}

void HefanDialog::tableGoodsUpdate()
{
    quint8 cabinetNo = this->cabinetNo;
    quint8 boxNums = getHefanHuodaoByIndex(cabinetNo,88,HEFAN_BOX_NUMS);
    qDebug()<<"boxNums="<<boxNums;

    quint8 rowIndex = 0;
    quint32 value;
    tableWidget->clearContents();
    tableWidget->setRowCount(boxNums);//设置行


    QFont font = this->font();
    font.setPointSize(12);
    font.setBold(false);

    for(quint8 i = 0;i < boxNums;i++)
    {
        rowIndex = i;

        //boxNo
        QString tempStr= QString("%1").arg(i + 1, 2, 10, QLatin1Char('0'));
        tableWidget->setItem(rowIndex, 0, new QTableWidgetItem(tempStr));


        //单价
        value = getHefanHuodaoByIndex(cabinetNo,i + 1,HEFAN_HUODAO_PRICE);
        tempStr = QString("%1.%2").arg(value / 100).arg(value % 100,2,10,QLatin1Char('0'));
        tableWidget->setItem(rowIndex,1,new QTableWidgetItem(tempStr));

        //状态
        QColor color;
        value = getHefanHuodaoByIndex(cabinetNo,i + 1,HEFAN_HUODAO_STATE);
        switch(value)
        {
            case HEFAN_HUODAO_NORMAL:
                 tempStr = tr("Normal");
                break;
            case HEFAN_HUODAO_FAULT:
                color.setRed(240);
                tempStr = tr("Fault");
                break;
             case HEFAN_HUODAO_EMPTY:
                color.setRgb(255, 0, 255);
                tempStr = tr("Without");
                break;
             case HEFAN_HUODAO_CLOSE:
                tempStr = tr("Disabled");
                break;
             default:
                tempStr = tr("Unknown");
                    break;
        }
        tableWidget->setItem(rowIndex,2,new QTableWidgetItem(tempStr));
        this->tableWidget->item(rowIndex,2)->setTextColor(color);

        //test
        QPushButton *button  = new  QPushButton();
        button->setText(tr("Test"));
        button->setFont(font);
        connect(button,SIGNAL(clicked()),signalMapper,SLOT(map()));
        signalMapper->setMapping(button,QString("%1%2")
                                 .arg(rowIndex,2,10,QLatin1Char('0'))
                                 .arg(rowIndex,2,10,QLatin1Char('0')));


        this->tableWidget->setCellWidget(rowIndex,3,button);


        for(int n = 0; n < 3;n++)
        {
            tableWidget->item(rowIndex, n)->setFont(font);
            tableWidget->item(rowIndex, n)->setTextAlignment(Qt::AlignRight | Qt::AlignBottom);
            if(n != 1)
                tableWidget->item(rowIndex, n)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        }



     }

}


void HefanDialog::tableReadInfo()
{
#if 0
    quint8 cabinetNo = this->cabinetNo;
    initHefanHuodao(cabinetNo);

    bool ok;
    QString tempStr;
    for(int i = 0;i < this->tableWidget->rowCount();i++)
    {
           tempStr = this->tableWidget->item(i,1)->text().replace(".","");
           setHefanHuodaoByIndex(cabinetNo,i + 1,tempStr.toULong(&ok,10),HEFAN_HUODAO_PRICE);
           tempStr = this->tableWidget->item(i,2)->text();
           setHefanHuodaoByIndex(cabinetNo,i + 1,tempStr.toInt(&ok,10),HEFAN_HUODAO_STATE);
    }
#endif

}


void HefanDialog::cabinetNoChangedSlot(int no)
{
    qDebug()<<"cabinetNoChangedSlot:"<<no;
    this->cabinetNo = (no + 3) % 10;

    if(isConnected)
        getBoxRequest(1);
}


void HefanDialog::cellChangedSlot(int row, int column)
{
    //qDebug()<<"cellChanged"<<row<<column;

 #if 1
    QTableWidgetItem *item =  tableWidget->item(row,column);
    if(item == NULL)
        return;

    if(column == 1)//price
    {
        QString str = item->text();
        QRegExp regExp("[0-9]{1,8}[.]{0,1}[0-9]{0,2}");
        if(!regExp.exactMatch(str))
        {
            str = "1.00";
            item->setText(str);
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
            item->setText(str);
        }
        bool ok;
        quint32 value = str.replace(".","").toUInt(&ok,10);
        setHefanHuodaoByIndex(this->cabinetNo,row + 1,value,HEFAN_HUODAO_PRICE);
    }

#endif

}

void HefanDialog::testBoxSlot(QString row_no)
{

    bool ok;
    int row = row_no.left(2).toInt(&ok,10);
    quint8 logicNo = row_no.right(2).toInt(&ok,10) + 1;

    qDebug()<<"logicNo"<<logicNo<<row_no;

    tableWidget->selectRow(row);

    QSize size  = this->size();
    QPoint point = this->pos();
    if(row < tableWidget->rowCount())
    {
        size = tableWidget->cellWidget(row,3)->size();
        point = tableWidget->cellWidget(row,3)->pos();
    }

    point = tableWidget->mapToGlobal(point);
    point.setX(point.x() + size.width());
    point.setY(point.y() + size.height());
    aisleTestdialog->move(point);

    aisleTestdialog->setText(QString("%1%2%3").arg(tr("Testting the column:")).arg(logicNo)
                             .arg(tr("...")));
    aisleTestdialog->show();
    for(int i = 0;i < tableWidget->rowCount();i++)
    {
       tableWidget->cellWidget(i,3)->setEnabled(false);
    }
    setButtonEnable(false);
    QByteArray sendArr;
    QDataStream in(&sendArr,QIODevice::WriteOnly);
    in<<(quint8)TEST_HUODAO_REQ;
    in<<(quint8)this->cabinetNo;
    in<<(quint8)logicNo;
    this->flowFlag = TESTBOX;
    emit pcRequestSignal(HUODAO_REQ,sendArr);
    if(!isAllTest)
    {
        pushButtonHefanTest->setEnabled(false);
    }

}


void HefanDialog::setConnect(const bool flag)
{
    pushButtonHefanGET->setEnabled(flag);
    pushButtonHefanSET->setEnabled(flag);
    pushButtonHefanAdd->setEnabled(flag);
    pushButtonOneKey->setEnabled(flag);
    pushButtonInit->setEnabled(flag);
    pushButtonHefanTest->setEnabled(flag);

    isConnected = flag;

}

void HefanDialog::setButtonEnable(const bool flag)
{

    pushButtonHefanGET->setEnabled(flag);
    pushButtonHefanSET->setEnabled(flag);
    pushButtonHefanAdd->setEnabled(flag);
    pushButtonOneKey->setEnabled(flag);
    pushButtonInit->setEnabled(flag);

    comboBoxCabinet->setEnabled(flag);



}

void HefanDialog::pcTestAisleRpt(const QByteArray &arr)
{
    quint8 index = 0;
    quint8 cabinet = (quint8)arr.at(index++);
    quint8 logicNo = (quint8)arr.at(index++);
    quint8 rst = (quint8)arr.at(index++);
    cabinet =cabinet;
    logicNo = logicNo;
    QString str;
    switch(rst)
    {
        case 0:
            str = tr("Normal");
            break;
        case 1:
            str = tr("Fault");
            break;
        case 2:
            str = tr("Not exist");
            break;
        case 3:
            str = tr("Motor fault");
            break;
        case 4:
            str = tr("Dispensing failure");
            break;
        case 5:
            str = tr("Communication fault");
            break;
        default:
            str = tr("Unknown");
            break;
    }

    QString text = QString("%1%2:%3").arg(tr("Column")).arg(logicNo).arg(str);
    aisleTestdialog->setText(text);

    testTimer->start(1500);

}


void HefanDialog::testTimeroutSLot()
{
    testTimer->stop();
    aisleTestdialog->setText("");
    if(isAllTest)
    {
        testAisleNo++;
        if(testAisleNo >= tableWidget->rowCount() || stoppedAllTest)
        {
            isAllTest = false;
            stoppedAllTest = true;
            aisleTestdialog->hide();
            for(int i = 0;i < tableWidget->rowCount();i++)
            {
               tableWidget->cellWidget(i,3)->setEnabled(true);
            }
            setButtonEnable(true);
            pushButtonHefanTest->setText(tr("AllTest"));
            pushButtonHefanTest->setChecked(false);
        }
        else
        {
            bool ok;
            quint8 boxNo = tableWidget->item(testAisleNo,0)->text().toUInt(&ok,10);
            QString str = QString("%1%2")
                    .arg(testAisleNo,2,10,QLatin1Char('0'))
                    .arg(boxNo - 1,2,10,QLatin1Char('0'));
            testBoxSlot(str);
        }

    }
    else
    {
        aisleTestdialog->hide();
        pushButtonHefanTest->setEnabled(true);
        for(int i = 0;i < tableWidget->rowCount();i++)
        {
           tableWidget->cellWidget(i,3)->setEnabled(true);
        }
        setButtonEnable(true);
    }
}

void HefanDialog::allTestSlot(bool checked)
{

    if(checked)
    {
        if(!isExsit)
        {
            pushButtonHefanTest->setChecked(false);
            return;
        }

        int ret = QMessageBox::information(this,tr("AllTest"),
                                 tr("Are you sure you want to test all the column?"),
                                           QMessageBox::Yes,QMessageBox::No);
        if(ret == QMessageBox::No)
        {
            pushButtonHefanTest->setChecked(false);
            return;
        }

        if(tableWidget->rowCount() <= 0)
        {
            pushButtonHefanTest->setChecked(false);
            return;
        }

        pushButtonHefanTest->setText(tr("StopTest"));
        isAllTest = true;
        stoppedAllTest = false;
        testAisleNo = 0;
        QString str = QString("%1%2")
                .arg(0,2,10,QLatin1Char('0'))
                .arg(0,2,QLatin1Char('0'));
        testBoxSlot(str);
    }
    else
    {
        pushButtonHefanTest->setText(tr("AllTest"));
        stoppedAllTest = true;
        isAllTest = false;
    }


}


QString HefanDialog::textTips(PROCESSTEXT pText)
{
    QString text;
    switch(pText)
    {
        case SET:
            text = tr("Are you sure want to configuration the commodity information?");
            break;
        case SAVING:
            text = tr("Saving the commodity information...");
            break;
        case SETFAIL:
            text = tr("Failed to set the commodity information!");
            break;
        case SETSUC:
            text = tr("Succeeded to set the commodity information!");
            break;
        case GETTING:
            text = tr("Getting the commodity information...");
            break;
        case GETSUC:
            text = tr("Succeeded to get the commodity information!");
            break;
        case ADDING:
            text = tr("Being added goods...");
            break;
        case ADDSUC:
            text = tr("Succeeded to add goods!");
            break;
        case ADDFAIL:
            text = tr("Failed to add goods!");
            break;
        case INIT:
            text = tr("Are you sure want to initialize the column?");
            break;
        case ADD_TIP:
            text = tr("Are you sure want to add the goods?");
            break;
        default:
            text =tr("err!");
            break;
    }

    return text;
}
