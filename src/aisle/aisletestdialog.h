#ifndef AISLETESTDIALOG_H
#define AISLETESTDIALOG_H
#include <QDialog>
class QLabel;
class QPainterPath;
class QTimer;
class QRect;

#define  SIDEW      4

class AisleTestdialog : public QDialog
{
    Q_OBJECT
public:
    AisleTestdialog(QWidget *parent = 0);

    void setText(const QString &text);
    QString text() const;
protected:
    void paintEvent(QPaintEvent *event);

private:
    QLabel *textLabel;
    //QString textLabel;

private slots:

};

#endif // AISLETESTDIALOG_H
