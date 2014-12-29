#include "printinfodialog.h"
#include "ui_printinfodialog.h"
#include <QtGui>
PrintInfoDialog::PrintInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrintInfoDialog)
{
    ui->setupUi(this);

    QRegExp regExp("[0-9][0-9][0-9][1-9]");
    ui->lineEdit->setValidator(new QRegExpValidator(regExp,this));
    ui->lineEdit->setText("1");

    ui->lineEdit_2->setValidator(new QRegExpValidator(regExp,this));
    setMaximumSize(sizeHint());

    connect(ui->OK_pushButton,SIGNAL(clicked()),
            this,SLOT(okButtonslot()));

    translateLanguage();
}

PrintInfoDialog::~PrintInfoDialog()
{
    delete ui;
}

void PrintInfoDialog::translateLanguage()
{
    ui->retranslateUi(this);
    setWindowTitle(tr("PrintConfig"));
    ui->labelTotal->setText(tr("TotalTradeNum:"));
    ui->labelTradeRange->setText(tr("Please Input TradeRange"));
    ui->OK_pushButton->setText(tr("OK"));
    ui->Cansel_pushButton->setText(tr("Cancel"));
}
void PrintInfoDialog::setTradeNumLabel(const quint16 tradeNum,const quint16 printedIndex)
{
    ui->tradeNum_label->setText(QString("%1").arg(tradeNum));
    if(printedIndex && (printedIndex < tradeNum))
         ui->lineEdit->setText(QString("%1").arg(printedIndex));
    else
         ui->lineEdit->setText("1");

    ui->lineEdit_2->setText(QString("%1").arg(tradeNum));
    maxTradeIndex = tradeNum;
}
void PrintInfoDialog::getTradeIndex(quint16 *startIndex, quint16 *endIndex)
{
    bool ok,ok1;
    quint16 temp = ui->lineEdit->text().toUInt(&ok,10);
    quint16 temp1 = ui->lineEdit_2->text().toUInt(&ok1,10);
    if(!ok)
        *startIndex = 1;
    else
    {
        if(temp1 > maxTradeIndex)
            temp1 = maxTradeIndex;
        if(temp > temp1)
            temp = temp1;
        *startIndex =  (temp == 0) ? 1 : temp;
    }
    if(!ok1)
        *endIndex = maxTradeIndex;
    else
        *endIndex = (temp1 == 0) ? 1 : temp1;
}

void PrintInfoDialog::okButtonslot()
{

    this->accept();
}
