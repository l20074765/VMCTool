#include "printdialog.h"
#include "ui_printdialog.h"
#include <QtGui>

PrintDialog::PrintDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrintDialog)
{
    ui->setupUi(this);

    QFont font = this->font();
   //font.setFamily("Helvetica");
    font.setPointSize(12);
    ui->textBrowser->setFont(font);
    ui->label->setFont(font);
    comConfigDialog = new ComConfigDialog(this);
    comConfigDialog->setSuperMode(false);

    connect(ui->textBrowser,SIGNAL(textChanged()),this,SLOT(textBrowserSlot()));
    connect(ui->open_pushButton,SIGNAL(clicked(bool)),this,SLOT(open_pushButton_clicked(bool)));
    connect(ui->config_pushButton,SIGNAL(clicked()),this,SLOT(config_pushButton_clicked()));
    connect(ui->startPrint_pushButton,SIGNAL(clicked()),this,SLOT(startPrint_pushButton_clicked()));
    connect(ui->toolButton,SIGNAL(clicked()),this,SLOT(tool_pushButton_clicked()));
    connect(ui->clearText_pushButton,SIGNAL(clicked()),ui->textBrowser,SLOT(clear()));
    connect(ui->clearSum_pushButton,SIGNAL(clicked()),this,SLOT(clearSum_pushButton_clicked()));
    connect(ui->pushButtonSearchPort,SIGNAL(clicked()),this,SLOT(serachPortClickedSlot()));
    printInfoDialog = new PrintInfoDialog(this);
    connect(printInfoDialog,SIGNAL(accepted()),this,SLOT(printInfoDialogSlot()));

    //清总交易
    //ui->clearSum_pushButton->hide();
    ui->groupBox->hide();  
    ui->lineEdit->setText(QDir::currentPath() + "/Trade/"+tr("T") + dateTimeFileName() + ".txt");

    init();
    setWindowFlags(Qt::FramelessWindowHint);
    //ui->textBrowser->setWordWrapMode(QTextOption::NoWrap);
    ui->startPrint_pushButton->setEnabled(false);
    ui->clearSum_pushButton->setEnabled(false);
    translateLanguage();
}

PrintDialog::~PrintDialog()
{
    delete ui;
}


void PrintDialog::translateLanguage()
{
    ui->retranslateUi(this);

    if(ui->open_pushButton->isChecked())
        ui->open_pushButton->setText(tr("Close"));
    else
        ui->open_pushButton->setText(tr("Open"));

    ui->open_pushButton->setToolTip(tr("Start the program"));
    ui->config_pushButton->setText(tr("Config"));
    ui->config_pushButton->setToolTip(tr("Configure serial port"));
    ui->clearSum_pushButton->setText(tr("ClearTrade"));
    ui->clearSum_pushButton->setToolTip(tr("Remove transaction records"));
    ui->clearText_pushButton->setText(tr("ClearText"));
    ui->clearText_pushButton->setToolTip(tr("Clear display area"));
    ui->startPrint_pushButton->setText(tr("DataExp"));
    ui->startPrint_pushButton->setToolTip(tr("Export transaction records"));
    ui->toolButton->setText(tr("Browse"));
    ui->toolButton->setToolTip(tr("Change directories"));

    ui->pushButtonSearchPort->setText(tr("SearchPort"));
    ui->pushButtonSearchPort->setToolTip(tr("Search available serial port..."));

    ui->labelOutDir->setText(tr("Output Directory:"));
    this->printInfoDialog->translateLanguage();

}
void PrintDialog::init()
{
    ui->txt_radioButton->setChecked(true);
    ui->open_pushButton->setPicName(":/button/images/button");
    ui->config_pushButton->setPicName(":/button/images/button");
    ui->clearSum_pushButton->setPicName(":/button/images/button");
    ui->clearText_pushButton->setPicName(":/button/images/button");
    ui->startPrint_pushButton->setPicName(":/button/images/button");

    ui->pushButtonSearchPort->setPicName(":/button/images/button");

    ui->toolButton->setPicName(":/button/images/button");

    ui->textBrowser->setObjectName("dataPrint");


    //初始化 变量
    printInit();
}
void PrintDialog::printInit()
{
    startTradeIndex = 0;
    endTradeIndex = 0;
    printNum = 0;
    printFlag = false;
    ui->progressBar->setValue(0);
    ui->progressBar->hide();
    printInfoDialog->hide();
    printStatusSet(PrintReady);

}
void PrintDialog::printStatusSet(PRINTSTATE state)
{
    this->printStatus = state;
    if(state == PrintReady)//打印前
    {
        ui->startPrint_pushButton->setEnabled(true);
        ui->startPrint_pushButton->setText(tr("DataExp"));
    }
    else if(state == Printting)//打印中
    {
        ui->startPrint_pushButton->setText(tr("StopExp"));
    }
    else if(state == PrintPause)//暂停打印
    {
        ui->startPrint_pushButton->setText(tr("Continue"));
    }

}
void PrintDialog::config_pushButton_clicked()
{
    comConfigDialog->show();
}

void PrintDialog::open_pushButton_clicked(bool checked)
{
    //qDebug()<<"open..";
    emit printDialogOpenSignal(checked);
}
void PrintDialog::comOpenOrClose(quint8 flag)
{
    if(flag == 1)
    {
       ui->textBrowser->clear();
       ui->open_pushButton->setText(tr("Close"));
    }
    else if(flag == 0)
    {
        ui->textBrowser->insertPlainText(tr("CloseCom...") +"\n");
        file.close();
        ui->open_pushButton->setText(tr("Open"));
        ui->open_pushButton->setChecked(false);
        init();
        qDebug()<<"closed...";
    }
    else
    {
        ui->open_pushButton->setChecked(false);
    }
}
void PrintDialog::startPrint_pushButton_clicked()
{

    if(this->printStatus == PrintReady)
    {
        QByteArray arr;
        QFileInfo fileInfo(ui->lineEdit->text());// D:/test/test.txt
        QDir dir(fileInfo.absolutePath());//D:/test
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);//设置过滤的属性
        dir.setSorting(QDir::Time | QDir::Reversed);                 //设置排序的方式
        QString fileName = fileInfo.absoluteFilePath();
        if(fileName.right(4) != ".txt")
        {
            QMessageBox::information(this,tr("inform"),
                                     tr("Invalid txt file type")
                                    ,QMessageBox::Yes);
            return;
        }
        file.setFileName(fileName);
        if(file.isOpen())
            file.close();
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text ))
        {
            QMessageBox::information(this,tr("inform"),file.fileName() + tr("can't open"),
                                     QMessageBox::Yes);

            return;
        }
//        QSettings *read = new QSettings("./config.ini",QSettings::IniFormat);

//        QString language = read->value("Language/Language").toString();
//        delete read;

//        if(language == "ch" || language == "CH")
//        {
//            printLanguage = CH;
//        }
//        else
//            printLanguage = EN;

        QDataStream in(&arr,QIODevice::WriteOnly);
        in <<(quint8)0x00;
        emit printRequestSignal(TRADE_REQ,arr);

    }
    else if(this->printStatus == Printting)
    {
        printStatusSet(PrintPause);
    }
    else if(this->printStatus == PrintPause)
    {
        printStatusSet(Printting);
        sendPrintTraderequest();
    }



}
void PrintDialog::stop_pushButton_clicked(bool checked)
{
    if(!curReadIndex)
    {

        printStatusSet(PrintReady);
        return;
    }
    if(checked)
    {
        qDebug()<<"stop..";
        printFlag = false;
        printStatusSet(PrintPause);

    }
    else
    {
        qDebug()<<"concitnu..";
        printFlag = true;
        printStatusSet(Printting);
        ui->textBrowser->insertPlainText(tr("Printing...") + "\n");
        sendPrintTraderequest();    
    }
}

//更改目录
void PrintDialog::tool_pushButton_clicked()
{
    QString fileName;

    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle(tr("File"));
    fileDialog.setDirectory(QDir::currentPath());
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);

   // qDebug()<< QDir::currentPath();

    if(fileDialog.exec() == QDialog::Accepted)
    {
        fileName = fileDialog.selectedFiles()[0];
    }
   // fileName = fileDialog.getExistingDirectory(this,tr("Save File"),"./Trade");
    if(!fileName.isEmpty())
    {
        if(fileName.indexOf("\\") >= 0)
            fileName.replace("\\","/");
        fileName = fileName + "/" + tr("T") + dateTimeFileName() + ".txt";
        ui->lineEdit->setText(fileName);
        qDebug()<<fileName;
    }

}

void PrintDialog::clearSum_pushButton_clicked()
{
    int ret = QMessageBox::information(this,tr("Clear Trade"),
                             tr("Are you sure clear the total transaction records?")
                             + "\n" + tr("After clearing all the records will be reset!"),
                             QMessageBox::Yes,QMessageBox::No);

    if(ret == QMessageBox::No)
        return;

    ui->textBrowser->insertPlainText(tr("Clearing transactions records...") + "\n");
    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);
    in<<(quint8)0x00;
    emit printRequestSignal(CLEAR_REQ,arr);
}
void PrintDialog::printInfoDialogSlot()
{

    printInfoDialog->hide();
    printInfoDialog->getTradeIndex(&startTradeIndex,&endTradeIndex);
    qDebug()<<"startTradeIndex"<<startTradeIndex<<"endTradeIndex"<<endTradeIndex;
    curReadIndex = startTradeIndex - 1;
    ui->progressBar->setRange(0,endTradeIndex - startTradeIndex + 1);
    ui->progressBar->show();
    printFlag = true;
    printStatusSet(Printting);

    //ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->insertPlainText(printHeadStr);
    //打印列头
    QStringList list = printStr.split("\t");
    qDebug()<<"dxpList"<<list;
    if(list.count() < 7)
    {
        qDebug()<<"list.count() < 7 the list.count() is"<<list.count();
        return;
    }
    QString str;

   str = QString("%1 %2  %3 %4 %5 %6 %7\n")
            .arg(list.at(0),-0,QLatin1Char(' '))
            .arg(list.at(1),-0,QLatin1Char(' '))
            .arg(list.at(2),-0,QLatin1Char(' '))
            .arg(list.at(3),-0,QLatin1Char(' '))
            .arg(list.at(4),-0,QLatin1Char(' '))
            .arg(list.at(5),-0,QLatin1Char(' '))
            .arg(list.at(6),-0,QLatin1Char(' '))
            ;




    qDebug()<<"headStr="<<str;
    printStr = str;

    ui->textBrowser->insertPlainText(str);
     // ui->textBrowser->insertPlainText(printStr);
    printNum = 0;
    sendPrintTraderequest();
}

void PrintDialog::vmcDisconnect()
{
    QFont font = ui->textBrowser->font();
    QFont fontOld = font;
    font.setBold(true);
   // font.setFamily("Helvetica");
    font.setPointSize(12);
    font.setItalic(true);
    ui->textBrowser->setCurrentFont(font);
    ui->textBrowser->setTextColor(QColor(Qt::red));
    ui->textBrowser->insertPlainText(tr("VendMachine Disconnected") + "\n");
    ui->textBrowser->setCurrentFont(fontOld);
    ui->textBrowser->setTextColor(QColor(Qt::black));

    ui->startPrint_pushButton->setEnabled(false);
    ui->clearSum_pushButton->setEnabled(false);
}
void PrintDialog::vmcConnect()
{
    QFont font = ui->textBrowser->font();
    QFont fontOld = font;
    font.setBold(true);
   // font.setFamily("Helvetica");
    font.setPointSize(12);
    font.setItalic(true);
    ui->textBrowser->setCurrentFont(font);
    ui->textBrowser->setTextColor(QColor(Qt::blue));
    ui->textBrowser->insertPlainText(tr("VendMachine Connected") + "\n");

    ui->textBrowser->setCurrentFont(fontOld);
    ui->textBrowser->setTextColor(QColor(Qt::black));

     ui->startPrint_pushButton->setEnabled(true);
     ui->clearSum_pushButton->setEnabled(true);

}


void PrintDialog::pcRequestFail()
{
   ui->textBrowser->insertPlainText(tr("pc CMD Send fail") + "\n");

}

void PrintDialog::sendPrintTraderequest()
{

    if(curReadIndex >= endTradeIndex)
    {
        if(endTradeIndex < curTradeIndex)//print is not over
        {
            ui->textBrowser->insertPlainText(tr("Print OVER...") + "\n");

            ui->textBrowser->insertPlainText(tr("Printed")
                                         + QString("%1").arg(printNum)+tr("page") + "\n");
            printStatusSet(PrintReady);
        }
        else
        {
            ui->textBrowser->insertPlainText(tr("Print OVER...") + "\n");
            ui->textBrowser->insertPlainText(tr("Transaction records print complete...") + "\n");
            ui->textBrowser->insertPlainText(tr("Printed Num:")
                                             +QString("%1").arg(printNum)+ "\n");
            printInit();
        }
        file.close();

        return;
    }
    if(printStatus == PrintPause)
    {
        ui->textBrowser->insertPlainText(tr("Print Puase...") + "\n");
        ui->textBrowser->insertPlainText(tr("Printed") + QString("%1").arg(curReadIndex)+tr("page")+ "\n");
        return;
    }
    curReadIndex++;
    ui->progressBar->setValue(curReadIndex);
    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);
    in<<(quint8)0x01;    
    in<<(quint16)curReadIndex;
    emit printRequestSignal(TRADE_REQ,arr);

    qDebug()<<"sendTradeInfo..curReadIndex="<<curReadIndex;
}

void PrintDialog::textBrowserSlot()
{
    ui->textBrowser->moveCursor(QTextCursor::End);
}


void PrintDialog::changeMode()
{
#if 0
    if(ui->clearSum_pushButton->isVisible())
        ui->clearSum_pushButton->hide();
    else
        ui->clearSum_pushButton->show();
#endif
}
bool PrintDialog::isSuperMode()
{
    return ui->clearSum_pushButton->isVisible();
}
void PrintDialog::paintEvent(QPaintEvent *event)
{
    event->accept();
}
//获取总信息
void PrintDialog::recvPrintTotalInfo(QByteArray arr)
{
   //目前总

    QString str;
    quint16 index = 0;
    quint32 dataTmp;
    bool ok;
    QDateTime date;
    str =  tr("Export data date:");
    str += date.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    str += "\n";

    maxTradeIndex = arr.mid(index,2).toHex().toUInt(&ok,16);
    index += 2;

    curTradeIndex = arr.mid(index,2).toHex().toUInt(&ok,16);
    index += 2;

    QSettings *readCurrency = new QSettings("./config.ini");
    QString cType = readCurrency->value("Currency/Type").toString();
    QString cUint = readCurrency->value("Currency/Uint").toString();

    if(cType.isEmpty() || cType == "RMB")
    {
      cType = tr("RMB");
      readCurrency->setValue("Currency/Type","RMB");
    }

    if(cUint.isEmpty() || cUint == "yuan")
    {
      readCurrency->setValue("Currency/Uint","yuan");
      cUint = tr("yuan");
    }
    delete readCurrency;

    str += QString("%1 %2   %3 %4\n")
            .arg(tr("CurrencyType:"),-tr("CurrencyType:").length(),QLatin1Char(' '))
            .arg(cType)
            .arg(tr("CurrencyUint:"),-tr("CurrencyUint:").length(),QLatin1Char(' '))
            .arg(cUint);


    //Total receive monetary amount
    dataTmp = arr.mid(index,4).toHex().toULong(&ok,16);
    dataTmp = amountFromScale(dataTmp);
    str += QString("%1%2.%3  ")
            .arg(tr("Total receive monetary amount:"),-tr("Total receive monetary amount:").length(),QLatin1Char(' '))
            .arg(dataTmp/100)
            .arg(dataTmp%100,2,10,QLatin1Char('0'));
    index += 4;

    //用户访问次数
   str += QString("%1%2\n")
            .arg(tr("User access number:"),-tr("User access number:").length(),QLatin1Char(' '))
            .arg(curTradeIndex);

    //Total sales quantity
    dataTmp = arr.mid(index,3).toHex().toULong(&ok,16);
    str += QString("%1%2  ")
            .arg(tr("Total sales quantity:"),-tr("Total sales quantity:").length(),QLatin1Char(' '))
            .arg(dataTmp);
    index += 3;
    //TotalSalesAmount
    dataTmp = arr.mid(index,4).toHex().toULong(&ok,16);
    dataTmp = amountFromScale(dataTmp);
    str += QString("%1%2.%3  ")
            .arg(tr("Total sales amount:"),-tr("Total sales amount:").length(),QLatin1Char(' '))
            .arg(dataTmp/100)
            .arg(dataTmp%100,2,10,QLatin1Char('0'));
    index += 4;


    //TotalChangeAmount
    dataTmp = arr.mid(index,4).toHex().toULong(&ok,16);
    dataTmp = amountFromScale(dataTmp);
    str += QString("%1%2.%3\n")
            .arg(tr("Total change amount:"),-tr("Total change amount:").length(),QLatin1Char(' '))
            .arg(dataTmp/100)
            .arg(dataTmp%100,2,10,QLatin1Char('0'));
    index += 4;

    //UnknownTradingAmount
    dataTmp = arr.mid(index,4).toHex().toULong(&ok,16);
    dataTmp = amountFromScale(dataTmp);
    str += QString("%1%2.%3  ")
            .arg(tr("Doubt transaction records amount:"),-tr("Doubt transaction records amount:").length(),QLatin1Char(' '))
            .arg(dataTmp/100)
            .arg(dataTmp%100,2,10,QLatin1Char('0'));
    index += 4;
    //DebtAmount
    dataTmp = arr.mid(index,4).toHex().toULong(&ok,16);
    dataTmp = amountFromScale(dataTmp);
    str += QString("%1%2.%3\n")
            .arg(tr("Debt amount:"),-tr("Debt amount:").length(),QLatin1Char(' '))
            .arg(dataTmp/100)
            .arg(dataTmp%100,2,10,QLatin1Char('0'));
    index += 4;

    printHeadStr = str;

#if 1
    printStr = QString("%1\t%2\t%3\t%4\t%5\t%6\t%7\t")
            .arg(tr("UserNo  "))
            .arg(tr("PurchasingDate  "))
            .arg(tr("ColumnNo"))
            .arg(tr("GoodsNo "))
            .arg(tr("Price   "))
            .arg(tr("Payment "))
            .arg(tr("Result  "))
            ;
    str += printStr;

    str += QString("%1\t%2\t%3\t%4\n")
            .arg(tr("ReceiveBillAmount"))
            .arg(tr("ReceiveCoinAmount"))
            .arg(tr("ReceiveCardAmount"))
            .arg(tr("ChangeAmount"))
            ;
#endif

    if(curTradeIndex == 0)
    {
        QString tradeStr = tr("No transaction records");
        ui->textBrowser->insertPlainText(tradeStr + "\n");
        QMessageBox::information(this,tr("Transaction records"),
                                 tradeStr,QMessageBox::Yes);
        return;
    }

    file.setFileName(ui->lineEdit->text());
    if(!file.isOpen())
    {

      if(!file.open(QIODevice::WriteOnly | QIODevice::Text ))
        {
            QMessageBox::information(this,tr("inform"),"test.txt" + tr("can't open"),
                                     QMessageBox::Yes);

            return;
        }

    }

    file.write(str.toAscii());

    printInfoDialog->show();
    printInfoDialog->setModal(true);
    printInfoDialog->setTradeNumLabel(curTradeIndex,0);


}
void PrintDialog::recvPrintTradeInfo(QByteArray arr)
{
    QString showStr,expStr,midStr,dateStr,tempStr;
    quint16 index = 0;
    quint32 dataTmp;
    quint8 data8tmp,tradeNum;
    bool ok;
    //页号
    dataTmp = (quint32)(arr.mid(index,2).toHex().toUInt(&ok,16));
    index += 2;
    if((curReadIndex) != (quint16)dataTmp)
    {
        ui->textBrowser->insertPlainText(QString("%1%2%3\n")
                                         .arg(tr("Page "))
                                         .arg(curReadIndex)
                                         .arg(tr(" read err!"))
                                         .arg(tr("Transaction record damage!")));
      sendPrintTraderequest();
      return;
    }
    //交易时间
    dateStr = QString(arr.mid(index,6).toHex())
            .insert(4,"-").insert(7,"-").insert(10," ")
            .insert(13,":");

    index += 7;
    //接收纸币
    dataTmp = (quint32)(arr.mid(index,2).toHex().toULong(&ok,16));
    dataTmp = amountFromScale(dataTmp);
    index += 2;
    midStr = QString("%1.%2\t")
      .arg(dataTmp/100)
      .arg(dataTmp%100,2,10,QLatin1Char('0'));
    //接收硬币
    dataTmp = (quint32)arr.mid(index,2).toHex().toULong(&ok,16);
    dataTmp = amountFromScale(dataTmp);
    index += 2;
    midStr += QString("%1.%2\t")
      .arg(dataTmp/100)
      .arg(dataTmp%100,2,10,QLatin1Char('0'));
    //接收卡
    dataTmp = (quint32)arr.mid(index,2).toHex().toULong(&ok,16);
    dataTmp = amountFromScale(dataTmp);
    index += 2;
    midStr += QString("%1.%2\t")
      .arg(dataTmp/100)
      .arg(dataTmp%100,2,10,QLatin1Char('0'));
    //找零
    dataTmp = (quint32)arr.mid(index,2).toHex().toULong(&ok,16);
    dataTmp = amountFromScale(dataTmp);
    index += 2;
    midStr += QString("%1.%2")
      .arg(dataTmp/100)
      .arg(dataTmp%100,2,10,QLatin1Char('0'));


    //购买次数
    tradeNum = (quint8)arr.at(index++);
    qDebug()<<"tradeNum="<<tradeNum;
    expStr.clear();
    showStr.clear();
    if(tradeNum == 0)
    {
        //没有交易纯找零
        tempStr= QString("%1\t%2\t%3\t%4\t%5\t%6\t%7\t")
                .arg(curReadIndex)//index
                .arg(dateStr)//date
                .arg(" ")//aisleNo
                .arg(" ")//goodsNo
                .arg(" ")//price
                .arg("        ")//payMode
                .arg(tr("NotBuy  "))//result
                ;

        showStr += tempStr;
        tempStr += midStr;
        expStr += tempStr + "\n";

    }
    else
    {
        for(int i = 0;i < tradeNum;i++)
        {
          tempStr.clear();
          if(0)
          {
              tempStr = QString("%1\t%2\t")
                      .arg(" ")
                      .arg(" ")
                      ;
          }
          else
          {
              tempStr = QString("%1\t%2\t")
                      .arg(curReadIndex)
                      .arg(dateStr)
                      ;
          }

          //柜号
          data8tmp = (quint8)(arr.at(index++) & 0xff);
          QString cabinetStr;
          if(data8tmp == 1)
              cabinetStr = "A";
          else if(data8tmp == 2)
              cabinetStr = "B";
          else if(data8tmp == 3)
              cabinetStr = "C";
          else
              cabinetStr = " ";
          //货道号18 index
          data8tmp = (quint8)(arr.at(index++) & 0xff);
          tempStr += QString("%1%2\t")
                  .arg(cabinetStr)
                  .arg((quint8)(data8tmp),2,10,QLatin1Char('0'));

          //商品序号1byte
          dataTmp = (quint32)(arr.at(index++) & 0xff);
          tempStr += QString("%1\t")
                  .arg(dataTmp,2,16,QLatin1Char('0')).toUpper();
          //单价 2byte
          dataTmp = (quint32)(arr.mid(index,2).toHex().toULong(&ok,16) &0xffff);
          dataTmp = amountFromScale(dataTmp);
          tempStr += QString("%1.%2\t")
                  .arg(dataTmp/100)
                  .arg(dataTmp%100,2,10,QLatin1Char('0'));
          index += 2;
          //支付方式
          data8tmp = (quint8)(arr.at(index++)&0xff);
          QString payModeStr;
          if(data8tmp == 0)
              payModeStr = tr("Cash    ");
          else if(data8tmp == 1)
              payModeStr = tr("OneCard ");
          else if(data8tmp == 2)
              payModeStr = tr("UnionPay");
          else if(data8tmp == 3)
              payModeStr = tr("PC      ");
          else if(data8tmp == 4)
              payModeStr = tr("Alipay  ");
          else if(data8tmp == 5)
              payModeStr = tr("Phone   ");
          else if(data8tmp == 6)
              payModeStr = tr("Game    ");
          else if(data8tmp == 7)
              payModeStr = tr("Card    ");
          else
              payModeStr = tr("Other   ");
          tempStr += QString("%1\t")
                  .arg(payModeStr);
          //出货结果

          data8tmp = (quint8)(arr.at(index++)&0xff);
          QString rstStr;
          if(data8tmp == 0)
              rstStr = tr("Failure ");
          else if(data8tmp == 1)
              rstStr = tr("Success ");
          else
              rstStr = tr("Unkown  ");
          tempStr += QString("%1\t")
                  .arg(rstStr);

          showStr += tempStr;
          if(i)
          {
              //空白打印
              tempStr += QString("%1\t%2\t%3\t%4")
                        .arg(" ")
                        .arg(" ")
                        .arg(" ")
                        .arg(" ")
                        ;
          }
          else
          {
              tempStr += midStr;
          }

          tempStr += "\n";
          expStr += tempStr;
        }
    }

  //  qDebug()<<"trade="<<showStr;
    if(file.isOpen())
    {
        file.write(expStr.toAscii());
        qDebug()<<"file.write...";
    }
    else
    {
        QMessageBox::information(this,tr("inform"),
                                 file.errorString(),
                                 QMessageBox::Yes);
        return;
    }

    QStringList list = showStr.replace("\n","").split("\t");
    showStr.clear();
    if(list.count() >= tradeNum * 7)
    {
        qDebug()<<"listCount"<<list.count()<<tradeNum * 7;
        for(int i = 0;i < tradeNum;i++)
        {
            int j = i*7;
            showStr += QString("%1 %2  %3 %4 %5 %6 %7\n")
                    .arg(list.at(j),-DXP_USERNO_L,QLatin1Char(' '))
                    .arg(list.at(j+1),-DXP_DATE_L,QLatin1Char(' '))
                    .arg(list.at(j+2),-DXP_AISLENO_L,QLatin1Char(' '))
                    .arg(list.at(j+3),-DXP_GOODSNO_L,QLatin1Char(' '))
                    .arg(list.at(j+4),-DXP_PRICE_L,QLatin1Char(' '))
                    .arg(list.at(j+5),-0,QLatin1Char(' '))
                    .arg(list.at(j+6),-0,QLatin1Char(' '))
                    ;
        }
    }





    //qDebug()<<"Insert showStr="<<"\n"
     //      <<printStr<<showStr;
    ui->textBrowser->insertPlainText(showStr);

    //ui->textBrowser->setCurrentFont(fontOld);
    printNum++;
    sendPrintTraderequest();

}
void PrintDialog::recvSumClear(QByteArray arr)
{
    quint8 result = (quint8)arr.at(0);
    QString str;
    if(result)
        str = tr("Transaction records clear success!");

    else
        str = tr("Transaction records clear failure!");

    ui->textBrowser->insertPlainText(str+"\n");
    QMessageBox::information(this,tr("Clear"),
                             str,QMessageBox::Yes);
}

void PrintDialog::openComInfo(const QString &portName)
{
    bool ok;
    quint8 index;
    QString str = portName;
    index = str.right(1).toInt(&ok,10);
    if(ok)
    {
         comConfigDialog->comComboBox->setCurrentIndex(index - 1);
    }
    ui->textBrowser->insertPlainText(tr("Open Success...")+"\n" +
           tr("Port:") + portName + "  " +
           tr("Buad:") + comConfigDialog->baudComboBox->currentText() + "\n");
    ui->textBrowser->insertPlainText(tr("Waiting for the vending machine connection...") + "\n");

}

void PrintDialog::labelStatusSetText(const QString &text)
{
    ui->label->setText(text);
}

QString PrintDialog::dateTimeFileName()
{

    QDateTime date;
    QString dateStr;
    dateStr = date.currentDateTime().toString("yyyyMMdd");
    //qDebug()<<dateStr<<date.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    return dateStr;

}


void PrintDialog::serachPortClickedSlot()
{

    QString portNameStr;
    //搜索串口
    QList<QextPortInfo> listPort = QextSerialEnumerator::getPorts();
    for(int i = 0;i < listPort.count();i++)
    {
       portNameStr += QString("%1").arg(listPort.at(i).portName.toLocal8Bit().constData());

       qDebug()<<"listPort.at(i).portName"<<listPort.at(i).portName.toLocal8Bit().constData()
                <<"i = "<<i;
#if 1
       Win_QextSerialPort port;
       port.setPortName(listPort.at(i).portName.toLocal8Bit().constData());
       if(!port.open(QIODevice::ReadOnly))
       {
           portNameStr += QString("(%1)").arg(tr("Busy"));
       }
       else
       {
           portNameStr += QString("(%1)").arg(tr("Idle"));
           port.close();
       }
#endif
       portNameStr += "\n";
    }
    if(portNameStr.isEmpty())
    {
        portNameStr = tr("Don't have any available serial port");
    }
    QMessageBox inform;
    QFont font = this->font();
    font.setPointSize(12);
    inform.setFont(font);

    inform.setText(tr("The currently available serial number:"));

    inform.setInformativeText(portNameStr);
    inform.setStandardButtons(QMessageBox::Yes);
    inform.setIcon(QMessageBox::Information);
    inform.exec();


}
