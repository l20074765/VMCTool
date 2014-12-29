#include "config.h"
#include <QtCore>


LEVEAISLE leveAisleST[8];
volatile static quint8 aisleType = 0,subAsileType = 0;
static quint8 aisleCabinet = 1;

static VMC_SYSTEM_PARA vmc_system_para;

static HEFAN_CABINET stHefanCabinet;//盒饭柜货道信息结构体
static quint8 hefanCabinetNo;



quint32 amountToScale(const quint32 amount)
{
    quint32 value = amount;
    for(int i = 0; i < vmc_system_para.scale;i++)
    {
        value /= 10;
    }
    return value;
}


quint32 amountFromScale(const quint32 amount)
{
    quint32 value = amount;
    for(int i = 0; i < vmc_system_para.scale;i++)
    {
        value *= 10;
    }
    return value;
}






void setVMCSystemPara(const quint8 type,const quint8 value)
{
    if(type == VMC_SYSTEM_SELECTBOARD)
    {
       vmc_system_para.isOpenSelectBoard = (bool)value;
    }
    else if(type == VMC_SYSTEM_PCTYPE)
    {
        vmc_system_para.isOpenPC = (bool)value;
    }
    else if(type == VMC_SYSTEM_LOGO)
    {
        vmc_system_para.logo = value;
    }
    else if(type == VMC_SYSTEM_SCALE)
        vmc_system_para.scale = value;
    else if(type == VMC_SYSTEM_SLAVE_CABINET)
        vmc_system_para.isOpenSlaveCabinet = (bool)value;

}

quint8 getVMCSystemPara(const quint8 type)
{

    if(type  == VMC_SYSTEM_SELECTBOARD)
    {
        return (quint8)vmc_system_para.isOpenSelectBoard;
    }
    else if(type == VMC_SYSTEM_PCTYPE)
    {
        return (quint8)vmc_system_para.isOpenPC;
    }
    else if(type == VMC_SYSTEM_LOGO)
    {
        return (quint8)vmc_system_para.logo;
    }
    else if(type == VMC_SYSTEM_SLAVE_CABINET)
        return (quint8)vmc_system_para.isOpenSlaveCabinet;
    else if(type == VMC_SYSTEM_SCALE)
        return (quint8)vmc_system_para.scale;
    else
        return 0;
}

void initAisle(const quint8 type)
{
    quint8 temp,temp1;
    temp = type;
    temp = aisleType;
    //8*8 and 8*10
    memset((void *)&leveAisleST,0,sizeof(leveAisleST));
    for(int i = 0;i < 8;i++)
    {
        leveAisleST[i].isOpen = false;
        leveAisleST[i].leveMaxCount = 0;
        leveAisleST[i].leveNum = i + 1;
        for(int j = 0;j < temp;j++)
        {
            if(j >= 9)
                temp1 = (i + 1) * 10;
            else
                temp1 = (i + 1) * 10 + j + 1;
            leveAisleST[i].aisle[j].physicNo = temp1;
            leveAisleST[i].aisle[j].isOpen = false;
            leveAisleST[i].aisle[j].logicNo = 0;
            leveAisleST[i].aisle[j].maxCount = 0;
            leveAisleST[i].aisle[j].id = 0;
            leveAisleST[i].aisle[j].price = 0;
            leveAisleST[i].aisle[j].remain = 0;
            leveAisleST[i].aisle[j].status = 0;
            leveAisleST[i].aisle[j].tradeNum = 0;
            leveAisleST[i].aisle[j].selectNo = 0;
            //qDebug()<<QString("[%1] %2").arg(i + 1).arg(leveAisleST[i].aisle[j].physicNo);
        }
    }

}

void setAisleType(const quint8 cabinetNo,const quint8 type)
{
    quint8 temp = (type == 0) ? 8: 10;
    if(cabinetNo == 1)
        aisleType = temp;
    else if(cabinetNo == 2)
        subAsileType  = temp;

}

quint8 getAisleType(const quint8 cabinetNo)
{
    //qDebug()<<"getAisleType: aisleType="<<aisleType;
    if(cabinetNo == 1)
        return (aisleType == 10) ? 10 : 8;
    else if(cabinetNo == 2)
        return (subAsileType == 10) ? 10 : 8;
    else
        return 0;

}


bool getAisleIndexOfLogicNo(quint8 *index_i,quint8 *index_j,quint8 logicNo,quint8 cabinetNo)
{
    quint8 i ,j;
    if(logicNo)
    {
       for(i = 0; i <  8;i++)
       {
           for(j = 0; j < getAisleType(cabinetNo);j++)
           {
               if(leveAisleST[i].aisle[j].logicNo == logicNo)
               {
                   *index_i = i;
                   *index_j = j;
                   return true;
               }
           }
       }
    }
    return false;
}
bool getAisleindexOfPhysicNo(quint8 *index_i, quint8 *index_j,quint8 physicNo,quint8 cabinetNo)
{
    quint8 i ,j;
    if(physicNo && (physicNo != 0xFF))
    {
       for(i = 0; i <  8;i++)
       {
           for(j = 0; j < getAisleType(cabinetNo);j++)
           {
               if(leveAisleST[i].aisle[j].physicNo == physicNo)
               {
                   *index_i = i;
                   *index_j = j;
                   return true;
               }
           }
       }
    }
    return false;

}
QByteArray upAisleInfo(quint8 leveNo)
{
    QByteArray arr;
    QDataStream in(&arr,QIODevice::WriteOnly);
    quint16 temp16;
    //leveNo 1byte
    in << (quint8)leveNo;
    //leveState 1byte
    in << (quint8)leveAisleST[leveNo - 1].isOpen;

    for(int j = 0;j < 10;j++)
    {
        in<<(quint8)leveAisleST[leveNo - 1].aisle[j].isOpen;
    }
    for(int j = 0;j < 10;j++)
    {
        //aisle State 1byte
       // in<<(quint8)leveAisleST[leveNo - 1].aisle[j].physicNum;
        temp16 = leveAisleST[leveNo - 1].aisle[j].id;
         in <<(quint8)(temp16 /256)
            <<(quint8)(temp16 % 256)
            <<(quint8)leveAisleST[leveNo - 1].aisle[j].status
            <<(quint8)(leveAisleST[leveNo - 1].aisle[j].price/256)
            <<(quint8)(leveAisleST[leveNo - 1].aisle[j].price%256)
            <<(quint8)leveAisleST[leveNo - 1].aisle[j].maxCount
            <<(quint8)leveAisleST[leveNo - 1].aisle[j].remain;

    }
    return arr;

}


quint16 getLogicAisleInfo(quint8 logicNo,quint8 type,quint8 cabinetNo)
{
    quint8 i,j;
    if(getAisleIndexOfLogicNo(&i,&j,logicNo,cabinetNo))
    {

        switch(type)
        {
            case AISLE_ID:
            return leveAisleST[i].aisle[j].id;

            case AISLE_REMAIN:
            return leveAisleST[i].aisle[j].remain;

            case AISLE_MAX:
            return leveAisleST[i].aisle[j].maxCount;

            case AISLE_PRICE:
            return leveAisleST[i].aisle[j].price;

            case AISLE_LOGICNO:
            return leveAisleST[i].aisle[j].logicNo;

            case AISLE_PHYSICNO:
            return leveAisleST[i].aisle[j].physicNo;

            case AISLE_SELLNUM:
            return leveAisleST[i].aisle[j].tradeNum;

            case AISLE_STATE:
            return leveAisleST[i].aisle[j].status;

            default:
            break;
        }
        return 0;

    }
    else
        return 0;
}




QList<quint16> sortAisleInfo(quint8 type)
{
    QList<quint16> list;

    type =type;
    for(int i = 0;i < 8;i++)
    {
        if(!leveAisleST[i].isOpen)
            continue;
        for(int j = 0;j < 10;j++)
        {
            if(leveAisleST[i].aisle[j].isOpen)
                list<<leveAisleST[i].aisle[j].id;
        }
    }

    qSort(list.begin(),list.end(),qLess<quint16>());

    return list;
}

quint8 getAisleCabinet()
{
    return aisleCabinet;
}


extern void setAisleCabinet(const quint8 cabinetNo)
{
    aisleCabinet = cabinetNo;
}


QString vmcAmountToUi(const QString &text)
{
    QString str = text;
    QRegExp regExp("[0-9]{1,8}[.]{0,1}[0-9]{0,2}");
    if(!regExp.exactMatch(str))
    {
        str = QString("1.00");
    }
    else
    {
        if(str.indexOf(".") < 0)
        {
            str += ".00";
        }
        else
        {
            QString tempStr = QString("%1")
                    .arg(str.split(".").at(1),-2,QLatin1Char('0'));
            str = str.split(".").at(0) + "." + tempStr;
        }
    }


    return str;
}

QString vmcUiToAmount(const QString &text)
{
    QString str = text;
    QRegExp regExp("[0-9]{1,8}[.]{0,1}[0-9]{0,2}");
    if(regExp.exactMatch(str))
    {
        str.remove(".");
    }
    else
        str = "100";
    return str;
}




//================================================================================
//盒饭柜的 货道信息接口函数
//================================================================================

quint8 initHefanHuodao(quint8 cabinet)
{
   hefanCabinetNo = cabinet;
   for(int i = 0;i < HE_FAN_MAX_HUODAO;i++)
   {
       stHefanCabinet.huodao[i].price =0;
       stHefanCabinet.huodao[i].id = 0;
       stHefanCabinet.huodao[i].state = 0;
   }

   return 1;
}

void setHefanHuodaoByIndex(quint8 cabinet,quint8 boxNo,quint32 value,quint8 type)
{
    hefanCabinetNo = cabinet;
    switch(type)
    {
        case HEFAN_HUODAO_PRICE:
            stHefanCabinet.huodao[boxNo - 1].price = value;
            break;
        case HEFAN_HUODAO_ID:
            stHefanCabinet.huodao[boxNo - 1].id = (quint8)value;
            break;
        case HEFAN_HUODAO_STATE:
            stHefanCabinet.huodao[boxNo - 1].state  = (quint8)value;
        break;
        case HEFAN_BOX_NUMS:
            stHefanCabinet.huodaoNums  = (quint8)value;
        break;
    }

}

quint32 getHefanHuodaoByIndex(quint8 cabinet,quint8 boxNo,quint8 type)
{
    quint32 value = 0;
    cabinet = cabinet;
    switch(type)
    {
        case HEFAN_HUODAO_PRICE:
            value = stHefanCabinet.huodao[boxNo - 1].price;
            break;
        case HEFAN_HUODAO_ID:
            value = stHefanCabinet.huodao[boxNo - 1].id;
            break;
        case HEFAN_HUODAO_STATE:
            value = stHefanCabinet.huodao[boxNo - 1].state;
        break;
        case HEFAN_BOX_NUMS:
            value = stHefanCabinet.huodaoNums;
        break;

    }

    return value;

}
