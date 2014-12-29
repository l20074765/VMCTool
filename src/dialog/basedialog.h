#ifndef BASEDIALOG_H
#define BASEDIALOG_H
#include<QDialog>

#define SHADOW_WIDTH 5
class BaseDialog : public QDialog
{
    Q_OBJECT
public:

    BaseDialog(QWidget *parent = 0);
    ~BaseDialog();

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);

private:

    QPoint move_point; //移动的距离
    bool mouse_press; //按下鼠标左键
};

#endif // BASEDIALOG_H
