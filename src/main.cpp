#include "./dialog/mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QDateTime>
#include <QTextStream>
#include <QtPlugin>
#include <QPluginLoader>
#include <QDebug>
#include <QTranslator>
//#define print_log qDebug
#include <QSettings>
#include <QTextEdit>
#include <QDir>


#define VMC_RELEASE  0
void myMessageOutput(QtMsgType type, const char *msg)
{
    QString text;

    switch (type) {
    case QtDebugMsg:
        text = "Debug:";
        break;
    case QtWarningMsg:
        text = "Warning:";
        break;
    case QtCriticalMsg:
        text = "Critical:";
        break;
    case QtFatalMsg:
        text = "Fatal:";
        break;
    }
    QString message = QDateTime::currentDateTime().toString("hh:mm:ss:zzz ");
    QString logName = QDateTime::currentDateTime().toString("yyyyMMdd");
    message.append(" ").append(text).append(msg);
    QFile file("./log/"+logName +".txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream messageFile(&file);
    messageFile<< message << endl;


    file.close();
}



int main(int argc,char *argv[])
{
#if(VMC_RELEASE == 1)
      qInstallMsgHandler(myMessageOutput);
#endif
    QApplication app(argc,argv);

    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);

    QFile qss(":/qss/vmctool.qss");
    qss.open(QFile::ReadOnly);
    app.setStyleSheet(qss.readAll());
    qss.close();

    LANGUAGE language;
    bool ok;
    QSettings *read = new  QSettings("./config.ini",QSettings::IniFormat);
    language = (LANGUAGE)read->value("Language/Language").toInt(&ok);
    delete read;

    QTranslator translator;
    QTranslator translator_qt;

    if(language == VMC_ZH)
    {
        translator.load(":/language/vmctool_ch.qm");
        translator_qt.load(":/language/qt_zh_CN.qm");
    }

    app.installTranslator(&translator);
    app.installTranslator(&translator_qt);





    MainWindow mainWindows;
    mainWindows.setTranslator(&translator,&translator_qt);
    mainWindows.setLanguage(language);

    mainWindows.show();

    return app.exec();


}

