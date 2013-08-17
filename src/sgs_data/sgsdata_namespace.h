#ifndef _SGSDATA_NAMESPACE_H
#define _SGSDATA_NAMESPACE_H

namespace sgs {

class Interface;

namespace DataType {
class Message;
class Card;
class Skill;
class Hero;
class Player;
class GameCard;
class Manager;
class Platform;
class History;
}

namespace Surface {
class GameData;
}

namespace Derive{
// 消息派生类
class CardMessage;			// 使用/打出卡牌
class ZhangBaMessage;		// 专用于处理丈八蛇矛的消息
class SkillMessage;			// 使用/触发技能
class JudgeMessage;			// 判定
class HurtMessage;			// 造成伤害或回复
class TransCardMessage;		// 转移卡牌
class SwitchPhaseMessage;	// 切换阶段
class DyingMessage;			// 濒死
class KillMessage;			// 击杀
class BeginMessage;			// 连锁开始标记
class MError;				// 错误消息
class FinaleMessage;		// 游戏结束

class WuShengMessage;		// 武圣技能
class GangLieMessage;		// 刚烈技能
class QingGuoMeesage;		// 倾国技能
class LongDanMessage;		// 龙胆技能
class QiXiMessage;			// 奇袭技能
class FanKuiMessage;		// 反馈技能
class JiJiuMessage;			// 急救技能
class QingNangMessage;		// 青囊技能
class RenDeMessage;			// 仁德技能
class JiJiangMessage;		// 激将技能
class LiJianMessage;		// 离间技能
class TuXiMessage;			// 突袭技能
class YiJiMessage;			// 遗计技能
class ZhiHengMessage;		// 制衡技能
class FanJianMessage;		// 反间技能
class JieYinMessage;		// 结姻技能
class GuoSeMessage;			// 国色技能
class LiuLiMessage;			// 流离技能

//武将派生类
class HSuJiang;				// 素将
class HNvSuJiang;			// 女素将
class HCaoCao;				// 曹操
class HSiMaYi;				// 司马懿
class HXiaHouDun;			// 夏侯惇
class HZhangLiao;			// 张辽
class HXuChu;				// 许褚
class HGuoJia;				// 郭嘉
class HZhenJi;				// 甄姬
class HLiuBei;				// 刘备
class HGuanYu;				// 关羽
class HZhangFei;			// 张飞
class HLaoZhuGe;			// 老诸葛
class HZhaoYun;				// 赵云
class HMaChao;				// 马超
class HHuangYueYing;		// 黄月英
class HSunQuan;				// 孙权
class HGanNing;				// 甘宁
class HLvMeng;				// 吕蒙
class HHuangGai;			// 黄盖
class HZhouYu;				// 周瑜
class HDaQiao;				// 大乔
class HLuXun;				// 陆逊
class HSunShangXiang;		// 孙尚香
class HHuaTuo;				// 华佗
class HLvBu;				// 吕布
class HDiaoChan;			// 貂蝉
// 后续的稍后补充
}
}

#endif /*_SGSDATA_NAMESPACE_H*/
