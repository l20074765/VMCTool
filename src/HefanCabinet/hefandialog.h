#ifndef HEFANDIALOG_H
#define HEFANDIALOG_H


#include "../dialog/basedialog.h"
#include "../SerialThead/serialthread.h"
#include "../dialog/pushbutton.h"

#include "../config.h"



class QSignalMapper;
class QTableWidget;
class QLabel;
class QComboBox;
class AisleTestdialog;

class HefanDialog : public QDialog
{
     Q_OBJECT
public:
    HefanDialog(QWidget *parent = 0);
    void languageTranslation();
    void tableInit();
    void comBoxcabinetInit();
    void setButtonEnable(const bool flag);

    void setConnect(const bool flag = true);

    enum PROCESSTEXT
    {
        INIT,INITING,INITSUC,INITFAIL,
        CLOSING,CLOSESUC,CLOSEFAIL,
        SAVING,SAVESUC,SAVEEFAIL,
        GETTING,GETSUC,GETFAIL,
        SET,SETTING,SETSUC,SETFAIL,
        ADDING,ADDSUC,ADDFAIL,ADD_TIP,
        CABINET_N_A
    };

    enum FLOW
    {
        IDLE,INITBOX,SAVEBOX,GETBOX,SETBOX,TESTBOX,ADDBOX
    };

    QString processinfoText(PROCESSTEXT text);
    QString textTips(PROCESSTEXT pText);

    void tableGoodsUpdate();
    void tableReadInfo();

signals:
    void pcRequestSignal(quint8 type,QByteArray arr);
    void processTextSignal(bool flag,QString text);

protected:
    void paintEvent(QPaintEvent *event);
private:

    PushButton *pushButtonHefanGET;//读取格子
    PushButton *pushButtonHefanSET;//设置格子
    PushButton *pushButtonHefanTest;//测试全部格子
    PushButton *pushButtonHefanAdd;//添货
    PushButton *pushButtonOneKey;//一键配置

    PushButton *pushButtonInit;

    QLabel *labelTitle;
    QComboBox *comboBoxCabinet;
    //tableWidget
    QTableWidget *tableWidget;

    QSignalMapper *signalMapper;
    AisleTestdialog *aisleTestdialog;
    QTimer *testTimer;

    quint8 flowFlag;
    quint8 levelNo;
    quint8 cabinetNo;
    bool isExsit;
    bool isConnected;
    bool isAllTest;
    quint8 testAisleNo;
    bool stoppedAllTest;
private:


private slots:
    void pushButtonClicked();
    void getBoxRequest(quint8 levelNo);
    void setBoxRequest(quint8 levelNo);
    void getBoxRpt(QByteArray arr);
    void setBoxRpt(QByteArray arr);
    void saveBoxRpt(QByteArray arr);
    void addBoxRpt(QByteArray arr);
    void initBoxRpt(QByteArray arr);
    void cabinetNoChangedSlot(int no);

    void cellChangedSlot(int row,int column);

    void testBoxSlot(QString row_no);
    void allTestSlot(bool checked);
    void pcTestAisleRpt(const QByteArray &arr);
    void testTimeroutSLot();


public slots:
    void huodaoRptSlot(const QByteArray &arr);

};

#endif // HEFANDIALOG_H
