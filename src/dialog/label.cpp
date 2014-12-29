#include "label.h"
#include <QtGui>
Label::Label(QWidget *parent)
    :QLabel(parent)
{

}
void Label::mousePressEvent(QMouseEvent *ev)
{
    QLabel::mousePressEvent(ev);
}
void Label::mouseReleaseEvent(QMouseEvent *ev)
{
    QLabel::mouseReleaseEvent(ev);
    emit cliked(this->text());
}

void Label::enterEvent(QEvent *)
{
    curCursor = this->cursor();
    this->setCursor(Qt::PointingHandCursor);
    QFont font = this->font();
    font.setPointSize(font.pointSize()+1);
    this->setFont(font);
    emit  enter();
}

void Label::leaveEvent(QEvent *)
{
    this->setCursor(curCursor);
    QFont font = this->font();
    font.setPointSize(font.pointSize()-1);
    this->setFont(font);
    emit leave();
}
