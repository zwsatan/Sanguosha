#include "sgsui_nameConvert.h"
#include "sgsui_output.h"
#include "sgsui_GUIStaticData.h"
#include "sgsdata_card.h"
#include "sgsdata_player.h"
#include "sgsdata_msg.h"
#include <QObject>
#include <cstdlib>

namespace sgsui {

QString phasePixmapStr(sgs::ConstData::PlayerPhase phase, bool isDying, bool isResponding)
{
	if (isResponding)
		return QString("images/phase/response.png");
	if (isDying)
		return QString("images/phase/sos.png");

	using namespace sgs::ConstData;
	switch (phase) {
	case STARTPHASE:
		return QString("images/phase/start.png");
	case JUDGEMENTPHASE:
		return QString("images/phase/judge.png");
	case GETCARDPHASE:
		return QString("images/phase/draw.png");
	case MAINPHASE:
		return QString("images/phase/play.png");
	case ABANDONCARDPHASE:
		return QString("images/phase/discard.png");
	case ENDPHASE:
		return QString("images/phase/finish.png");
	case OTHERPHASE:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "phasePixmapStr: you should not call this when the phase is OTHER");
		return QString();
	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "phasePixmapStr: Invalid Phase");
		return QString();
	}
}

QString phaseFramePixmapStr(sgs::ConstData::PlayerPhase phase, bool isDying, bool isResponding)
{
	if (isResponding)
		return QString("images/frame/responsing.png");

	if (isDying)
		return QString("images/frame/sos.png");

	using namespace sgs::ConstData;
	switch (phase)
	{
	case STARTPHASE:
	case JUDGEMENTPHASE:
	case GETCARDPHASE:
	case MAINPHASE:
	case ABANDONCARDPHASE:
	case ENDPHASE:
		return QString("images/frame/playing.png");
	case OTHERPHASE:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "phaseFramePixmapStr: you should not call this when the phase is OTHER");
		return QString();
	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "phaseFramePixmapStr: Invalid Phase");
		return QString();
	}
}

QString roleDisplayName(sgs::ConstData::PlayerRole playerRole)
{
	switch (playerRole)
	{
	case sgs::ConstData::ZHU:
		return QObject::trUtf8("主公");
	case sgs::ConstData::ZHONG:
		return QObject::trUtf8("忠臣");
	case sgs::ConstData::FAN:
		return QObject::trUtf8("反贼");
	case sgs::ConstData::NEI:
		return QObject::trUtf8("内奸");
	default:
		return QString();
	}
}

QString roleSmallPixmapStr(sgs::ConstData::PlayerRole playerRole, bool dead/* = false*/)
{
	QString isDeadStatus = dead ? "dead" : "small";
	switch (playerRole)
	{
	case sgs::ConstData::ZHU:
		return QString("images/roles/%1-lord.png").arg(isDeadStatus);
	case sgs::ConstData::ZHONG:
		return QString("images/roles/%1-loyalist.png").arg(isDeadStatus);
	case sgs::ConstData::FAN:
		return QString("images/roles/%1-rebel.png").arg(isDeadStatus);
	case sgs::ConstData::NEI:
		return QString("images/roles/%1-renegade.png").arg(isDeadStatus);
	default:
		return QString();
	}
}

QString rolePixmapStr(sgs::ConstData::PlayerRole playerRole, bool visible/* = true*/)
{
	if (visible)
	{
		switch (playerRole)
		{
		case sgs::ConstData::ZHU:
			return QString("images/roles/lord.png");
		case sgs::ConstData::ZHONG:
			return QString("images/roles/loyalist.png");
		case sgs::ConstData::FAN:
			return QString("images/roles/rebel.png");
		case sgs::ConstData::NEI:
			return QString("images/roles/renegade.png");
		default:
			return QString("images/roles/unknown.png");
		}
	}
	else
	{
		switch (playerRole)
		{
		case sgs::ConstData::ZHU:
			return QString("images/roles/lord.png");
		default:
			return QString("images/roles/unknown.png");
		}
	}
}

QString deadRolePixmapStr(sgs::ConstData::PlayerRole playerRole)
{
	switch (playerRole)
	{
	case sgs::ConstData::ZHU:
		return QString("images/death/lord.png");
	case sgs::ConstData::ZHONG:
		return QString("images/death/loyalist.png");
	case sgs::ConstData::FAN:
		return QString("images/death/rebel.png");
	case sgs::ConstData::NEI:
		return QString("images/death/renegade.png");
	default:
		return QString("images/death/unknown.png");
	}
}

QString HPPixmapStr(int hp)
{
	switch (hp)
	{
	case 0:
		return QString("images/HP/0.png");
	case 1:
		return QString("images/HP/1.png");
	case 2:
		return QString("images/HP/2.png");
	case 3:
		return QString("images/HP/3.png");
	case 4:
		return QString("images/HP/4.png");
	default:
		return QString("images/HP/5.png");
	}
}

QString smallHPPixmapStr(int hp)
{
	switch (hp)
	{
	case 0:
		return QString("images/HP/small-0.png");
	case 1:
		return QString("images/HP/small-1.png");
	case 2:
		return QString("images/HP/small-2.png");
	case 3:
		return QString("images/HP/small-3.png");
	case 4:
		return QString("images/HP/small-4.png");
	default:
		return QString("images/HP/small-5.png");
	}
}

QString countrySmallPixmapStr(sgs::ConstData::HeroSide heroSide)
{
	switch (heroSide)
	{
	case sgs::ConstData::WEI:
		return QString("images/country/wei.png");
	case sgs::ConstData::SHU:
		return QString("images/country/shu.png");
	case sgs::ConstData::WU:
		return QString("images/country/wu.png");
	case sgs::ConstData::QUN:
		return QString("images/country/qun.png");
	default:
		return QString("images/country/god.png");
	}
}

QString countryFramePixmapStr(sgs::ConstData::HeroSide heroSide)
{
	switch (heroSide)
	{
	case sgs::ConstData::WEI:
		return QString("images/frame/wei.png");
	case sgs::ConstData::SHU:
		return QString("images/frame/shu.png");
	case sgs::ConstData::WU:
		return QString("images/frame/wu.png");
	case sgs::ConstData::QUN:
		return QString("images/frame/qun.png");
	default:
		return QString("images/frame/god.png");
	}
}

QString cardNumberStr(int number, sgs::ConstData::CardColor cardColor)
{
	QString cardNumberString;

	if (cardColor == sgs::ConstData::HEART || cardColor == sgs::ConstData::DIAMOND)
		cardNumberString.append("images/red/");
	else
		cardNumberString.append("images/black/");
	return cardNumberString.append(QString::number(number) + ".png");
}

QString cardColorStr(sgs::ConstData::CardColor cardColor)
{
	switch (cardColor)
	{
	case sgs::ConstData::HEART:
		return QString("images/color/heart.png");
	case sgs::ConstData::DIAMOND:
		return QString("images/color/diamond.png");
	case sgs::ConstData::CLUB:
		return QString("images/color/club.png");
	default:
		return QString("images/color/spade.png");
	}
}

QString cardColorDisplayName(sgs::ConstData::CardColor cardColor, bool richText/* = true*/)
{
	QString colorName;
	if (richText)
	{
		switch (cardColor)
		{
		case sgs::ConstData::HEART:
		case sgs::ConstData::DIAMOND:
			colorName.append("<font color=red>");
			break;
		default:
			colorName.append("<font color=black>");
			break;
		}
	}

	switch (cardColor)
	{
	case sgs::ConstData::HEART:
		colorName.append(QObject::trUtf8("\342\231\245"));
		break;
	case sgs::ConstData::DIAMOND:
		colorName.append(QObject::trUtf8("\342\231\246"));
		break;
	case sgs::ConstData::CLUB:
		colorName.append(QObject::trUtf8("\342\231\243"));
		break;
	default:
		colorName.append(QObject::trUtf8("\342\231\240"));
		break;
	}

	if (richText)
		colorName.append("</font>");
	return colorName;
}

QString bigCardColorStr(sgs::ConstData::CardColor cardColor)
{
	switch (cardColor)
	{
	case sgs::ConstData::HEART:
		return QString("images/big-color/heart.png");
	case sgs::ConstData::DIAMOND:
		return QString("images/big-color/diamond.png");
	case sgs::ConstData::CLUB:
		return QString("images/big-color/club.png");
	default:
		return QString("images/big-color/spade.png");
	}
}

QString cardPixmapStr(sgs::ConstData::CardType cardType)
{
	QString cardPixmapString("images/card/");
	return cardPixmapString.append(cardFilenameStr(cardType)).append(".jpg");
}

QString cardSoundStr(sgs::ConstData::CardType cardType, bool sex/* = true*/)
{
	if (isZhuangbeiCard(cardType))
		return QString("sound/system/install-equip.mp3");

	QString cardSoundString("sound/card/");
	cardSoundString.append(sex ? "male/" : "female/");
	return cardSoundString.append(cardFilenameStr(cardType)).append(".mp3");
}

bool isZhuangbeiCard(sgs::ConstData::CardType cardType)
{
	return getEquipType(cardType) != NotEquip;
}

bool isJinnangCard(sgs::ConstData::CardType cardType)
{
	using namespace sgs::ConstData;
	switch (cardType)
	{
	case BINGLIANG:
	case SISHU:
	case SHANDIAN:
	case GUOCHAI:
	case HUOGONG:
	case JIEDAO:
	case JUEDOU:
	case NANMAN:
	case SHUNQIAN:
	case TAOYUAN:
	case TIESUO:
	case WANJIAN:
	case WUXIE:
	case WUZHONG:
	case WUGU:
		return true;
	default:
		return false;
	}
}

bool isJudgeCard(sgs::ConstData::CardType cardType)
{
	using namespace sgs::ConstData;
	switch (cardType)
	{
	case BINGLIANG:
	case SISHU:
	case SHANDIAN:
		return true;

	default:
		return false;
	}
}

QString cardDisplayName(sgs::ConstData::CardType cardType)
{
	using namespace sgs::ConstData;
	switch (cardType)
	{
	//Jiben
	case HUOSHA:
		return QObject::trUtf8("火杀");
	case LEISHA:
		return QObject::trUtf8("雷杀");
	case SHA:
		return QObject::trUtf8("杀");
	case SHAN:
		return QObject::trUtf8("闪");
	case TAO:
		return QObject::trUtf8("桃");
	case JIU:
		return QObject::trUtf8("酒");

	case CHITU:
		return QObject::trUtf8("赤兔");
	case DAWAN:
		return QObject::trUtf8("大宛");
	case ZICI:
		return QObject::trUtf8("紫騂￼");

	case DILU:
		return QObject::trUtf8("的卢");
	case JUEYING:
		return QObject::trUtf8("绝影");
	case ZHUAHUANG:
		return QObject::trUtf8("爪黄飞电");
	case HUALIU:
		return QObject::trUtf8("骅骝");

		//Wuqi
	case BAIYIN:
		return QObject::trUtf8("白银狮子");
	case TENGJIA:
		return QObject::trUtf8("藤甲");
	case BAGUA:
		return QObject::trUtf8("八卦阵");
	case RENWANG:
		return QObject::trUtf8("仁王盾");
	case ZHUGE:
		return QObject::trUtf8("诸葛连弩");
	case QINGGANG:
		return QObject::trUtf8("青釭剑");
	case HANBING:
		return QObject::trUtf8("寒冰剑");
	case CIXIONG:
		return QObject::trUtf8("雌雄双股剑");
	case GUANSHI:
		return QObject::trUtf8("贯石斧");
	case YINYUE:
		return QObject::trUtf8("银月枪");
	case QINGLONG:
		return QObject::trUtf8("青龙偃月刀");
	case GUDING:
		return QObject::trUtf8("古锭刀");
	case ZHANGBA:
		return QObject::trUtf8("丈八蛇矛");
	case FANGTIAN:
		return QObject::trUtf8("方天画戟");
	case QILIN:
		return QObject::trUtf8("麒麟弓");
	case ZHUQUE:
		return QObject::trUtf8("朱雀羽扇");

		//Yanshi
	case BINGLIANG:
		return QObject::trUtf8("兵粮寸断");
	case SISHU:
		return QObject::trUtf8("乐不思蜀");
	case SHANDIAN:
		return QObject::trUtf8("闪电");

		//Feiyanshi
	case GUOCHAI:
		return QObject::trUtf8("过河拆桥");
	case HUOGONG:
		return QObject::trUtf8("火攻");
	case JIEDAO:
		return QObject::trUtf8("借刀杀人");
	case JUEDOU:
		return QObject::trUtf8("决斗");
	case NANMAN:
		return QObject::trUtf8("南蛮入侵");
	case SHUNQIAN:
		return QObject::trUtf8("顺手牵羊");
	case TAOYUAN:
		return QObject::trUtf8("桃园结义");
	case TIESUO:
		return QObject::trUtf8("铁索连舟");
	case WANJIAN:
		return QObject::trUtf8("万箭齐发");
	case WUXIE:
		return QObject::trUtf8("无懈可击");
	case WUZHONG:
		return QObject::trUtf8("无中生有");
	case WUGU:
		return QObject::trUtf8("五谷丰登");
	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "cardDisplayName: Invalid Card Type");
		return QString();
	}
}

QString cardFullDisplayName(const sgs::DataType::Card * card, bool richText/* = true*/)
{
	if (card)
	{
		QString cardName = cardDisplayName(card->type());
		QString cardColor = cardColorDisplayName(card->color(), richText);
		QString cardNumber = cardNumberChar(card->number());
		return cardName + cardColor + cardNumber;
	}

	return "null pointer";
}

QString cardFullDisplayName(sgs::ConstData::CardType cardType, sgs::ConstData::CardColor cardColor, int number, bool richText/* = true*/)
{
	return cardDisplayName(cardType) + cardColorDisplayName(cardColor, richText) + cardNumberChar(number);
}

QString cardNumberChar(int number)
{
	switch (number)
	{
	case 1:
		return QString("A");
	case 11:
		return QString("J");
	case 12:
		return QString("Q");
	case 13:
		return QString("K");
	default:
		return QString::number(number);
	}
}

QString zhuangbeiPixmapStr(sgs::ConstData::CardType cardType)
{
	QString zhuangbeiPixmapString("images/zhuangbei/");
	return zhuangbeiPixmapString.append(cardFilenameStr(cardType)).append(".png");
}

QString smallZhuangbeiPixmapStr(sgs::ConstData::CardType cardType)
{
	QString zhuangbeiPixmapString("images/small-zhuangbei/");
	return zhuangbeiPixmapString.append(cardFilenameStr(cardType)).append(".png");
}

QString cardFilenameStr(sgs::ConstData::CardType cardType)
{
	using namespace sgs::ConstData;
	switch (cardType)
	{
	// 基本卡
	case HUOSHA:
		return QString("fire_slash");
	case LEISHA:
		return QString("thunder_slash");
	case SHA:
		return QString("slash");
	case SHAN:
		return QString("jink");
	case TAO:
		return QString("peach");
	case JIU:
		return QString("analeptic");

	case CHITU:
		return QString("chitu");
	case DAWAN:
		return QString("dayuan");
	case ZICI:
		return QString("zixing");

	case DILU:
		return QString("dilu");
	case JUEYING:
		return QString("jueying");
	case ZHUAHUANG:
		return QString("zhuahuangfeidian");
	case HUALIU:
		return QString("hualiu");

	// 武器卡
	case BAIYIN:
		return QString("silver_lion");
	case TENGJIA:
		return QString("vine");
	case BAGUA:
		return QString("eight_diagram");
	case RENWANG:
		return QString("renwang_shield");
	case ZHUGE:
		return QString("crossbow");
	case QINGGANG:
		return QString("qinggang_sword");
	case HANBING:
		return QString("ice_sword");
	case CIXIONG:
		return QString("double_sword");
	case GUANSHI:
		return QString("axe");
	case YINYUE:
		return QString("moon_spear");
	case QINGLONG:
		return QString("blade");
	case GUDING:
		return QString("guding_blade");
	case ZHANGBA:
		return QString("spear");
	case FANGTIAN:
		return QString("halberd");
	case QILIN:
		return QString("kylin_bow");
	case ZHUQUE:
		return QString("fan");

	// 延时计谋卡
	case BINGLIANG:
		return QString("supply_shortage");
	case SISHU:
		return QString("indulgence");
	case SHANDIAN:
		return QString("lightning");

	// 非延时计谋卡
	case GUOCHAI:
		return QString("dismantlement");
	case HUOGONG:
		return QString("fire_attack");
	case JIEDAO:
		return QString("collateral");
	case JUEDOU:
		return QString("duel");
	case NANMAN:
		return QString("savage_assault");
	case SHUNQIAN:
		return QString("snatch");
	case TAOYUAN:
		return QString("god_salvation");
	case TIESUO:
		return QString("iron_chain");
	case WANJIAN:
		return QString("archery_attack");
	case WUXIE:
		return QString("nullification");
	case WUZHONG:
		return QString("ex_nihilo");
	case WUGU:
		return QString("amazing_grace");
	default:
		return QString("card-back");
	}
}

QString wujiangSelfPixmapStr(sgs::ConstData::HeroType heroType)
{
	QString wujiangSelfPixmapString("images/generals/big/");
	return wujiangSelfPixmapString.append(wujiangFilenameStr(heroType)).append(".png");
}

QString wujiangMiniPixmapStr(sgs::ConstData::HeroType heroType)
{
	QString wujiangSelfPixmapString("images/generals/tiny/");
	return wujiangSelfPixmapString.append(wujiangFilenameStr(heroType)).append(".png");
}

QString wujiangOtherPixmapStr(sgs::ConstData::HeroType heroType)
{
	QString wujiangOtherPixmapString("images/generals/small/");
	return wujiangOtherPixmapString.append(wujiangFilenameStr(heroType)).append(".png");
}

QString wujiangCardPixmapStr(sgs::ConstData::HeroType heroType)
{
	QString wujiangCardPixmapString("images/generals/card/");
	return wujiangCardPixmapString.append(wujiangFilenameStr(heroType)).append(".jpg");
}

QString wujiangFilenameStr(sgs::ConstData::HeroType heroType)
{
	using namespace sgs::ConstData;
	switch (heroType)
	{
	case CAOCAO:
		return QString("caocao");
	case SIMAYI:
		return QString("simayi");
	case XIAHOUDUN:
		return QString("xiahoudun");
	case ZHANGLIAO:
		return QString("zhangliao");
	case XUCHU:
		return QString("xuchu");
	case GUOJIA:
		return QString("guojia");
	case ZHENJI:
		return QString("zhenji");
	case XIAHOUYUAN:
		return QString("xiahouyuan");
	case ZHANGHE:
		return QString("zhanghe");
	case XUHUANG:
		return QString("xuhuang");
	case CAOREN:
		return QString("caoren");
	case DIAWEI:
		return QString("dianwei");
	case XUNYU:
		return QString("xunyu");
	case CAOPI:
		return QString("caopi");
	case DENGAI:
		return QString("dengai");
	case CAOZHI:
		return QString("caozhi");
	case XUSHU:
		return QString("xushu");
	case YUJIN:
		return QString("yujin");
	case ZHANGCHUNHUA:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "wujiangFilenameStr: Using ZHANGCHUNHUA, whick should not be called");
		return QString();
	case YANGXIU:
		return QString("yangxiu");
	case LIUBEI:
		return QString("liubei");
	case GUANYU:
		return QString("guanyu");
	case ZHANGFEI:
		return QString("zhangfei");
	case LAOZHUGE:
		return QString("zhugeliang");
	case ZHAOYUN:
		return QString("zhaoyun");
	case MACHAO:
		return QString("machao");
	case HUANGYUEYING:
		return QString("huangyueying");
	case HUANGZHONG:
		return QString("huangzhong");
	case WEIYAN:
		return QString("weiyan");
	case PANGTONG:
		return QString("pangtong");
	case HUOZHUGE:
		return QString("shenzhugeliang");
	case JIANGWEI:
		return QString("jiangwei");
	case LIUSHAN:
		return QString("liushan");
	case MENGHUO:
		return QString("menghuo");
	case ZHURONG:
		return QString("zhurong");
	case FAZHENG:
		return QString("fazheng");
	case MASU:
		return QString("masu");
	case SUNQUAN:
		return QString("sunquan");
	case GANNING:
		return QString("ganning");
	case LVMENG:
		return QString("lumeng");
	case HUANGGAI:
		return QString("huanggai");
	case ZHOUYU:
		return QString("zhouyu");
	case DAQIAO:
		return QString("daqiao");
	case LUXUN:
		return QString("luxun");
	case SUNSHANGXIANG:
		return QString("sunshangxiang");
	case SUNJIAN:
		return QString("sunjian");
	case SUNCE:
		return QString("sunce");
	case XIAOQIAO:
		return QString("xiaoqiao");
	case TAISHICI:
		return QString("taishici");
	case ZHOUTAI:
		return QString("zhoutai");
	case LUSU:
		return QString("lusu");
	case ZHANGZHAO:
		return QString("erzhang");
	case LINGTONG:
		return QString("lingtong");
	case WUGUOTAI:
		return QString("wuguotai");
	case XUSHENG:
		return QString("xusheng");
	case HUATUO:
		return QString("huatuo");
	case LVBU:
		return QString("lubu");
	case DIAOCHAN:
		return QString("diaochan");
	case YUANSHAO:
		return QString("yuanshao");
	case YANLIANG:
		return QString("shuangxiong");
	case DONGZHUO:
		return QString("dongzhuo");
	case JIAXU:
		return QString("jiaxu");
	case PANGDE:
		return QString("pangde");
	case ZUOCI:
		return QString("zuoci");
	case ZHANGJIAO:
		return QString("zhangjiao");
	case YUJI:
		return QString("yuji");
	case CAIWENJI:
		return QString("caiwenji");
	case CHENGONG:
		return QString("chengong");
	case GAOSHUN:
		return QString("gaoshun");
	case GONGSUNZAN:
		return QString("gongsunzan");
	case YUANSHU:
		return QString("yuanshu");
	case SUJIANG:
		return QString("sujiang");
	case NVSUJIANG:
		return QString("sujiangf");
	default:
		return QString();
	}
}

QString wujiangDeathSoundStr(sgs::ConstData::HeroType heroType)
{
	return QString("sound/death/").append(wujiangFilenameStr(heroType)).append(".mp3");
}

QString wujiangDisplayName(sgs::ConstData::HeroType heroType)
{
	using namespace sgs::ConstData;
	switch (heroType)
	{
	case SUJIANG:
		return QObject::trUtf8("素将");
	case NVSUJIANG:
		return QObject::trUtf8("女素将");
	case HUATUO:
		return QObject::trUtf8("华佗");
	case LVBU:
		return QObject::trUtf8("吕布");
	case DIAOCHAN:
		return QObject::trUtf8("貂蝉");
	case CAOCAO:
		return QObject::trUtf8("曹操");
	case XIAHOUDUN:
		return QObject::trUtf8("夏侯惇");
	case XUCHU:
		return QObject::trUtf8("许褚");
	case ZHENJI:
		return QObject::trUtf8("甄姬");
	case ZHANGLIAO:
		return QObject::trUtf8("张辽");
	case GUOJIA:
		return QObject::trUtf8("郭嘉");
	case SIMAYI:
		return QObject::trUtf8("司马懿");
	case SUNQUAN:
		return QObject::trUtf8("孙权");
	case ZHOUYU:
		return QObject::trUtf8("周瑜");
	case GANNING:
		return QObject::trUtf8("甘宁");
	case LVMENG:
		return QObject::trUtf8("吕蒙");
	case HUANGGAI:
		return QObject::trUtf8("黄盖");
	case LUXUN:
		return QObject::trUtf8("陆逊");
	case DAQIAO:
		return QObject::trUtf8("大乔");
	case SUNSHANGXIANG:
		return QObject::trUtf8("孙尚香");
	case LIUBEI:
		return QObject::trUtf8("刘备");
	case GUANYU:
		return QObject::trUtf8("关羽");
	case ZHANGFEI:
		return QObject::trUtf8("张飞");
	case LAOZHUGE:
		return QObject::trUtf8("诸葛亮");
	case ZHAOYUN:
		return QObject::trUtf8("赵云");
	case MACHAO:
		return QObject::trUtf8("马超");
	case HUANGYUEYING:
		return QObject::trUtf8("黄月英");
	default:
		return QObject::trUtf8("非标准包武将");
	}
}

QString skillSoundStr(sgs::ConstData::HeroSkill heroSkill)
{
	using namespace sgs::ConstData;
	QString skillSoundString("sound/skill/");
	switch (heroSkill) {
	case JIJIU:
		skillSoundString.append("jijiu");
		break;
	case QINGNANG:
		skillSoundString.append("qingnang");
		break;
	case WUSHUANG:
		skillSoundString.append("wushuang");
		break;
	case LIJIAN:
		skillSoundString.append("lijian");
		break;
	case BIYUE:
		skillSoundString.append("biyue");
		break;
	case JIANXIONG:
		skillSoundString.append("jianxiong");
		break;
	case HUJIA:
		skillSoundString.append("hujia");
		break;
	case GANGLIE:
		skillSoundString.append("ganglie");
		break;
	case QINGGUO:
		skillSoundString.append("qingguo");
		break;
	case LUOSHEN:
		skillSoundString.append("luoshen");
		break;
	case TUXI:
		skillSoundString.append("tuxi");
		break;
	case LUOYI:
		skillSoundString.append("luoyi");
		break;
	case TIANDU:
		skillSoundString.append("tiandu");
		break;
	case YIJI:
		skillSoundString.append("yiji");
		break;
	case FANKUI:
		skillSoundString.append("fankui");
		break;
	case GUICAI:
		skillSoundString.append("guicai");
		break;
	case ZHIHENG:
		skillSoundString.append("zhiheng");
		break;
	case JIUYUAN:
		skillSoundString.append("jiuyuan");
		break;
	case YINGZI:
		skillSoundString.append("yingzi");
		break;
	case FANJIAN:
		skillSoundString.append("fanjian");
		break;
	case QIXI:
		skillSoundString.append("qixi");
		break;
	case KEJI:
		skillSoundString.append("keji");
		break;
	case KUROU:
		skillSoundString.append("kurou");
		break;
	case QIANXUN:
		skillSoundString.append("qianxun");
		break;
	case LIANYING:
		skillSoundString.append("lianying");
		break;
	case GUOSE:
		skillSoundString.append("guose");
		break;
	case LIULI:
		skillSoundString.append("liuli");
		break;
	case JIEYIN:
		skillSoundString.append("jieyin");
		break;
	case XIAOJI:
		skillSoundString.append("xiaoji");
		break;
	case RENDE:
		skillSoundString.append("rende");
		break;
	case JIJIANG:
		skillSoundString.append("jijiang");
		break;
	case WUSHENG:
		skillSoundString.append("wusheng");
		break;
	case PAOXIAO:
		skillSoundString.append("paoxiao");
		break;
	case GUANXING:
		skillSoundString.append("guanxing");
		break;
	case KONGCHENG:
		skillSoundString.append("kongcheng");
		break;
	case LONGDAN:
		skillSoundString.append("longdan");
		break;
	case MASHU:
		skillSoundString.append("mashu");
		break;
	case TIEQI:
		skillSoundString.append("tieqi");
		break;
	case JIZHI:
		skillSoundString.append("jizhi");
		break;
	case QICAI:
		skillSoundString.append("qicai");
		break;
	default:
		printDebug(
					"<font color=red><b>Warning: </b></font>skillSoundStr: Invalid Skill Type");
		break;
	}
	return skillSoundString.append((rand() % 2) ? '1' : '2').append(".mp3");
}

QString skillDisplayName(sgs::ConstData::HeroSkill heroSkill)
{
	using namespace sgs::ConstData;
	switch (heroSkill)
	{
	case JIJIU:
		return QObject::trUtf8("急救");
	case QINGNANG:
		return QObject::trUtf8("青囊");
	case WUSHUANG:
		return QObject::trUtf8("无双");
	case LIJIAN:
		return QObject::trUtf8("离间");
	case BIYUE:
		return QObject::trUtf8("闭月");
	case JIANXIONG:
		return QObject::trUtf8("奸雄");
	case HUJIA:
		return QObject::trUtf8("护驾");
	case GANGLIE:
		return QObject::trUtf8("刚烈");
	case QINGGUO:
		return QObject::trUtf8("倾国");
	case LUOSHEN:
		return QObject::trUtf8("洛神");
	case TUXI:
		return QObject::trUtf8("突袭");
	case LUOYI:
		return QObject::trUtf8("裸衣");
	case TIANDU:
		return QObject::trUtf8("天妒");
	case YIJI:
		return QObject::trUtf8("遗计");
	case FANKUI:
		return QObject::trUtf8("反馈");
	case GUICAI:
		return QObject::trUtf8("鬼才");
	case ZHIHENG:
		return QObject::trUtf8("制衡");
	case JIUYUAN:
		return QObject::trUtf8("救援");
	case YINGZI:
		return QObject::trUtf8("英姿");
	case FANJIAN:
		return QObject::trUtf8("反间");
	case QIXI:
		return QObject::trUtf8("奇袭");
	case KEJI:
		return QObject::trUtf8("克己");
	case KUROU:
		return QObject::trUtf8("苦肉");
	case QIANXUN:
		return QObject::trUtf8("谦逊");
	case LIANYING:
		return QObject::trUtf8("连营");
	case GUOSE:
		return QObject::trUtf8("国色");
	case LIULI:
		return QObject::trUtf8("流离");
	case JIEYIN:
		return QObject::trUtf8("结姻");
	case XIAOJI:
		return QObject::trUtf8("枭姬");
	case RENDE:
		return QObject::trUtf8("仁德");
	case JIJIANG:
		return QObject::trUtf8("激将");
	case WUSHENG:
		return QObject::trUtf8("武圣");
	case PAOXIAO:
		return QObject::trUtf8("咆哮");
	case GUANXING:
		return QObject::trUtf8("观星");
	case KONGCHENG:
		return QObject::trUtf8("空城");
	case LONGDAN:
		return QObject::trUtf8("龙胆");
	case MASHU:
		return QObject::trUtf8("马术");
	case TIEQI:
		return QObject::trUtf8("铁骑");
	case JIZHI:
		return QObject::trUtf8("急智");
	case QICAI:
		return QObject::trUtf8("奇才");
	default:
		return QObject::trUtf8("非标准包技能");
	}
}

QString resultDisplayStr(bool win)
{
	static const QString winOrFailStr[] =
	{
		QObject::trUtf8("胜利"),
		QObject::trUtf8("失败")
	};
	int index = win ? 0 : 1;
	return winOrFailStr[index];
}

QString messageInterpret(const sgs::Derive::CardMessage * message)
{
	if (!message || !GUIStaticData::isDebugLogOn())
	{
		return "<font color=red><b>Message: </b></font>"
				"cannot interpret due to null pointer, or debug log is off";
	}

	QString interpret("<font color=red><b>Message: </b></font>MCard: ");
	sgs::DataType::Player * from = message->from();
	sgs::DataType::Player * to = 0;
	const sgs::DataType::Card * card = message->card();

	if (card)
	{
		interpret.append(" card: " + cardFullDisplayName(card, false));
		interpret.append(" mask: " + cardDisplayName(card->mask()));
	}
	else
	{
		interpret.append(" card: none");
	}

	if (from)
	{
		interpret.append(" from: " + QString::number(from->seat()));
		interpret.append(wujiangDisplayName(message->from()->type()));
	}
	else
	{
		interpret.append(" from: nobody");
	}

	if (message->targets() == 0)
	{
		interpret.append(" to: nobody");
	}
	else
	{
		interpret.append(" to: ");
		for (int i = 0; i < message->targets(); ++i)
		{
			to = message->to(i);
			if (to)
			{
				interpret.append(QString::number(to->seat()));
				interpret.append(wujiangDisplayName(to->type()));
				interpret.append(' ');
			}
			else
			{
				interpret.append("invalid pointer");
			}
		}
	}

	std::pair<sgs::ConstData::PlayerRegionType, int> pos = message->pos();
	interpret.append(" pos: first = ");

	switch (pos.first)
	{
	case sgs::ConstData::playerRegionTypeNone:
		interpret.append("prtNone");
		break;

	case sgs::ConstData::PHAND:
		interpret.append("PHAND");
		break;

	case sgs::ConstData::PEQUIP:
		interpret.append("PEQUIP");
		break;

	case sgs::ConstData::PJUDGE:
		interpret.append("PJUDGE");
		break;

	default:
		interpret.append("Invalid");
		break;
	}

	interpret.append(" second = ");
	interpret.append(QString::number(pos.second));
	return interpret;
}

QString messageInterpret(const sgs::Derive::ZhangBaMessage * message)
{
	if (!message || !GUIStaticData::isDebugLogOn())
	{
		return "<font color=red><b>Message: </b></font>"
				"cannot interpret due to null pointer, or debug log is off";
	}

	QString interpret("<font color=red><b>Message: </b></font>MZhangBa: ");
	sgs::DataType::Player * from = message->from(), *to = 0;
	const sgs::DataType::Card * card = message->card();

	if (card)
		interpret.append(" card: " + cardDisplayName(card->type()) + cardColorDisplayName(card->color(), false) + QString::number(card->number()));
	else
		interpret.append(" card: none");

	if (from)
	{
		interpret.append(" from: " + QString::number(from->seat()));
		interpret.append(wujiangDisplayName(message->from()->type()));
	}
	else
	{
		interpret.append(" from: nobody");
	}

	if (message->targets() == 0)
	{
		interpret.append(" to: nobody");
	}
	else
	{
		interpret.append(" to: ");
		for (int i = 0; i < message->targets(); ++i)
		{
			to = message->to(i);
			if (to)
			{
				interpret.append(QString::number(to->seat()));
				interpret.append(wujiangDisplayName(to->type()));
				interpret.append(' ');
			}
			else
			{
				interpret.append("invalid pointer");
			}
		}
	}

	std::pair<sgs::ConstData::PlayerRegionType, int> pos = message->pos();
	interpret.append(" pos: first = ");
	switch (pos.first)
	{
	case sgs::ConstData::playerRegionTypeNone:
		interpret.append("prtNone");
		break;

	case sgs::ConstData::PHAND:
		interpret.append("PHAND");
		break;

	case sgs::ConstData::PEQUIP:
		interpret.append("PEQUIP");
		break;

	case sgs::ConstData::PJUDGE:
		interpret.append("PJUDGE");
		break;

	default:
		interpret.append("Invalid");
		break;
	}

	interpret.append(" second = ");
	interpret.append(QString::number(pos.second));
	interpret.append(" Cost 0:");
	interpret.append(cardFullDisplayName(message->cost(0).first, false));
	interpret.append(" Cost 1:");
	interpret.append(cardFullDisplayName(message->cost(1).first, false));
	return interpret;
}

QString messageInterpret(const sgs::Derive::SkillMessage * message)
{
	if (!message || !GUIStaticData::isDebugLogOn())
	{
		return "<font color=red><b>Message: </b></font>"
				"cannot interpret due to null pointer, or debug log is off";
	}

	QString interpret("<font color=red><b>Message: </b></font>MSkill: ");

	using namespace sgs::ConstData;
	using namespace sgs::Derive;
	using namespace sgs::DataType;
	sgs::DataType::Player * fromPlayer = message->from();
	interpret.append(" skill: " + skillDisplayName(message->skill()));
	interpret.append(" from: ");
	interpret.append(fromPlayer ? QString::number(fromPlayer->seat()) : QString("nobody"));

	if (const QingNangMessage * qingNangMsg = dynamic_cast<const QingNangMessage *>(message))
	{
		interpret.append(" real type: MQingNang");
		interpret.append(" card: ");
		interpret.append(cardFullDisplayName(qingNangMsg->card(), false));
		interpret.append(" to: ");
		interpret.append(qingNangMsg->to() ? QString::number(qingNangMsg->to()->seat()) : "nobody");
		interpret.append(" pos: ");
		interpret.append(QString::number(qingNangMsg->pos()));
	}

	if (const JiJiuMessage * jiJiuMsg = dynamic_cast<const JiJiuMessage *>(message))
	{
		interpret.append(" real type: MJiJiu");
		interpret.append(" card: ");
		interpret.append(cardFullDisplayName(jiJiuMsg->card(), false));

		interpret.append(" pos.first: ");
		switch (jiJiuMsg->pos().first)
		{
		case PHAND:
			interpret.append("PHAND");
			break;
		case PEQUIP:
			interpret.append("PEQUIP");
			break;
		case PJUDGE:
			interpret.append("PJUDGE");
			break;
		default:
			interpret.append("prtNone");
			break;
		}
		interpret.append(" pos.second: ");
		interpret.append(QString::number(jiJiuMsg->pos().second));
	}

	if (const GangLieMessage * ganglieMsg = dynamic_cast<const GangLieMessage *>(message))
	{
		interpret.append(" real type: MGangLie");
		interpret.append(" to: ");
		interpret.append(ganglieMsg->to() ? QString::number(ganglieMsg->to()->seat()) : "nobody");
	}

	if (const FanKuiMessage * fanKuiMsg = dynamic_cast<const FanKuiMessage *>(message))
	{
		interpret.append(" real type: MFanKui");
		interpret.append(" to: ");
		interpret.append(fanKuiMsg->to() ? QString::number(fanKuiMsg->to()->seat()) : "nobody");
	}

	if (const QiXiMessage * qiXiMsg = dynamic_cast<const QiXiMessage *>(message))
	{
		interpret.append(" real type: MQiXi");
		interpret.append(" card: ");
		interpret.append(cardFullDisplayName(qiXiMsg->card(), false));

		interpret.append(" to: ");
		interpret.append(QString::number(qiXiMsg->to()->seat()));

		interpret.append(" pos.first: ");
		switch (qiXiMsg->pos().first)
		{
		case PHAND:
			interpret.append("PHAND");
			break;

		case PEQUIP:
			interpret.append("PEQUIP");
			break;

		case PJUDGE:
			interpret.append("PJUDGE");
			break;

		default:
			interpret.append("prtNone");
			break;
		}

		interpret.append(" pos.second: ");
		interpret.append(QString::number(qiXiMsg->pos().second));
	}

	if (const LongDanMessage * longDanMsg = dynamic_cast<const LongDanMessage *>(message))
	{
		interpret.append(" real type: MLongDan");
		interpret.append(" card: ");
		interpret.append(cardFullDisplayName(longDanMsg->card(), false));
		interpret.append(" pos: ");
		interpret.append(QString::number(longDanMsg->pos()));
		interpret.append(" targets: ");
		interpret.append(QString::number(longDanMsg->targets()));
		interpret.append(" to: ");
		for (int i = 0; i < longDanMsg->targets(); ++i)
		{
			Player * player = longDanMsg->to(i);
			interpret.append(player ? QString::number(player->seat()) : "nobody");
			interpret.append(", ");
		}
	}

	if (const QingGuoMeesage * qingGuoMsg = dynamic_cast<const QingGuoMeesage *>(message))
	{
		interpret.append(" real type: MQingGuo");
		interpret.append(" card: ");
		interpret.append(cardFullDisplayName(qingGuoMsg->card(), false));

		interpret.append(" pos: ");
		interpret.append(QString::number(qingGuoMsg->pos()));
	}

	if (const WuShengMessage * wuShengMsg = dynamic_cast<const WuShengMessage *>(message))
	{
		interpret.append(" real type: MWuSheng");
		interpret.append(" card: ");
		interpret.append(cardFullDisplayName(wuShengMsg->card(), false));

		interpret.append(" pos.first: ");
		switch (wuShengMsg->pos().first)
		{
		case PHAND:
			interpret.append("PHAND");
			break;

		case PEQUIP:
			interpret.append("PEQUIP");
			break;

		case PJUDGE:
			interpret.append("PJUDGE");
			break;

		default:
			interpret.append("prtNone");
			break;
		}

		interpret.append(" pos.second: ");
		interpret.append(QString::number(wuShengMsg->pos().second));
		interpret.append(" targets: ");
		interpret.append(QString::number(wuShengMsg->targets()));
		interpret.append(" to: ");
		for (int i = 0; i < wuShengMsg->targets(); ++i)
		{
			Player * player = wuShengMsg->to(i);
			interpret.append(player ? QString::number(player->seat()) : "nobody");
			interpret.append(", ");
		}
	}

	if (const RenDeMessage * renDeMsg = dynamic_cast<const RenDeMessage *>(message))
	{
		interpret.append(" real type: MRenDe");
		interpret.append(" to: ");
		Player * player = renDeMsg->to();
		interpret.append(player ? QString::number(player->seat()) : "nobody");
		interpret.append(", ");
		interpret.append(" cards: ");
		interpret.append(QString::number(renDeMsg->cards()));
		interpret.append(" card and pos: ");
		for (int i = 0; i < renDeMsg->cards(); ++i)
		{
			interpret.append("(");
			interpret.append(cardFullDisplayName( renDeMsg->card(i), false));
			interpret.append(", ");
			interpret.append(QString::number( renDeMsg->pos(i)));
			interpret.append(") ");
		}
	}

	return interpret;
}

QString messageInterpret(const sgs::Derive::HurtMessage * message)
{
	if (!message || !GUIStaticData::isDebugLogOn())
	{
		return "<font color=red><b>Message: </b></font>"
				"cannot interpret due to null pointer, or debug log is off";
	}

	QString interpret("<font color=red><b>Message: </b></font>MHurt: ");
	sgs::DataType::Player * from = message->from(), *to = message->to();

	if (from)
	{
		interpret.append(" from: " + QString::number(from->seat()));
		interpret.append(wujiangDisplayName(message->from()->type()));
	}
	else
	{
		interpret.append(" from: god");
	}

	if (to)
	{
		interpret.append(" to: ");
		interpret.append(QString::number(to->seat()));
		interpret.append(wujiangDisplayName(to->type()));
	}
	else
	{
		interpret.append(" to: nobody");
	}

	interpret.append(" hurt: " + QString::number(message->hurt()));
	return interpret;
}

QString messageInterpret(const sgs::Derive::TransCardMessage * message)
{
	if (!message || !GUIStaticData::isDebugLogOn())
	{
		return "<font color=red><b>Message: </b></font>"
				"cannot interpret due to null pointer, or debug log is off";
	}

	QString interpret("<font color=red><b>Message: </b></font>MTransCard: ");
	sgs::DataType::Player * from = message->from(), *to = message->to();
	const sgs::DataType::Card * card = 0;
	interpret.append(" card: ");
	for (int i = 0; i < message->cards(); ++i)
	{
		card = message->trans(i);
		interpret.append(cardFullDisplayName(card, false));
		interpret.append(' ');
	}

	if (from)
	{
		interpret.append(" from: " + QString::number(from->seat()));
		interpret.append(wujiangDisplayName(message->from()->type()));
	}
	else
	{
		interpret.append(" from: nobody(maybe card deck)");
	}

	if (to)
	{
		interpret.append(" to: ");
		interpret.append(QString::number(to->seat()));
		interpret.append(wujiangDisplayName(to->type()));
	}
	else
	{
		interpret.append(" to: nobody(maybe used card deck)");
	}

	interpret.append(" frompos: ");
	switch (message->frompos())
	{
	case sgs::ConstData::playerRegionTypeNone:
		interpret.append("prtNone");
		break;

	case sgs::ConstData::PHAND:
		interpret.append("PHAND");
		break;

	case sgs::ConstData::PEQUIP:
		interpret.append("PEQUIP");
		break;

	case sgs::ConstData::PJUDGE:
		interpret.append("PJUDGE");
		break;

	default:
		interpret.append("Invalid");
		break;
	}

	interpret.append(" open = ");
	interpret.append(message->open() ? "true" : "false");
	interpret.append(" tojudge = ");
	interpret.append(message->tojudge() ? "true" : "false");
	return interpret;
}

QString messageInterpret(const sgs::Derive::SwitchPhaseMessage * message)
{
	if (!message || !GUIStaticData::isDebugLogOn())
	{
		return "<font color=red><b>Message: </b></font>"
				"cannot interpret due to null pointer, or debug log is off";
	}

	QString interpret("<font color=red><b>Message: </b></font>MSwitchPhase: ");
	sgs::DataType::Player * from = message->from();

	if (from)
	{
		interpret.append(" from: " + QString::number(from->seat()));
		interpret.append(wujiangDisplayName(message->from()->type()));
	}
	else
	{
		interpret.append(" from: nobody");
	}

	using namespace sgs::ConstData;
	interpret.append(" before: ");
	switch (message->before())
	{
	case STARTPHASE:
		interpret.append("START");
		break;

	case JUDGEMENTPHASE:
		interpret.append("JUDGEMENT");
		break;

	case GETCARDPHASE:
		interpret.append("GETCARD");
		break;

	case MAINPHASE:
		interpret.append("MAIN");
		break;

	case ABANDONCARDPHASE:
		interpret.append("ABANDONCARD");
		break;

	case ENDPHASE:
		interpret.append("END");
		break;

	case OTHERPHASE:
		interpret.append("OTHER");
		break;

	default:
		interpret.append("invalid phase");
		break;
	}

	interpret.append(" after: ");
	switch (message->after())
	{
	case STARTPHASE:
		interpret.append("START");
		break;

	case JUDGEMENTPHASE:
		interpret.append("JUDGEMENT");
		break;

	case GETCARDPHASE:
		interpret.append("GETCARD");
		break;

	case MAINPHASE:
		interpret.append("MAIN");
		break;

	case ABANDONCARDPHASE:
		interpret.append("ABANDONCARD");
		break;

	case ENDPHASE:
		interpret.append("END");
		break;

	case OTHERPHASE:
		interpret.append("OTHER");
		break;

	default:
		interpret.append("invalid phase");
		break;
	}
	return interpret;
}

QString messageInterpret(const sgs::Derive::DyingMessage * message)
{
	if (!message || !GUIStaticData::isDebugLogOn())
	{
		return "<font color=red><b>Message: </b></font>"
				"cannot interpret due to null pointer, or debug log is off";
	}

	QString interpret("<font color=red><b>Message: </b></font>MDying: ");
	sgs::DataType::Player * from = message->from();
	if (from)
	{
		interpret.append(" from: " + QString::number(from->seat()));
		interpret.append(wujiangDisplayName(message->from()->type()));
	}
	else
	{
		interpret.append(" from: nobody");
	}
	interpret.append(" neg: " + QString::number(message->neg()));
	return interpret;
}

QString messageInterpret(const sgs::Derive::KillMessage * message)
{
	if (!message || !GUIStaticData::isDebugLogOn())
	{
		return "<font color=red><b>Message: </b></font>"
				"cannot interpret due to null pointer, or debug log is off";
	}

	QString interpret("<font color=red><b>Message: </b></font>MKill: ");
	sgs::DataType::Player * from = message->from(), *to = message->to();

	if (from)
	{
		interpret.append(" from: " + QString::number(from->seat()));
		interpret.append(wujiangDisplayName(message->from()->type()));
	}
	else
	{
		interpret.append(" from: god");
	}

	if (to)
	{
		interpret.append(" to: ");
		interpret.append(QString::number(to->seat()));
		interpret.append(wujiangDisplayName(to->type()));
	}
	else
	{
		interpret.append(" to: nobody");
	}
	return interpret;
}
QString messageInterpret(const sgs::Derive::JudgeMessage * message)
{
	if (!message || !GUIStaticData::isDebugLogOn())
	{
		return "<font color=red><b>Message: </b></font>"
				"cannot interpret due to null pointer, or debug log is off";
	}

	QString interpret("<font color=red><b>Message: </b></font>MJudge: ");
	sgs::DataType::Player * from = message->from();
	const sgs::DataType::Card * card = message->card();
	const sgs::DataType::Card * result = message->result();

	QString cardJudge = message->cardJudge() ? QString("true") : QString("false");
	interpret.append(" cardJudge: " + cardJudge);

	if (card)
	{
		QString cardName = cardDisplayName(card->type());
		QString cardColor = cardColorDisplayName(card->color(), false);
		interpret.append(" card: " + cardName + cardColor + QString::number(card->number()));
	}
	else
	{
		interpret.append(" card: none");
	}

	interpret.append(" skill: " + skillDisplayName(message->skill()));

	if (result)
	{
		QString cardName = cardDisplayName(result->type());
		QString cardColor = cardColorDisplayName(result->color(), false);
		interpret.append(" result (the judge card): " + cardName + cardColor + QString::number(result->number()));
	}
	else
	{
		interpret.append(" result: none");
	}

	if (from)
	{
		interpret.append(" from: " + QString::number(from->seat()));
		interpret.append(wujiangDisplayName(message->from()->type()));
	}
	else
	{
		interpret.append(" from: nobody");
	}

	interpret.append(" effect = ");
	interpret.append(message->effect() ? QString("true") : QString("false"));

	return interpret;
}

QString allMessageInterpret(const sgs::DataType::Message * message)
{
	if (!message || !GUIStaticData::isDebugLogOn())
	{
		return "<font color=red><b>Message: </b></font>"
				"cannot interpret due to null pointer, or debug log is off";
	}

	using namespace sgs::ConstData;
	using namespace sgs::Derive;
	switch (message->type())
	{
	case sgs::ConstData::HITCARD:
		if (dynamic_cast<const ZhangBaMessage *>(message))
			return messageInterpret(static_cast<const ZhangBaMessage *>(message));
		else
			return messageInterpret(static_cast<const CardMessage *>(message));

	case sgs::ConstData::USECARD:
		if (dynamic_cast<const ZhangBaMessage *>(message))
			return messageInterpret(static_cast<const ZhangBaMessage *>(message));
		else
			return messageInterpret(static_cast<const CardMessage *>(message));

	case sgs::ConstData::USESKILL:
		return messageInterpret(static_cast<const SkillMessage *>(message));

	case sgs::ConstData::HURT:
		return messageInterpret(static_cast<const HurtMessage *>(message));

	case sgs::ConstData::TRANSCARD:
		return messageInterpret(static_cast<const TransCardMessage *>(message));

	case sgs::ConstData::SWITCHPHASE:
		return messageInterpret(static_cast<const SwitchPhaseMessage *>(message));

	case sgs::ConstData::DYING:
		return messageInterpret(static_cast<const DyingMessage *>(message));

	case sgs::ConstData::KILL:
		return messageInterpret(static_cast<const KillMessage *>(message));

	case sgs::ConstData::JUDGE:
		return messageInterpret(static_cast<const JudgeMessage *>(message));

	default:
		return QString("Invalid Message Type");
	}
}

QString discardInterpret(const std::vector<int>& discardVec, sgs::DataType::Player * from)
{
	if (!from)
	{
		return "<font color=red><b>Discard: </b></font>"
				"cannot interpret due to null pointer, or debug log is off";
	}

	QString interpret("<font color=red><b>Discard: </b></font>card: ");
	const sgs::DataType::Card * card = 0;
	for (auto iter = discardVec.begin(); iter != discardVec.end(); ++iter)
	{
		card = from->hand(*iter);
		if (card)
		{
			QString cardName = cardDisplayName(card->type());
			QString cardColor = cardColorDisplayName(card->color(), false);
			interpret.append(cardName + cardColor + QString::number(card->number()));
			interpret.append(' ');
		}
		else
		{
			interpret.append("non-exist card ");
		}
	}
	return interpret;
}

QString selectInterpret(const std::vector<std::pair<sgs::ConstData::PlayerRegionType, int> > &selectVec)
{
	QString interpret("<font color=red><b>"
					  "Select: </b></font>card pair: ");
	for (auto citer = selectVec.begin(); citer != selectVec.end(); ++citer)
	{
		switch (citer->first)
		{
		case sgs::ConstData::PHAND:
			interpret.append("PHAND(");
			break;
		case sgs::ConstData::PJUDGE:
			interpret.append("PJUDGE(");
			break;
		case sgs::ConstData::PEQUIP:
			interpret.append("PEQUIP(");
			break;
		default:
			interpret.append("<font color=red><b>Warning: </b></font>"
							 "Invalid PlayerRegionType(");
			break;
		}
		interpret.append(QString::number(citer->second));
		interpret.append(") ");
	}
	return interpret;
}

QString setColored(const QString &originalString, bool bold/* = true*/)
{
	if (bold)
		return QString("<font color=\"white\"><b>%1</b></font>").arg(originalString);
	return QString("<font color=\"white\">%1</font>").arg(originalString);
}

QString splashStr()
{
	// 每次启动游戏随机采用一种图片
	static const QString splashes[] =
	{
		"images/splash/splash1.png",
		"images/splash/splash2.png",
		"images/splash/splash3.png"
	};
	int randIndex = rand()% 3;
	return splashes[randIndex];
}

EquipType getEquipType(sgs::ConstData::CardType cardType)
{
	using namespace sgs::ConstData;
	switch (cardType)
	{
	case ZHUGE:
	case QINGGANG:
	case HANBING:
	case CIXIONG:
	case GUANSHI:
	case YINYUE:
	case QINGLONG:
	case GUDING:
	case ZHANGBA:
	case FANGTIAN:
	case QILIN:
	case ZHUQUE:
		return Wuqi;

	case BAIYIN:
	case TENGJIA:
	case BAGUA:
	case RENWANG:
		return Fangju;

	case CHITU:
	case DAWAN:
	case ZICI:
		return Jianma;

	case DILU:
	case JUEYING:
	case ZHUAHUANG:
	case HUALIU:
		return Jiama;

	default:
		return NotEquip;
	}
}

} /* namespace sgsui */
