#include "statusdialog.h"

#include <QtGui>
StatusDialog::StatusDialog(QWidget *parent) :
    QDialog(parent)

{

    timerStatusReq = new QTimer(this);
    connect(timerStatusReq,SIGNAL(timeout()),
            this,SLOT(timerStatusReqTimeoutSlot()));


    uiInit();
    languageTranslation();

    vmcConnect(false);

}

StatusDialog::~StatusDialog()
{

}

void StatusDialog::uiInit()
{

    groupBoxVMC = new QGroupBox(this);

    labelVMCRunState = new QLabel(groupBoxVMC);
    labelVMCRunStateRst = new QLabel(groupBoxVMC);

    //售货机运行状态
    QHBoxLayout *layoutVMCRunState = new QHBoxLayout();
    layoutVMCRunState->addWidget(labelVMCRunState,Qt::AlignLeft | Qt::AlignTop);
    layoutVMCRunState->addWidget(labelVMCRunStateRst,Qt::AlignLeft | Qt::AlignTop);


    QVBoxLayout *layoutVMCRun = new QVBoxLayout(groupBoxVMC);
    layoutVMCRun->addLayout(layoutVMCRunState);
    //layoutVMCRun->addStretch();



    groupBill = new QGroupBox(this);
    //纸币器类型
    labelBillType = new QLabel(groupBill);
    labelBillTypeRst = new QLabel(groupBill);
    QHBoxLayout *layoutBillType = new QHBoxLayout();
    layoutBillType->addWidget(labelBillType,Qt::AlignLeft | Qt::AlignTop);
    layoutBillType->addWidget(labelBillTypeRst,Qt::AlignLeft | Qt::AlignTop);

    //纸币器状态
    labelBillState =  new QLabel(groupBill);
    labelBillStateRst = new QLabel(groupBill);
    QHBoxLayout *layoutBillState = new QHBoxLayout();
    layoutBillState->addWidget(labelBillState,Qt::AlignLeft | Qt::AlignTop);
    layoutBillState->addWidget(labelBillStateRst,Qt::AlignLeft | Qt::AlignTop);

    QVBoxLayout *layoutBill  = new QVBoxLayout(groupBill);
    layoutBill->addLayout(layoutBillType);
    layoutBill->addLayout(layoutBillState);
    layoutBill->addStretch();

    //硬币器类型
    groupCoin = new QGroupBox(this);
    //硬币器接收器类型
    labelCoinRecvType = new QLabel(groupCoin);
    labelCoinRecvTypeRst = new QLabel(groupCoin);
    QHBoxLayout *layoutCoinRecvType = new QHBoxLayout();
    layoutCoinRecvType->addWidget(labelCoinRecvType,Qt::AlignLeft | Qt::AlignTop);
    layoutCoinRecvType->addWidget(labelCoinRecvTypeRst,Qt::AlignLeft | Qt::AlignTop);


    //硬币器找零器类型
    labelCoinChangeType = new QLabel(groupCoin);
    labelCoinChangeTypeRst = new QLabel(groupCoin);
    QHBoxLayout *layoutCoinChangeType = new QHBoxLayout();
    layoutCoinChangeType->addWidget(labelCoinChangeType,Qt::AlignLeft | Qt::AlignTop);
    layoutCoinChangeType->addWidget(labelCoinChangeTypeRst,Qt::AlignLeft | Qt::AlignTop);

    //硬币器状态 包括接收器和找零器
    labelCoinState = new QLabel(groupCoin);
    labelCoinStateRst = new QLabel(groupCoin);
    QHBoxLayout *layoutCoinState = new QHBoxLayout();
    layoutCoinState->addWidget(labelCoinState,Qt::AlignLeft | Qt::AlignTop);
    layoutCoinState->addWidget(labelCoinStateRst,Qt::AlignLeft | Qt::AlignTop);

    //hopper找零器额外状态
    labelCoinHopperState = new QLabel(groupCoin);
    labelCoinHopperStateRst = new QLabel(groupCoin);
    QHBoxLayout *layoutCoinHopperState = new QHBoxLayout();
    layoutCoinHopperState->addWidget(labelCoinHopperState,Qt::AlignLeft | Qt::AlignTop);
    layoutCoinHopperState->addWidget(labelCoinHopperStateRst,Qt::AlignLeft | Qt::AlignTop);

    QVBoxLayout *layoutCoin = new QVBoxLayout(groupCoin);
    layoutCoin->addLayout(layoutCoinRecvType);
    layoutCoin->addLayout(layoutCoinChangeType);
    layoutCoin->addLayout(layoutCoinState);
    layoutCoin->addLayout(layoutCoinHopperState);
    layoutCoin->addStretch();

    //主柜室状态
    groupCabine1 = new QGroupBox(this);
    labelCabinet1RunState = new QLabel(groupCabine1);
    labelCabinet1RunStateRst = new QLabel(groupCabine1);
    QHBoxLayout *layoutCabinet1Run = new QHBoxLayout();
    layoutCabinet1Run->addWidget(labelCabinet1RunState,Qt::AlignLeft | Qt::AlignTop);
    layoutCabinet1Run->addWidget(labelCabinet1RunStateRst,Qt::AlignLeft | Qt::AlignTop);

    //主柜故障状态
    labelCabinet1FaultState = new QLabel(groupCabine1);
    labelCabinet1FaultStateRst = new QLabel(groupCabine1);
    QHBoxLayout *layoutCabinet1Fault = new QHBoxLayout();
    layoutCabinet1Fault->addWidget(labelCabinet1FaultState,Qt::AlignLeft | Qt::AlignTop);
    layoutCabinet1Fault->addWidget(labelCabinet1FaultStateRst,Qt::AlignLeft | Qt::AlignTop);

    QVBoxLayout *layoutCabinet1 = new QVBoxLayout(groupCabine1);
    layoutCabinet1->addLayout(layoutCabinet1Run);
    layoutCabinet1->addLayout(layoutCabinet1Fault);
    layoutCabinet1->addStretch();

    QVBoxLayout *layout_bill_coin = new QVBoxLayout();
    layout_bill_coin->addWidget(groupBoxVMC);
    layout_bill_coin->addWidget(groupBill);
    layout_bill_coin->addWidget(groupCoin);

    QHBoxLayout *layout = new QHBoxLayout();

    layout->addLayout(layout_bill_coin);
    layout->addWidget(groupCabine1);



    //layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);


    setFixedSize(800,500);



}

void StatusDialog::languageTranslation()
{
    QFont font = this->font();
    font.setPointSize(12);
    QList<QLabel*> list;
    list<<labelVMCRunState<<labelBillType<<labelBillState
          <<labelCoinRecvType<<labelCoinChangeType<<labelCoinState
            <<labelCoinHopperState<<labelCoinHopperState<<labelCabinet1FaultState
              <<labelCabinet1RunState;

    foreach(QLabel *label,list)
    {
        label->setFont(font);
    }

    labelVMCRunState->setText(tr("VMCRunState:"));
    labelBillType->setText(tr("BillType"));
    labelBillState->setText(tr("BillState:"));
    labelCoinRecvType->setText(tr("CoinRecvType:"));
    labelCoinChangeType->setText(tr("CoinChangeType:"));

    labelCoinState->setText(tr("CoinState:"));
    labelCoinHopperState->setText(tr("CoinHopperState:"));
    labelCabinet1RunState->setText(tr("Cabinet1RunState:"));
    labelCabinet1FaultState->setText(tr("Cabinet1FaultState:"));



    groupBoxVMC->setTitle(tr("VMC"));
    groupBill->setTitle(tr("Bill"));
    groupCoin->setTitle(tr("Coin"));
    groupCabine1->setTitle(tr("Cabinet1"));

    groupBoxVMC->setObjectName("vmcStatus");
    groupBill->setObjectName("vmcStatus");
    groupCoin->setObjectName("vmcStatus");
    groupCabine1->setObjectName("vmcStatus");




}

void StatusDialog::timerStatusReqStart()
{
    this->timerStatusReq->start(1000);
}
void StatusDialog::pcStatusRpt(const QByteArray &arr)
{
    quint8 index = 0;
    quint8 type = (quint8)arr.at(index++);
    if(type == 0)
    {
       quint8 vmcRunState =  (quint8)arr.at(index++);
       QString tempStr;
       if(vmcRunState == 0x00)
       {
           tempStr = tr("Normal");
       }
       else if(vmcRunState == 0x01)
       {
           tempStr = tr("GoodsEmpty");
       }
       else if(vmcRunState == 0x02)
       {
           tempStr = tr("Fault");
       }
       else if(vmcRunState == 0x03)
       {
           tempStr = tr("Maintenance");
       }
       else if(vmcRunState == 0x04)
       {
           tempStr = tr("Initializing");
       }
       else
           tempStr = tr("Unknown state");

       labelVMCRunStateRst->setText(tempStr);

       //纸币器类型
       quint8 billType =  (quint8)arr.at(index++);
       switch(billType)
       {

            case 0:
                tempStr = tr("Close");
                break;
           case 1:
               tempStr = tr("SSP");
               break;
           case 2:
               tempStr = tr("MDB");
               break;
            default:
                tempStr = tr("Unknown");
                break;
       }
       labelBillTypeRst->setText(tempStr);


       //纸币器故障状态
       quint8 billstate =  (quint8)arr.at(index++);
       billstate = billstate;
       quint8 billstate1 =  (quint8)arr.at(index++);
       tempStr.clear();
       tempStr += (billstate1 & 0x01) ? tr("Communication failure\n"): "";
       tempStr += (billstate1 & 0x02) ? tr("Motor fault\n"): "";
       tempStr += (billstate1 & 0x04) ? tr("Sensor fault \n"): "";
       tempStr += (billstate1 & 0x08) ? tr("ROM fault\n"): "";
       tempStr += (billstate1 & 0x10) ? tr("Bill slot fault\n"): "";
       tempStr += (billstate1 & 0x20) ? tr("Remove the bill box\n"): "";
       tempStr += (billstate1 & 0x40) ? tr("Bill disabled\n"): "";
       tempStr += (billstate1 & 0x80) ? tr("Bill suspend\n"): "";
       tempStr += (tempStr.isEmpty() && billType) ? tr("Normal\n"): "";

       labelBillStateRst->setText(tempStr);

       quint8 coinType = (quint8)arr.at(index++);
       switch((coinType & 0xF0) >> 4)
       {
          case 0:
               tempStr = tr("Close");
               break;
          case 1:
              tempStr = tr("Parallel pulse");
              break;
          case 2:
              tempStr = tr("MDB");
              break;
           case 3:
               tempStr = tr("Serial pulse");
               break;
           default:
               tempStr = tr("Unknown");
               break;
       }
       labelCoinRecvTypeRst->setText(tempStr);

       switch(coinType & 0x0F)
       {
          case 0:
               tempStr = tr("Close");
               break;
          case 1:
              tempStr = tr("hopper");
              break;
          case 2:
              tempStr = tr("MDB");
              break;
           default:
               tempStr = tr("Unknown");
               break;
       }
       labelCoinChangeTypeRst->setText(tempStr);

       quint8 coinState1 = (quint8)arr.at(index++);
       coinState1 = coinState1;
       quint8 coinState2 = (quint8)arr.at(index++);
       quint8 coinState3 = (quint8)arr.at(index++);
       quint8 coinState4 = (quint8)arr.at(index++);
       tempStr.clear();
       tempStr += (coinState4 & 0x01) ? tr("Communication failure\n"): "";
       tempStr += (coinState4 & 0x02) ? tr("Sensor fault\n"): "";
       tempStr += (coinState4 & 0x04) ? tr("Coin slot fault\n"): "";
       tempStr += (coinState4 & 0x08) ? tr("ROM fault\n"): "";
       tempStr += (coinState4 & 0x10) ? tr("Coin receiver fault\n"): "";
       tempStr += (coinState4 & 0x20) ? tr("Remove the coin bucket\n"): "";
       tempStr += (coinState4 & 0x40) ? tr("Coin disabled\n"): "";
       tempStr += (coinState4 & 0x80) ? tr("Coin suspend\n"): "";
       //第二字节
       tempStr += (coinState3 & 0x01) ? tr("Other fault\n"): "";

       tempStr += (tempStr.isEmpty() && (coinType & 0xF0)) ? tr("Normal"): "";
       labelCoinStateRst->setText(tempStr);

       if((coinType & 0x0F) == 1)//hopper附加状态
       {
           tempStr.clear();
           tempStr += (coinState2 & 0x01) ? tr("hopper1 lack\n"): "";
           tempStr += (coinState2 & 0x02) ? tr("hopper1 fault\n"): "";
           tempStr += (coinState2 & 0x04) ? tr("hopper2 lack\n"): "";
           tempStr += (coinState2 & 0x08) ? tr("hopper2 lack\n"): "";
           tempStr += (coinState2 & 0x10) ? tr("hopper3 lack\n"): "";
           tempStr += (coinState2 & 0x20) ? tr("hopper3 lack\n"): "";
           labelCoinHopperStateRst->setText(tempStr);

       }

       //主柜室运行状态
       quint8 cabinet1Run = (quint8)arr.at(index++);
       quint8 cabinet1Run1 = (quint8)arr.at(index++);
        cabinet1Run = cabinet1Run;
        cabinet1Run1 = cabinet1Run1;



       //主柜故障状态
       quint8 cabinet1Fault = (quint8)arr.at(index++);
       cabinet1Fault = cabinet1Fault;
       quint8 cabinet1Fault1 = (quint8)arr.at(index++);

       tempStr.clear();
       tempStr += (cabinet1Fault1 & 0x01) ? tr("All column fault\n"): "";
       tempStr += (cabinet1Fault1 & 0x02) ? tr("Column driver fault\n"): "";
       tempStr += (cabinet1Fault1 & 0x04) ? tr("GOC fault\n"): "";
       tempStr += (cabinet1Fault1 & 0x08) ? tr("Elevator fault\n"): "";
       tempStr += (cabinet1Fault1 & 0x10) ? tr("QUHUO  fault\n"): "";
       tempStr += (cabinet1Fault1 & 0x20) ? tr("VTD fault\n"): "";
       tempStr += (cabinet1Fault1 & 0x40) ? tr("Room light fault\n"): "";
       tempStr += (cabinet1Fault1 & 0x80) ? tr("Room compressor fault\n"): "";
       tempStr += (tempStr.isEmpty()) ? tr("Normal\n"): "";
       labelCabinet1FaultStateRst->setText(tempStr);




    }



  //  this->textEdit->insertPlainText(QString(arr.toHex()));
}

void StatusDialog::timerStatusReqTimeoutSlot()
{
   if(!isConnected())
       return;
   QByteArray arr;
   QDataStream in(&arr,QIODevice::WriteOnly);
   in<<(quint8)0x00;
   emit pcStatusReq(STATUS_REQ,arr);
}


void StatusDialog::hideEvent(QHideEvent *ev)
{
    ev->accept();
    timerStatusReq->stop();
}
