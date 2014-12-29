#include "titlewidget.h"
#include "../config.h"
#include <Qtgui>
TitleWidget::TitleWidget(QWidget *parent) :
    QWidget(parent)
{
    versionTitle = new QLabel();
    mainMenuButton = new PushButton();
    minButton = new PushButton();
    closeButton = new PushButton();

    QFont versionFont = versionTitle->font();
    versionFont.setBold(true);
    versionFont.setPointSize(10);
    versionTitle->setFont(versionFont);
    versionTitle->setObjectName("whiteLabel");

    minButton->setPicName(QString(":/system_button/images/sysButton/min"));
    closeButton->setPicName(QString(":/system_button/images/sysButton/close"));
    mainMenuButton->setPicName(QString(":/system_button/images/sysButton/menu"));

    connect(mainMenuButton, SIGNAL(clicked()), this, SIGNAL(showMainMenuWidget()));
    connect(minButton, SIGNAL(clicked()), this, SIGNAL(showMinWidget()));
    connect(closeButton, SIGNAL(clicked()), this, SIGNAL(closeWidegt()));

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(versionTitle,0,Qt::AlignTop);
    titleLayout->addStretch();
    titleLayout->addWidget(mainMenuButton, 0, Qt::AlignTop);
    titleLayout->addWidget(minButton, 0, Qt::AlignTop);
    titleLayout->addWidget(closeButton, 0, Qt::AlignTop);
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(1, 1, 4, 0);
    versionTitle->setContentsMargins(5,0,0,0);

    QStringList buttonImage;
    buttonImage<<":/button/images/data"<<":/button/images/system"<<":/button/images/column"
              <<":/button/images/goods"<<":/button/images/serial";

 #if 1
        buttonImage <<":/button/images/goods";//新增盒饭柜
 #endif


    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QSignalMapper *signalMapper = new QSignalMapper(this);
    for(int i = 0;i < buttonImage.count();i++)
    {
        ToolButton *toolButton = new ToolButton(buttonImage.at(i));
        connect(toolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
        signalMapper->setMapping(toolButton,QString::number(i,10));
        buttonLayout->addWidget(toolButton,0,Qt::AlignBottom);

        buttonList<<toolButton;
        if(i == 5)//盒饭柜
            toolButton->hide();
    }
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(turnPage(QString)));

    easivendLabel = new QLabel();
    easivendLabelCh = new QLabel();
    QVBoxLayout *easivendLayout = new QVBoxLayout();
    easivendLayout->addWidget(easivendLabelCh);
    easivendLayout->addWidget(easivendLabel);
    easivendLayout->setSpacing(0);
    easivendLayout->setContentsMargins(0,0,0,0);

    QLabel *logoLabel = new QLabel();
    QPixmap pixmap(":/logo/images/logo");
    logoLabel->setPixmap(pixmap);
    logoLabel->setFixedSize(pixmap.size());

    easivendLabel->setObjectName("whiteLabel");
    easivendLabelCh->setObjectName("whiteLabel");
    QFont easivendChFont = easivendLabelCh->font();
    easivendChFont.setPointSize(13);
    //easivendChFont.setBold(true);
    easivendChFont.setWeight(75);
    easivendChFont.setItalic(true);
   // easivendChFont.setFamily("微软雅黑");
    easivendLabelCh->setFont(easivendChFont);

    QFont easivendFont = easivendLabel->font();
    easivendFont.setPointSize(11);
    //easivendFont.setBold(true);
   // easivendFont.setFamily("微软雅黑");
    easivendFont.setWeight(75);
    easivendFont.setItalic(true);
    easivendLabel->setFont(easivendFont);

    easivendLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    easivendLabelCh->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    buttonLayout->addStretch();
    buttonLayout->addLayout(easivendLayout);
    buttonLayout->addWidget(logoLabel,0,Qt::AlignCenter);
    buttonLayout->setSpacing(8);
    buttonLayout->setContentsMargins(15, 0, 15, 0);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(titleLayout);
    layout->addLayout(buttonLayout);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,1);

    translateLanguage();
    setLayout(layout);
    setFixedHeight(88);


    //暂时隐藏盒饭柜
  //  buttonList.at(5)->hide();


}


void TitleWidget::turnPage(QString pageNo)
{
    //qDebug()<<"pageNo="<<pageNo;
    bool ok;
    quint8 curIndex = pageNo.toInt(&ok,10);

    for(quint8 i = 0;i < buttonList.count();i++)
    {
        if(i == curIndex)
            buttonList.at(i)->setMousePressed(true);
        else
            buttonList.at(i)->setMousePressed(false);
    }
    emit turnPage(curIndex);
}


void TitleWidget::hidePage(quint8 pageNo)
{
    if(pageNo > buttonList.count() || pageNo <= 0)
        return;
    buttonList.at(pageNo -1)->hide();
}

void TitleWidget::showPage(quint8 pageNo)
{
    if(pageNo > buttonList.count() || pageNo <= 0)
        return;
    buttonList.at(pageNo -1)->show();
}

void TitleWidget::translateLanguage()
{
    versionTitle->setText(QString("%1-%2").arg(tr("VMCTool"))
                                          .arg(VMC_VERSION_STRING));

    mainMenuButton->setToolTip(tr("Main menu"));
    minButton->setToolTip(tr("Minisize"));
    closeButton->setToolTip(tr("Close"));

    quint8 i = 0;
    QStringList buttonnames;
    buttonnames<<tr("DataExp")<<tr("System")
              <<tr("Column")<<tr("Goods")<<tr("Serial")
             <<tr("vcs");
    foreach(ToolButton *button,buttonList)
    {
        button->setText(buttonnames.at(i++));
    }


    easivendLabel->setText("Easivend");
    QPixmap pixMap(":/logo/images/EFlogo.png");
    easivendLabelCh->setPixmap(pixMap);
    //easivendLabelCh->setText(tr("易丰"));
}
