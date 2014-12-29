#include "settingdialog.h"
#include <QtGui>
SettingDialog::SettingDialog(QWidget *parent)
    :BaseDialog(parent)
{
    this->setFixedSize(450,310);
    titleLabel = new QLabel();
    titleLabel->setObjectName("whiteLabel");
    titleIconLabel = new QLabel();
    closeButton = new PushButton();
    okButton = new QPushButton();
    okButton->setFixedSize(75,25);

    vmcDebugInit();
    QPixmap titlePixmap(":/logo/images/tool");
    titleIconLabel->setPixmap(titlePixmap);
    titleIconLabel->setFixedSize(16,16);
    titleIconLabel->setScaledContents(true);
    closeButton->setPicName(":/system_button/images/sysButton/close");
    okButton->setObjectName("okButton");

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleIconLabel,0,Qt::AlignVCenter);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addStretch();
    titleLayout->addWidget(closeButton,0,Qt::AlignTop);
    titleLayout->setSpacing(2);
    titleLayout->setContentsMargins(2,2,2,8);

    vmcDebugDialog = new VMCDebugDialog();
    connect(vmcDebugDialog,SIGNAL(debugRequstSignal(quint8,QByteArray)),
            this,SLOT(setRequestSlot(quint8,QByteArray)));

    QHBoxLayout *okButtonLayout = new QHBoxLayout();
    okButtonLayout->addStretch(0);
    okButtonLayout->addWidget(okButton);
    okButtonLayout->setSpacing(16);
    okButtonLayout->setContentsMargins(0,0,9,5);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(titleLayout);
    layout->addWidget(vmcDebugDialog);
    layout->addStretch();
    layout->addLayout(okButtonLayout);
    layout->setSpacing(0);
    layout->setContentsMargins(SHADOW_WIDTH,SHADOW_WIDTH,SHADOW_WIDTH,SHADOW_WIDTH);
    setLayout(layout);

    connect(closeButton,SIGNAL(clicked()),
            this,SLOT(okAceept()));
    connect(okButton,SIGNAL(clicked()),
            this,SLOT(okAceept()));
    translateLanguage();
}


void SettingDialog::paintEvent(QPaintEvent *event)
{
    BaseDialog::paintEvent(event);

    QImage image;
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    QRect rectBack = QRect(rect().x() + SHADOW_WIDTH,rect().y() + SHADOW_WIDTH,
                       rect().width() - 2*SHADOW_WIDTH,
                       rect().height() - 2*SHADOW_WIDTH);
    if(image.load(QDir::currentPath() + "/images/back_ground"))
    {
       painter.drawImage(rectBack,image);
    }
    else
    {
        painter.setBrush(Qt::blue);
        painter.drawRoundedRect(rectBack,2,2);
    }
   // qDebug()<<rect().x()<<rect().y()<<rect().width()<<rect().height();
    painter.setBrush(Qt::white);
    painter.drawRect(QRect(rect().x() + SHADOW_WIDTH,30 + SHADOW_WIDTH,
                           rect().width() - 2*SHADOW_WIDTH,
                           rect().height() - 30 - 2*SHADOW_WIDTH));
}


SettingDialog::~SettingDialog()
{

}


void SettingDialog::translateLanguage()
{
    titleLabel->setText("VMCTool");
    okButton->setText(tr("Confirm"));
    this->vmcDebugDialog->translateLanguage();
}


void SettingDialog::vmcDebugInit()
{
    //售货机调试配置 调试总开关 纸币调试 硬币调试 找零调试 读卡器调试 货道调试 压缩机调试 照明调试
    //选货按键板调试 PC通信调试

    setConnect(false);
}


void SettingDialog::updateDebug(QByteArray arr)
{
    vmcDebugDialog->updateDebug(arr);
}
void SettingDialog::updateDebug(const quint8 type,const bool checked)
{
    //qDebug()<<"updateDebug="<<type<<checked;
    vmcDebugDialog->updateDebug(type,checked);
}


void SettingDialog::okAceept()
{
    qDebug()<<"okAceept";
    if(isConnect())
    {
        //发送保存命令让售货机将 调试开关变量写入flash
        QByteArray arr;
        QDataStream in(&arr,QIODevice::WriteOnly);
        in<<(quint8)0xF0;
        emit setRequestSignal(DEBUG_REQ,arr);
    }
    this->hide();
}


void SettingDialog::setRequestSlot(quint8 type, QByteArray arr)
{
    //qDebug()<<"setRequestSlot"<<type<<arr.toHex();
    if(isConnect())
        emit setRequestSignal(type,arr);
}
