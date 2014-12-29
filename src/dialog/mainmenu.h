#ifndef MAINMENU_H
#define MAINMENU_H
#include <QMenu>

#include "../config.h"
class MainMenu :public QMenu
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget * parent = 0);

    void translateLanguage();
    void setLanguage(LANGUAGE language);
signals:
    void showAbountUs();

    void showSetting();
    void languageSignal(LANGUAGE language);
    void showVMCStatus();
private:
    QAction *actionAboutUs;//关于我们
    QAction *actionLanguage_ch;
    QAction *actionLanguage_en;

    QAction *actionSetting;

#if ACTION_SHOW_VMC_STATUS == 1
    QAction *actionStatus;//整机状态
#endif

#if ACTION_SHOW_HEFAN_DIALOG  == 1
    QAction *actionHefan;//盒饭柜
#endif


    QMenu *menuLanguage;

    void createActions();

private slots:
    void showLanguageMenu();
    void languageSelect(bool checked);
    void languageSelect(QAction *action);

};

#endif // MAINMENU_H
