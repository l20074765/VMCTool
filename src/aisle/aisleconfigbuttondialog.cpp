#include "aisleconfigbuttondialog.h"
#include "ui_aisleconfigbuttondialog.h"
#include <QtGui>
AisleConfigButtonDialog::AisleConfigButtonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AisleConfigButtonDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);


    //开关布局
    QVBoxLayout *layoutRadioON = new QVBoxLayout();
    layoutRadioON->addWidget(ui->radioButtonON,Qt::AlignCenter);
    layoutRadioON->addWidget(ui->labelON,50,Qt::AlignCenter);

    layoutRadioON->setContentsMargins(4,0,0,0);
    layoutRadioON->setSpacing(0);

    QVBoxLayout *layoutRadioOFF = new QVBoxLayout();
    layoutRadioOFF->addWidget(ui->radioButtonOFF,Qt::AlignCenter);
    layoutRadioOFF->addWidget(ui->labelOFF,500,Qt::AlignCenter);
    layoutRadioOFF->setContentsMargins(0,0,4,0);
    layoutRadioOFF->setSpacing(0);

    QHBoxLayout *layoutRadio = new QHBoxLayout();
    layoutRadio->addLayout(layoutRadioON);
    layoutRadio->addStretch();
    layoutRadio->addLayout(layoutRadioOFF);
    layoutRadio->setContentsMargins(0,0,0,0);
    layoutRadio->setSpacing(1);


    //储货量
    QHBoxLayout *layoutStore = new QHBoxLayout();
    layoutStore->addWidget(ui->labelMaxCount,Qt::AlignCenter);
    layoutStore->addStretch();
    layoutStore->addWidget(ui->lineEditMaxCount,Qt::AlignCenter);
    layoutStore->setContentsMargins(0,0,0,0);
    layoutStore->setSpacing(0);



    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(ui->labelNo);
    layout->addStretch();
    layout->addLayout(layoutStore);
    layout->addLayout(layoutRadio);
    layout->setContentsMargins(1,1,1,1);
    layout->setSpacing(0);


    this->setLayout(layout);

    entered = false;
//    connect(ui->radioButtonOFF,SIGNAL(toggled(bool)),
//            this,SLOT(buttonOFFSelectSlot(bool)));
    connect(ui->radioButtonON,SIGNAL(toggled(bool)),
            this,SLOT(buttonONSelectSlot(bool)));

    connect(ui->lineEditMaxCount,SIGNAL(textChanged(QString)),
            this,SIGNAL(maxCountChangedSignal(QString)));


    QFont font = this->font();
    QPalette palette = this->palette();
    font.setBold(false);
    font.setPointSize(10);
    palette.setColor(QPalette::WindowText,QColor(Qt::black));
    ui->labelMaxCount->setFont(font);
    ui->labelMaxCount->setPalette(palette);
    ui->lineEditMaxCount->setFont(font);
    ui->lineEditMaxCount->setPalette(palette);

    ui->lineEditMaxCount->setObjectName("maxCount");
    this->setContextMenuPolicy(Qt::DefaultContextMenu);




}

AisleConfigButtonDialog::~AisleConfigButtonDialog()
{
    delete ui;
}
void AisleConfigButtonDialog::translateLanguage()
{
    //ui->retranslateUi(this);
    ui->labelMaxCount->setText(tr("Store"));
    ui->labelON->setText(tr("ON"));
    ui->labelOFF->setText(tr("OFF"));
}

void AisleConfigButtonDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        this->setOpen(!this->getOpen());
    }
    event->ignore();

}

void AisleConfigButtonDialog::setText(const QString &text)
{
    QFont font = this->font();
    QPalette palette = this->palette();

    if(this->isGroup())
    {
        font.setBold(true);
        font.setPointSize(12);
        palette.setColor(QPalette::WindowText,QColor(255, 170, 0));
    }
    else
    {
        font.setBold(false);
        font.setPointSize(13);
        palette.setColor(QPalette::WindowText,QColor(170, 0, 0));
    }
    ui->labelNo->setFont(font);
    ui->labelNo->setPalette(palette);
    ui->labelNo->setText(text);
}


bool AisleConfigButtonDialog::getOpen()
{
    //qDebug()<<"getButtonState"<<ui->radioButtonON->isChecked();
    return ui->radioButtonON->isChecked();
}
void AisleConfigButtonDialog::setOpen(const bool flag)
{
    if(flag)
    {
        if(this->isGroup())
        {
           setButtonStyle(LEVEON);
           ui->lineEditMaxCount->show();
           ui->labelMaxCount->show();
        }
        else
        {
            setButtonStyle(AISLEON);
        }
        ui->lineEditMaxCount->setText("10");
        ui->radioButtonON->setChecked(true);
        ui->radioButtonOFF->setChecked(false);

    }
    else
    {
        if(this->isGroup())
        {
            setButtonStyle(LEVEOFF);
            ui->lineEditMaxCount->hide();
            ui->labelMaxCount->hide();
        }
        else
            setButtonStyle(AISLEOFF);
        ui->lineEditMaxCount->setText("0");
        ui->radioButtonON->setChecked(false);
        ui->radioButtonOFF->setChecked(true);
    }
}

void AisleConfigButtonDialog::buttonOFFSelectSlot(bool flag)
{
    qDebug()<<"buttonOFFSelectSlot"<<flag;

}
void AisleConfigButtonDialog::buttonONSelectSlot(bool flag)
{
    //qDebug()<<"buttonONSelectSlot"<<flag;
    if(this->isGroup())
    {
        emit openClickedSinal(flag);
    }
    else
    {
        this->setOpen(flag);
    }
}
void AisleConfigButtonDialog::setGroup(bool flag)
{
    groupflag = flag;
    if(groupflag)
    {
        ui->labelMaxCount->hide();
        ui->lineEditMaxCount->hide();
    }
    else
    {
        ui->labelMaxCount->show();
        ui->lineEditMaxCount->show();
    }
}
void AisleConfigButtonDialog::setButtonStyle(quint8 style)
{
    if(style == LEVEON)
    {
        setStyleSheet("QDialog{background-color: rgb(0, 0, 255,180);}");

    }
    else if(style == LEVEOFF)
    {
        setStyleSheet("QDialog{background-color: rgba(106, 106, 106, 255);}");

    }
    else if(style == AISLEON)
    {
        setStyleSheet("QDialog{background-color: rgb(0, 170, 225,240);}");

    }
    else if(style == AISLEOFF)
    {
        setStyleSheet("QDialog{background-color: rgba(106, 106, 106, 150);}");

    }
}

quint8 AisleConfigButtonDialog::getMaxCount()
{
    bool ok;
    return ui->lineEditMaxCount->text().toInt(&ok,10);
}

void AisleConfigButtonDialog::setMaxCount(quint8 count)
{
    ui->lineEditMaxCount->setText(QString("%1").arg(count));
}

void AisleConfigButtonDialog::setMaxCount(QString text)
{
    ui->lineEditMaxCount->setText(text);
}

void AisleConfigButtonDialog::contextMenuEvent(QContextMenuEvent *ev)
{
    ev->ignore();

    //目前没时间过年了暂时不实现
    return;

    QCursor cur = this->cursor();

#if 0
    QMenu *menu = new QMenu(this);
    QAction *action_test = new QAction(this);
    QFont font = this->font();
    font.setPointSize(12);
    action_test->setFont(font);

    menu->addAction(action_test);
    if(!this->isGroup())
        action_test->setText(tr("Test the column"));
    else
    {
        action_test->setText(tr("Test the columns of a layer"));
        QAction *action_all_test = new QAction(this);
        action_all_test->setFont(font);
        action_all_test->setText(tr("Test behind all of the goods"));
        menu->addAction(action_all_test);
        connect(action_all_test,SIGNAL(triggered()),this,SLOT(actionAllTestSlot()));
    }

    qDebug()<<"AisleConfigButtonDialog::contextMenuEvent"<<menu
              <<action_test;
    connect(action_test,SIGNAL(triggered()),this,SLOT(actionTestSlot()));
    menu->exec(cur.pos());
#endif

    if(!this->getOpen())
        return;

    QMenu menu(this);
    QAction action_test(this);
    QFont font = this->font();
    font.setPointSize(12);
    action_test.setFont(font);
    menu.addAction(&action_test);
    if(!this->isGroup())
        action_test.setText(tr("Test the column"));
    else
    {
        action_test.setText(tr("Test the columns of a layer"));
        QAction action_all_test(this);
        action_all_test.setFont(font);
        action_all_test.setText(tr("Test behind all of the goods"));
        menu.addAction(&action_all_test);
        connect(&action_all_test,SIGNAL(triggered()),this,SLOT(actionAllTestSlot()));
    }

    qDebug()<<"AisleConfigButtonDialog::contextMenuEvent"<<&menu
              <<&action_test;
    connect(&action_test,SIGNAL(triggered()),this,SLOT(actionTestSlot()));
    menu.exec(cur.pos());





}


void AisleConfigButtonDialog::actionTestSlot()
{
    emit actionTestSignal(false,*this);
}

void AisleConfigButtonDialog::actionAllTestSlot()
{
    emit actionTestSignal(true,*this);
}


void AisleConfigButtonDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(QColor(Qt::black),2,
                   Qt::SolidLine,Qt::RoundCap,
                   Qt::RoundJoin));
    if(entered)
    {
        painter.setBrush(QColor(0,0,0,100));
    }
    painter.drawRect(this->rect());

}


void AisleConfigButtonDialog::enterEvent(QEvent *)
{
    entered = true;
    this->update();
}

void AisleConfigButtonDialog::leaveEvent(QEvent *)
{
    entered = false;
    this->update();
}
