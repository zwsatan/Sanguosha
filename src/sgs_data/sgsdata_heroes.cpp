#include "sgsdata_hero.h"
#include "sgsdata_heroes.h"
#include "sgsdata_msg.h"
#include "sgsdata_card.h"
#include "sgsdata_card_func.h"
#include "sgsdata_skill_func.h"
#include "sgsdata_extern.h"
#include "sgsdata_history.h"
#include "sgsdata_platform.h"
#include "sgs_Interface.h"
#include "sgsui_output.h"
#include "sgsui_nameConvert.h"

using namespace sgs::ConstData;
using namespace sgs::Derive;
using namespace sgs::DataType;

HSuJiang::HSuJiang()
	: Hero(4, SUJIANG, MALE, QUN)
{
}

HSuJiang::~HSuJiang()
{
}

HNvSuJiang::HNvSuJiang()
	: Hero(4, NVSUJIANG, FEMALE, QUN)
{
}

HNvSuJiang::~HNvSuJiang()
{
}

HZhangFei::HZhangFei()
	: Hero(4, ZHANGFEI, MALE, SHU, PAOXIAO)
{
}

HZhangFei::~HZhangFei()
{
}

Message * HZhangFei::mainPhase(Player * player)
{
	printDebug("HZhangFei::core: start");
	Message * msg = player->input()->useCardResponse();

	if (msg == 0)
	{
		msg = new SwitchPhaseMessage(player, MAINPHASE, ABANDONCARDPHASE);
		player->setphase(ABANDONCARDPHASE);
	}
	else
	{
		switch (msg->type())
		{
		case HITCARD:
		{
			CardMessage * cardMsg = static_cast<CardMessage*>(msg);
			switch (cardMsg->pos().first)
			{
			case PHAND:
				cardMsg->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, cardMsg->pos().second));
				break;
			default:
				break;
			}

			if (cardMsg->card()->mask() == SHA)
			{
				if (player->status() & SkillOneUsed)
				{
					// 对于张飞来说，SkillOneUsed意思是开始触发咆哮效果
					SkillMessage * paoxiao = new SkillMessage(player, PAOXIAO);
					ExternData::sgsout << paoxiao;
					ExternData::history.push(paoxiao);
				}
				else
				{
					player->setstatus() |= SkillOneUsed;
				}
			}
		}
			break;

		case USECARD:
		{
			CardMessage * temp = static_cast<CardMessage*>(msg);
			ZhangBaMessage * zhangBaMsg = new ZhangBaMessage(temp->from(), temp->to());
			delete temp;
			zhangBaMsg = func::pczhangba(zhangBaMsg);
			printDebug("HZhangFei::core: message: " + sgsui::messageInterpret(zhangBaMsg));
			msg = zhangBaMsg;
			if (msg != 0)
			{
				if (player->status() & SkillOneUsed)
				{
					//对于张飞来说，SkillOneUsed意思是开始触发咆哮效果
					SkillMessage * paoxiao = new SkillMessage(player, PAOXIAO);
					ExternData::sgsout << paoxiao;
					ExternData::history.push(paoxiao);
				}
				else
				{
					player->setstatus() |= SkillOneUsed;
				}
			}
		}
			break;

		default:
			msg = 0;
			break;
		}
	}
	printDebug("HZhangFei::core: over");
	return msg;
}

HLvMeng::HLvMeng()
	: Hero(4, LVMENG, MALE, WU, KEJI)
{
}

HLvMeng::~HLvMeng()
{
}

Message * HLvMeng::getPhase(Player * player)
{
	static bool got = false;
	Message * msg = 0;
	if (!got)
	{
		bool isOpen = (player->seat() == 0) ? true : false;
		TransCardMessage* transMsg = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, player);
		for (int i = 0; i < 2; ++i)
		{
			const Card * temp = ExternData::platform.get();
			transMsg->transpush(temp);
		}
		msg = transMsg;
		got = true;
	}
	else if (player->status() & LE)
	{
		if (player->input()->useSkillOrNot(KEJI))
		{
			SkillMessage * kejiMsg = new SkillMessage(player, KEJI);
			ExternData::sgsout<<kejiMsg;
			ExternData::history.push(kejiMsg);

			msg = new SwitchPhaseMessage(player, GETCARDPHASE, ENDPHASE);
			player->setphase(ENDPHASE);
		}
		else
		{
			msg = new SwitchPhaseMessage(player, GETCARDPHASE, ABANDONCARDPHASE);
			player->setphase(ABANDONCARDPHASE);
		}
		player->setstatus() = player->setstatus() & ~LE;
		got = false;
	}
	else
	{
		msg = new SwitchPhaseMessage(player, GETCARDPHASE, MAINPHASE);
		player->setphase(MAINPHASE);
		got = false;
	}
	return msg;
}

Message * HLvMeng::mainPhase(Player * player)
{
	printDebug("HLvMeng::core: start");
	Message * msg = player->input()->useCardResponse();
	if (msg == 0)
	{
		if (!(player->status() & ATTACKED))
		{
			// 未主动出杀,且使用克己技能
			bool useKeJi = player->input()->useSkillOrNot(KEJI);
			if (useKeJi)
			{
				SkillMessage * kejiMsg = new SkillMessage(player, KEJI);
				ExternData::sgsout << kejiMsg;
				ExternData::history.push(kejiMsg);
				msg = new SwitchPhaseMessage(player, MAINPHASE, ENDPHASE);
				player->setphase(ENDPHASE);
				printDebug("HLvMeng::core: over");
				return msg;
			}
		}

		msg = new SwitchPhaseMessage(player, MAINPHASE, ABANDONCARDPHASE);
		player->setphase(ABANDONCARDPHASE);
	}
	else
	{
		switch (msg->type())
		{
		case HITCARD:
		{
			CardMessage * cardMsg = static_cast<CardMessage*>(msg);
			switch (cardMsg->pos().first) {
			case PHAND:
				cardMsg->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, cardMsg->pos().second));
				break;
			default:
				break;
			}
		}
			break;

		case USECARD:
		{
			CardMessage * temp = static_cast<CardMessage*>(msg);
			ZhangBaMessage * zhangbaMsg = new ZhangBaMessage(temp->from(), temp->to());
			delete temp;
			zhangbaMsg = func::pczhangba(zhangbaMsg);
			printDebug("HLvMeng::core: message: " + sgsui::messageInterpret(zhangbaMsg));
			msg = zhangbaMsg;
		}
			break;

		default:
			msg = 0;
			break;
		}
	}
	printDebug("HLvMeng::core: over");
	return msg;
}

HMaChao::HMaChao()
	: Hero(4, MACHAO, MALE, SHU, MASHU, TIEQI)
{
}

HMaChao::~HMaChao()
{
}

HGuanYu::HGuanYu()
	: Hero(4, GUANYU, MALE, SHU, WUSHENG)
{
}

HGuanYu::~HGuanYu()
{
}

HXiaHouDun::HXiaHouDun()
	: Hero(4, XIAHOUDUN, MALE, WEI, GANGLIE)
{
}

HXiaHouDun::~HXiaHouDun()
{
}

HXuChu::HXuChu()
	: Hero(4, XUCHU, MALE, WEI, LUOYI)
{
}

HXuChu::~HXuChu()
{
}

Message * HXuChu::getPhase(Player * player)
{
	static bool got = false;
	Message * msg = 0;
	if (!got)
	{
		if (player->input()->useSkillOrNot(LUOYI))
		{
			SkillMessage * luoyiMsg = new SkillMessage(player, LUOYI);
			ExternData::sgsout << luoyiMsg;
			ExternData::history.push(luoyiMsg);

			bool isOpen = (player->seat() == 0) ? true : false;
			TransCardMessage * transMsg = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, player);
			const Card * temp = ExternData::platform.get();
			transMsg->transpush(temp);
			msg = transMsg;
			player->setstatus() |= SkillOneUsed;
		}
		else
		{
			bool isOpen = (player->seat() == 0) ? true : false;
			TransCardMessage * transMsg = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, player);
			for (int i = 0; i < 2; ++i)
			{
				const Card * temp = ExternData::platform.get();
				transMsg->transpush(temp);
				msg = transMsg;
			}
		}

		got = true;
	}
	else if (player->status() & LE)
	{
		msg = new SwitchPhaseMessage(player, GETCARDPHASE, ABANDONCARDPHASE);
		player->setphase(ABANDONCARDPHASE);
		player->setstatus() = player->setstatus() & ~LE;
		got = false;
	}
	else
	{
		msg = new SwitchPhaseMessage(player, GETCARDPHASE, MAINPHASE);
		player->setphase(MAINPHASE);
		got = false;
	}
	return msg;
}

HZhenJi::HZhenJi()
	: Hero(3, ZHENJI, FEMALE, WEI, QINGGUO, LUOSHEN)
{
}

HZhenJi::~HZhenJi()
{
}

Message * HZhenJi::startPhase(Player * player)
{
	bool LuoShenFailed = false;
	while (!LuoShenFailed && player->input()->useSkillOrNot(LUOSHEN))
	{
		SkillMessage * luoshenMsg = new SkillMessage(player, LUOSHEN);
		ExternData::sgsout << luoshenMsg;
		ExternData::history.push(luoshenMsg);

		const Card * judge = ExternData::platform.getJudge();
		JudgeMessage * judgeResult = 0;
		if (judge->color() == SPADE || judge->color() == CLUB)
			judgeResult = new JudgeMessage(player, LUOSHEN, judge, true);
		else
			judgeResult = new JudgeMessage(player, LUOSHEN, judge, false);

		judgeResult = static_cast<JudgeMessage *>(func::pcguicai(judgeResult));
		ExternData::sgsout << judgeResult;
		ExternData::history.push(judgeResult);

		if (judgeResult->effect())
		{
			TransCardMessage * transMsg = new TransCardMessage(POOLTOP, PLAYER, true, 0, playerRegionTypeNone, player);
			transMsg->transpush(judge);
			ExternData::platform.analyze(transMsg);
		}
		else
		{
			LuoShenFailed = true;
			ExternData::platform.abandon(judge);
		}
	}

	Message * msg = new SwitchPhaseMessage(player, STARTPHASE, JUDGEMENTPHASE);
	player->setphase(JUDGEMENTPHASE);
	return msg;
}

HLaoZhuGe::HLaoZhuGe()
	: Hero(3, LAOZHUGE, MALE, SHU, GUANXING, KONGCHENG)
{
}

HLaoZhuGe::~HLaoZhuGe()
{
}

Message * HLaoZhuGe::startPhase(Player * player)
{
	if (player->input()->useSkillOrNot(GUANXING))
	{
		SkillMessage * guanxing = new SkillMessage(player, GUANXING);
		ExternData::sgsout << guanxing;
		ExternData::history.push(guanxing);

		int playerAlive = 1;
		for (Player * temp = player->after(); temp != player; temp = temp->after())
			playerAlive++;

		std::vector<const Card *> top;
		std::vector<const Card *> bottom;
		for (int i = 0; i < playerAlive && i < 5; ++i)
			top.push_back(ExternData::platform.get());

		printDebug("HLaoZhuGe::start: about to choose");
		player->input()->chooseGuanxing(top, bottom);
		printDebug("HLaoZhuGe::start: choose over");

		for (unsigned i = 0, topsize = top.size(); i < topsize; ++i)
			ExternData::platform.pooltop(top[topsize - 1 - i]);
		for (unsigned i = 0, buttomsize = bottom.size(); i < buttomsize; ++i)
			ExternData::platform.poolbottom(bottom[i]);
	}

	Message * msg = new SwitchPhaseMessage(player, STARTPHASE, JUDGEMENTPHASE);
	player->setphase(JUDGEMENTPHASE);
	return msg;
}

HZhaoYun::HZhaoYun()
	: Hero(4, ZHAOYUN, MALE, SHU, LONGDAN)
{
}

HZhaoYun::~HZhaoYun()
{
}

HGanNing::HGanNing()
	: Hero(4, GANNING, MALE, WU, QIXI)
{
}

HGanNing::~HGanNing()
{
}

HSiMaYi::HSiMaYi()
	: Hero(3, SIMAYI, MALE, WEI, FANKUI, GUICAI)
{
}

HSiMaYi::~HSiMaYi()
{
}

HHuangGai::HHuangGai()
	: Hero(4, HUANGGAI, MALE, WU, KUROU)
{
}

HHuangGai::~HHuangGai()
{
}

HHuaTuo::HHuaTuo()
	: Hero(3, HUATUO, MALE, QUN, JIJIU, QINGNANG)
{
}

HHuaTuo::~HHuaTuo()
{
}

HLiuBei::HLiuBei()
	: Hero(4, LIUBEI, MALE, SHU, RENDE, JIJIANG)
{
}

HLiuBei::~HLiuBei()
{

}

HCaoCao::HCaoCao()
	: Hero(4, CAOCAO, MALE, WEI, JIANXIONG, HUJIA)
{
}

HCaoCao::~HCaoCao()
{
}

HHuangYueYing::HHuangYueYing()
	: Hero(3, HUANGYUEYING, FEMALE, SHU, JIZHI, QICAI)
{
}

HHuangYueYing::~HHuangYueYing()
{
}

HDiaoChan::HDiaoChan()
	: Hero(3, DIAOCHAN, FEMALE, QUN, LIJIAN, BIYUE)
{
}
HDiaoChan::~HDiaoChan()
{
}

Message * HDiaoChan::endPhase(Player * player)
{
	// 貂蝉结束阶段可以获取一张卡牌,技能闭月
	if (player->input()->useSkillOrNot(BIYUE))
	{
		SkillMessage * biyueMsg = new SkillMessage(player, BIYUE);
		ExternData::sgsout << biyueMsg;
		ExternData::history.push(biyueMsg);

		bool isOpen = player->seat() == 0;
		TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, player);
		trans->transpush(ExternData::platform.get());
		ExternData::platform.analyze(trans);
	}

	Message * msg = new SwitchPhaseMessage(player, ENDPHASE, OTHERPHASE);
	player->setstatus() &= ~ATTACKED;
	player->setstatus() &= ~SkillOneUsed;
	player->setstatus() &= ~SkillTwoUsed;
	player->clearShaCount();
	player->setphase(OTHERPHASE);
	return msg;
}

HZhangLiao::HZhangLiao()
	: Hero(4, ZHANGLIAO, MALE, WEI, TUXI)
{
}

HZhangLiao::~HZhangLiao()
{
}

Message * HZhangLiao::getPhase(Player * player)
{
	static bool got = false;
	Message * msg = 0;
	if (!got)
	{
		TuXiMessage * tuxiMsg = dynamic_cast<TuXiMessage *>(player->input()->getTuxi());
		if (tuxiMsg != 0)
		{
			ExternData::sgsout << tuxiMsg;
			ExternData::history.push(tuxiMsg);

			got = true;
			for (int i = 0; i < tuxiMsg->targets(); ++i)
			{
				bool isOpen = player->seat() == 0;
				TransCardMessage * transMsg = new TransCardMessage(PLAYER, PLAYER, isOpen, tuxiMsg->pos(i).first, PHAND, player);
				transMsg->transpush(tuxiMsg->pos(i).second);
				tuxiMsg->pos(i).first->popCard(std::pair<PlayerRegionType, int>(PHAND, tuxiMsg->pos(i).second));
				ExternData::platform.analyze(transMsg);
			}

			return 0;
		}

		bool isOpen = (player->seat() == 0) ? true : false;
		TransCardMessage * transMsg = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, player);
		for (int i = 0; i < 2; ++i)
		{
			const Card * temp = ExternData::platform.get();
			transMsg->transpush(temp);
		}
		msg = transMsg;
		got = true;
	}
	else if (player->status() & LE)
	{
		msg = new SwitchPhaseMessage(player, GETCARDPHASE, ABANDONCARDPHASE);
		player->setphase(ABANDONCARDPHASE);
		player->setstatus() = player->setstatus() & ~LE;
		got = false;
	}
	else
	{
		msg = new SwitchPhaseMessage(player, GETCARDPHASE, MAINPHASE);
		player->setphase(MAINPHASE);
		got = false;
	}

	return msg;
}

HGuoJia::HGuoJia()
	: Hero(3, GUOJIA, MALE, WEI, TIANDU, YIJI)
{
}

HGuoJia::~HGuoJia()
{
}

HSunQuan::HSunQuan()
	: Hero(4, SUNQUAN, MALE, WU, ZHIHENG, JIUYUAN)
{
}

HSunQuan::~HSunQuan()
{
}

HLvBu::HLvBu()
	: Hero(4, LVBU, MALE, QUN, WUSHUANG)
{
}

HLvBu::~HLvBu()
{
}

HZhouYu::HZhouYu()
	: Hero(3, ZHOUYU, MALE, WU, FANJIAN, YINGZI)
{
}

HZhouYu::~HZhouYu()
{
}

Message* HZhouYu::getPhase(Player * player)
{
	static bool got = false;
	Message * msg = 0;
	if (!got)
	{
		bool isOpen = (player->seat() == 0) ? true : false;
		TransCardMessage * transMsg = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, player);
		for (int i = 0; i < 2; ++i)
		{
			const Card * temp = ExternData::platform.get();
			transMsg->transpush(temp);
		}

		if (player->input()->useSkillOrNot(YINGZI))
		{
			SkillMessage * yingzi = new SkillMessage(player, YINGZI);
			ExternData::sgsout << yingzi;
			ExternData::history.push(yingzi);

			transMsg->transpush(ExternData::platform.get());
		}
		msg = transMsg;
		got = true;
	}
	else if (player->status() & LE)
	{
		msg = new SwitchPhaseMessage(player, GETCARDPHASE, ABANDONCARDPHASE);
		player->setphase(ABANDONCARDPHASE);
		player->setstatus() = player->setstatus() & ~LE;
		got = false;
	}
	else
	{
		msg = new SwitchPhaseMessage(player, GETCARDPHASE, MAINPHASE);
		player->setphase(MAINPHASE);
		got = false;
	}
	return msg;
}

HSunShangXiang::HSunShangXiang()
	: Hero(3, SUNSHANGXIANG, FEMALE, WU, JIEYIN, XIAOJI)
{
}

HSunShangXiang::~HSunShangXiang()
{
}

HLuXun::HLuXun()
	: Hero(3, LUXUN, MALE, WU, QIANXUN, LIANYING)
{
}

HLuXun::~HLuXun()
{
}

HDaQiao::HDaQiao()
	: Hero(3, DAQIAO, FEMALE, WU, GUOSE, LIULI)
{
}

HDaQiao::~HDaQiao()
{
}
