#include "mainmenu.h"

#include <QtGui>
MainMenu::MainMenu(QWidget *parent)
    :QMenu(parent)
{

    this->createActions();
    this->translateLanguage();

}


void MainMenu::createActions()
{

    actionAboutUs = new QAction(this);
    actionLanguage_ch = new QAction(this);
    actionLanguage_ch->setCheckable(true);
    actionLanguage_en = new QAction(this);
    actionLanguage_en->setCheckable(true);
    QSettings *read = new QSettings("./config.ini",QSettings::IniFormat);

    if(read->value("Language/Language").toString() == "ch")
    {
        languageSelect(actionLanguage_ch);
    }
    else
        languageSelect(actionLanguage_en);

    delete read;
    menuLanguage = new QMenu(this);
    QActionGroup *actionGroup = new QActionGroup(this);
    actionGroup->addAction(actionLanguage_ch);
    actionGroup->addAction(actionLanguage_en);
    actionGroup->setExclusive(true);
    menuLanguage->addAction(actionLanguage_ch);
    menuLanguage->addAction(actionLanguage_en);
    menuLanguage->setTitle("Language");

    actionSetting = new QAction(this);
    this->addAction(actionSetting);


//添加整机状态 Action
#if ACTION_SHOW_VMC_STATUS == 1
    actionStatus = new QAction(this);
    this->addAction(actionStatus);
    connect(actionStatus,SIGNAL(triggered()),
            this,SIGNAL(showVMCStatus()));
#endif

//盒饭柜action
#if ACTION_SHOW_HEFAN_DIALOG == 1
    actionHefan = new QAction(this);
    this->addAction(actionHefan);
//    connect(actionHefan,SIGNAL(triggered()),
//            this,SIGNAL(showHefanDialogSignal()));
#endif


    this->addMenu(menuLanguage);
    this->addAction(actionAboutUs);
    //动态切换语言 信号槽
    connect(actionLanguage_en,SIGNAL(toggled(bool))
            ,this,SLOT(languageSelect(bool)));
    connect(actionLanguage_ch,SIGNAL(toggled(bool))
            ,this,SLOT(languageSelect(bool)));
    connect(actionSetting,SIGNAL(triggered()),this,SIGNAL(showSetting()));
    connect(menuLanguage,SIGNAL(aboutToShow()),
            this,SLOT(showLanguageMenu()));
    connect(actionAboutUs,SIGNAL(triggered()),
            this,SIGNAL(showAbountUs()));

}



void MainMenu::translateLanguage()
{
    //繁体traditional
    actionAboutUs->setText(tr("Abount US"));
    actionLanguage_ch->setText("Chinese(simplifie)");
    actionLanguage_en->setText("English");
    actionSetting->setText(tr("Setting"));

#if ACTION_SHOW_VMC_STATUS == 1
    actionStatus->setText(tr("VMCStatus"));
#endif

#if ACTION_SHOW_HEFAN_DIALOG == 1
    actionHefan->setText(tr("Hefan"));
#endif
}


void MainMenu::showLanguageMenu()
{

    //qDebug()<<"showLanguageMenu";
}


void MainMenu::languageSelect(bool checked)
{
    if(!checked)
    {
        return;
    }
    LANGUAGE language;
    //qDebug()<<"checked"<<checked;
    QAction *action = qobject_cast<QAction *>(sender());
   // qDebug()<<"languageSelect action"<<action;
    if(action == actionLanguage_en)
    {
       // qDebug()<<"actionLanguage_en";
        language = VMC_EN;
    }
    else if(action == actionLanguage_ch)
    {
       // qDebug()<<"actionLanguage_ch";
        language = VMC_ZH;
    }
    else
    {
        return;
    }

    emit languageSignal(language);

}


void MainMenu::languageSelect(QAction *action)
{

    if(action)
    {
       actionLanguage_ch->setChecked(false);
       actionLanguage_en->setChecked(false);
       action->setChecked(true);
    }
}

void MainMenu::setLanguage(LANGUAGE language)
{
    if(language == VMC_EN)
    {
        actionLanguage_en->setChecked(true);
    }
    else if(language == VMC_ZH)
    {
        actionLanguage_ch->setChecked(true);
    }
    else
    {}

}
