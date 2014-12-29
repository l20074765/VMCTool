#ifndef AISLEBUTTON_H
#define AISLEBUTTON_H
#include <QLabel>
class AisleButton : public QLabel
{
    Q_OBJECT
public:
    AisleButton(QWidget *parent = 0);
    ~AisleButton();
    inline bool isChecked() const {return checked;}

    inline bool isEnabled() const {return enabled;}
    inline void setEnabled(bool flag = false){enabled = flag;}
    inline void setChecked(bool flag = false){checked = flag;}

signals:
    void clicked(bool checked);

private:

    bool enabled;
    bool checked;
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
public slots:
    void click();
};

#endif // AISLEBUTTON_H
