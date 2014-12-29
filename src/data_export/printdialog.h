#ifndef PRINTDIALOG_H
#define PRINTDIALOG_H

#include <QDialog>
#include <QFile>
#include "../comconfigdialog.h"
#include "printinfodialog.h"
#include "../SerialThead/serialthread.h"
#include "../dialog/pushbutton.h"
#include "../config.h"

//字体导出长度 DXP  data export
#define DXP_USERNO_L        8
#define DXP_DATE_L          16
#define DXP_AISLENO_L       8
#define DXP_GOODSNO_L       8
#define DXP_PRICE_L         8
#define DXP_PAYMODE_L       8
#define DXP_RST_L           8
#define DXP_RECVBILL_L      18
#define DXP_RECVCOIN_L      18
#define DXP_CHANGE_L        18
#define DXP_RECVCARD_L      18




namespace Ui {
class PrintDialog;
}


enum PRINTSTATE
{
    PrintReady,
    Printting,
    PrintPause

};
class PrintDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PrintDialog(QWidget *parent = 0);
    ~PrintDialog();
    void init();
    void printInit();
    void printStatusSet(PRINTSTATE state);
    PrintInfoDialog *printInfoDialog;
    ComConfigDialog *comConfigDialog;
    quint16  maxTradeIndex,curTradeIndex,curReadIndex,startTradeIndex,endTradeIndex;
    quint16  printNum;
    bool printFlag;
    bool isSuperMode();
    void changeMode();
    void pcRequestFail();
    void openComInfo(const QString &portName);
    void labelStatusSetText(const QString &text);
    void translateLanguage();
signals:
    void recvSystemParaSignal(QByteArray arr);
    void printDialogOpenSignal(bool checked);
    void printRequestSignal(quint8 type,QByteArray arr);
public slots:
    void textBrowserSlot();
    void comOpenOrClose(quint8 flag);
    void vmcDisconnect();
    void vmcConnect();
    void sendPrintTraderequest();
    void recvPrintTotalInfo(QByteArray arr);
    void recvPrintTradeInfo(QByteArray arr);
    void recvSumClear(QByteArray arr);
    //button clicked
    void open_pushButton_clicked(bool  checked);
    void config_pushButton_clicked();
    void startPrint_pushButton_clicked();
    void stop_pushButton_clicked(bool checked);
    void tool_pushButton_clicked();
    void printInfoDialogSlot();
    void clearSum_pushButton_clicked();
    void serachPortClickedSlot();
private:
     Ui::PrintDialog *ui;
     QString printStr;
     QString printHeadStr;
     quint8 printcx;
     QString dateTimeFileName();


    // quint8  printLanguage;
     PRINTSTATE printStatus;
     QFile file;
protected:
    void paintEvent(QPaintEvent *event);
private slots:

};

#endif // PRINTDIALOG_H
