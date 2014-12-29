#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>

#include <QPoint>

//枚举按钮的几种状态
enum ButtonStatus{NORMAL, ENTER, PRESS,FORBIDDEN,NOSTATUS};


class PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QWidget *parent = 0);
    ~PushButton();
    void setPicName(QString picName);
    void setPicName(QString picName,QSize size);

protected:

    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
    void enabledChange(bool flag);


private:


    ButtonStatus status;
    QString picName;
    bool mousePress; //按钮左键是否按下


};

#endif // PUSHBUTTON_H
