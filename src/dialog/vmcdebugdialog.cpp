#include "vmcdebugdialog.h"
#include "ui_vmcdebugdialog.h"
#include <QtGui>
VMCDebugDialog::VMCDebugDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VMCDebugDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    int w,h;
    w = 80,h = 30;

    listButton<<ui->widgetMain<<ui->widgetBill<<ui->widgetCoin
              <<ui->widgetChange<<ui->widgetCard<<ui->widgetColumn
              <<ui->widgetComPressor<<ui->widgetLed
              <<ui->widgetSelect<<ui->widgetPC;

    QSignalMapper *signalMapper = new QSignalMapper(this);
    for(int i = 0;i < listButton.count();i++)
    {
        listButton.at(i)->setFixedSize(w,h);
        listButton.at(i)->resize(w,h);
        connect(listButton.at(i),SIGNAL(clicked()),
                signalMapper,SLOT(map()));
        signalMapper->setMapping(listButton.at(i),i);
    }
    connect(signalMapper,SIGNAL(mapped(int)),this,SLOT(buttonClickedSlot(int)));

    QFont font = this->font();
    font.setBold(true);
    font.setPointSize(10);
    QList<QLabel*> list;
    list<<ui->label_main<<ui->label_bill<<ui->label_coin<<ui->label_changer<<ui->label_card
       <<ui->label_column<<ui->label_compressor<<ui->label_led<<ui->label_select<<ui->label_pc;
    foreach(QLabel *label,list)
    {
        label->setFont(font);
    }


    font.setPointSize(12);
    ui->groupBox->setFont(font);

    translateLanguage();
}

VMCDebugDialog::~VMCDebugDialog()
{
    delete ui;
}

void VMCDebugDialog::translateLanguage()
{
    ui->retranslateUi(this);
    foreach(HDButton *button,listButton)
    {
        button->setText(tr("ON"),tr("OFF"));
    }

    QList<QLabel*> list;
    list<<ui->label_main<<ui->label_bill<<ui->label_coin<<ui->label_changer<<ui->label_card
       <<ui->label_column<<ui->label_compressor<<ui->label_led<<ui->label_select<<ui->label_pc;
    QStringList listText;
    listText<<tr("MainSwitch:")<<tr("BillSwitch:")<<tr("CoinSwitch")<<tr("ChangeSwitch:")<<tr("CardSwitch:")
            <<tr("ColumnSwitch:")<<tr("CompressorSwitch:")<<tr("LedSwitch:")<<tr("SelectSwitch:")<<tr("PCSwitch:");

    int i = 0;
    foreach(QLabel *label,list)
    {
        label->setText(listText.at(i++));
    }
    ui->groupBox->setTitle(tr("VMC Debug Switch"));
    ui->groupBox->setObjectName("debug");

}
void VMCDebugDialog::updateDebug(QByteArray arr)
{
    //qDebug()<<"VMCDebugDialog::updateDebug"<<arr.toHex();
    quint8 index = 0;
    bool checked;
    foreach(HDButton *button,listButton)
    {
        checked = ((quint8)arr.at(index) == 1) ? true:false;
        //qDebug()<<"button="<<button<<checked;
        if(index == 0)//总开关
        {
            if(!checked)//总调试开关都关闭了
            {
                setSwicth(false);
                return;
            }
        }
        button->setChecked(checked);
        index++;
    }


}


void VMCDebugDialog::updateDebug(const quint8 type, const bool checked)
{
    //qDebug()<<"VMCDebugDialog::updateDebug="<<type<<checked;
    quint8 buttonNo = type - 1;
    if(buttonNo >= listButton.count())
    {
        //qDebug()<<"updateDebug:buttonNo >= listButton.count()"<<buttonNo<<listButton.count();
        return;
    }
    listButton.at(buttonNo)->setChecked(checked);
}
void VMCDebugDialog::setSwicth(const bool checked)
{
    foreach(HDButton *button,listButton)
    {
        button->setChecked(checked);
    }
}


void VMCDebugDialog::buttonClickedSlot(int id)
{

    qDebug()<<"buttonClickedSlot"<<id;
    quint8 buttonNo = (quint8)id;
    if(buttonNo >= listButton.count())
    {
        return;
    }
    bool checked = listButton.at(buttonNo)->isChecked();
    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);
    in<<(quint8)(buttonNo + 1);
    in<<(quint8)checked;
    emit debugRequstSignal(DEBUG_REQ,arr);
}
