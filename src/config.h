#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QMetaType>
#include <QVariant>
#include <QDebug>

#define  PRINT_ENABLE         1
#if PRINT_ENABLE
#define  print_log(...)       qDebug(__VA_ARGS__)
#define  print_err(errno)     qDebug("ERROR: In %s %s at Line %u - errno = %d\n", __FUNCTION__,__FILE__, __LINE__, errno)
#define  print_warning(no)	  qDebug("Warning: In %s %s at Line %u - no = %d\n", __FUNCTION__,__FILE__, __LINE__, no)
#define  print_debug(...)     do{ qDebug("Debug: In %s %s at Line %u\n", __FUNCTION__,__FILE__, __LINE__); \
                                  qDebug(__VA_ARGS__);}while(0)

#else
#define  print_log(...)       do {} while(0)
#define  print_err(errno)      do {} while(0)
#define  print_warning(no)	  do {} while(0)
#define  print_debug(...)     do{}while(0)
#endif





#define EN   0x01
#define PINTU 0x02
#define VMCCONTYPE  PINTU //0x01 English 0x02 品途



#define VMC_DRINK   0x01
#define VMC_FOOD    0x11
#define VMC_YITUO   0x20
#define VMC_OTHER   0x21



//#define CABINET_1   0x01
//#define CABINET_2   0x02


#define VMC_OK      0x01
#define VMC_FAIL    0x00



#define HE_FAN_MAX_HUODAO 88
#define HE_FAN_MAX_CABINET 10

#define HEFAN_LIGHT    0
#define HEFAN_LIGHT_OPEN    1

#define HEFAN_HOT      2
#define HEFAN_HOT_OPEN      3
#define HEFAN_COOL     4
#define HEFAN_COOL_OPEN     5
#define HEFAN_EXIST 6






#define HEFAN_HUODAO_NORMAL   1
#define HEFAN_HUODAO_FAULT   2
#define HEFAN_HUODAO_EMPTY   3
#define HEFAN_HUODAO_CLOSE   4

/*
type：1——货道单价
      2——货道数量
      3——货道状态
      4——货道最大存货量
      5——选货按键值
      6——成功交易次数
      7——商品编号

*/
#define HEFAN_HUODAO_PRICE  1
#define HEFAN_HUODAO_STATE  3
#define HEFAN_HUODAO_TRADE_NUM  6
#define HEFAN_HUODAO_ID     7
#define HEFAN_BOX_NUMS   8

#define HEFAN_CABINET_NUMS   11



/***************************************************************************************
 *以下是一些定制宏
 ************************************************************************************/
#define ACTION_SHOW_VMC_STATUS   0  //显示售货机状态 1显示 0关闭

#define ACTION_SHOW_HEFAN_DIALOG   0  //显示盒饭柜界面

/***************************************************************************************
 *版本号
 * v1.1.8 新增读卡器类型，和一键配置文件
 ************************************************************************************/
enum VMC_TOOL_VERSION
{
    VMC_VER_1_1_4 = 0x0114,
    VMC_VER_1_1_6 = 0x0116,
    VMC_VER_1_1_7 = 0x0117,
    VMC_VER_1_1_8 = 0x0118
};


#define VMC_VERSION VMC_VER_1_1_8
#define VMC_VERSION_STRING "v1.1.8"




enum LANGUAGE
{
    VMC_EN,VMC_ZH
};

enum AISLETYPE
{
    AISLE_ID,
    AISLE_REMAIN,
    AISLE_MAX,
    AISLE_PRICE,
    AISLE_LOGICNO,
    AISLE_PHYSICNO,
    AISLE_SELLNUM,
    AISLE_SELECTNO,
    AISLE_STATE
};


typedef struct _ailse
{
    quint16 id;
    bool   isOpen;
    quint32 price;
    quint8 status;
    quint8 remain;
    quint8 maxCount;

    quint8 tradeNum ;
    quint8 physicNo;//新增物理货道编号
    quint8 logicNo;//逻辑编号
    quint8 selectNo;//选货按键号 新增

}AISLE;

Q_DECLARE_METATYPE(AISLE)

typedef struct _leveaisle
{
     quint8 leveNum;
     bool isOpen;
     quint8 leveMaxCount;
     AISLE aisle[10];
}LEVEAISLE;

Q_DECLARE_METATYPE(LEVEAISLE)


enum VMC_SYSTEM_TYPE
{
    VMC_SYSTEM_SELECTBOARD,//选货按键板属性
    VMC_SYSTEM_PCTYPE,//PC类型
    VMC_SYSTEM_LOGO,   //LOGO类型
    VMC_SYSTEM_SLAVE_CABINET,  //从柜开启
    VMC_SYSTEM_SCALE,  //金额比例
    VMC_SYSTEM_VERSION //版本号
};


typedef struct _vmc_system_para
{
    bool isOpenSelectBoard;
    bool isOpenPC;
    bool isOpenSlaveCabinet;
    quint8 logo;
    quint8 scale;


}VMC_SYSTEM_PARA;
Q_DECLARE_METATYPE(VMC_SYSTEM_PARA)




typedef struct _huodao_{

    quint32 price;
    quint16 tradeNum;//销售数量
    quint8  state;	//  1正常  2故障3 缺货 4 关闭
    quint8  id;


}HUODAO;


typedef struct _hefan_cabinet_{

    HUODAO huodao[HE_FAN_MAX_HUODAO];
    unsigned char state;// xx cc hh ll       xx 表示柜子存在与否1存在 0不存在 cc柜内制冷状态 hh柜内加热状态 ll柜内照明装填
    unsigned char huodaoNums;//货道总数

}HEFAN_CABINET;

Q_DECLARE_METATYPE(HEFAN_CABINET)
quint32 getHefanHuodaoByIndex(quint8 cabinet,quint8 boxNo,quint8 type);
void setHefanHuodaoByIndex(quint8 cabinet,quint8 boxNo,quint32 value,quint8 type);
quint8 initHefanHuodao(quint8 cabinet);



extern LEVEAISLE leveAisleST[8];
extern void initAisle(const quint8 type);
extern void setAisleType(const quint8 cabinetNo,const quint8 type);
extern quint8 getAisleType(const quint8 cabinetNo);
bool getAisleIndexOfLogicNo(quint8 *index_i,quint8 *index_j,quint8 logicNo,quint8 cabinetNo);
extern bool getAisleindexOfPhysicNo(quint8 *index_i, quint8 *index_j,quint8 physicNo,quint8 cabinetNo);
extern QByteArray upAisleInfo(quint8 leveNo);
//extern bool setPhsicAisleInfo(quint8 phsicNo,quint8 type,quint16 value);
//extern bool setLogicAisleInfo(quint8 logicNo,quint8 type,quint16 value);
extern quint16 getPhsicAisleInfo(quint8 physicNo,quint8 type);
extern quint16 getLogicAisleInfo(quint8 logicNo,quint8 type,quint8 cabinetNo);
extern QList<quint16> sortAisleInfo(quint8 type);
extern quint8 getAisleCabinet();
extern void setAisleCabinet(const quint8 cabinetNo);
extern quint8 getVMCSystemPara(const quint8 type);
extern void setVMCSystemPara(const quint8 type,const quint8 value);
extern quint32 amountFromScale(const quint32 amount);
extern quint32 amountToScale(const quint32 amount);
extern QString vmcAmountToUi(const QString &text);
extern QString vmcUiToAmount(const QString &text);




#endif // CONFIG_H
