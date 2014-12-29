#include "addgoodsdialog.h"
#include "ui_addgoodsdialog.h"
#include <QtGui>
AddGoodsDialog::AddGoodsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddGoodsDialog)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint);

    leveHide();
    aisleHide();
    setModal(true);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->radioButtonAll,0);
    buttonGroup->addButton(ui->radioButtonLeve,1);
    buttonGroup->addButton(ui->radioButtonAisle,2);
    buttonGroup->setExclusive(true);
    ui->radioButtonAll->setChecked(true);

    connect(ui->pushButtonOK,SIGNAL(clicked()),
            this,SLOT(pushButtonOKClicked()));

    connect(ui->radioButtonLeve,SIGNAL(clicked()),
            this,SLOT(leveShow()));
    connect(ui->radioButtonAll,SIGNAL(clicked()),
            this,SLOT(leveHide()));

    connect(ui->radioButtonAll,SIGNAL(clicked()),
            this,SLOT(aisleHide()));

    connect(ui->radioButtonAisle,SIGNAL(clicked()),
            this,SLOT(aisleShow()));


    QRegExp regExp("[1-8]");
    ui->lineEditLeve->setValidator(new QRegExpValidator(regExp,this));
    ui->lineEditLeve->setText("1");

    QRegExp regExp1("[1-8][0-9]");
    ui->lineEditAisleNo->setValidator(new QRegExpValidator(regExp1,this));
    ui->lineEditAisleNo->setText("11");


    //设置字体
    QFont font = this->font();

    font.setBold(true);
    font.setPointSize(14);
    ui->labelTitle->setFont(font);
    font.setBold(false);
    font.setPointSize(12);
    ui->radioButtonAll->setFont(font);
    ui->radioButtonAisle->setFont(font);
    ui->radioButtonLeve->setFont(font);

}

AddGoodsDialog::~AddGoodsDialog()
{
    delete ui;
}

void AddGoodsDialog::pushButtonOKClicked()
{
    QByteArray  arr;
    QDataStream in(&arr,QIODevice::WriteOnly);
    quint8 leveNo;
    bool ok;
    //
    in<<(quint8)ADD_HUODAO_REQ;
    //柜号
    in<<(quint8)0x01;
    int temp = buttonGroup->checkedId();
    if(temp == 0)//all
    {
        //qDebug()<<tr("0");
        in<<(quint8)0x00<<(quint8)0x00;
        //str = "0000";
    }
    else if(temp == 1)//leve
    {
       //qDebug()<<tr("1");

       leveNo = ui->lineEditLeve->text().toInt(&ok,10);
       if(!leveNo || !ok)
       {
           QMessageBox::information(this,tr("inform"),
                                    tr("the leveNo is not right")
                                    +"\n"+tr("Please choose again")
                                    + "\n" +tr("the leveNo is 1-8"),
                                    QMessageBox::Yes);

           return;
       }
       in<<(quint8)0x01<<(quint8)leveNo;
     //  str = QString("01%1").arg(leveNo,2,10,QLatin1Char('0'));
    }
    else if(temp == 2)//aisle
    {
        //qDebug()<<tr("2");
        quint8 aisleNo = ui->lineEditAisleNo->text().toInt(&ok,10);
        if(!aisleNo | !ok)
        {
            QMessageBox::information(this,tr("AisleNO"),
                                     tr("the aisleNo is not right")
                                     ,
                                     QMessageBox::Yes);

            return;
        }
        in<<(quint8)0x02<<(quint8)aisleNo;
        //str = QString("02%1").arg(aisleNo,2,10,QLatin1Char('0'));
    }
    else
        return;

    //qDebug()<<"addGoodsStr="<<arr;
    emit addGoodsSignal(arr);


    this->hide();

}

void AddGoodsDialog::aisleHide()
{
    ui->labelAisle->hide();
    ui->lineEditAisleNo->hide();
}
void AddGoodsDialog::aisleShow()
{
    leveHide();
    ui->labelAisle->show();
    ui->lineEditAisleNo->show();
}
void AddGoodsDialog::leveShow()
{
    //qDebug()<<"leveShow";
    ui->labelLeve->show();
    ui->lineEditLeve->show();
    ui->labelTips->show();

    aisleHide();

}
void AddGoodsDialog::leveHide()
{
    //qDebug()<<"leveHide";
    ui->labelLeve->hide();
    ui->lineEditLeve->hide();
    ui->labelTips->hide();


}
void AddGoodsDialog::translateLanguage()
{
    ui->retranslateUi(this);
    setWindowTitle(tr("AddGoods"));
}
