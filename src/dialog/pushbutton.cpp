#include "pushbutton.h"
#include <QtGui>

PushButton::PushButton(QWidget *parent)
    :QPushButton(parent)
{
    status = NORMAL;
    mousePress = false;

    QFont font = this->font();
    font.setBold(true);
    font.setPointSize(12);
    setFont(font);
}

PushButton::~PushButton()
{

}

void PushButton::setPicName(QString picName)
{
    this->picName = picName;
    setFixedSize(QPixmap(picName).size());
}
void PushButton::setPicName(QString picName,QSize size)
{
    this->picName = picName;
    setFixedSize(size);
}


void PushButton::enabledChange(bool flag)
{
    QPushButton::enabledChange(flag);
    if(!flag)
    {
        status = NORMAL;
    }
    else
        status = FORBIDDEN;

    this->repaint();
    //qDebug()<<"enabledChange"<<flag;
}
void PushButton::enterEvent(QEvent *)
{
    if(!this->isEnabled())
        return;
    status = ENTER;
    update();
}

void PushButton::mousePressEvent(QMouseEvent *event)
{
    //若点击鼠标左键
    if(!this->isEnabled())
        return;
    if(event->button() == Qt::LeftButton)
    {
        mousePress = true;
        status = PRESS;

        this->move(QPoint(this->pos().x()+1,this->pos().y()+1));
        update();
    }
}

void PushButton::mouseReleaseEvent(QMouseEvent *event)
{    
    //若点击鼠标左键
    if(mousePress && this->isEnabled())
    {
        this->move(QPoint(this->pos().x() - 1,this->pos().y() - 1));
        if(!this->rect().contains(event->pos()))
                return;
        if(this->isCheckable())
        {
            mousePress = false;
            if(!this->isChecked())
            {
               this->setChecked(true);
               status = ENTER;
               this->setFlat(false);
            }
            else
            {
               this->setChecked(false);
               status = ENTER;
               this->setFlat(true);
            }
            //qDebug()<<"this->isChecked()"<<this->isChecked();
            update();
            emit clicked(this->isChecked());
        }
        else
        {
            mousePress = false;
            status = ENTER;
            update();
            emit clicked();
        }
    }



}

void PushButton::leaveEvent(QEvent *)
{
    if(!this->isEnabled())
        return;
    status = NORMAL;
    update();
}

void PushButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QImage pixmap;
//    if(!this->isEnabled())
//        status = FORBIDDEN;
    switch(status)
    {
    case NORMAL:
        {

            pixmap.load(picName);
            break;
        }
    case ENTER:
        {
            pixmap.load(picName + QString("_hover"));
            break;
        }
    case PRESS:
        {
            pixmap.load(picName + QString("_pressed"));
            break;
        }
    case FORBIDDEN:
        {
            pixmap.load(picName + QString("_forbidden"));
            break;
        }

    case NOSTATUS:
        {
            pixmap.load(picName);
            break;
        }
    default:
        pixmap.load(picName);
    }
    painter.drawImage(this->rect(),pixmap);
    if(!this->text().isEmpty())
    {

        painter.setFont(this->font());

        if(this->isEnabled())
        {
            painter.setPen(QColor(Qt::blue));
        }
        else
        {
            painter.setPen(QColor(Qt::lightGray));
        }

        painter.drawText(this->rect(),Qt::AlignCenter,this->text());
    }

}
