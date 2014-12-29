#ifndef SYSPARADIALOG_H
#define SYSPARADIALOG_H

#include <QDialog>
#include "../dialog/datedialog.h"
#include "../config.h"



namespace Ui {
class SysParaDialog;
}

class SysParaDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SysParaDialog(QWidget *parent = 0);
    ~SysParaDialog();
     void pcRequestFail();
     void SystemParaSetResult(bool flag);

     QString priceFormat(const QString &str);

     void translateLanguage();
     void setFontAndColor();


     quint8 encodAmount(quint32 amount);
     quint32 analysisAmount(quint8 data);

     enum SYS_CHANGED{
         HUODAO_TYPE,  //系统参数 主柜货道类型改变（升降机/弹簧）
         HUODAO_SUB_TYPE,
         HUODAO_MAX_COLUMN,        // 8 or  10
         HUODAO_SUB_MAX_COLUMN,
         PC_ENABLED,
         SELECT_BOARD,
         CABINET_SUB_OPEN,
         HEFAN_ENABLED  ,//盒饭柜
         VMC_SCALE  //金额比率
     };
signals:

    void systemParaRequestSignal(quint8 type,QByteArray arr);
    void resultSignal(bool flag,QString text);
    void systemChangedSignal(const quint8 type,QByteArray arr);
private:
    Ui::SysParaDialog *ui;
    enum PTEXT{SYSGET,SYSGETTING,SYSGETSUC,SYSGETFAIL,
               SYSSET,SYSSETTING,SYSSETSUC,SYSSETFAIL,
              FACTORY,FACTORYSETTING,FACTORYSUC,FACTORYFAIL,
              DATESET,DATESETTING,DATESETSUC,DATESETFAIL};


    PTEXT processState;
    DateDialog *dateDialog;
    QString processText(PTEXT pText);
public slots:
    void on_save_pushButton_clicked();
    void on_recover_pushButton_clicked();
    void recvSystemParaSlot(QByteArray arr);

    void dateSetSlot(QByteArray arr);
    void factorySetSlot(QByteArray arr);
    void setFactorySlot(QByteArray arr);
    void vmcConnect();
    void vmcDisconnect();

    void superMode(bool flag);

    void initLayout();
private slots:
    void on_defaultSet_pushButton_clicked();
    void on_pushButtonSetDate_clicked();
    void pushButtonFactoryResetClicked();

    void subBinOpenSlot(int index);
    void userSelectKeyBoardSlot(int index);
    void pcEnableSlot(int index);
    void channelTypeSLot(int index);
    void hefanOpenSlot(int index);
    void vmcScaleSlot(int index);
    void lineEditChangedSlot();
};

#endif // SYSPARADIALOG_H
