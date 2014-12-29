#ifndef STATUSDIALOG_H
#define STATUSDIALOG_H

#include <QDialog>
#include "../SerialThead/serialthread.h"
class QTextEdit;
class QLabel;
class QGroupBox;
class StatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatusDialog(QWidget *parent = 0);
    ~StatusDialog();


    void pcStatusRpt(const QByteArray &arr);
    void timerStatusReqStart();
    void vmcConnect(const bool connected){this->connected = connected;}
    bool isConnected() const {return this->connected;}
    void uiInit();
    void languageTranslation();
signals:
    void pcStatusReq(quint8 type,QByteArray arr);

protected:
    void hideEvent(QHideEvent *ev);
private:

    QTimer *timerStatusReq;
    QGroupBox *groupBoxVMC,*groupBill,*groupCoin,*groupCabine1;
    QLabel *labelVMCRunState;
    QLabel *labelBillState,*labelBillType;
    QLabel *labelCoinState,*labelCoinHopperState,*labelCoinRecvType,*labelCoinChangeType;
    QLabel *labelCabinet1RunState,*labelCabinet1FaultState;

    QLabel *labelVMCRunStateRst;
    QLabel *labelBillStateRst,*labelBillTypeRst;
    QLabel *labelCoinStateRst,*labelCoinRecvTypeRst,*labelCoinChangeTypeRst;
    QLabel *labelCoinHopperStateRst;
    QLabel *labelCabinet1RunStateRst,*labelCabinet1FaultStateRst;


    bool connected;
private slots:
    void timerStatusReqTimeoutSlot();


};

#endif // STATUSDIALOG_H
