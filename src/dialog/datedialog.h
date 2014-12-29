#ifndef DATEDIALOG_H
#define DATEDIALOG_H

#include <QDialog>
#include "../SerialThead/serialthread.h"

class QDateTime;
namespace Ui {
class DateDialog;
}

class DateDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DateDialog(QWidget *parent = 0);
    ~DateDialog();
    void translateLanguage();
signals:
    void dateTimeSetSiganl(quint8 type,QByteArray arr);
    void resultSignal(bool flag,QString text);
public slots:
    void setDateTime();
private slots:
    void on_pushButton_2_clicked();

    void lineEditedSlot(QString text);
private:
    Ui::DateDialog *ui;
    enum PTEXT{DATESETTING};
    QString processText(PTEXT pText);



};

#endif // DATEDIALOG_H
