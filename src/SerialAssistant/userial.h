#ifndef USERIAL_H
#define USERIAL_H
#include <QObject>
#include <QThread>
#include <QTimer>
#include "../serial/win_qextserialport.h"
class USerial :public QThread
{
    Q_OBJECT
public:
    USerial(QObject *parent = 0);
    ~USerial();

    bool init(const QString &portName);

    void stopSerialThread();
    void startSerialThread();
    bool openCom(const QString &portName);

    QString getPortName();
signals:
    void recvDataSignal(const QByteArray &readArr);
protected:
    void run();


private:
    Win_QextSerialPort *serialPort;

    bool stopped;

    QByteArray readArr,sendArr;
    void readAll();

public slots:
    void writeAll(const QByteArray &writeArr);
};

#endif // USERIAL_H
