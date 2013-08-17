#ifndef _SGSDATA_ENUM_H
#define _SGSDATA_ENUM_H
#include "chain.h"

namespace sgs{
namespace ConstData{

//游戏牌类型
enum CardType
{
	cardTypeNone = 0,

	//基本牌
	HUOSHA,
	LEISHA,
	SHA,
	SHAN,
	TAO,
	JIU,

	//减一马
	CHITU = 10,
	DAWAN,
	ZICI,

	//加一马
	DILU = 15,
	JUEYING,
	ZHUAHUANG,
	HUALIU,

	//防具
	BAIYIN = 20,
	TENGJIA,
	BAGUA,
	RENWANG,

	//武器
	ZHUGE = 30,
	QINGGANG,
	HANBING,
	CIXIONG, GUANSHI,
	YINYUE,
	QINGLONG,
	GUDING,
	ZHANGBA,
	FANGTIAN,
	QILIN,
	ZHUQUE,

	//延时锦囊
	BINGLIANG = 50,	// 兵粮寸断
	SISHU,			// 乐不思蜀
	SHANDIAN,		// 闪电

	//非延时锦囊
	GUOCHAI = 60,	// 过河拆桥
	HUOGONG,		// 火攻
	JIEDAO,			// 借刀杀人
	JUEDOU,			// 决斗
	NANMAN,			// 南蛮入侵
	SHUNQIAN,		// 顺手牵羊
	TAOYUAN,		// 桃园结义
	TIESUO,			// 铁锁连舟
	WANJIAN,		// 万箭齐发
	WUXIE,			// 无懈可击
	WUZHONG,		// 无中生有
	WUGU			// 五谷丰登
};

//卡牌花色
enum CardColor
{
	cardColorNone = 0,
	HEART,
	SPADE,
	DIAMOND,
	CLUB
};

//势力
enum HeroSide
{
	heroSideNone = 0,
	WEI,
	SHU,
	WU,
	QUN
};

enum HeroSex
{
	herSexNone = 0,
	MALE,
	FEMALE
};//性别

//武将
enum HeroType
{
	heroTypeNone = 0,
	SUJIANG,
	NVSUJIANG,

	CAOCAO,
	SIMAYI,
	XIAHOUDUN,
	ZHANGLIAO,
	XUCHU,
	GUOJIA,
	ZHENJI,
	XIAHOUYUAN,
	ZHANGHE,
	XUHUANG,
	CAOREN,
	DIAWEI,
	XUNYU,
	CAOPI,
	DENGAI,
	CAOZHI,
	XUSHU,
	YUJIN,
	ZHANGCHUNHUA,
	YANGXIU,

	LIUBEI,
	GUANYU,
	ZHANGFEI,
	LAOZHUGE,
	ZHAOYUN,
	MACHAO,
	HUANGYUEYING,
	HUANGZHONG,
	WEIYAN,
	PANGTONG,
	HUOZHUGE,
	JIANGWEI,
	LIUSHAN,
	MENGHUO,
	ZHURONG,
	FAZHENG,
	MASU,

	SUNQUAN,
	GANNING,
	LVMENG,
	HUANGGAI,
	ZHOUYU,
	DAQIAO,
	LUXUN,
	SUNSHANGXIANG,
	SUNJIAN,
	SUNCE,
	XIAOQIAO,
	TAISHICI,
	ZHOUTAI,
	LUSU,
	ZHANGZHAO,
	LINGTONG,
	WUGUOTAI,
	XUSHENG,

	HUATUO,
	LVBU,
	DIAOCHAN,
	YUANSHAO,
	YANLIANG,
	DONGZHUO,
	JIAXU,
	PANGDE,
	ZUOCI,
	ZHANGJIAO,
	YUJI,
	CAIWENJI,
	CHENGONG,
	GAOSHUN,
	GONGSUNZAN,
	YUANSHU
};

//技能
enum HeroSkill
{
	heroSkillNone = 0,
	JIANXIONG,
	HUJIA,
	FANKUI,
	GUICAI,
	GANGLIE,
	TUXI,
	LUOYI,
	TIANDU,
	YIJI,
	QINGGUO,
	LUOSHEN,
	SHENSU,
	QIAOBIAN,
	DUANLIANG,
	JUSHOU,
	QIANGXI,
	QUHU,
	JIEMING,
	XINGSHANG,
	FANGZHU,
	SONGWEI,
	TUNTIAN,
	JIANXIAN,
	LUOYING,
	JIUSHI,
	WUYAN,
	JUJIAN,
	YIZHONG,
	JUEQING,
	SHANGSHI,
	DANLUO,
	JILEI,
	RENDE,
	JIJIANG,
	WUSHENG,
	PAOXIAO,
	GUANXING,
	KONGCHENG,
	LONGDAN,
	MASHU,
	TIEQI,
	JIZHI,
	QICAI,
	LIEGONG,
	KUANGGU,
	LIANHUAN,
	NIEPAN,
	BAZHEN,
	HUOJI,
	KANPO,
	TIAOXIN,
	ZHIJI,
	XIANGLE,
	FANGQUAN,
	RUOYU,
	HUOSHOU,
	ZAIQI,
	JUXIANG,
	LIEREN,
	ENYUAN,
	XUANHUO,
	XINZHAN,
	HUILEI,
	ZHIHENG,
	JIUYUAN,
	QIXI,
	KEJI,
	KUROU,
	YINGZI,
	FANJIAN,
	GUOSE,
	LIULI,
	QIANXUN,
	LIANYING,
	JIEYIN,
	XIAOJI,
	YINGHUN,
	JIANG,
	HUNZI,
	ZHIBA,
	TIANXIANG,
	HONGYAN,
	TIANYI,
	BUQU,
	HAOSHI,
	DIMENG,
	ZHIJIAN,
	GUZHENG,
	XUANFENG,
	GANLU,
	BUYI,
	POJUN,
	JIJIU,
	QINGNANG,
	WUSHUANG,
	LIJIAN,
	BIYUE,
	LUANJI,
	XUEYI,
	SHUANGXIONG,
	JIUCHI,
	ROULIN,
	BENGHUAI,
	BAONUE,
	WANSHA,
	LUANWU,
	WEIMU,
	MENGJIN,
	HUASHEN,
	XINSHENG,
	LEIJI,
	GUIDAO,
	HUANGTIAN,
	GUHUO,
	BEIGE,
	DUANCHANG,
	MINGCE,
	ZHICHI,
	XIANZHEN,
	JINJIU,
	YICONG,
	YONGSI,
	WEIDI
};

//身份
enum PlayerRole
{
	playerRoleNone = 0,
	ZHU,
	ZHONG,
	FAN,
	NEI
};

//阶段
enum PlayerPhase
{
	ppNone = 0,				// 选手无阶段
	STARTPHASE,				// 开始阶段
	JUDGEMENTPHASE,			// 判断阶段
	GETCARDPHASE,			// 发牌阶段
	MAINPHASE,				// 主要阶段,出牌,出技能等
	ABANDONCARDPHASE,		// 弃牌阶段
	ENDPHASE,				// 结束阶段
	OTHERPHASE				// 其他阶段
};

//状态
enum PlayerStatus
{
	playerStatusNone	= 0,	// 无状态
	SkillOneUsed		= 1,	// 使用了第一个技能
	SkillTwoUsed		= 2,	// 使用了第二个技能
	ATTACKED			= 4,	// 已经进行了攻击
	LE					= 8,	// 被乐
	BING				= 16,	// 被兵粮
	DEAD				= 32	// 死亡状态
};

// 消息标记，HURT包括回血，TRANSCARD指卡牌转移（从玩家到牌堆、玩家到玩家、场上到玩家等），begin用于标记一个连锁相应的开始。
enum MessageType
{
	messageTypeNone = 0,
	USECARD,
	HITCARD,
	USESKILL,
	JUDGE,
	HURT,
	TRANSCARD,
	SWITCHPHASE,
	DYING,
	KILL,
	ERROR,
	BEGIN,
	FINALE
};

// 错误讯息：溢出，操作错误，在一个指令收集的过程中得到了不相干的指令导致退出
enum ErrorType
{
	etNone = 0,
	OPERATION
	/*,OVERFLOW,HOP*/
};

// 区域标记，SYSTEM中包括确定（1），取消（0），弃牌（2）
enum RegionType
{
	regionTypeNone = 0,
	HAND,
	EQUIP,
	SKILL,
	PLAYER,
	WINDOW,
	SYSTEM,
	DEFAULT,
	DESK,
	POOLTOP,
	POOLBOTTOM,
	DUST
};

// 玩家的区域
enum PlayerRegionType
{
	playerRegionTypeNone = 0,
	PHAND,
	PEQUIP,
	PJUDGE
};

}
}

#endif /*_SGSDATA_ENUM_H*/
