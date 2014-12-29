#ifndef VMCDEBUGDIALOG_H
#define VMCDEBUGDIALOG_H

#include <QDialog>
#include "hdbutton.h"
#include "../SerialThead/serialthread.h"
namespace Ui {
class VMCDebugDialog;
}

class VMCDebugDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit VMCDebugDialog(QWidget *parent = 0);
    ~VMCDebugDialog();
    void setSwicth(const bool checked);
    void translateLanguage();
public slots:
    void updateDebug(QByteArray arr);
    void updateDebug(const quint8 type,const bool checked);
signals:
    void debugRequstSignal(quint8 type,QByteArray arr);
private:
    Ui::VMCDebugDialog *ui;
    QList<HDButton *> listButton;

private slots:
    void buttonClickedSlot(int id);

};

#endif // VMCDEBUGDIALOG_H
