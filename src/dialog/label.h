#ifndef LABEL_H
#define LABEL_H
#include <QLabel>
class Label : public QLabel
{
    Q_OBJECT
public:
    Label(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

signals:
    void cliked(QString link);
    void leave();
    void enter();


private:
    QCursor curCursor;
};

#endif // LABEL_H
