#include "serialassistantdialog.h"
#include "ui_serialassistantdialog.h"
#include <QtGui>
SerialAssistantDialog::SerialAssistantDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialAssistantDialog)
{
    ui->setupUi(this);
    serialAss = new USerial(this);    
    this->setWindowFlags( Qt::Window);
    this->setModal(true);
    connect(ui->open_pushButton,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonOpenSlot(bool)));

    connect(ui->clearText_pushButton,SIGNAL(clicked()),
            this->ui->textBrowser,SLOT(clear()));
    connect(ui->clearSendText_pushButton,SIGNAL(clicked()),
            this->ui->textEdit,SLOT(clear()));

    connect(ui->clearText_pushButton,SIGNAL(clicked()),
            this,SLOT(clearRecvBytes()));
    connect(ui->clearSendText_pushButton,SIGNAL(clicked()),
            this,SLOT(clearSendBytes()));



    connect(this,SIGNAL(recvAndSendbytesSignal()),
            this,SLOT(recvAndSendTimerSlot()));

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->radioButtonRecvHex,0);
    buttonGroup->addButton(ui->radioButtonRecvStr,1);
    buttonGroup->setExclusive(true);
    ui->radioButtonRecvStr->setChecked(true);

    QButtonGroup *buttonGroup1 = new QButtonGroup(this);
    buttonGroup1->addButton(ui->radioButtonSendHex,0);
    buttonGroup1->addButton(ui->radioButtonSendStr,1);
    buttonGroup1->setExclusive(true);
    ui->radioButtonSendStr->setChecked(true);

    connect(ui->pushButtonSend,SIGNAL(clicked()),
            this,SLOT(pushButtonSendSlot()));

    connect(serialAss,SIGNAL(recvDataSignal(QByteArray)),
            this,SLOT(recvDataSlot(QByteArray)),Qt::QueuedConnection);


    connect(ui->textBrowser,SIGNAL(textChanged()),
            this,SLOT(autoScroll()));

    connect(ui->checkBoxRecvSave,SIGNAL(toggled(bool)),
            this,SLOT(recvSaveToFileSlot()));

    //定位文件输出
    QDir dir;
    bool exist = dir.exists("./log/com_log");
    if(!exist)
    {
        dir.mkdir("./log");
        dir.mkdir("./log/com_log");
        //qDebug()<<"dir.dirName()"<<dir.dirName();
    }
    QString fileName = "./log/com_log/" +
            dateTimeFileName();
    file.setFileName(fileName);
    //初始化选择端口号
    setPort();
    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),
            this,SLOT(writeConfigCom(QString)));

    recvbytes = 0;
    sendbytes = 0;
    ui->labelRecvSize->setText("0");
    ui->labelSendSize->setText("0");
    maxLength = 100000;

    //按钮图片加载
    ui->pushButtonSend->setObjectName("serial");
    ui->clearSendText_pushButton->setObjectName("serial");
    ui->clearText_pushButton->setObjectName("serial");
    ui->open_pushButton->setObjectName("serial");
 #if 0
    QFont font = this->font();
    font.setBold(true);
    font.setPointSize(10);
    ui->pushButtonSend->setFont(font);
    ui->pushButtonSend->setFixedSize(100,30);
    ui->pushButtonSend->setPicName("./images/button_serial",ui->pushButtonSend->size());

    ui->clearSendText_pushButton->setFont(font);
    ui->clearSendText_pushButton->setFixedSize(100,30);
    ui->clearSendText_pushButton->setPicName("./images/button_serial",ui->clearSendText_pushButton->size());

    ui->clearText_pushButton->setFont(font);
    ui->clearText_pushButton->setFixedSize(100,30);
    ui->clearText_pushButton->setPicName("./images/button_serial",ui->clearText_pushButton->size());

    font.setPointSize(12);
    ui->open_pushButton->setFont(font);
    ui->open_pushButton->setFixedSize(100,40);
    ui->open_pushButton->setPicName("./images/button_serial",ui->open_pushButton->size());
  #endif
    translate();
}

SerialAssistantDialog::~SerialAssistantDialog()
{
    delete ui;
}



void SerialAssistantDialog::translate()
{
    ui->retranslateUi(this);
    this->setWindowTitle(tr("SerialAss"));
    QFont font = this->font();
  // font.setFamily("Adobe Arabic");
    font.setPointSize(12);
    font.setBold(true);
    ui->labelPort->setFont(font);
    ui->open_pushButton->setFont(font);
    ui->labelPort->setText(tr("Port:"));
    ui->open_pushButton->setText(tr("OpenCom"));



}
void SerialAssistantDialog::pushButtonOpenSlot(bool checked)
{
    if(checked)
    {
        if(!serialAss->init(ui->comboBox->currentText()))
        {
            QMessageBox::warning(this,tr("com"),
                                 tr("Error in open COM port:")
                                 + ui->comboBox->currentText(),
                                 QMessageBox::Yes);


            ui->open_pushButton->setChecked(false);
            return ;
        }
        if(serialAss->getPortName() != ui->comboBox->currentText())
        {
           bool ok;
           quint8 index = serialAss->getPortName().right(1).toInt(&ok,10);
           if(ok)
            ui->comboBox->setCurrentIndex(index - 1);

           writeConfigCom(ui->comboBox->currentText());
        }
        ui->open_pushButton->setText(tr("CloseCom"));
        serialAss->startSerialThread();

    }
    else
    {
        ui->open_pushButton->setText(tr("OpenCom"));
        serialAss->stopSerialThread();
        //关闭串口时 如果缓冲区还有数据 则要导出到txt
        if(!bufferStr.isEmpty() && ui->checkBoxRecvSave->isChecked())
        {
            if(!file.isOpen())
            {
                if(!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
                {
                    qDebug()<<"Can't open file:"<<file.fileName();
                    return;
                }
            }
             QTextStream in(&file);
             in<<QDateTime::currentDateTime().toString("hh:mm:ss ");
             in<<bufferStr + "\n";
             bufferStr.clear();
             file.close();
        }
        else
        {
            if(file.isOpen())
                file.close();
        }

    }
}


void SerialAssistantDialog::recvDataSlot(const QByteArray &readArr)
{

    QString str;
    if(ui->radioButtonRecvHex->isChecked())
    {
        QByteArray arr = readArr;
        QDataStream out(&arr,QIODevice::ReadOnly);
        quint8 temp;
        while(!out.atEnd())
        {
            out>>temp;
            recvbytes++;
            str += QString("%1 ").arg(temp,2,16,QLatin1Char('0'));
        }
        ui->textBrowser->insertPlainText(str.toUpper());

    }
    else
    {
        str = QString(readArr).replace("\r","");
        recvbytes += str.size() - str.count("\n");
        ui->textBrowser->insertPlainText(str);
    }
    if(ui->checkBoxRecvSave->isChecked())
    {

        file.setFileName("./log/com_log/" + dateTimeFileName());
        if(!file.isOpen())
        {
            if(!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
            {
                qDebug()<<"Can't open file:"<<file.fileName();
                return;
            }
        }       
        QTextStream in(&file);
        QStringList list = str.split("\n");
        bufferStr += list.at(0);
        if(list.count() > 1)//有换行符
        {
            in<<QDateTime::currentDateTime().toString("hh:mm:ss ");
            in<<bufferStr + "\n";
            for(int i = 1;i < list.count() - 1;i++)
            {
               in<<QDateTime::currentDateTime().toString("hh:mm:ss ");
               in<<list.at(i) + "\n";
            }
            bufferStr = list.at(list.count() - 1);
        }
        if(bufferStr.length() >= 50)//必须换行
        {
            in<<QDateTime::currentDateTime().toString("hh:mm:ss ");
            in<<bufferStr + "\n";
            bufferStr.clear();
        }

        file.close();

    }

    emit recvAndSendbytesSignal();


}

void SerialAssistantDialog::pushButtonSendSlot()
{

    if(ui->radioButtonSendHex->isChecked())
    {
      QByteArray sendArr;
      bool ok;
      quint8 temp;
      QStringList strList = ui->textEdit->toPlainText().split(" ");
      QDataStream in(&sendArr,QIODevice::WriteOnly);
      QString str;
      foreach (str, strList)
      {
          temp = str.toInt(&ok,16);
          if(ok)
              in<<temp;
          sendbytes ++;
      }
      serialAss->writeAll(sendArr);

    }
    else
    {
        QString str = ui->textEdit->toPlainText().replace("\r","");
                serialAss->writeAll(str.toAscii());
                sendbytes +=  str.replace("\n","").length();
    }
    emit recvAndSendbytesSignal();
}
void SerialAssistantDialog::closeEvent(QCloseEvent *event)
{
    emit closeSignal();
    event->ignore();
}
void SerialAssistantDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
   // qDebug()<<"SerialAssistantDialog::mouseDoubleClickEvent";
    emit  independSignal();
    event->accept();
}
void SerialAssistantDialog::autoScroll()
{
    //qDebug()<<"recvbytes=="<<recvbytes<<maxLength;
    if(recvbytes > maxLength)
    {

        QString remainStr = ui->textBrowser->toPlainText();
        if(maxLength <= 1000)
        {
            recvbytes = 0;
            ui->textBrowser->clear();
        }
        else
        {
          remainStr.remove(0,maxLength - 1000);
          QString s = remainStr;
          recvbytes = s.replace("\n","").length();
          ui->textBrowser->clear();
          ui->textBrowser->insertPlainText(remainStr);

        }
    }
    else
       ui->textBrowser->moveCursor(QTextCursor::End);
}

void SerialAssistantDialog::recvSaveToFileSlot()
{
    if(ui->checkBoxRecvSave->isChecked())
    {
        QString fileName =  "./log/com_log/" +
                dateTimeFileName();

        file.setFileName(fileName);
        if(!file.exists() && !file.isOpen())
        {
            //qDebug()<<"Create new File:"<<fileName;
            if(!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
            {
                qDebug()<<"can't Create file!"<<fileName;
            }
        }
    }
}
QString SerialAssistantDialog::dateTimeFileName()
{

    QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd")
                         + ".txt";
    return fileName;
}


QString SerialAssistantDialog::readConfigCom()
{
    QSettings *read  = new QSettings("./config.ini",QSettings::IniFormat);
    QString str = read->value("SerialAss/COM").toString();
    delete read;

    return str;
}

void SerialAssistantDialog::writeConfigCom(const QString &str)
{
    QSettings *write  = new QSettings("./config.ini",QSettings::IniFormat);
    write->setValue("SerialAss/COM",str);
    delete write;

}

void SerialAssistantDialog::setPort()
{
    bool ok;
    quint8 index;
    index = readConfigCom().right(1).toInt(&ok,10);
    if(ok)
    {
      ui->comboBox->setCurrentIndex(index - 1);
    }

}

void SerialAssistantDialog::recvAndSendTimerSlot()
{
    ui->labelRecvSize->setText(QString("%1").arg(recvbytes));
    ui->labelSendSize->setText(QString("%1").arg(sendbytes));
}
