#include "userial.h"
#include <QtCore>
USerial::USerial(QObject *parent)
     :QThread(parent)
{
    serialPort = new Win_QextSerialPort;
}


USerial::~USerial()
{
    serialPort->deleteLater();
}

void USerial::run()
{

    while(!stopped)
    {
        if(serialPort->bytesAvailable())
        {
            readAll();
        }
        msleep(50);
    }
}
void USerial::stopSerialThread()
{

    if(!this->isRunning())
        return;
    stopped = true;
    serialPort->close();
}
void USerial::startSerialThread()
{
   stopped = false;
   start();
}
bool USerial::init(const QString &portName)
{
    if(serialPort->isOpen())
         serialPort->close();
    serialPort->setQueryMode(QextSerialBase::Polling);
    serialPort->setPortName(portName);
    if(!openCom(portName))
    {     
        return false;
    }
    //set
    serialPort->setBaudRate(BAUD9600);
    serialPort->setDataBits(DATA_8);
    serialPort->setParity(PAR_NONE);
    serialPort->setStopBits(STOP_1);
    serialPort->setFlowControl(FLOW_OFF);
    serialPort->setTimeout(10);
    return true;
}

void USerial::readAll()
{

    readArr = serialPort->readAll();
    if(readArr.isEmpty())
        return;
    emit recvDataSignal(readArr);
}

void USerial::writeAll(const QByteArray &writeArr)
{
    if(serialPort->isWritable())
    {
        serialPort->write(writeArr);        
    }
    else
    {
        qDebug()<<tr("can't write to serialport!");
    }
}

bool USerial::openCom(const QString &portName)
{
    if(serialPort->open(QIODevice::ReadWrite) == 1)
    {
       return true;
    }
    else
    {
        qDebug()<<"open err:"<<portName<<"Serch the available COM...";
        QString name,name1;
        for(int i = 0;i < 18;i++)
        {
            name = QString("COM%1").arg(i);         
            qDebug()<<"open:"<<name;
            serialPort->setPortName(name);
            if(serialPort->open(QIODevice::ReadWrite) == 1)
            {
                return true;
            }
            name1 = "\\\\.\\" + name;
            serialPort->setPortName(name1);
            if(serialPort->open(QIODevice::ReadWrite) == 1)
            {
                qDebug()<<"open suc:";
                return true;
            }

        }
    }
    qDebug()<<"PC don't have any available serial port";
    return false;
}

QString USerial::getPortName()
{
   return serialPort->portName();
}
