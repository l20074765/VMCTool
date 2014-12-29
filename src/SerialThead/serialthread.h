#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H
#include <QObject>
#include <QThread>
#include <QTimer>
#include "../serial/win_qextserialport.h"
#include "../serial/qextserialenumerator.h"
//#include <QSerialPort>
//#include <QSerialPortInfo>
#include <QMutex>
//串口协议模式
enum PMODE
{
    JP_EF,
    UBOX_E7

};
#define COM_TIMEOUT    10 //10秒级定时器 由主线程控制


#define HEAD_EF       0xEF
#define HEAD_LEN        4
#define CRC_LEN         2
/*************************************************************************************
*PC请求类型
*
*************************************************************************************/
#define PC_ACK          0x80
#define PC_NAK          0x81
#define START_REQ       0x82
#define TRADE_REQ       0x83
#define CLEAR_REQ       0x84
#define SYSPARA_REQ     0x85
#define HUODAO_REQ      0x86
#define FACTORRECOVER_REQ  0x87
#define DATETIME_REQ    0x88
#define STATUS_REQ       0x89
#define DEBUG_REQ        0x8A
#define FACTOR_SET_REQ  0x8B





#define GET_HUODAO_REQ  0x00
#define SET_HUODAO_REQ  0x01
#define INIT_HUODAO_REQ 0x02
#define WRITE_HUODAO_REQ 0x03
#define ONOFF_HUODAO_REQ 0x04
#define ADD_HUODAO_REQ   0x05
#define TEST_HUODAO_REQ 0x06
#define GET_HUODAO_SELECT_REQ 0x07
#define SET_HUODAO_SELECT_REQ 0x08

/*************************************************************************************
*PC请求类型包数据长度 只包括内容数据
*
*************************************************************************************/
#define START_REQ_LEN 0
#define TRADE_REQ_T0_LEN 1 //总交易获取
#define TRADE_REQ_T1_LEN 3   //详细交易记录




#define START_RPT_LEN 0
#define TRADE_RPT_T0_LEN 27 //总交易获取
#define TRADE_RPT_T1_LEN 3   //详细交易记录
/*************************************************************************************
*VMC MT类型
*
*************************************************************************************/
#define VMC_ACK         0x00
#define VMC_NAK         0x01
#define VMC_POLL        0x02
#define VMC_SEEK        0x03
#define START_RPT       0x82
#define TRADE_RPT       0x83
#define CLEAR_RPT       0x84
#define SYSPARA_RPT     0x85
#define HUODAO_RPT      0x86
#define FACTORRECOVER_RPT  0x87
#define DATETIME_RPT    0x88
#define STATUS_RPT       0x89
#define DEBUG_RPT        0x8A
#define FACTOR_SET_RPT  0x8B

/*************************************************************************************
*VMC发送包数据长度 只包括内容数据
*
*************************************************************************************/


#define VMCTIMEOUT    500ul

class SerialThread :public QThread
{
       Q_OBJECT
public:
    SerialThread(QObject *parent = 0);
    ~SerialThread();
    bool init();
    void stopSerialThread();
    void startSerialThread();
    bool serialRead();
    inline bool vmcComIsOpen(){return printCom->isOpen();}
    inline void updatePcCmdType(quint8 type){pcType = type;}
    inline void updatePcSendArr(QByteArray sendArr){pcSendArr = sendArr;}
    inline void setPcRequestFlag(bool flag)
    {
        rcx = 3;
        pcRequestMutex.lock();
        pcRequestFlag = flag;       
        pcRequestMutex.unlock();

    }
    inline bool vmcisConnected(){return vmcStatus;}
    bool isPCRequest();
    bool openCom(const QString &portName);
    QString getPortName();
    QString getOpenPortInfo();

    inline void setTime()
    {
       timeMutex.lock();
       if(this->timeout)
            this->timeout--;
       timeMutex.unlock();
    }
    inline void setTime(quint32 timeout)
    {
       timeMutex.lock();
       this->timeout = timeout;
       timeMutex.unlock();
    }
    inline quint32 getTime()const
    {
       return this->timeout;
    }

    QString getBaud();

signals:

    void serialConnectSignal();
    void serialDisconnectSignal();   
    void serialRptSignal(const quint8 type,const QByteArray &arr);


protected:
    void run();
private:     
     Win_QextSerialPort *printCom;
     bool vmcStatus;
     bool stopped;
     QMutex pcRequestMutex;
     bool  pcRequestFlag;
     quint8 pcType;
     quint8 rcx;
     QByteArray pcSendArr,pcRecvArr;
     quint8 snNo;

     QMutex timeMutex;
     quint32 timeout;
     bool connectVMC();
     unsigned char recvbuf[256];
     unsigned char sendbuf[256];
     quint16 crcCheck(const QByteArray &dataArr);
     quint16 crcCheck(unsigned char *msg,unsigned char len);
     unsigned char readCh();   
     void clearCOM();
     bool waiteVmcAck();
     void sendAck(quint8 ACK = PC_ACK);

private slots:
     void sendRequestToVMC();
     void comDisconnectSLot();

};

#endif // SERIALTHREAD_H
