#include "abountusdialog.h"
#include <QtGui>
#include "../config.h"
AbountUsDialog::AbountUsDialog(QWidget *parent)
    :BaseDialog(parent)
{

    this->setFixedSize(500,300);

    titleLabel = new QLabel();
    titleIconLabel = new QLabel();
    closeButton = new PushButton();
    okButton = new QPushButton();
    iconLabel =  new QLabel();



    //公司名称
    mantitleLabel = new QLabel();
    companyLabel = new QLabel();
    companySubLabel =new QLabel();
    companyUrlLabel = new Label();
    connect(companyUrlLabel,SIGNAL(cliked(QString)),
            this,SLOT(linkOpenSlot(QString)));
    connect(companyUrlLabel,SIGNAL(enter()),
            this,SLOT(linkActivatedSlot()));

    versionLabel =  new QLabel();

    QPixmap titlePixmap(":/logo/images/tool");    
    titleIconLabel->setPixmap(titlePixmap);
    titleIconLabel->setFixedSize(16,16);
    titleIconLabel->setScaledContents(true);

    QPixmap title_pixmap(":/logo/images/logo_big");
    iconLabel->setPixmap(title_pixmap);
    iconLabel->setFixedSize(title_pixmap.size());
    iconLabel->setScaledContents(true);


    QFont okFont = this->font();
    okFont.setPointSize(10);
    okButton->setFont(okFont);
    closeButton->setPicName(":/system_button/images/sysButton/close");
    okButton->setObjectName("okButton");

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleIconLabel,0,Qt::AlignVCenter);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addStretch();
    titleLayout->addWidget(closeButton,0,Qt::AlignTop);
    titleLayout->setSpacing(4);
    titleLayout->setContentsMargins(2,2,2,8);

    QVBoxLayout *companyLayout = new QVBoxLayout();
    QFont companyFont = this->font();
    companyFont.setBold(true);
    companyFont.setPointSize(18);
    mantitleLabel->setFont(companyFont);
    companyLabel->setFont(companyFont);
    companyFont.setPointSize(12);
    companySubLabel->setFont(companyFont);
    companyFont.setBold(false);
    companyFont.setPointSize(12);
    companyFont.setUnderline(true);
    companyUrlLabel->setFont(companyFont);
    companyLayout->addWidget(mantitleLabel,0,Qt::AlignHCenter);
    companyLayout->addWidget(companyLabel,0,Qt::AlignHCenter);
    companyLayout->addWidget(companySubLabel,0,Qt::AlignHCenter);
    companyLayout->addWidget(companyUrlLabel,0,Qt::AlignHCenter);
    companyLayout->setSpacing(8);

    QHBoxLayout *logoHLayout = new QHBoxLayout();
    logoHLayout->addWidget(iconLabel);
    logoHLayout->addStretch();
    logoHLayout->setSpacing(1);

    QHBoxLayout *versionLayout = new QHBoxLayout();
    versionLayout->addWidget(versionLabel);
    versionLayout->addStretch();
    versionLayout->addWidget(okButton);
    versionLayout->setContentsMargins(2,2,10,5);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(titleLayout);

    layout->addLayout(logoHLayout);
    layout->addLayout(companyLayout);
    layout->addStretch();
    layout->addLayout(versionLayout);

    layout->setSpacing(0);
    layout->setContentsMargins(SHADOW_WIDTH1,SHADOW_WIDTH1,SHADOW_WIDTH1,SHADOW_WIDTH1);
    setLayout(layout);

    translateLanguage();

    connect(closeButton,SIGNAL(clicked()),
            this,SLOT(hide()));
    connect(okButton,SIGNAL(clicked()),
            this,SLOT(hide()));

}


void AbountUsDialog::paintEvent(QPaintEvent *event)
{
    BaseDialog::paintEvent(event);

    QImage image;
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);   
    QRect rectBack = QRect(rect().x() + SHADOW_WIDTH1,rect().y() + SHADOW_WIDTH1,
                       rect().width() - 2*SHADOW_WIDTH1,
                       rect().height() - 2*SHADOW_WIDTH1);
    if(image.load(QDir::currentPath() + "/images/back_ground"))
    {
       painter.drawImage(rectBack,image);
    }
    else
    {
        painter.setBrush(Qt::blue);
        painter.drawRoundedRect(rectBack,2,2);
    }
    //qDebug()<<rect().x()<<rect().y()<<rect().width()<<rect().height();
    painter.setBrush(Qt::white);
    painter.drawRect(QRect(rect().x() + SHADOW_WIDTH1,30 + SHADOW_WIDTH1,
                           rect().width() - 2*SHADOW_WIDTH1,
                           rect().height() - 30 - 2*SHADOW_WIDTH1));
}


void AbountUsDialog::translateLanguage()
{

    titleLabel->setText("VMCTool");
    versionLabel->setText(QString("%1%2%3").arg("Version").arg(VMC_VERSION_STRING).arg(" by Yoc"));
    titleLabel->setObjectName("whiteLabel");
    //versionLabel->setObjectName("whiteLabel");
    mantitleLabel->setText(tr("Vending machine management tools"));
   // companyLabel->setText(tr("福建骏鹏通信科技有限公司"));
    QPixmap pixMap(":/logo/images/logoText.png");
    companyLabel->setPixmap(pixMap);
    companySubLabel->setText(tr("Fujian Junpeng Communicaiton Technology Co.,Ltd."));
    companyUrlLabel->setText(QString("http://www.easivend.com.cn"));
    okButton->setText(tr("Confirm"));
    okButton->setFixedSize(75,25);
}


void AbountUsDialog::linkOpenSlot(QString link)
{
    //qDebug()<<link;
    QUrl url(link);
    QDesktopServices::openUrl(url);
}
void AbountUsDialog::linkActivatedSlot()
{
    //qDebug()<<"AbountUsDialog::linkActivatedSlot";
}
