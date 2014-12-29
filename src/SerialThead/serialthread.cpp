#include "serialthread.h"
#include <QtCore>
#include <QObject>

SerialThread::SerialThread(QObject *parent)
    :QThread(parent)
{
    printCom = new Win_QextSerialPort();
}

SerialThread::~SerialThread()
{
    printCom->deleteLater();
}
void SerialThread::run()
{
    pcRequestFlag  = false;
    pcType = 0x00;
    vmcStatus = false;
    setTime(COM_TIMEOUT);

    while(!stopped)
    {

        if(!vmcStatus)
        {
           if(connectVMC())
               setTime(COM_TIMEOUT);
            msleep(500);
        }
        else
        {
           if(serialRead())
               setTime(COM_TIMEOUT);
           if(!getTime())
               comDisconnectSLot();
            msleep(5);

        }
    }
}
bool SerialThread::init()
{
    //Read comconfig
    QSettings *configRead = new QSettings("config.ini",QSettings::IniFormat);
    QString portName = configRead->value("/Srial/COM").toString();
    QString configbaud = configRead->value("/Srial/BAUD").toString();
    qDebug()<<"COM"<<portName<<configbaud;
    delete configRead;
    if(printCom->isOpen())
        printCom->close();   
    if(!openCom(portName))
    {
        qDebug()<<"SerialThread::init() Open fail!";
        return false;
    }
    printCom->setQueryMode(QextSerialBase::Polling);
    //set
    if(configbaud == "9600")
       printCom->setBaudRate(BAUD9600);
    else if(configbaud == "19200")
       printCom->setBaudRate(BAUD19200);
    else if(configbaud == "115200")
       printCom->setBaudRate(BAUD115200);
    else
       printCom->setBaudRate(BAUD9600);

    printCom->setDataBits(DATA_8);
    printCom->setParity(PAR_NONE);
    printCom->setStopBits(STOP_1);
    printCom->setFlowControl(FLOW_OFF);
    printCom->setTimeout(10);
    return true;
}

//读串口数据
bool SerialThread::serialRead()
{
    if(!printCom->bytesAvailable())
    {
      return false;
    } 

    memset(recvbuf,0,sizeof(recvbuf));
    quint16 crc = 0;
    quint8 index = 0,len,head,MT;
    head = readCh();//head
    if(head != HEAD_EF)
    {
        qDebug("head = %x is not 0xEF\n",head);
        clearCOM();
        return false;
    }
    recvbuf[index++] = head;
    //等等包长
    quint8 lenTimeout = 50;
    while(lenTimeout)
    {
        if(printCom->bytesAvailable())
            break;
        else
            msleep(1);
    }
    if(!lenTimeout)
    {
        qDebug()<<"Recv msg len timeout";
        return false;
    }

    len = readCh();//len
    if(len < HEAD_LEN)
    {
        qDebug("len is %d < 4\n",len);
        clearCOM();
        return false;
    }
    recvbuf[index++] = len;

    // sn + MT + data + crc
    quint8 timeout = 200;//200ms
    while(timeout)
    {
        if(printCom->bytesAvailable())
        {
           recvbuf[index++] = readCh();
           if(index >= (len + 2))
               break;
        }
        else
        {
          timeout--;
          msleep(1);
        }
    }
    if(!timeout)
    {
        clearCOM();
         qDebug()<<QByteArray((char *)recvbuf,index).toHex().toUpper();
        qDebug()<<"readSerial timeOut..";
        return false;
    }

    if(snNo == recvbuf[2])//重包
    {
        qDebug("The sn is same sn=%x recv_sn=%x",snNo,recvbuf[2]);
        sendAck(PC_ACK);

        return true;
    }
    snNo = recvbuf[2] ;//sn
    MT = recvbuf[3];//MT
    crc = crcCheck(recvbuf,len);
    if(crc/256 != recvbuf[len] || crc % 256 != recvbuf[len + 1])
    {
        qDebug()<<"crc is err ..";
        return false;
    }

    if(MT == VMC_POLL)
        sendRequestToVMC();
    else
    {
        sendAck(PC_ACK);
        qDebug()<<QString("VMC-->PC(MT[%1]-SN[%2]-LEN[%3]):%4")
                  .arg(recvbuf[3],2,16,QLatin1Char('0'))
                  .arg(recvbuf[2],2,16,QLatin1Char('0'))
                  .arg(len + 2)
                  .arg(QString(QByteArray((char *)recvbuf,len + 2).toHex().toUpper()));

    }

    pcRecvArr = QByteArray((char *)recvbuf,sizeof(recvbuf));
    pcRecvArr.remove(0,HEAD_LEN);
    emit serialRptSignal(MT,pcRecvArr);//发送信号给主线程

    return true;
}

void SerialThread::sendRequestToVMC()
{
    QByteArray sendArr;
    QDataStream in(&sendArr,QIODevice::WriteOnly);
    QDataStream out(&pcSendArr,QIODevice::ReadOnly);
    quint8 temp;
    quint16 crc;
    if(pcRequestFlag)
    {
        qDebug()<<"rcx="<<rcx;
        //send fail
        if(rcx)
        {
            pcRequestMutex.lock();
            rcx--;
            pcRequestMutex.unlock();
        }
        else
        {
            setPcRequestFlag(false);
            return;
        }
        quint8 len = HEAD_LEN;
        in <<(quint8)HEAD_EF<<(quint8)HEAD_LEN<<(quint8)snNo<<(quint8)pcType;
        while(!out.atEnd())
        {
            out>>temp;           
            in<<temp;
            len++;
        }
        in.device()->seek(1);
        in<<(quint8)len;//重定位长度
        //crc
        crc = crcCheck(sendArr);
        in.device()->seek(len);
        in<<(quint16)crc;
        if(printCom->isWritable() && (sendArr.size() >= 4))
        {
            qDebug()<<QString("PC-->VMC(type[%1]-size[%2]):")
                      .arg(pcType,2,16,QLatin1Char('0'))
                      .arg(sendArr.size())
                  <<QString(sendArr.toHex().toUpper());


            printCom->write(sendArr);
            while(printCom->bytesToWrite())
            {
                msleep(1);
            }
        }
        else
        {
           qDebug()<<"Message is not right can't send to VMC:\n"
                  <<QString(sendArr.toHex().toUpper());
           setPcRequestFlag(false);

           return;
        }
        if(waiteVmcAck())//send OK
        {          
            setPcRequestFlag(false);
            qDebug()<<"Send OK...."<<this->pcRequestFlag;
        }

    }
    else
    {   
        //没有请求发送ACK
        sendAck(PC_ACK);

    }    
}
void SerialThread::comDisconnectSLot()
{
    vmcStatus = false;   
    setPcRequestFlag(false);
    emit serialDisconnectSignal();
}

void SerialThread::stopSerialThread()
{

    printCom->close();
    if(!this->isRunning())
        return;
    stopped = true;
    qDebug()<<"SerialThread::stopped="<<stopped;
    qDebug()<<"SerialThread::printCom->close()";

}
void SerialThread::startSerialThread()
{
   stopped = false;
   start();
}
bool SerialThread::connectVMC()
{
    if(printCom->isWritable())
    {
        sendbuf[0] = 0xEF;
        sendbuf[1] = 0x04;
        sendbuf[2] = 0x01;
        sendbuf[3] = START_RPT;
        sendbuf[4] = 0x0E;
        sendbuf[5] = 0x3F;
        printCom->write((char *)sendbuf,6);
    }
    quint32 timeout = 300;//300毫秒超时
    while(timeout)
    {
        if(this->serialRead())//返回真则表示连接上
        {
            vmcStatus = true;
            qDebug()<<"connected...";
            emit serialConnectSignal();
            return true;
        }
        else
        {
            timeout--;
        }
    }
    return false;

}

bool SerialThread::isPCRequest()
{
    bool temp;
    pcRequestMutex.lock();
    temp = pcRequestFlag;
    pcRequestMutex.unlock();
    qDebug()<<"isRequest="<<temp;
    return temp;
}


bool SerialThread::openCom(const QString &portName)
{   
    //首选
    printCom->setPortName(portName);
    if(printCom->open(QIODevice::ReadWrite) == 1)
    {
       return true;
    }
    else//所选串口打开失败
    {        
        qDebug()<<"open err:"<<portName<<"Serch the available COM...";
       //搜索串口
       QList<QextPortInfo> listPort = QextSerialEnumerator::getPorts();
        foreach(const QextPortInfo &portInfo,listPort)
        {
            printCom->setPortName(portInfo.portName.toLocal8Bit().constData());
            if(printCom->open(QIODevice::ReadWrite) == 1)
            {
                qDebug()<<portInfo.portName<<"open suc:";
                return true;
            }
            qDebug()<<portInfo.portName<<"open fail:";
        }
    }
    qDebug()<<"PC don't have any available serial port";
    return false;
}


QString SerialThread::getPortName()
{
   return printCom->portName();
}
QString SerialThread::getOpenPortInfo()
{
    return  QString("%1:%2 %3:%4")
            .arg(tr("Port"))
            .arg(printCom->portName())
            .arg(tr("Baud"))
            .arg(getBaud())
            ;
}

QString SerialThread::getBaud()
{
    QString baud;
    switch(printCom->baudRate())
    {
        case BAUD4800:
             baud = "4800";
             break;
        case BAUD9600:
             baud = "9600";
             break;
        case BAUD19200:
            baud = "19200";
             break;
        case BAUD38400:
            baud = "38400";
             break;
        case BAUD115200:
             baud = "115200";
             break;
        case BAUD256000:
             baud = "256000";
             break;
        default:
        baud = "";
           break;
    }

    return baud;
}

quint16 SerialThread::crcCheck(const QByteArray &dataArr)
{
    QByteArray arr = dataArr;
    QDataStream out(&arr,QIODevice::ReadOnly);
    quint8 temp;
    unsigned short j, crc = 0, current = 0;
    while(!out.atEnd())
    {
        out >> temp;
        current = temp << 8;
        for(j=0;j<8;j++) {
            if((short)(crc^current)<0)
                crc = (crc<<1)^0x1021;
            else
                crc <<= 1;
            current <<= 1;
        }
    }
    return crc;
}
quint16 SerialThread::crcCheck(unsigned char *msg,unsigned char len)
{
    unsigned short i, j, crc = 0, current = 0;
        for(i=0;i<len;i++) {
            current = msg[i] << 8;
            for(j=0;j<8;j++) {
                if((short)(crc^current)<0)
                    crc = (crc<<1)^0x1021;
                else
                    crc <<= 1;
                current <<= 1;
            }
        }
        return crc;

}


unsigned char SerialThread::readCh()
{
    char ch = 0;
   // printCom->read_data((char *)&ch,1);
    printCom->getChar(&ch);
    return (unsigned char)ch;
}


void SerialThread::clearCOM()
{
    printCom->readAll();
  // printCom->clear();
}

bool SerialThread::waiteVmcAck()
{

    QByteArray arr;
    QDataStream in(&arr,QIODevice::ReadWrite);
    quint16 timeout = 300;
    while(timeout)
    {
        if(printCom->bytesAvailable())
        {
            in<<(quint8)readCh();
            if(arr.size() >= (HEAD_LEN + 2))
                break;
        }
        else
        {
            timeout--;
            msleep(1);
        }

    }
    if(!timeout)
    {
       qDebug()<<"recvACK TimeOut";
       return false;
    }
    if((quint8)arr.at(0) == HEAD_EF  &&
       (quint8)arr.at(1) == HEAD_LEN &&
       (quint8)arr.at(2) == snNo     &&
       (quint8)arr.at(3) == VMC_ACK    )
    {
        qDebug()<<"recv ACK="<<QString(arr.toHex()).toUpper();
        return true;
    }
    else
    {
         qDebug()<<"recv err"<<QString(arr.toHex()).toUpper();
         return false;
    }

}

void SerialThread::sendAck(quint8 ACK)
{
    sendbuf[0] = 0xEF;
    sendbuf[1] = 4;
    sendbuf[2] = snNo;
    sendbuf[3] = ACK;
    quint16 crc = crcCheck(sendbuf,4);
    sendbuf[4] = crc / 256;
    sendbuf[5] = crc % 256;

    printCom->write((char *)sendbuf,6);
//    qDebug()<<QString("PC-->VMC(MT[%1]-SN[%2]-LEN[%3]):%4")
//              .arg(ACK,2,16,QLatin1Char('0'))
//             .arg(snNo,2,16,QLatin1Char('0'))
//             .arg(6)
//              .arg(QString(QByteArray((char *)sendbuf,6).toHex().toUpper()))
//             ;


}




