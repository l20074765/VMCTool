#ifndef HDBUTTON_H
#define HDBUTTON_H

#include <QWidget>

class HDButton : public QWidget
{
    Q_OBJECT
public:
    explicit HDButton(QWidget *parent = 0);

    void setChecked(const bool checked);
    bool isChecked()const{return checked;}
    inline void setText(const QString &textON = "ON",const QString &textOFF = "OFF")
    {
        text_on = textON;
        text_off = textOFF;
    }
    inline QString textON()const { return this->text_on;}
    inline QString textOFF()const{return this->text_off;}

    inline void setAutoHd(const bool autoHd){this->autoHd = autoHd;}

signals:
    void clicked();
    void clicked(bool checked);
protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void resizePainter();
    void resizeEvent(QResizeEvent *ev);

private:
    QTimer *timer;
    qreal rd,round,w,h,pieR;
    QLinearGradient linearON,linearOFF;
    QRadialGradient radialON,radialOFF;
    QRectF rect;
    QRect wRect;
    QPointF pieON,pieOFF;
    qreal  framed;
    bool mPress;
    bool hover;
    bool checked;
    QString text_on,text_off;
    bool autoHd;

private slots:
    void timerout();
    
};

#endif // HDBUTTON_H
