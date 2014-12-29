#include "aislemanagementdialog.h"
#include "ui_aislemanagementdialog.h"
#include <QtGui>


AisleManagementDialog::AisleManagementDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AisleManagementDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->pushButtonGetInfo->setPicName(":/button/images/button");
    ui->pushButtonSaveGoods->setPicName(":/button/images/button");
    ui->pushButtonAddGoods->setPicName(":/button/images/button");
    ui->pushButtonAllTest->setPicName(":/button/images/button");
#if 1
    signalMapper = new QSignalMapper(this);
    connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(testAisleSlot(QString)));

    addGoodsDialog = new AddGoodsDialog(this);
    initGoodsInfo();
    translate();
    //testDialog
    aisleTestdialog = new AisleTestdialog(this);
    aisleTestdialog->hide();
    testTimer = new QTimer(this);
    connect(testTimer,SIGNAL(timeout()),this,SLOT(testTimeroutSLot()));

    //alltest
    isAllTest = false;


    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),
            this,SLOT(cellDoubleClickedSlot(int,int)));
    connect(ui->tableWidget->horizontalHeader(),
            SIGNAL(sectionClicked(int)),this,SLOT(goodsInfoSort(int)));

    connect(ui->pushButtonSaveGoods,SIGNAL(clicked()),this,SLOT(pcConGoodsReq()));
    connect(ui->pushButtonAddGoods,SIGNAL(clicked()),this->addGoodsDialog,SLOT(show()));
    connect(ui->pushButtonGetInfo,SIGNAL(clicked()),this,SIGNAL(getGoodsInfoSignal()));
    connect(addGoodsDialog,SIGNAL(addGoodsSignal(QByteArray)),
            this,SLOT(pcAddGoodsReq(QByteArray)));
    connect(ui->pushButtonAllTest,SIGNAL(clicked(bool)),
            this,SLOT(allTestSlot(bool)));


    setVMCState(false);

    QFont font = this->font();
    font.setBold(true);
    font.setPointSize(11);
    ui->comboBox->setFont(font);
    this->showCabinet(false);
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox->setObjectName("cabinet");
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),
            this,SIGNAL(vmcCabinetNoChangedSignal(int)));


#endif
}

AisleManagementDialog::~AisleManagementDialog()
{
    delete ui;

}


QString AisleManagementDialog::processText(PTEXT pText)
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
        case CONSELECTTING:
            text = tr("Setting the Choose goods number keys");
            break;
        default:
            text =tr("err!");
            break;
    }

    return text;
}

QString AisleManagementDialog::processText(PTEXT pText, quint8 levelNo)
{
    QString text;
    switch(pText)
    {
        case SETTING:
            text = QString("%1%2%3")
                    .arg(tr("Setting "))
                    .arg(levelNo).arg(tr(" goods information..."));
            break;
        case GETTING:
            text = QString("%1%2%3")
                .arg(tr("Getting "))
                .arg(levelNo).arg(tr(" goods information..."));
            break;
        default:
            break;
    }


    return text;
}

void AisleManagementDialog::translate()
{
    ui->retranslateUi(this);
    ui->pushButtonGetInfo->setText(tr("GetInfo"));
    ui->pushButtonSaveGoods->setText(tr("SaveInfo"));
    ui->pushButtonAddGoods->setText(tr("AddGoods"));
    ui->pushButtonAllTest->setText(tr("AllTest"));
    addGoodsDialog->translateLanguage();
}



void AisleManagementDialog::cabinetCurindexChanged(int no)
{
    ui->comboBox->setCurrentIndex(no);
}



void AisleManagementDialog::cellDoubleClickedSlot(int row, int column)
{
    qDebug()<<"cellDoubleClickedSlot"<<row<<column;
}

void AisleManagementDialog::showCabinet(const bool isShow)
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
void AisleManagementDialog::upGoodsDialog(quint8 levelNo)
{

    if(levelNo < 1 || levelNo > 8)
    {
        print_warning(levelNo);
        return;
    }

    QFont font = this->font();//("Helvetica",12,50);
    font.setPointSize(12);
    font.setWeight(50);

    if(levelNo == 1)
    {
        disconnect(ui->tableWidget,SIGNAL(cellChanged(int,int)),
                this,SLOT(cellChangedSlot(int,int)));
        disconnect(ui->tableWidget,SIGNAL(cellClicked(int,int)),
                this,SLOT(cellClickedSlot(int,int)));
        setButtonEnable(true);
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(80);//设置行
        rowIndex = 0;
    }


    if(ui->tableWidget->rowCount() != 80 || rowIndex > 80)
    {
        print_warning(ui->tableWidget->rowCount());
        print_warning(rowIndex);
        return;
    }
    //setStateText(processText(GETTING,levelNo));

    quint8 i = levelNo - 1;

    for(int j = 0;j < 10;j++)
    {
        if(!leveAisleST[i].aisle[j].isOpen)
        {
            continue;
        }
        //logicNo
        QString tempStr = QString("%1").arg(leveAisleST[i].aisle[j].logicNo);
        ui->tableWidget->setItem(rowIndex,goods_table_st.aisle_no,new QTableWidgetItem(tempStr));
        ui->tableWidget->item(rowIndex, goods_table_st.aisle_no)->setTextAlignment(Qt::AlignRight | Qt::AlignBottom);
        ui->tableWidget->item(rowIndex, goods_table_st.aisle_no)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->item(rowIndex, goods_table_st.aisle_no)->setFont(font);
        //id
        if(getVMCSystemPara(VMC_SYSTEM_PCTYPE))
        {
            tempStr = QString("%1").arg(leveAisleST[i].aisle[j].id,2,16,QLatin1Char('0'));
            ui->tableWidget->setItem(rowIndex,this->goods_table_st.id,new QTableWidgetItem(tempStr.toUpper()));
            ui->tableWidget->item(rowIndex, this->goods_table_st.id)->setTextAlignment(Qt::AlignRight | Qt::AlignBottom);
            ui->tableWidget->item(rowIndex, this->goods_table_st.id)->setFont(font);
        }

        //keyNo
        if(getVMCSystemPara(VMC_SYSTEM_SELECTBOARD))
        {
            tempStr = QString("%1").arg(leveAisleST[i].aisle[j].selectNo,2,10,QLatin1Char('0'));
            ui->tableWidget->setItem(rowIndex,this->goods_table_st.key_no,new QTableWidgetItem(tempStr));
            ui->tableWidget->item(rowIndex, this->goods_table_st.key_no)->setTextAlignment(Qt::AlignRight | Qt::AlignBottom);
            ui->tableWidget->item(rowIndex, this->goods_table_st.key_no)->setFont(font);
        }

        //price
        tempStr = QString("%1.%2")
                .arg(leveAisleST[i].aisle[j].price / 100)
                .arg(leveAisleST[i].aisle[j].price % 100,2,10,QLatin1Char('0'));
        ui->tableWidget->setItem(rowIndex,this->goods_table_st.price,new QTableWidgetItem(tempStr));
        ui->tableWidget->item(rowIndex, this->goods_table_st.price)->setTextAlignment(Qt::AlignRight | Qt::AlignBottom);
        ui->tableWidget->item(rowIndex, this->goods_table_st.price)->setFont(font);
        //remain
        tempStr = QString("%1").arg(leveAisleST[i].aisle[j].remain);
        ui->tableWidget->setItem(rowIndex,this->goods_table_st.remain,new QTableWidgetItem(tempStr));
        ui->tableWidget->item(rowIndex, this->goods_table_st.remain)->setTextAlignment(Qt::AlignRight | Qt::AlignBottom);
       // ui->tableWidget->item(rowIndex, this->goods_table_st.remain)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->item(rowIndex, this->goods_table_st.remain)->setFont(font);
        //maxcount

        tempStr = QString("%1").arg(leveAisleST[i].aisle[j].maxCount);
        ui->tableWidget->setItem(rowIndex,this->goods_table_st.max_count,new QTableWidgetItem(tempStr));
        ui->tableWidget->item(rowIndex, this->goods_table_st.max_count)->setTextAlignment(Qt::AlignRight | Qt::AlignBottom);
        ui->tableWidget->item(rowIndex, this->goods_table_st.max_count)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->item(rowIndex, this->goods_table_st.max_count)->setFont(font);

        //state
        quint8 temp8 = leveAisleST[i].aisle[j].status;
        QColor color(Qt::black);
        if(temp8 == 1)//normal
        {
            tempStr = tr("Normal");
        }
        else if(temp8 == 2)//fault
        {
            color.setRed(240);
            tempStr = tr("Fault");
        }
        else if(temp8 == 3)//without
        {
            color.setRgb(255, 0, 255);
            tempStr = tr("Without");
        }
        else if(temp8 == 4)//暂不可用
        {
            color.setRgb(116, 116, 173);
            tempStr = tr("Disabled");
        }
        else
        {
            tempStr = tr("Unknown");
        }
        ui->tableWidget->setItem(rowIndex,this->goods_table_st.state,new QTableWidgetItem(tempStr));
        ui->tableWidget->item(rowIndex, this->goods_table_st.state)->setTextAlignment(Qt::AlignRight | Qt::AlignBottom);
        ui->tableWidget->item(rowIndex, this->goods_table_st.state)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->item(rowIndex, this->goods_table_st.state)->setFont(font);
        ui->tableWidget->item(rowIndex, this->goods_table_st.state)->setTextColor(color);

        //test
        QPushButton *button  = new  QPushButton();
        button->setFont(font);
        button->setText(tr("Test"));
        connect(button,SIGNAL(clicked()),signalMapper,SLOT(map()));
        signalMapper->setMapping(button,QString("%1%2")
                                 .arg(rowIndex,2,10,QLatin1Char('0'))
                                 .arg(leveAisleST[i].aisle[j].logicNo,2,10,QLatin1Char('0')));
        ui->tableWidget->setCellWidget(rowIndex,this->goods_table_st.test,button);

        rowIndex++;

    }

    if(levelNo == 8)
    {
        ui->tableWidget->setRowCount(rowIndex);//重新定义行
        connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),
                this,SLOT(cellChangedSlot(int,int)));
        connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),
                this,SLOT(cellClickedSlot(int,int)));
        //qDebug()<<"upGoodsDialog"<<ui->tableWidget->rowCount();
        //setStateText(processText(GETSUC));
    }
}


void AisleManagementDialog::initGoodsInfo()
{

    //QFont font("Helvetica",15,50);
    QFont headFont = this->font();//("Helvetica",12,50);
   // headFont.setFamily("Helvetica");
    headFont.setPointSize(12);
    headFont.setWeight(50);
    this->setFont(headFont);
    QStringList listHeader,listHeaderTip;
    listHeader<<tr("ColumnNo")<<tr("GoodsPrice")<<tr("Remain")
            <<tr("Store")<<tr("ColumnState")<<tr("TestColumn");

    listHeaderTip<<tr("The number of column   ")
                   <<tr("The price of goods   ")
                     <<tr("Goods remaining   ")
                       <<tr("The biggest reserves of goods   ")
                         <<tr("The state of the column   ")
                           <<tr("Test the column   ");
    //<<tr("GoodsID")
    //开启了工控机 和开启选货按键板 2个共四种情况
    //1 开 ID 开 KEY
    if(getVMCSystemPara(VMC_SYSTEM_PCTYPE))
    {
        if(getVMCSystemPara(VMC_SYSTEM_SELECTBOARD))
        {
            listHeader.insert(1,tr("GoodsID"));
            listHeader.insert(2,tr("KeyNo"));
            listHeaderTip.insert(1,tr("The number of column,For the PC!   "));
            listHeaderTip.insert(2,tr("The number of select key board   "));
            goods_table_st.aisle_no = 0;
            goods_table_st.id = 1;
            goods_table_st.key_no = 2;
            goods_table_st.price = 3;
            goods_table_st.remain = 4;
            goods_table_st.max_count = 5;
            goods_table_st.state = 6;
            goods_table_st.test = 7;

        }
        else
        {
            listHeader.insert(1,tr("GoodsID"));
            listHeaderTip.insert(1,tr("The number of column,For the PC!   "));
            goods_table_st.aisle_no = 0;
            goods_table_st.id = 1;
            goods_table_st.key_no = 0;
            goods_table_st.price = 2;
            goods_table_st.remain = 3;
            goods_table_st.max_count = 4;
            goods_table_st.state = 5;
            goods_table_st.test = 6;
        }
    }
    else
    {
        goods_table_st.id = 0;
        if(getVMCSystemPara(VMC_SYSTEM_SELECTBOARD))
        {
           listHeader.insert(1,tr("KeyNo"));
           listHeaderTip.insert(1,tr("The number of select key board   "));
           goods_table_st.aisle_no = 0;
           goods_table_st.key_no = 1;
           goods_table_st.price = 2;
           goods_table_st.remain = 3;
           goods_table_st.max_count = 4;
           goods_table_st.state = 5;
           goods_table_st.test = 6;

        }
        else
        {
           goods_table_st.aisle_no = 0;
           goods_table_st.key_no = 0;
           goods_table_st.price = 1;
           goods_table_st.remain = 2;
           goods_table_st.max_count = 3;
           goods_table_st.state = 4;
           goods_table_st.test = 5;
        }
    }

    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(listHeader.count());//设置列
    ui->tableWidget->setHorizontalHeaderLabels(listHeader);//设置列头
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems); //选中的方式
    ui->tableWidget->horizontalHeader()->setVisible(true); //隐藏行表
    ui->tableWidget->verticalHeader()->setVisible(false);
    //ui->tableWidget->resizeRowsToContents();
    for(int i = 0;i < ui->tableWidget->columnCount();i++)
    {
        QTableWidgetItem *item = ui->tableWidget->horizontalHeaderItem(i);        
        item->setFont(headFont); //设置字体
        item->setBackgroundColor(QColor(0,60,10)); //设置单元格背景颜色
        item->setTextColor(QColor(200,111,30)); //设置文字颜色

        item->setToolTip(listHeaderTip.at(i));
    }
    //还可以将行和列的大小设为与内容相匹配
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->clearContents();
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条

}

void AisleManagementDialog::pcAddGoodsReq(QByteArray arr)
{
    quint8 type,leveNo,aisleNo;
    quint8 cabinetNo = ui->comboBox->currentIndex() + 1;
    //ui->labelState->setText(tr("Add goods..."));
    type = (quint8)arr.at(0);
    if(type == 1)
    {
        leveNo = (quint8)arr.at(1);
        if(!leveAisleST[leveNo - 1].isOpen)
        {
            QMessageBox::information(this,tr("AddGoods"),
                                     tr("Can not add goods!"),
                                     QMessageBox::Yes);
            return;
        }
    }
    else if(type == 2)
    {
        aisleNo = (quint8)arr.at(1);
        if(getLogicAisleInfo(aisleNo,AISLE_STATE,cabinetNo) == 2)
        {
            QMessageBox::information(this,tr("AddGoods"),
                                     tr("Can not add goods!"),
                                     QMessageBox::Yes);
            return;
        }

    }
    setButtonEnable(false);
    emit processTextSignal(false,processText(ADDING));
    emit goodsRequestSignal(HUODAO_REQ,arr);
}

void AisleManagementDialog::pcConGoodsReq()
{
    int ret;
    ret = QMessageBox::information(this,tr("GoodsInfo"),
                                   processText(SET),
                                   QMessageBox::Yes,QMessageBox::No);
    if(ret == QMessageBox::No)
        return;

    setButtonEnable(false);
    readGoodsDialogInfo();
    //如果配置选货按键板 需先配置选货按键板
    if(getVMCSystemPara(VMC_SYSTEM_SELECTBOARD))
    {
        pcConSelectBoardReq();
    }
    else
    {
        leveNo = 1;
        //ui->labelState->setText(processText(SETTING,leveNo));
        emit processTextSignal(false,processText(SETTING,leveNo));
        sendSetAisle();
    }

}


void AisleManagementDialog::pcGetGoodsReq()
{


}
void AisleManagementDialog::pcConSelectBoardReq()
{
    emit processTextSignal(false,processText(CONSELECTTING));
    QByteArray sendArr;
    QDataStream in(&sendArr,QIODevice::WriteOnly);
    in<<(quint8)SET_HUODAO_SELECT_REQ;
    in<<(quint8)(ui->comboBox->currentIndex() + 1);
    in<<(quint8)0x00;//方式 按照层方式设置
    for(int i = 0; i < 8;i++)
    {
        for(int j = 0; j < 10;j++)
        {
            in<<(quint8)leveAisleST[i].aisle[j].selectNo;
        }
    }
    emit goodsRequestSignal(HUODAO_REQ,sendArr);
}

void AisleManagementDialog::pcConSelectBoardRpt(const QByteArray &arr)
{
    //解析柜号
    quint8 cabinet = (quint8)arr.at(0);
    cabinet = cabinet;

    quint8 type = (quint8)arr.at(1);
    //rst
    quint8 rst = (quint8)arr.at(2);
    if(rst == VMC_OK && type == 0x00)
    {
        leveNo = 1;
        emit processTextSignal(false,processText(SETTING,leveNo));
        qDebug()<<"pcConSelectBoardRpt...ok";
        sendSetAisle();
    }
    else //配置失败
    {
         emit processTextSignal(true,processText(SETFAIL));
         setButtonEnable(true);
    }
}

void AisleManagementDialog::pcAddGoodsRpt(const QByteArray &arr)
{
    quint8 index = 0;
    quint8 cabinet = (quint8)arr.at(index++);
    quint8 addType = (quint8)arr.at(index++);
    quint8 rst = (quint8)arr.at(index++);
    cabinet = cabinet;
    addType = addType;
    if(rst == VMC_OK)
    {
        //ui->labelState->setText(tr("Add goods suc!"));
        emit processTextSignal(true,processText(ADDSUC));
    }
    else
    {
        emit processTextSignal(true,processText(ADDFAIL));
    }
}
void AisleManagementDialog::pcConGoodsRpt(const QByteArray &arr)
{
   quint8 index = 0;
   quint8 cabinet = (quint8)arr.at(index++);
   cabinet = cabinet;
   quint8 setType = (quint8)arr.at(index++);
   quint8 rst = (quint8)arr.at(index++);

    if(setType != 0x01)//
    {
        //ui->labelState->setText(processText(SETFAIL));
        emit processTextSignal(true,processText(SETFAIL));
        return;
    }
    if((rst == VMC_OK) )
    {
        if(leveNo == 8)//配置结束 发送写入FLASH命令
        {
            emit processTextSignal(false,processText(SAVING));
            QByteArray sendArr;
            QDataStream in(&sendArr,QIODevice::WriteOnly);
            in<<(quint8)WRITE_HUODAO_REQ;
            in<<(quint8)(ui->comboBox->currentIndex() + 1);
            emit goodsRequestSignal(HUODAO_REQ,sendArr);
        }
        else
        {
            leveNo++;
            emit processTextSignal(false,processText(SETTING,leveNo));
            sendSetAisle();
        }
    }
    else
        emit processTextSignal(true,processText(SETFAIL));
}

void AisleManagementDialog::pcWriteRpt(const QByteArray &arr)
{

    if((quint8)arr.at(0) == 1)
    {
       // ui->labelState->setText(processText(GETTING));
        emit getGoodsInfoSignal();
    }
    else
    {
        //ui->labelState->setText(processText(SETFAIL));
        emit processTextSignal(true,processText(SETFAIL));
    }

}

void AisleManagementDialog::vmcConnect()
{
    setVMCState(true);
}

void AisleManagementDialog::vmcDisconnect()
{
    setVMCState(false);
    ui->tableWidget->clearContents();
    if(!aisleTestdialog->isHidden())
        aisleTestdialog->hide();
    isAllTest = false;
    ui->tableWidget->clearContents();
}

void AisleManagementDialog::sendGetAisle()
{
    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);
    in<<(quint8)leveNo;
  //  emit goodsRequestSignal(AISLE_GET_REQ,arr);
}
void AisleManagementDialog::sendSetAisle()
{
    QByteArray arr;
    quint16 temp16;
    QDataStream in(&arr,QIODevice::WriteOnly);
    in<<(quint8)SET_HUODAO_REQ;
    in<<(quint8)(ui->comboBox->currentIndex() + 1);
    in<<(quint8)0x01;
    in << (quint8)leveNo;
    for(int j = 0;j < 10;j++)
    {
        in<<(quint8)leveAisleST[leveNo - 1].aisle[j].physicNo;
        temp16 = amountToScale(leveAisleST[leveNo - 1].aisle[j].price);
        qDebug()<<"SendSetAisle"<<temp16;
        in<<(quint8)(temp16 /256)
        <<(quint8)(temp16 % 256)
        <<(quint8)leveAisleST[leveNo - 1].aisle[j].status     
        <<(quint8)leveAisleST[leveNo - 1].aisle[j].remain
        <<(quint8)leveAisleST[leveNo - 1].aisle[j].maxCount
        <<(quint8)(leveAisleST[leveNo - 1].aisle[j].id);
    }
    //qDebug()<<"AisleManagementDialog arr.size()"<<arr.size()<<arr.toHex();
    emit goodsRequestSignal(HUODAO_REQ,arr);
}

void AisleManagementDialog::
readGoodsDialogInfo()
{
    quint8 rowCount,i,j,temp8;
    quint8 cabinetNo = ui->comboBox->currentIndex() + 1;
    bool ok;
    QString tempStr;
    rowCount = ui->tableWidget->rowCount();
    //qDebug()<<"ui->tableWidget->rowCount()="<<rowCount;

    for(int index = 0;index < rowCount;index++)
    {
        //aisleNo
        temp8 = (quint8)ui->tableWidget->item(index,this->goods_table_st.aisle_no)->text().toInt(&ok,10);
        if(ok && getAisleIndexOfLogicNo(&i,&j,temp8,cabinetNo))
        {
            //id
            leveAisleST[i].aisle[j].id = ui->tableWidget->item(index,this->goods_table_st.id)->text().toInt(&ok,16);
            //keyNo
            if(getVMCSystemPara(VMC_SYSTEM_SELECTBOARD))
            {
                leveAisleST[i].aisle[j].selectNo = ui->tableWidget->item(index,this->goods_table_st.key_no)->text().toInt(&ok,10);
            }
            //商品单价
            tempStr = ui->tableWidget->item(index,this->goods_table_st.price)->text();

            qDebug()<<"i="<<i<<"j="<<j<<tempStr;
            tempStr.remove(".");
            leveAisleST[i].aisle[j].price = tempStr.toULong(&ok,10);

            //剩余量
            leveAisleST[i].aisle[j].remain = ui->tableWidget->item(index,this->goods_table_st.remain)->text().toInt(&ok,10);

        }
    }
}


void AisleManagementDialog::cellChangedSlot(int row, int column)
{
   // qDebug()<<"cellChanged"<<row<<column;
    QTableWidgetItem *item =  ui->tableWidget->item(row,column);
    if(!item)
        return;
    if(column == this->goods_table_st.id)//ID
    {
        QString str = item->text();
        QRegExp regExp("[0-9a-fA-F]{1,4}");
        if(!regExp.exactMatch(str))
        {
           item->setText("1");
        }
        else
        {
           item->setText(str.toUpper());
        }
    }
    else if(column == this->goods_table_st.price)//price
    {
        QString str = item->text();
        QRegExp regExp("[0-9]{1,8}[.]{0,1}[0-9]{0,2}");
        if(!regExp.exactMatch(str))
        {
            item->setText("1.00");
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
    }


}

void AisleManagementDialog::cellClickedSlot(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    if(!item)
        return;

    curText = item->text();
    //qDebug()<<"cellClickedSlot"<<curText;
}
void AisleManagementDialog::setVMCState(const bool flag)
{
    setButtonEnable(flag);
    ui->pushButtonAllTest->setEnabled(flag);
}
void AisleManagementDialog::setButtonEnable(const bool flag)
{
    ui->pushButtonAddGoods->setEnabled(flag);
    ui->pushButtonSaveGoods->setEnabled(flag);
    ui->pushButtonGetInfo->setEnabled(flag);


}


void AisleManagementDialog::goodsInfoSort(int index)
{
    //qDebug()<<"sectionClickedSlot"<<index;
    if(index == this->goods_table_st.aisle_no || index == this->goods_table_st.id)
        ui->tableWidget->sortByColumn(index,Qt::AscendingOrder);
    QPushButton *button = NULL;
    bool ok;
    for(int i = 0;i < ui->tableWidget->rowCount();i++)
    {
        button = qobject_cast<QPushButton *> (ui->tableWidget->cellWidget(i,this->goods_table_st.test));
        //qDebug()<<"str = button"<<button;
        quint8 logicNo = ui->tableWidget->item(i,this->goods_table_st.aisle_no)->text().toInt(&ok,10);
        if(button)
        {
            QString str = QString("%1%2")
                    .arg(i,2,10,QLatin1Char('0'))
                    .arg(logicNo,2,10,QLatin1Char('0'));
            signalMapper->setMapping(button,str);

        }
    }

}


void AisleManagementDialog::addIntem(int row, int column, const QString &text)
{
    QFont font;//("Helvetica",12,50);
    font.setPointSize(12);
    font.setWeight(50);

    ui->tableWidget->setItem(row,column,new QTableWidgetItem(text));
    ui->tableWidget->item(row, column)->setTextAlignment(Qt::AlignRight | Qt::AlignBottom);
    ui->tableWidget->item(row, column)->setFont(font);
    if(column == this->goods_table_st.id || column == this->goods_table_st.price)
        ui->tableWidget->item(row, column)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

}


void AisleManagementDialog::testAisleSlot(QString row_no)
{

    bool ok;
    int row = row_no.left(2).toInt(&ok,10);
    quint8 logicNo = row_no.right(2).toInt(&ok,10);
    //qDebug()<<"testAisleSlot=="<<row<<logicNo;
    ui->tableWidget->selectRow(row);


    QSize size  = this->size();
    QPoint point = this->pos();
    if(row < ui->tableWidget->rowCount())
    {
        size = ui->tableWidget->cellWidget(row,this->goods_table_st.test)->size();
        point = ui->tableWidget->cellWidget(row,this->goods_table_st.test)->pos();
    }

    point = ui->tableWidget->mapToGlobal(point);
    point.setX(point.x() + size.width());
    point.setY(point.y() + size.height());
    aisleTestdialog->move(point);    

    aisleTestdialog->setText(QString("%1%2%3").arg(tr("Testting the column:")).arg(logicNo)
                             .arg(tr("...")));
    aisleTestdialog->show();
    for(int i = 0;i < ui->tableWidget->rowCount();i++)
    {
       ui->tableWidget->cellWidget(i,this->goods_table_st.test)->setEnabled(false);
    }
    setButtonEnable(false);
    QByteArray sendArr;
    QDataStream in(&sendArr,QIODevice::WriteOnly);
    in<<(quint8)TEST_HUODAO_REQ;
    in<<(quint8)(quint8)(ui->comboBox->currentIndex() + 1);
    in<<(quint8)logicNo;
    emit goodsRequestSignal(HUODAO_REQ,sendArr);
    if(!isAllTest)
    {
        ui->pushButtonAllTest->setEnabled(false);
    }

}

void AisleManagementDialog::testTimeroutSLot()
{
    testTimer->stop();
    aisleTestdialog->setText("");
    if(isAllTest)
    {
        testAisleNo++;
        if(testAisleNo >= ui->tableWidget->rowCount() || stoppedAllTest)
        {
            isAllTest = false;
            stoppedAllTest = true;
            aisleTestdialog->hide();
            for(int i = 0;i < ui->tableWidget->rowCount();i++)
            {
               ui->tableWidget->cellWidget(i,this->goods_table_st.test)->setEnabled(true);
            }
            setButtonEnable(true);
            ui->pushButtonAllTest->setText(tr("AllTest"));
            ui->pushButtonAllTest->setChecked(false);
        }
        else
        {
            QString str = QString("%1%2").arg(testAisleNo,2,10,QLatin1Char('0'))
                    .arg(ui->tableWidget->item(testAisleNo,this->goods_table_st.aisle_no)->text(),2,QLatin1Char('0'));
            testAisleSlot(str);
        }

    }
    else
    {
        aisleTestdialog->hide();
        ui->pushButtonAllTest->setEnabled(true);
        for(int i = 0;i < ui->tableWidget->rowCount();i++)
        {
           ui->tableWidget->cellWidget(i,this->goods_table_st.test)->setEnabled(true);
        }
        setButtonEnable(true);
    }
}


void AisleManagementDialog::pcTestAisleRpt(const QByteArray &arr)
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


void AisleManagementDialog::requestTimeoutSlot()
{
    //ui->labelState->clear();

    if(!aisleTestdialog->isHidden())
        testTimeroutSLot();

}


void AisleManagementDialog::allTestSlot(bool checked)
{

    if(checked)
    {

        int ret = QMessageBox::information(this,tr("AllTest"),
                                 tr("Are you sure you want to test all the column?"),
                                           QMessageBox::Yes,QMessageBox::No);
        if(ret == QMessageBox::No)
        {
            ui->pushButtonAllTest->setChecked(false);
            return;
        }
        if(ui->tableWidget->rowCount() <= 0)
        {
            //qDebug()<<"ui->tableWidget->rowCount() <= 0"<<ui->tableWidget->rowCount();
            ui->pushButtonAllTest->setChecked(false);
            return;
        }

        ui->pushButtonAllTest->setText(tr("StopTest"));
       // goodsInfoSort(AISLENO);
        isAllTest = true;
        stoppedAllTest = false;
        testAisleNo = 0;

        QString str = QString("%1%2").arg(testAisleNo,2,10,QLatin1Char('0'))
                .arg(ui->tableWidget->item(testAisleNo,this->goods_table_st.aisle_no)->text(),2,QLatin1Char('0'));
        testAisleSlot(str);
    }
    else
    {
        ui->pushButtonAllTest->setText(tr("AllTest"));
        stoppedAllTest = true;
        isAllTest = false;
    }


}

