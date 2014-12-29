#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>

class ToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ToolButton(QString pixName,QWidget * parent = 0);
    ~ToolButton();
    void setMousePressed(bool mousePressed = false);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    bool mousePress;
    bool mouseHover;


    void paintHovered();

};

#endif // TOOLBUTTON_H
