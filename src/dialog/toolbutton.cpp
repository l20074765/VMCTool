#include "toolbutton.h"
#include <QMouseEvent>
#include <QPainter>

ToolButton::ToolButton(QString pixName, QWidget *parent)
    :QToolButton(parent)
{

    //设置文本颜色
    QPalette textPaltte = palette();
    textPaltte.setColor(QPalette::ButtonText,QColor(Qt::white));
    setPalette(textPaltte);
    //设置文本粗体
    QFont &textFont = const_cast<QFont &>(this->font());
  //  textFont.setFamily("Helvetica");
    textFont.setBold(true);
    textFont.setPointSize(12);

    //文字在下图片在上
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QPixmap pixmap(pixName);
    setIcon(pixmap);
    setIconSize(pixmap.size());
    setFixedSize(pixmap.width() + 25,pixmap.height()+25);
    setAutoRaise(true);
    setObjectName("transparentToolButton");

    mousePress = false;
    mouseHover = false;
}

ToolButton::~ToolButton()
{
}

void ToolButton::enterEvent(QEvent *)
{
    mouseHover = true;
}
void ToolButton::leaveEvent(QEvent *)
{
    mouseHover = false;
}


void ToolButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
}

void ToolButton::mouseReleaseEvent(QMouseEvent *event)
{
   event = event;
}

void ToolButton::paintEvent(QPaintEvent *event)
{
    if(mouseHover)
    {
        paintHovered();
    }
    else
    {
        if(mousePress)
             paintHovered();
    }

    QToolButton::paintEvent(event);
}

void ToolButton::setMousePressed(bool mousePressed)
{
    this->mousePress = mousePressed;
    update();
}

void ToolButton::paintHovered()
{
    QPainter painter(this);
    QPen pen(Qt::NoBrush,1);
    painter.setPen(pen);
    QLinearGradient linear(rect().topLeft(),rect().bottomLeft());
    linear.setColorAt(0,QColor(230,230,230,0));
    linear.setColorAt(0.5,QColor(230,230,230,100));
    linear.setColorAt(1,QColor(230,230,230,150));

    painter.setBrush(linear);
    painter.drawRect(rect());
}
