#ifndef SERIALASSISTANTDIALOG_H
#define SERIALASSISTANTDIALOG_H

#include <QDialog>
#include "userial.h"
#include "../dialog/pushbutton.h"

namespace Ui {
class SerialAssistantDialog;
}

class SerialAssistantDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SerialAssistantDialog(QWidget *parent = 0);
    ~SerialAssistantDialog();

    QString dateTimeFileName();

    void translate();

signals:
    void closeSignal();
    void independSignal();
    void serialAssWrite(const QByteArray &sendArr);
    void recvAndSendbytesSignal();
private:
    Ui::SerialAssistantDialog *ui;
    USerial *serialAss;

    QFile file;
    quint64 recvbytes,sendbytes;
    quint64 maxLength;
    QString bufferStr;//写文件缓冲区
    void setPort();
protected:
    void closeEvent(QCloseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void pushButtonOpenSlot(bool checked);
    void recvDataSlot(const QByteArray &readArr);
    void pushButtonSendSlot();

    void autoScroll();

    void recvSaveToFileSlot();
    void recvAndSendTimerSlot();

    QString readConfigCom();
    void writeConfigCom(const QString &str);

    inline void clearRecvBytes(){recvbytes = 0;}
    inline void clearSendBytes(){sendbytes = 0;}
};

#endif // SERIALASSISTANTDIALOG_H
