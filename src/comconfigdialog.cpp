#include "comconfigdialog.h"
#include <QtGui>
ComConfigDialog::ComConfigDialog(QWidget  *parent)
    :QDialog(parent)
{


    setWindowTitle(tr("ConfigureSerialPort"));
    comLabel = new QLabel;
    baudLabel = new QLabel;
    dataLabel = new QLabel;
    parLabel = new QLabel;
    stopLabel = new QLabel;
    comComboBox = new QComboBox;
    baudComboBox = new QComboBox;
    dataComboBox = new QComboBox;
    parComboBox = new QComboBox;
    stopComboBox = new QComboBox;
    comComboBox->setMinimumSize(baudComboBox->sizeHint());
    dataComboBox->setMinimumSize(baudComboBox->sizeHint());
    parComboBox->setMinimumSize(baudComboBox->sizeHint());
    stopComboBox->setMinimumSize(baudComboBox->sizeHint());
    baudComboBox->setMinimumSize(baudComboBox->sizeHint());
    QGroupBox *groupBox = new QGroupBox(tr("Flow Control"));

    dCheckBox = new QCheckBox(tr("D&TR/DSR"));
    rcCheckBox = new QCheckBox(tr("&RTS/CTS"));
    xCheckBox = new QCheckBox(tr("&XON/XOFF"));
    QVBoxLayout *groupVBoxLayout =  new QVBoxLayout;

    groupVBoxLayout->addWidget(dCheckBox);
    groupVBoxLayout->addWidget(rcCheckBox);
    groupVBoxLayout->addWidget(xCheckBox);
    groupBox->setLayout(groupVBoxLayout);

    QHBoxLayout *layoutH1 = new QHBoxLayout;
    QHBoxLayout *layoutH2 = new QHBoxLayout;
    QHBoxLayout *layoutH3 = new QHBoxLayout;
    QHBoxLayout *layoutH4 = new QHBoxLayout;
    QHBoxLayout *layoutH5 = new QHBoxLayout;
    QVBoxLayout *layoutV = new QVBoxLayout;
    QHBoxLayout *layoutH6 = new QHBoxLayout;
    layoutH1->addWidget(comLabel);
    layoutH1->addWidget(comComboBox);
    layoutH2->addWidget(baudLabel);
    layoutH2->addWidget(baudComboBox);
    layoutH3->addWidget(dataLabel);
    layoutH3->addWidget(dataComboBox);
    layoutH4->addWidget(parLabel);
    layoutH4->addWidget(parComboBox);
    layoutH5->addWidget(stopLabel);
    layoutH5->addWidget(stopComboBox);
    layoutV->addLayout(layoutH1);
    layoutV->addLayout(layoutH2);
    layoutV->addLayout(layoutH3);
    layoutV->addLayout(layoutH4);
    layoutV->addLayout(layoutH5);

    layoutH6->addLayout(layoutV);
    layoutH6->addWidget(groupBox);

    QVBoxLayout *layout = new QVBoxLayout;
    okPushButton = new QPushButton(tr("OK"));
    okPushButton->setMaximumSize(okPushButton->sizeHint());
    connect(okPushButton,SIGNAL(clicked()),this,SLOT(on_clicked_ok()));

    cancelPushButton = new QPushButton(tr("CANCEL"));
    cancelPushButton->setMaximumSize(okPushButton->sizeHint());
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(hide()));

    QHBoxLayout *layoutH7 = new QHBoxLayout;
    QSpacerItem *spacer_prev = new QSpacerItem(5,0,QSizePolicy::Expanding,QSizePolicy::Minimum);
   // QSpacerItem *spacer_next = new QSpacerItem(5,0,QSizePolicy::Expanding,QSizePolicy::Minimum);

    layoutH7->addWidget(okPushButton);
    layoutH7->addItem(spacer_prev);
    layoutH7->addWidget(cancelPushButton);
    layout->addLayout(layoutH6);
    layout->addLayout(layoutH7);

    setLayout(layout);

    comLabel->setText(tr("COM:"));
    baudLabel->setText(tr("BAUD:"));
    dataLabel->setText(tr("DATA:"));
    parLabel->setText(tr("PAR:"));
    stopLabel->setText(tr("STOP:"));
    QStringList list;
    list.clear();
    list<<tr("COM1")<<tr("COM2")<<tr("COM3")<<tr("COM4")
          <<tr("COM5")<<tr("COM6")<<tr("COM7")<<tr("COM8")
          <<tr("COM9")<<tr("COM10")<<tr("COM11")<<tr("COM12")
          <<tr("COM13")<<tr("COM14")<<tr("COM15")<<tr("COM16")
          <<tr("COM17")<<tr("COM18")<<tr("COM19")<<tr("COM20")
            ;
    comComboBox->addItems(list);
    comComboBox->setCurrentIndex(0);
    list.clear();
    list<<tr("2400")<<tr("4800")<<tr("9600")
        <<tr("14400")<<tr("19200")<<tr("38400")
       <<tr("57600")<<tr("115200")<<tr("256000");
    baudComboBox->addItems(list);
    baudComboBox->setCurrentIndex(2);
    list.clear();
    list<<tr("8")<<tr("7")<<tr("6");
    dataComboBox->addItems(list);
    list.clear();
    list<<tr("NONE")<<tr("ODD")<<tr("EVE");
    parComboBox->addItems(list);
    list.clear();
    list<<tr("1")<<tr("2");
    stopComboBox->addItems(list);

    //Read comconfig
    QSettings *configRead = new QSettings("config.ini",
                                  QSettings::IniFormat);

    QString portName = configRead->value("/Srial/COM").toString();
    QString configbaud = configRead->value("/Srial/BAUD").toString();
   // qDebug()<<"comdialog:configData_read_result"<<portName<<configbaud;
    delete configRead;
   // qDebug()<<"Read COMconifg:"<<"portname="<<portName.right(1).toInt()<<"baud="<<configbaud;
    comComboBox->setCurrentIndex(portName.right(1).toInt() - 1);
    if(configbaud == tr("9600"))
         baudComboBox->setCurrentIndex(2);
    else if(configbaud == tr("19200"))
         baudComboBox->setCurrentIndex(4);
    else if(configbaud == tr("115200"))
         baudComboBox->setCurrentIndex(7);





}
void ComConfigDialog::on_clicked_ok()
{
    QSettings *configWrite = new QSettings("config.ini",QSettings::IniFormat);
    configWrite->setValue("/Srial/COM",comComboBox->currentText());
    configWrite->setValue("/Srial/BAUD",baudComboBox->currentText());
    delete configWrite;
    hide();
}



void ComConfigDialog::setSuperMode(const bool flag)
{
    baudComboBox->setEnabled(flag);
    dataComboBox->setEnabled(flag);
    parComboBox->setEnabled(flag);
    stopComboBox->setEnabled(flag);

    dCheckBox->setEnabled(flag);
    rcCheckBox->setEnabled(flag);
    xCheckBox->setEnabled(flag);
}
