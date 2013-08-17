/*
 * sgs_Interface.cpp
 *
 *  Created on: May 6, 2012
 *      Author: latios
 */

#include "sgs_Interface.h"
#include "sgsui_MainWindow.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_output.h"
#include "sgsdata_extern.h"
#include "sgsdata_gamedata.h"
#include "sgsdata_player.h"
#include "sgsdata_card.h"
#include "sgsai_AISujiang.h"
#include "sgsai_AICaocao.h"
#include "sgsai_AIDaqiao.h"
#include "sgsai_AIDiaochan.h"
#include "sgsai_AIGanning.h"
#include "sgsai_AIGuanyu.h"
#include "sgsai_AIGuojia.h"
#include "sgsai_AIHuanggai.h"
#include "sgsai_AIHuangyueying.h"
#include "sgsai_AIHuatuo.h"
#include "sgsai_AILiubei.h"
#include "sgsai_AILuxun.h"
#include "sgsai_AILvbu.h"
#include "sgsai_AILvmeng.h"
#include "sgsai_AIMachao.h"
#include "sgsai_AISimayi.h"
#include "sgsai_AISunquan.h"
#include "sgsai_AISunshangxiang.h"
#include "sgsai_AIXiahoudun.h"
#include "sgsai_AIXuchu.h"
#include "sgsai_AIZhangfei.h"
#include "sgsai_AIZhangliao.h"
#include "sgsai_AIZhaoyun.h"
#include "sgsai_AIZhenji.h"
#include "sgsai_AIZhouyu.h"
#include "sgsai_AIZhugeliang.h"

namespace sgs {

int Interface::m_maxShaUse = 1;

Interface::Interface(int seat)
	: m_seat(seat)
	, m_myPlayer(0)
{
}

Interface::~Interface()
{
}

Interface * Interface::createInterface(int seat, ConstData::HeroType heroType, bool forceToBeAI)
{
	if (!forceToBeAI && (seat + 1 == sgsui::GUIStaticData::getHumanIndex()))
		return sgsui::MainWindow::mainWindowPtr();

	using namespace ConstData;
	using namespace sgsai;
	switch (heroType)
	{
	case SUJIANG:
	case NVSUJIANG:
		return new AISujiang(seat);
	case ZHANGFEI:
		return new AIZhangfei(seat);
	case LVMENG:
		return new AILvmeng(seat);
	case MACHAO:
		return new AIMachao(seat);
	case GUANYU:
		return new AIGuanyu(seat);
	case XIAHOUDUN:
		return new AIXiahoudun(seat);
	case XUCHU:
		return new AIXuchu(seat);
	case ZHENJI:
		return new AIZhenji(seat);
	case LAOZHUGE:
		return new AIZhugeliang(seat);
	case ZHAOYUN:
		return new AIZhaoyun(seat);
	case GANNING:
		return new AIGanning(seat);
	case SIMAYI:
		return new AISimayi(seat);
	case HUANGGAI:
		return new AIHuanggai(seat);
	case HUATUO:
		return new AIHuatuo(seat);
	case LIUBEI:
		return new AILiubei(seat);
	case CAOCAO:
		return new AICaocao(seat);
	case HUANGYUEYING:
		return new AIHuangyueying(seat);
	case DIAOCHAN:
		return new AIDiaochan(seat);
	case ZHANGLIAO:
		return new AIZhangliao(seat);
	case GUOJIA:
		return new AIGuojia(seat);
	case SUNQUAN:
		return new AISunquan(seat);
	case LVBU:
		return new AILvbu(seat);
	case ZHOUYU:
		return new AIZhouyu(seat);
	case SUNSHANGXIANG:
		return new AISunshangxiang(seat);
	case LUXUN:
		return new AILuxun(seat);
	case DAQIAO:
		return new AIDaqiao(seat);
	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "Interface::createInterface: "
				   "no interface created, check bug");
		return 0;
	}
}

int Interface::mySeat() const
{
	return m_seat;
}

DataType::Player * Interface::myPlayer() const
{
	if (!m_myPlayer)
		m_myPlayer = ExternData::gamedata.players(m_seat);
	return m_myPlayer;
}

bool Interface::canUseSha() const
{
	// 拥有武器诸葛连弩,无限出杀
	if (myPlayer()->weapon() && myPlayer()->weapon()->type() == ConstData::ZHUGE)
		return true;

	// 英雄张飞,无限出杀
	if (myPlayer()->type() == ConstData::ZHANGFEI)
		return true;

	// m_maxShaUse is -1 when there is no limit on sha use
	return (m_maxShaUse < 0) || (myPlayer()->shaCount() < m_maxShaUse);
}

bool Interface::canUseFangtian() const
{
	if (ExternData::gamedata.playernum() < 3)
		return false;

	DataType::Player * thisPlayer = myPlayer();
	int handnumber = thisPlayer->handnum();
	const DataType::Card* weapon = thisPlayer->weapon();
	// 若武器为方天画戟,且手牌只有1张,且此手牌为杀,则返回true
	bool result = (weapon && (weapon->type() == ConstData::FANGTIAN) && (handnumber == 1) && (thisPlayer->hand(0)->type() == ConstData::SHA));
	return result;
}

bool Interface::canUseZhangba() const
{
	bool isZhangba = myPlayer()->weapon() && myPlayer()->weapon()->type() == ConstData::ZHANGBA;
	bool returnValue = isZhangba && myPlayer()->handnum() > 1;

	if (myPlayer()->phase() == ConstData::MAINPHASE)
		returnValue &= canUseSha();

	return returnValue;
}

bool Interface::canUseJieyin() const
{
	bool returnValue = false;
	if (myPlayer()->status() & sgs::ConstData::SkillOneUsed)
		return false;

	if (myPlayer()->handnum() < 2)
		return false;

	int playerCount = ExternData::gamedata.playernum();
	for (int i = 0; i < playerCount; ++i)
	{
		sgs::DataType::Player * player = ExternData::gamedata.players(i);
		if (player->sex() == sgs::ConstData::MALE && player->hp() < player->maxHP()) {
			returnValue = true;
			break;
		}
	}
	return returnValue;
}

bool Interface::haveWuxie() const
{
	DataType::Player * thisPlayer = myPlayer();
	int handnum = thisPlayer->handnum();

	for (int i = 0; i < handnum; ++i)
	{
		if (thisPlayer->hand(i)->type() == ConstData::WUXIE)
			return true;
	}
	return false;
}

bool Interface::haveRedCard() const
{
	// 检查手牌和装备卡中是否有红牌存在
	DataType::Player* thisPlayer = myPlayer();
	int handnum = thisPlayer->handnum();
	ConstData::CardColor color;

	for (int i = 0; i < handnum; ++i)
	{
		color = thisPlayer->hand(i)->color();
		if (color == ConstData::HEART || color == ConstData::DIAMOND)
			return true;
	}

	if (thisPlayer->weapon())
	{
		color = thisPlayer->weapon()->color();
		if (color == ConstData::HEART || color == ConstData::DIAMOND)
			return true;
	}

	if (thisPlayer->armor())
	{
		color = thisPlayer->armor()->color();
		if (color == ConstData::HEART || color == ConstData::DIAMOND)
			return true;
	}

	if (thisPlayer->atkhorse())
	{
		color = thisPlayer->atkhorse()->color();
		if (color == ConstData::HEART || color == ConstData::DIAMOND)
			return true;
	}

	if (thisPlayer->dfdhorse())
	{
		color = thisPlayer->dfdhorse()->color();
		if (color == ConstData::HEART || color == ConstData::DIAMOND)
			return true;
	}
	return false;
}

bool Interface::haveBlackCard() const
{
	// 检查手牌和装备卡是否有黑牌存在
	DataType::Player * thisPlayer = myPlayer();
	int handnum = thisPlayer->handnum();
	ConstData::CardColor color;

	for (int i = 0; i < handnum; ++i)
	{
		color = thisPlayer->hand(i)->color();
		if (color == ConstData::SPADE || color == ConstData::CLUB)
			return true;
	}

	if (thisPlayer->weapon())
	{
		color = thisPlayer->weapon()->color();
		if (color == ConstData::SPADE || color == ConstData::CLUB)
			return true;
	}

	if (thisPlayer->armor())
	{
		color = thisPlayer->armor()->color();
		if (color == ConstData::SPADE || color == ConstData::CLUB)
			return true;
	}

	if (thisPlayer->atkhorse())
	{
		color = thisPlayer->atkhorse()->color();
		if (color == ConstData::SPADE || color == ConstData::CLUB)
			return true;
	}

	if (thisPlayer->dfdhorse())
	{
		color = thisPlayer->dfdhorse()->color();
		if (color == ConstData::SPADE || color == ConstData::CLUB)
			return true;
	}

	return false;
}

bool Interface::haveDiamondCard() const
{
	// 检查手牌和装备卡中有无方片牌
	DataType::Player * thisPlayer = myPlayer();
	int handnum = thisPlayer->handnum();

	ConstData::CardColor color;
	for (int i = 0; i < handnum; ++i)
	{
		color = thisPlayer->hand(i)->color();
		if (color == ConstData::DIAMOND)
			return true;
	}

	if (thisPlayer->weapon())
	{
		color = thisPlayer->weapon()->color();
		if (color == ConstData::DIAMOND)
			return true;
	}

	if (thisPlayer->armor())
	{
		color = thisPlayer->armor()->color();
		if (color == ConstData::DIAMOND)
			return true;
	}

	if (thisPlayer->atkhorse())
	{
		color = thisPlayer->atkhorse()->color();
		if (color == ConstData::DIAMOND)
			return true;
	}

	if (thisPlayer->dfdhorse())
	{
		color = thisPlayer->dfdhorse()->color();
		if (color == ConstData::DIAMOND)
			return true;
	}

	return false;
}

bool Interface::isDead(int seat) {
	return ExternData::gamedata.players(seat)->status() & ConstData::DEAD;
}

bool Interface::isDead(DataType::Player * player) {
	return player->status() & ConstData::DEAD;
}

bool Interface::hasWeapon(DataType::Player * player)
{
	return player->weapon();
}

// 判断在旁定区是否有乐不思蜀
bool Interface::hasSishuInJudge(DataType::Player * player)
{
	for (int i = 0; i < player->judgenum(); ++i)
	{
		if (player->judge(i)->type() == ConstData::SISHU)
			return true;
	}
	return false;
}

bool Interface::hasShandianInJudge(DataType::Player * player)
{
	for (int i = 0; i < player->judgenum(); ++i)
	{
		if (player->judge(i)->type() == ConstData::SHANDIAN)
			return true;
	}
	return false;
}

void Interface::showWugu(
		const std::vector<std::pair<const DataType::Card *, bool> >& wuguCards) {
	sgsui::MainWindow::mainWindowPtr()->showWugu(wuguCards);
}

int Interface::handEquipNum(DataType::Player * player)
{
	int totalNum = 0;

	if (player->weapon())
		++totalNum;

	if (player->armor())
		++totalNum;

	if (player->atkhorse())
		++totalNum;

	if (player->dfdhorse())
		++totalNum;

	totalNum += player->handnum();
	return totalNum;
}

int Interface::totalCardNum(DataType::Player * player)
{
	int totalNum = 0;

	if (player->weapon())
		++totalNum;

	if (player->armor())
		++totalNum;

	if (player->atkhorse())
		++totalNum;

	if (player->dfdhorse())
		++totalNum;

	totalNum += player->handnum();
	totalNum += player->judgenum();
	return totalNum;
}

} /* namespace sgs */
