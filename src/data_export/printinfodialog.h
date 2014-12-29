#ifndef PRINTINFODIALOG_H
#define PRINTINFODIALOG_H

#include <QDialog>

namespace Ui {
class PrintInfoDialog;
}

class PrintInfoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PrintInfoDialog(QWidget *parent = 0);
    ~PrintInfoDialog();
    void setTradeNumLabel(const quint16 tradeNum,const quint16 printedIndex);
    void getTradeIndex(quint16 *startIndex,quint16 *endIndex);

    void translateLanguage();
signals:

private:
    Ui::PrintInfoDialog *ui;
    quint16 maxTradeIndex;
private slots:

    void okButtonslot();
};

#endif // PRINTINFODIALOG_H
