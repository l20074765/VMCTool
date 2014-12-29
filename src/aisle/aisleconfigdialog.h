#ifndef AISLECONFIGDIALOG_H
#define AISLECONFIGDIALOG_H
#include "aisleconfigbuttondialog.h"
#include "../config.h"
#include "../SerialThead/serialthread.h"
#include <QDialog>


enum PROCESSTEXT
{
    INITING,
    INITSUC,
    INITFAIL,
    CLOSING,
    CLOSESUC,
    CLOSEFAIL,
    CONFIGING,
    CONFIGSUC,
    CONFIGFAIL,
    SAVING,
    SAVESUC,
    SAVEEFAIL,
    GETTING,
    GETSUC,
    GETFAIL,
    CONSELECT,
    CONSELECTTING,
    GETTINGSELECT,
    READFAIL
};



namespace Ui {
class AisleConfigDialog;
}

class AisleConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AisleConfigDialog(QWidget *parent = 0);
    ~AisleConfigDialog();
    void setLeveButtonOpen(bool flag,quint8 groupNo);
    void setLeveButtonStore(QString text,quint8 groupNo);

    void pcInitAisleRpt(const QByteArray &arr);
    void pcConAisleRpt(const QByteArray &arr);
    void pcGetAisleRpt(const QByteArray &arr);
    void pcCloseAisleRpt(const QByteArray &arr);
    void pcWriteRpt(const QByteArray &arr);
    void pcConSelectBoardRpt(const QByteArray &arr);
    void pcGetSelectBoardRpt(const QByteArray &arr);
    void sendGetAisle();
    void sendConAisleInfo();

    bool getAisleindex(quint8 *index_i, quint8 *index_j,quint8 physicNo);
    void upAisleDialog(quint8 leveNo);
    void readAisleDialog();
    bool readAisleOneKeyCon(const QString &fileName,const quint8 cabinet);
    void setLeveNo(const quint8 no)
    {
        leveNoMutex.lock();
        leveNo = no;
        leveNoMutex.unlock();
    }

    quint8 getLeveNo()
    {
        quint8 no;
        leveNoMutex.lock();
        no = leveNo;
        leveNoMutex.unlock();
        return no;
    }
    void showCabinet(const bool isShow);
    void cabinetCurindexChanged(int no);

    QString processinfoText(PROCESSTEXT text);
    QString processinfoText(PROCESSTEXT text,quint8 leveNo);
    void translateLanguage();

    enum AISLE_BUTTON
    {
        INIT = 1,
        POS,
        SHOW,
        HIDE
    };

signals:
     void aisleRequestSignal(quint8,QByteArray);
     void oneKeyConfigSignal(QString fileName);
     void getAisleLevelNo(quint8 levelNo);
     void processTextSignal(bool flag,QString text);
     void vmcCabinetNoChangedSignal(int no);
protected:
     //void paintEvent(QPaintEvent *event);
private:
    Ui::AisleConfigDialog *ui;
    QList<AisleConfigButtonDialog *> buttonList;
    quint8 leveNo;
    QMutex leveNoMutex;
    quint8 groupW,groupH,aisleW,aisleH;
    void initAisleButton(quint8 type);
    bool isAllBinConfig;
    QString aisleFileName;

public slots:
    void openClickedSlot(bool checked);

    void pcInitAisleReq();
    void pcOneKeyConReq();
    void pcConAisleReq();
    void pcGetAisleReq();
    void pcCloseAisleReq();
    void pcConSelectBoardReq();
    void pcGetSelectBoardReq();
    void pcRequestTimeout();
    void setButtonEnable(const bool flag);
    void setVMCState(const bool flag);
    void vmcConnect();
    void vmcDisconnect();
    void maxCountChanged(QString text);
    void actionTestSlot(const bool allTest,const AisleConfigButtonDialog &button);

    

};

#endif // AISLECONFIGDIALOG_H
