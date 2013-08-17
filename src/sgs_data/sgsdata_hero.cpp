#include "sgsdata_hero.h"
#include "sgs_Interface.h"
#include "sgsdata_extern.h"
#include "sgsdata_skill.h"
#include "sgsdata_gamedata.h"
#include "sgsdata_platform.h"
#include "sgsdata_player.h"
#include "sgsdata_card_func.h"
#include "sgsui_nameConvert.h"
#include "sgsui_output.h"
#include <algorithm>
using namespace sgs;
using namespace ConstData;
using namespace DataType;
using namespace Derive;
using namespace ExternData;
 
Hero::Hero(int maxHP,
		   HeroType type,
		   HeroSex sex,
		   HeroSide side,
		   HeroSkill skill0/* = sgs::ConstData::heroSkillNone*/,
		   HeroSkill skill1/* = sgs::ConstData::heroSkillNone*/,
		   HeroSkill skill2/* = sgs::ConstData::heroSkillNone*/,
		   HeroSkill skill3/* = sgs::ConstData::heroSkillNone*/)
	: m_maxHP(maxHP)
	, m_type(type)
	, m_sex(sex)
	, m_side(side)
{
	if ((m_skill[0] = skill0).skill() == heroSkillNone)
	{
		m_skills = 0;
		return;
	}

	m_skills = 1;
	if ((m_skill[1] = skill1).skill() != heroSkillNone)
	{
		m_skills = 2;
		if ((m_skill[2] = skill2).skill() != heroSkillNone)
		{
			m_skills=3;
			if ((m_skill[3] = skill3).skill() != heroSkillNone)
			{
				m_skills = 4;
			}
		}
	}
}

Hero::~Hero()
{
}

int Hero::maxHP() const
{
	return m_maxHP;
}

HeroSex Hero::sex() const
{
	return m_sex;
}

HeroSide Hero::side() const
{
	return m_side;
}

Skill Hero::skill(int pos)
{
	if (pos<0 || pos>=m_skills)
		return heroSkillNone;
	return m_skill[pos];
}

int Hero::skillnum() const
{
	return m_skills;
}

HeroType Hero::type() const
{
	return m_type;
}
 
Message * Hero::startPhase(Player * player)
{
	Message * m = new SwitchPhaseMessage(player, STARTPHASE, JUDGEMENTPHASE);
	player->setphase(JUDGEMENTPHASE);
	return m;
}

Message * Hero::judgePhase(Player * player)
{
	if (player->judgenum() == 0)
	{
		Message * m = new SwitchPhaseMessage(player, JUDGEMENTPHASE, GETCARDPHASE);
		player->setphase(GETCARDPHASE);
		return m;
	}
	else
	{
		CardMessage * m = new CardMessage(true, player, PJUDGE, 0, player);
		player->popCard(m->pos());
		return m;
	}
}

Message * Hero::getPhase(Player * player)
{
	static bool got = false;
	Message * msg = 0;
	if (!got)
	{
		// 只有玩家才需要在牌卡转移的时候显示是什么牌
		bool isOpen = (player->seat() == 0) ? true : false;
		TransCardMessage* transMsg = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, player);
		for (int i = 0; i < 2; i++)
		{
			const Card * temp = platform.get();
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

Message * Hero::mainPhase(Player * player)
{
	printDebug("Hero::core: start");
	Message * m = player->input()->useCardResponse();
	if (m == 0)
	{
		m = new SwitchPhaseMessage(player, MAINPHASE, ABANDONCARDPHASE);
		player->setphase(ABANDONCARDPHASE);
	}
	else
	{
		switch (m->type())
		{
		case HITCARD:
		{
			CardMessage * message = static_cast<CardMessage*>(m);
			switch (message->pos().first)
			{
			case PHAND:
				message->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, message->pos().second ));
				break;
			default:
				break;
			}
		}
			break;

		case USECARD:
		{
			CardMessage * temp = static_cast<CardMessage*>(m);
			ZhangBaMessage * message = new ZhangBaMessage(temp->from(), temp->to());
			delete temp;
			message = func::pczhangba(message);
			printDebug("Hero::core: message: " + sgsui::messageInterpret(message));
			m = message;
		}
			break;

		case USESKILL:
			break;

		default:
			m = 0;
			break;
		}
	}
	printDebug("Hero::core: over");
	return m;
}

Message * Hero::abandonPhase(Player * player)
{
	printDebug("Hero::abandon: start");
	Message * m = 0;
	static bool abandoned = false;
	if (abandoned || player->handnum() <= player->hp())
	{
		m = new SwitchPhaseMessage(player, ABANDONCARDPHASE, ENDPHASE);
		player->setphase(ENDPHASE);
		abandoned = false;
	}
	else
	{
		TransCardMessage * t = new TransCardMessage(PLAYER, DUST, true, player, PHAND);
		int needs = player->handnum() - player->hp();
		std::vector<int> abandon = player->input()->discardCard(needs);

		std::sort(abandon.begin(), abandon.end());

		for (int i = needs - 1;	i >= 0; i--)
		{
			t->transpush(abandon.back());
			player->popCard(std::pair<PlayerRegionType,int>(PHAND, abandon.back()));
			abandon.pop_back();
		}
		m = t;
		abandoned = true;
	}
	printDebug("Hero::abandon: over");
	return m;
}

Message * Hero::endPhase(Player * player)
{
	Message * m = new SwitchPhaseMessage(player, ENDPHASE, OTHERPHASE);
	player->setstatus() &= ~ATTACKED;
	player->setstatus() &= ~SkillOneUsed;
	player->setstatus() &= ~SkillTwoUsed;
	player->clearShaCount(); // added by Hu Ronghang
	player->setphase(OTHERPHASE);
	return m;
}
