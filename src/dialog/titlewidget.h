#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include "pushbutton.h"
#include "toolbutton.h"
class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = 0);
    void translateLanguage();
signals:
    void closeWidegt();
    void showMinWidget();
    void showMainMenuWidget();
    void turnPage(int curIndex);
public slots:
    void turnPage(QString pageNo);
    void hidePage(quint8 pageNo);
    void showPage(quint8 pageNo);
private:

    QPoint pressPoint;//鼠标按下去的点
    bool is_move;

    QLabel *versionTitle; //标题
    PushButton *mainMenuButton; //主菜单
    PushButton *minButton; //最小化
    PushButton *closeButton; //关闭


    QLabel *easivendLabel;
    QLabel *easivendLabelCh;
    QList<ToolButton *> buttonList;



};

#endif // TITLEWIDGET_H
