#ifndef ADDGOODSDIALOG_H
#define ADDGOODSDIALOG_H

#include <QDialog>
#include "../SerialThead/serialthread.h"

class QButtonGroup;
namespace Ui {
class AddGoodsDialog;
}

class AddGoodsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddGoodsDialog(QWidget *parent = 0);
    ~AddGoodsDialog();
    
    void translateLanguage();
signals:
    void addGoodsSignal(QByteArray arr);
private:
    Ui::AddGoodsDialog *ui;

    QButtonGroup *buttonGroup;

private slots:
    void pushButtonOKClicked();
    void leveShow();
    void leveHide();
    void aisleShow();
    void aisleHide();
};

#endif // ADDGOODSDIALOG_H
