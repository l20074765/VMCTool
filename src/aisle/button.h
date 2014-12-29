#ifndef BUTTON_H
#define BUTTON_H
#include <QWidget>
#include <QLabel>
#include "aislebutton.h"

enum TYPE{
    ENTER,
    LEAVE,
    SELECT,
    PRESSD,
    HEAVED,
    NORMAL
};
class Button : public QWidget
{
    Q_OBJECT
public:
    Button(QWidget * parent = 0);
    ~Button();
    void setText(const QString &text);
    QString text(){return logicNoLabel->text();}
    inline bool isGroup() const {return groupflag;}
    inline void setGroup( bool flag = false){groupflag = flag;}
    inline void setGroupNo(quint8 no){groupNo = no;}
    inline quint8 getGroupNo()const{return groupNo;}
    inline void setAisleNo(quint8 no = 0){aisleNo = no;}
    inline quint8 getAisleNo()const {return aisleNo;}


    inline bool isChecked() const {return openAndCloseButton->isChecked();}
    inline bool isEnabled() const {return openAndCloseButton->isChecked();}
    inline void setEnabled(bool flag = false){openAndCloseButton->setEnabled(flag);}
    inline void setChecked(bool flag = false){openAndCloseButton->setChecked(flag);}

    inline void setButtonEnabled(bool flag = false){buttonEnable = flag;}
    inline bool getButtonEnabled() const {return buttonEnable;}

    inline bool IsOpen(){return openAndCloseButton->isChecked();}
    void setOpen(bool flag = false);
    void setON(bool flag = false);
    inline quint8 getLabelStatus(){return labelStatus;}
    inline quint8 getButtonStatus(){return buttonStatus;}
    inline void setLabelStatus(quint8 type){ labelStatus = type;}
    inline void setButtonStatus(quint8 type){buttonStatus = type;}
    void buttonStye(quint8 type);
    void labelStye(quint8 type);
    void upStye(quint8 label,quint8 button);

    void unselect();
    void select();
    bool isSelect()const{return selected;}

    inline void buttonHide(){openAndCloseButton->hide();}

signals:
    void ButtonClickedSignal(bool checked);
    void ButtonEnterSignal();
    void ButtonleaveSignal();
    void buttonSelectSignal();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);



private slots:
  void button_clicked(bool checked);
private:

    QLabel *logicNoLabel;
    AisleButton *openAndCloseButton;
    bool groupflag;
    quint8 groupNo;
    quint8 aisleNo;
    quint8 labelStatus;
    quint8 buttonStatus;
    bool selected;
    bool buttonEnable;



};

#endif // BUTTON_H
