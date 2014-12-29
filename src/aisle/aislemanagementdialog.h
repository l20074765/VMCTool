#ifndef AISLEMANAGEMENTDIALOG_H
#define AISLEMANAGEMENTDIALOG_H
#include "../config.h"
#include "../SerialThead/serialthread.h"
#include <QDialog>
#include "addgoodsdialog.h"
#include "aisletestdialog.h"



class QSignalMapper;
namespace Ui {
class AisleManagementDialog;
}

typedef struct _goods_table_st
{
    quint8 aisle_no;
    quint8 id;
    quint8 key_no;
    quint8 price;
    quint8 remain;
    quint8 max_count;
    quint8 state;
    quint8 test;

}GOODS_TABLE_ST;


class AisleManagementDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AisleManagementDialog(QWidget *parent = 0);
    ~AisleManagementDialog();

    void pcConGoodsRpt(const QByteArray &arr);
    void pcAddGoodsRpt(const QByteArray &arr);
    void pcWriteRpt(const QByteArray &arr);
    //void pcGetAisleRpt(const QByteArray &arr);
    void pcTestAisleRpt(const QByteArray &arr);
    void pcConSelectBoardRpt(const QByteArray &arr);
    void sendGetAisle();
    void sendSetAisle();
    void readGoodsDialogInfo();
    void showCabinet(const bool isShow);
    void cabinetCurindexChanged(int no);
    void addIntem(int row,int column,const QString &text);
    void translate();
    void initGoodsInfo();

signals:
    void goodsRequestSignal(quint8,QByteArray);
    void getGoodsInfoSignal();
    void processTextSignal(bool flag,QString text);
    void vmcCabinetNoChangedSignal(int no);
private:
    Ui::AisleManagementDialog *ui;
    quint8 leveNo;
    quint8 aisleType;
    quint8 rowIndex;

    GOODS_TABLE_ST goods_table_st;
    //quint8 aisle_no,id,key_no,price,remain,max_count,state,test;
    AddGoodsDialog *addGoodsDialog;
    QString curText;
    QSignalMapper *signalMapper;

    //testDialog
    AisleTestdialog *aisleTestdialog;
    QTimer *testTimer;
    //allTest
    bool isAllTest;
    bool stoppedAllTest;
    quint8 testAisleNo;


    quint8 column;
    enum PTEXT{SET,SETTING,SETFAIL,SETSUC,SAVE,SAVING,SAVESUC,GET,GETTING,GETSUC,GETFAIL,
              ADD,ADDING,ADDFAIL,ADDSUC,CONSELECTTING};

    QString processText(PTEXT pText);
    QString processText(PTEXT pText,quint8 levelNo);

public slots:
    void cellDoubleClickedSlot( int row, int column );

    void requestTimeoutSlot();
    void pcConGoodsReq();
    void pcAddGoodsReq(QByteArray arr);
    void pcGetGoodsReq();
    void pcConSelectBoardReq();
    void upGoodsDialog(quint8 levelNo);
    void setButtonEnable(const bool flag);
    void setVMCState(const bool flag);
    void vmcConnect();
    void vmcDisconnect();

    void goodsInfoSort(int index);
private slots:
    void cellChangedSlot(int row, int column);

    void cellClickedSlot(int row, int column);

    void testAisleSlot(QString row_no);
    void testTimeroutSLot();
    void allTestSlot(bool checked);
};

#endif // AISLEMANAGEMENTDIALOG_H
