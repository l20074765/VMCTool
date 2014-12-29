#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTranslator>
#include "basedialog.h"
#include "titlewidget.h"
#include "mainmenu.h"
#include "abountusdialog.h"
#include "settingdialog.h"
#include "../data_export/printdialog.h"
#include "../system_para/sysparadialog.h"
#include "../aisle/aisleconfigdialog.h"
#include "../aisle/aislemanagementdialog.h"
#include "../SerialThead/serialthread.h"
#include "../SerialAssistant/serialassistantdialog.h"
#include "../VMCStatus/statusdialog.h"
#include "../HefanCabinet/hefandialog.h"


//宏定义
#define PC_REQUEST_TIMEOUT     10000UL
#define PC_REQUEST_TIMEOUT_CHUHUO  100000UL
#define PC_HEART_TIMEOUT  10000UL


class QStackedWidget;
class QMessageBox;

class MainWindow :public BaseDialog
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    inline void setConnectState(bool flag){vmcConnectFlag = flag;}
    inline bool getConnectState()const{return vmcConnectFlag;}
    inline void setTranslator(QTranslator *translator,QTranslator *translator_qt)
    {
        this->translator = translator;
        this->translator_qt = translator_qt;
    }
    void setLanguage(LANGUAGE language);
    void translateLanguage();

signals:
    void vmcConnectSignal();
    void vmcDisconnectSignal();

protected:
    void paintEvent(QPaintEvent *event);
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    QTranslator *translator;
    QTranslator *translator_qt;
    QStackedWidget *statkedWidget;

    TitleWidget *titleWidget; //标题栏
    MainMenu *mainMenu;    

    AbountUsDialog *abountUsDialog;
    //设置窗口
    SettingDialog *settingDialog;
    void  SettingDialogConnect();

    //售货机状态界面
    StatusDialog *statusDialog;
    void statusDialogConnect();

    //盒饭柜界面
    HefanDialog *hefanDialog;
    void hefanDialogConnect();

    LANGUAGE language;
    enum PCSENDER
    {
        OTHERDIALOG = 0,
        AISLEDIALOG,
        ONEKEYCONDIALOG,
        AISLECONFIGDIALOG,
        AISLEMANAGEMENTDIALOG,
        ONEKEYCON,
        HEFAN_DIALOG
    };
    PCSENDER pcSender;


    QTimer *pcRequestTimer;
    bool vmcConnectFlag;
    bool isGetSys;
    bool isGetHuodao;
    //serialThread
    SerialThread *serialThread;
    QTimer *serialTimer;
    //dataExport
    PrintDialog *printDialog;
    //systemParament
    SysParaDialog *sysParaDialog;
    //aisleConfig
    AisleConfigDialog *aisleConfigDialog;
    //aisleManagement
    AisleManagementDialog *aisleManagementDialog;
    //SerialAssitant
    SerialAssistantDialog *serialAssistantDialog;

    //stateDialog
    AisleTestdialog *stateDialog;
    QTimer *stateTimer;


    //dateDialog
    DateDialog *dateDialog;
    void dateDialogConnect();
    void serialThreadConnect();
    void printDialogConnect();
    void sysParaDialogConnect();
    void aisleConfigDialogConnect();
    void aisleManagementDialogConnect();
    void serialAssistantDialogConnect();

    //menu init
    void mainMenuInit();



private slots:
    void turnPage(int curIndex);
    void showMainMenuWidget();
    void showAbountUs();
    void showSettingDialog();

    //显示VMC状态界面
    void showVMCStatusSlot();

    void systemChangedSlot(const quint8 type,QByteArray arr);


    void vmcCabinetNoChangedSlot(int no);
    void printDialogOpenSlot(bool checked);
    //serial request slot
    void pcRequestSendSlot(quint8 type,QByteArray arr);
    void serialConnectSlot();
    void serialDisconnectSlot();
    void serialRptSlot(const quint8 type,const QByteArray &arr);

    void serialPrintSlot(const QByteArray &arr);//交易信息包
    void serialSumClearSlot(const QByteArray &arr);
    void serialSystemParaSlot(const QByteArray &arr);
    void serialHuoDaoSlot(const QByteArray &arr);
    void serialAisleGetSlot(const QByteArray &arrr);
    void serialAisleInitSlot(const QByteArray &arr);
    void serialAisleSetSlot(const QByteArray &arr);
    void serialAisleWRSlot(const QByteArray &arr);
    void serialAisleCloseSlot(const QByteArray &arr);
    void serialAisleTestSlot(const QByteArray &arr);
    void serialAddGoodsSlot(const QByteArray &arr);
    void serialDateSlot(const QByteArray &arr);
    void serialFactorySlot(const QByteArray &arr);
    void serialStatusSlot(const QByteArray &arr);
    void serialGetSelectSlot(const QByteArray &arr);
    void serialSetSelectSlot(const QByteArray &arr);
    void serialDebugSlot(const QByteArray &arr); //debug


    void serialTimeoutSlot();
    //
    void updateTable(quint8 levelNo);
    void pcRequestTimeoutSlot();

    //stateDialogSlot
    void stateDialogShowSlot(bool flag,const QString &text);
    void stateDialogShowSlot(bool flag,const QString &text,quint32 msec);
    void stateTimeroutSlot();

    //languageselect
    void languageSelectSlot(LANGUAGE language);
    void showCabinetSlot(bool showCabinet);

    void vmcUpdateSystemPageSLot(const bool flag);
    void vmcUpdateHuodaoPageSLot(const bool flag);

};

#endif // MAINWINDOW_H
