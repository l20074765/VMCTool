#ifndef AISLECONFIGBUTTONDIALOG_H
#define AISLECONFIGBUTTONDIALOG_H

#include <QDialog>

enum BUTTONSTYLE
{
    LEVEON,
    LEVEOFF,
    AISLEON,
    AISLEOFF
};
namespace Ui {
class AisleConfigButtonDialog;
}

class AisleConfigButtonDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AisleConfigButtonDialog(QWidget *parent = 0);
    ~AisleConfigButtonDialog();

    void setText(const QString &text);
    inline bool isGroup() const {return groupflag;}
    void setGroup( bool flag = false);
    inline void setGroupNo(quint8 no){groupNo = no;}
    inline quint8 getGroupNo()const{return groupNo;}
    inline void setAisleNo(quint8 no = 0){aisleNo = no;}
    inline quint8 getAisleNo()const {return aisleNo;}

    void setButtonStyle(quint8 style);
    bool getOpen();
    void setOpen(const bool flag);

    void setMaxCount(quint8 count);
    void setMaxCount(QString text);
    quint8 getMaxCount();
    void translateLanguage();
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *ev);
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
signals:
    void openClickedSinal(bool checked);
    void maxCountChangedSignal(QString text);
    void actionTestSignal(const bool allTest,const AisleConfigButtonDialog &button);
private:
    Ui::AisleConfigButtonDialog *ui;

    bool groupflag;
    quint8 groupNo;
    quint8 aisleNo;
    bool entered;

private slots:
    void buttonONSelectSlot(bool flag);
    void buttonOFFSelectSlot(bool flag);
    void actionTestSlot();
    void actionAllTestSlot();


};

#endif // AISLECONFIGBUTTONDIALOG_H
