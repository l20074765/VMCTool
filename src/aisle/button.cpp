#include "button.h"
#include <QtGui>
Button::Button(QWidget *parent)
    :QWidget(parent)
{
    logicNoLabel = new QLabel(this);
    openAndCloseButton = new AisleButton(this);
    logicNoLabel->setFrameShape(QFrame::Box);
    openAndCloseButton->setFrameShape(QFrame::Box);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(logicNoLabel);
    layout->addWidget(openAndCloseButton);
    setLayout(layout);
    setButtonEnabled(true);
    setGroup(false);
    setAisleNo(0);
    setGroupNo(0);
    selected = false;
    upStye(NORMAL,HEAVED);
    connect(openAndCloseButton,SIGNAL(clicked(bool)),
            this,SLOT(button_clicked(bool)));
}

Button::~Button()
{

    //qDebug()<<tr("Button DELET");
}
void Button::upStye(quint8 label, quint8 button)
{
    labelStye(label);
    buttonStye(button);
}
void Button::labelStye(quint8 type)
{

    switch (type)
    {
        case NORMAL://常态
        logicNoLabel->setStyleSheet("QLabel{color :red; background-color: rgb(255, 255, 255);;\
                                    border-width: 1px;border-style:solid;border-color:gray;\
                                    font: 200 15pt;}");
       // qDebug()<<"labelStyeNORMAL";
        break;

        case SELECT://被选中
        logicNoLabel->setStyleSheet("QLabel{color :red; background-color: rgba(0, 85, 0, 150);\
                                   border-width: 1px;\
                                    border-style: solid;border-color: gray;font: 300 18pt;}");
       // qDebug()<<"labelStyeSELECT";
        return;
        case ENTER://进入
        logicNoLabel->setStyleSheet("QLabel{color :red; background-color: rgba(0, 85, 0, 120);\
                                    border-width: 1px;\
                                    border-style: solid;border-color: gray;font: 300 18pt;}");

       // qDebug()<<"labelStyeENTER";
        return;
        case LEAVE://离开
        break;
        default:
        break;
    }
    labelStatus = type;
    // qDebug()<<"labelStatus"<<type;

}
void Button::buttonStye(quint8 type)
{

    switch (type)
    {
        case NORMAL://常态
        openAndCloseButton->setStyleSheet("QLabel{color :blue; background-color:; border-width: 1px;\
                        border-style: solid;border-color: gray;font: 50 10pt;}");
        //qDebug()<<"buttonStyeNORMAL";
        return;
        case SELECT://被选中
        openAndCloseButton->setStyleSheet("QLabel{color :blue; background-color: rgba(0, 85, 0, 150); border-width: 1px;\
                               border-style: solid;border-color: gray;font: 50 10pt;}");
        //qDebug()<<"buttonStyeSELECT";
        return;
        case PRESSD://按下 状态关闭
        openAndCloseButton->setStyleSheet("QLabel{color :blue; background-color: rgb(170, 170, 170); border-width: 1px;\
                                border-style: solid;border-color: gray;font: 50 10pt;}");
       // qDebug()<<"buttonStyePRESSD";

        break;
        case HEAVED://弹起 状态开启
        openAndCloseButton->setStyleSheet("QLabel{color :blue;background-color: rgb(85, 255, 127); border-width: 1px;\
                    border-style: solid;border-color: gray;font: 50 10pt;}");
        // qDebug()<<"buttonStyeHEAVED";
        break;
        case ENTER://进入
        openAndCloseButton->setStyleSheet("QLabel{color :blue; background-color: rgba(0, 85, 0, 120); border-width: 1px;\
                               border-style: solid;border-color: gray;font: 50 10pt;}");
       // qDebug()<<"buttonStyeENTER";
         return;
        default:
        break;

    }
    buttonStatus = type;
    //qDebug()<<"buttonStatus"<<type;
}
void Button::button_clicked(bool checked)
{
    qDebug()<<tr("checked")<<checked;
    emit ButtonClickedSignal(checked);
}
void Button::setText(const QString &text)
{
    logicNoLabel->setText(text);
}

void Button::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug()<<tr("Button mouseMoveEvent");
    event->accept();
}
void Button::mousePressEvent(QMouseEvent *event)
{
    // qDebug()<<tr("Button mousePressEvent");

     event->accept();
}
void Button::mouseReleaseEvent(QMouseEvent *event)
{
    //qDebug()<<tr("Button mouseReleaseEvent");
    if(buttonEnable)
        emit buttonSelectSignal();
    event->accept();
}
void Button::enterEvent(QEvent *event)
{
    // qDebug()<<tr("Button enterEvent");
    if(buttonEnable)
    {
        upStye(ENTER,ENTER);
        emit ButtonEnterSignal();
    }
     event->accept();
}
void Button::leaveEvent(QEvent *event)
{
    //qDebug()<<tr("Button leaveEvent");
    if(buttonEnable)
    {
        upStye(labelStatus,buttonStatus);
        emit ButtonleaveSignal();
    }
    event->accept();
}

void Button::setOpen(bool flag)
{
    openAndCloseButton->setChecked(flag);
    if(flag)
    {
        openAndCloseButton->setText(tr("ON"));
        buttonStye(HEAVED);
    }
    else
    {
        openAndCloseButton->setText(tr("OFF"));
        buttonStye(PRESSD);
    }

}
void Button::setON(bool flag)
{
    if(flag)
    {
        openAndCloseButton->setText(tr("ON"));
        buttonStye(buttonStatus);
    }
    else
    {
        openAndCloseButton->setText(tr("OFF"));
        buttonStye(buttonStatus);
    }
}
void Button::closeEvent(QCloseEvent *event)
{
    qDebug()<<tr("closeEvent");
    event->accept();
}
void Button::unselect()
{
    setLabelStatus(NORMAL);
    labelStye(NORMAL);
    if(IsOpen())//kaiqi
    {
        setButtonStatus(HEAVED);
        buttonStye(HEAVED);
    }
    else
    {
        setButtonStatus(PRESSD);
        buttonStye(PRESSD);
    }
    setEnabled(false);
    selected = false;
}
void Button::select()
{
   upStye(SELECT,SELECT);
   setLabelStatus(SELECT);
   setButtonStatus(SELECT);
   setEnabled(true);
   selected = true;
}
