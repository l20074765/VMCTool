#ifndef COMCONFIGDIALOG_H
#define COMCONFIGDIALOG_H

#include <QDialog>
class QComboBox;
class QLabel;
class QCheckBox;
class QPushButton;
class ComConfigDialog : public QDialog
{
    Q_OBJECT
public:
    ComConfigDialog(QWidget *parent = 0);
    QLabel *comLabel;
    QLabel *baudLabel;
    QLabel *dataLabel;
    QLabel *parLabel;
    QLabel *stopLabel;
    QCheckBox *dCheckBox;
    QCheckBox *rcCheckBox;
    QCheckBox *xCheckBox;
    QPushButton *okPushButton;
    QPushButton *cancelPushButton;
    QComboBox *comComboBox;
    QComboBox *baudComboBox;
    QComboBox *dataComboBox;
    QComboBox *parComboBox;
    QComboBox *stopComboBox;


    void setSuperMode(const bool flag);
private slots:
    void on_clicked_ok();

};

#endif // COMCONFIGDIALOG_H
