#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H
#include "basedialog.h"
#include "pushbutton.h"
#include "vmcdebugdialog.h"

#include "../SerialThead/serialthread.h"
class QLabel;
class SettingDialog :public BaseDialog
{
    Q_OBJECT
public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();

    void translateLanguage();

    void vmcDebugInit();


signals:
    void setRequestSignal(quint8 type,QByteArray arr);
public slots:
    void updateDebug(QByteArray arr);
    void updateDebug(const quint8 type,const bool checked);

    bool isConnect()const {return connected;}
    void setConnect(const bool connected){this->connected = connected;}
protected:
    void paintEvent(QPaintEvent *event);
  //  void hideEvent(QHideEvent *ev);
private:
    //基本框架
    QLabel *titleLabel;//标题
    QLabel *titleIconLabel;//标题图标
    PushButton *closeButton;
    QPushButton *okButton;
    QList<QLabel *> labelList;
    VMCDebugDialog *vmcDebugDialog;


    bool connected;
private slots:
    void setRequestSlot(quint8 type,QByteArray arr);
    void okAceept();

};

#endif // SETTINGDIALOG_H
