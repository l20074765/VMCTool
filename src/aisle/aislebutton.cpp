#include "aislebutton.h"
#include <QtGui>
AisleButton::AisleButton(QWidget *parent)
    :QLabel(parent)
{
    checked = false;
    enabled = false;
}
AisleButton::~AisleButton()
{
   // qDebug()<<tr("AisleButton DELET");
}
void AisleButton::mousePressEvent(QMouseEvent *ev)
{
    // qDebug()<<tr("AisleButtonmousePressEvent1");
     ev->accept();
}
void AisleButton::mouseReleaseEvent(QMouseEvent *ev)
{
   // qDebug()<<tr("AisleButtonmouseReleaseEvent1");
    if(enabled)
    {
        click();
        ev->accept();
    }
    else
    {
        ev->ignore();
    }
}
void AisleButton::enterEvent(QEvent *event)
{
     //qDebug()<<tr("AisleButtonenterEvent1");
     event->ignore();
}
void AisleButton::leaveEvent(QEvent *event)
{
    //qDebug()<<tr("AisleButtonleaveEvent1");
     event->ignore();
}
void AisleButton::click()
{
    if(checked)
        checked = false;
    else
        checked = true;
    //qDebug()<<tr("checked===")<<checked;
    emit clicked(checked);

}
void AisleButton::closeEvent(QCloseEvent *event)
{
   // qDebug()<<tr("AisleButton closeEvent");
    event->accept();
}
