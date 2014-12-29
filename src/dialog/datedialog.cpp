#include "datedialog.h"
#include "ui_datedialog.h"
#include <QtGui>
DateDialog::DateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DateDialog)
{
    ui->setupUi(this);

    QFont font = this->font();

    font.setPointSize(16);
    ui->lineEditYear->setFont(font);


    font.setPointSize(28);
    ui->lineEditTime->setFont(font);
    this->setFixedSize(200,120);

    translateLanguage();

    ui->lineEditYear->setInputMask("0000-00-00");
    ui->lineEditTime->setInputMask("00:00:00");
}

DateDialog::~DateDialog()
{
    delete ui;
}


void DateDialog::translateLanguage()
{
    ui->retranslateUi(this);
    setWindowTitle(tr("SetDateTime"));
}

void DateDialog::on_pushButton_2_clicked()
{
    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);

    in<< (quint8)0x00;//type =0 设置时间
    quint16 temp;
    bool ok;
    QStringList dateList = ui->lineEditYear->text().split("-");
    if(dateList.count() != 3)
    {
        qDebug()<<"dateList.count() != 3"<<dateList.count();
        return;
    }

    temp = dateList.at(0).toInt(&ok,16);
    in << (quint16)temp;

    temp = dateList.at(1).toInt(&ok,16);
    in<<(quint8)temp;
    temp = dateList.at(2).toInt(&ok,16);
    in<<(quint8)temp;
    //time
    QStringList timeList = ui->lineEditTime->text().split(":");
    if(timeList.count() != 3)
    {
        qDebug()<<"timeList.count() != 3"<<timeList.count();
        return;
    }

    temp = timeList.at(0).toInt(&ok,16);
    in<<(quint8)temp;
    temp = timeList.at(1).toInt(&ok,16);
    in<<(quint8)temp;
    temp = timeList.at(2).toInt(&ok,16);
    in<<(quint8)temp;

//    //新增星期
//    temp = ui->comboBoxWeek->currentIndex()+1;
//    in<<(quint8)temp;

    emit resultSignal(false,processText(DATESETTING));
    emit dateTimeSetSiganl(DATETIME_REQ,arr);

    this->hide();
}
void DateDialog::setDateTime()
{
    QString dateStr = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString timeStr = QDateTime::currentDateTime().toString("hh:mm:ss");

    ui->lineEditTime->setText(timeStr);
    ui->lineEditYear->setText(dateStr);

    //ui->dateTimeEdit->setDateTime(dateTime);
}


QString DateDialog::processText(PTEXT pText)
{
    QString text;
    if(pText == DATESETTING)
    {
        text = tr("Setting DateTime ...");
    }
    return text;

}


void DateDialog::lineEditedSlot(QString text)
{

    qDebug()<<"DateDialog::lineEditedSlot"<<text;
}
