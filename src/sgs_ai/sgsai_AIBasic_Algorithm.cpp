#include "sgsai_AIBasic.h"
#include "sgsui_MainWindow.h"
#include "sgsui_PlayerSort.h"
#include "sgsui_nameConvert.h"
#include "sgsui_output.h"
#include "sgsdata_enum.h"
#include "sgsdata_msg.h"
#include "sgsdata_player.h"
#include "sgsdata_card.h"
#include "sgsdata_extern.h"
#include "sgsdata_gamedata.h"
#include <algorithm>

using namespace sgs::DataType;
using namespace sgs::ConstData;
using sgs::ExternData::gamedata;

namespace sgsai {

const PlayerVec& AIBasic::shaTarget()
{
	m_shaTargetVec.clear();

	for (int i = 0; i < gamedata.playernum(); ++i)
	{
		if (i == mySeat())
			continue;

		Player * enemy = gamedata.players(i);
		if (enemy->status() & DEAD)
			continue;

		// 略过手牌为0的老诸葛
		if (enemy->type() == LAOZHUGE && enemy->handnum() == 0)
			continue;

		if (!isMyFriend(enemy) && (myPlayer()->distance(enemy) <= myPlayer()->range()))
			m_shaTargetVec.push_back(enemy);
	}

	sgsui::PlayerSort arrange(mySeat(), sgsui::ByThreat);
	std::sort(m_shaTargetVec.begin(), m_shaTargetVec.end(), arrange);

	return m_shaTargetVec;
}

const PlayerVec& AIBasic::juedouTarget()
{
	m_juedouTargetVec.clear();
	for (int i = 0; i < gamedata.playernum(); ++i)
	{
		if (i == mySeat())
			continue;

		Player * enemy = gamedata.players(i);
		if (enemy->status() & DEAD)
			continue;

		// 略过手牌为0的老诸葛
		if (enemy->type() == LAOZHUGE && enemy->handnum() == 0)
			continue;

		if (!isMyFriend(enemy))
			m_juedouTargetVec.push_back(enemy);
	}
	sgsui::PlayerSort arrange(mySeat(), sgsui::ByThreat);
	std::sort(m_juedouTargetVec.begin(), m_juedouTargetVec.end(), arrange);

	return m_juedouTargetVec;
}

const PlayerVec& AIBasic::sishuTarget()
{
	m_sishuTargetVec.clear();

	for (int i = 0; i < gamedata.playernum(); ++i)
	{
		if (i == mySeat())
			continue;

		Player * enemy = gamedata.players(i);
		if (enemy->type() == LUXUN)
			continue;
		if (enemy->status() & DEAD)
			continue;

		if (!isMyFriend(enemy) && !hasSishuInJudge(enemy))
			m_sishuTargetVec.push_back(enemy);
	}

	sgsui::PlayerSort arrange(mySeat(), sgsui::ByThreat);
	std::sort(m_sishuTargetVec.begin(), m_sishuTargetVec.end(), arrange);

	return m_sishuTargetVec;
}

const PlayerVec& AIBasic::guochaiTarget()
{
	m_guochaiTargetVec.clear();

	for (int i = 0; i < gamedata.playernum(); ++i)
	{
		if (i == mySeat())
			continue;

		Player * enemy = gamedata.players(i);
		if (enemy->status() & DEAD)
			continue;

		// 不拆对方判断卡牌区的卡
		if (!isMyFriend(enemy) && (totalCardNum(enemy) - enemy->judgenum() > 0))
		{
			printDebug("AIBasic::guochaiTarget: spoted target, seat = " + QString::number(i));
			m_guochaiTargetVec.push_back(enemy);
		}
	}

	sgsui::PlayerSort arrange(mySeat(), sgsui::ByThreat);
	std::sort(m_guochaiTargetVec.begin(), m_guochaiTargetVec.end(), arrange);

	return m_guochaiTargetVec;
}

// 返回可以顺手牵羊的目标
PlayerVec AIBasic::shunqianTarget()
{
	// 尾部存放威胁性最大的玩家
	PlayerVec targetVec;

	for (int i = 0; i < gamedata.playernum(); ++i)
	{
		if (i == mySeat())
			continue;

		Player * enemy = gamedata.players(i);
		if (enemy->type() == LUXUN)
			continue;
		if (enemy->status() & DEAD)
			continue;

		// 不顺对方的判定区的卡牌
		if (!isMyFriend(enemy)
			&& (totalCardNum(enemy) - enemy->judgenum() > 0)
			&& ((myPlayer()->distance(enemy) <= 1) || myPlayer()->type() == HUANGYUEYING))
		{
			printDebug("AIBasic::shunqianTarget: spoted target, seat = " + QString::number(i));
			targetVec.push_back(enemy);
		}
	}

	// 排序,按威胁性排序
	sgsui::PlayerSort arrange(mySeat(), sgsui::ByThreat);
	std::sort(targetVec.begin(), targetVec.end(), arrange);

	return targetVec;
}

PlayerVec AIBasic::playersHavingWeapon()
{
	PlayerVec weaponOwnerVec;

	for (int i = 0; i < gamedata.playernum(); ++i)
	{
		if (i == mySeat())
			continue;

		Player * weaponOwner = gamedata.players(i);
		if (weaponOwner->status() & DEAD)
			continue;

		if (weaponOwner->weapon())
		{
			printDebug("AIBasic::playersHavingWeapon: spoted weapon owner, seat = " + QString::number(i));
			weaponOwnerVec.push_back(weaponOwner);
		}
	}

	sgsui::PlayerSort arrange(mySeat(), sgsui::ByThreat);
	std::sort(weaponOwnerVec.begin(), weaponOwnerVec.end(), arrange);

	return weaponOwnerVec;
}

PlayerVec AIBasic::jiedaoTarget(Player * weaponOwner)
{
	PlayerVec targetVec;

	for (int i = 0; i < gamedata.playernum(); ++i)
	{
		// 略过攻击者
		if (i == weaponOwner->seat())
			continue;

		Player * target = gamedata.players(i);
		if (target->status() & DEAD)
			continue;

		if (weaponOwner->range() >= weaponOwner->distance(target))
		{
			printDebug("AIBasic::jiedaoTarget: spoted target, seat = " + QString::number(i));
			targetVec.push_back(target);
		}
	}

	sgsui::PlayerSort arrange(mySeat(), sgsui::ByThreat);
	std::sort(targetVec.begin(), targetVec.end(), arrange);

	return targetVec;
}

std::pair<Player *, Player *> AIBasic::makeJiedaoPair()
{
	PlayerVec attackerVec(playersHavingWeapon());
	if (attackerVec.empty())
		return ms_invalidJiedaoPair;

	// 遍历所有有武器的目标
	// 逐个让他们作为借刀的来源
	// 只要他们的攻击范围内有目标
	// 就返回他和依据他威胁性最大的敌人
	for (auto iter = attackerVec.begin(); iter != attackerVec.end(); ++iter)
	{
		PlayerVec victimVec(jiedaoTarget(*iter));

		if (!victimVec.empty())
			return std::pair<Player *, Player *>(*iter, victimVec.back());
	}
	return ms_invalidJiedaoPair;
}

Player * AIBasic::lord()
{
	printDebug("AIBasic::lord");

	// when lord has not been marked, pick it out first
	if (!m_lord)
	{
		Player * player = 0;
		for (int i = 0; i < gamedata.playernum(); ++i)
		{
			player = gamedata.players(i);
			if (player->role() == ZHU)
			{
				m_lord = player;
				break;
			}
		}
	}

	// return the lord player pointer only when the lord is alive
	// maybe it is unnecessary because game is over when to lord is killed
	// but to be careful, keep it
	return (isDead(m_lord) ? 0 : m_lord);
}

bool AIBasic::isMyFriend(Player * player) const
{
	if (!player)
	{
		printDebug("<font color=red><b>Warning: </b></font>AIBasic::isMyFriend: null pointer");
		return false;
	}

	// 主公和忠诚是friend
	// 和内奸在反贼死亡前是friend
	// 内奸,反贼则没死完,就同主公,反则死亡了,就马上反目
	// 反贼和反贼是friend,和其他是敌人
	switch (myPlayer()->role())
	{
	case ZHU:
	case ZHONG:
		switch (player->role())
		{
		case ZHU:
		case ZHONG:
			return true;
		case NEI:
			return !isAllFanzeiDead();
		case FAN:
			return false;
		default:
			break;
		}
		break;
	case NEI:
		switch (player->role())
		{
		case ZHU:
		case ZHONG:
			return !isAllFanzeiDead();
		case NEI:
			return true;
		case FAN:
			return false;
		default:
			break;
		}
		break;
	case FAN:
		switch (player->role())
		{
		case ZHU:
		case ZHONG:
		case NEI:
			return false;
		case FAN:
			return true;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return true;
}

bool AIBasic::isAllFanzeiDead() const
{
	for (int i = 0; i < gamedata.playernum(); ++i)
	{
		// 死亡Player不考虑
		if (isDead(i))
			continue;

		PlayerRole role = gamedata.players(i)->role();
		if (role == FAN)
			return false;
	}
	return true;
}

} /* namespace sgsai */
