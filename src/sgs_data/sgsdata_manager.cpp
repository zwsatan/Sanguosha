#include "sgsdata_manager.h"
#include "sgsdata_player.h"
#include "sgsdata_extern.h"
#include "sgsdata_heroes.h"
#include "sgs_Interface.h"
#include <cstdlib>

using namespace sgs;
using namespace DataType;
using namespace ConstData;

void shuffleHero(std::vector<sgs::ConstData::HeroType>& heroVec);

Manager::Manager()
	: m_players(0)
	, m_playersCount(0)
{
}

void Manager::clearplayer()
{
	if (m_players)
	{
		Player * firstPlayer = m_players;
		do
		{
			Player * del = m_players;
			m_players = m_players->m_next;
			delete del;
		} while (m_players != firstPlayer);
	}

	m_playersCount = 0;
}

Manager::~Manager()
{
	// remove this destructor, move all the destruction work into platfrom::end();
	// consider use std::list instead of your list
}

Player * Manager::create(int playersCount)
{
	if (playersCount < 2 || playersCount > 8)
		return 0;

	m_playersCount = playersCount;
	PlayerRole roles[8] = { ZHU, FAN, NEI, ZHONG, FAN, FAN, ZHONG, FAN };
	PlayerRole seat[8] = { };

	for (int i = 0; i < playersCount; ++i)
	{
		int p = rand() % (playersCount - i);
		seat[i] = roles[p];
		for (int j = p; j < 7 - i; j++)
			roles[j] = roles[j + 1];
	}

	m_players = new Player(0, seat[0]);

	Player * begin = 0;
	if (seat[0] == ZHU)
		begin = m_players;

	Player * creator = m_players;
	for (int i = 1; i < playersCount; i++)
	{
		creator->m_next = new Player(i, seat[i]);
		if (seat[i] == ZHU)
			begin = creator->m_next;

		creator = creator->m_next;
	}

	creator->m_next = m_players;
	for (creator = m_players->m_next; creator != m_players; creator = creator->m_next)
		creator->m_next->m_prev = creator;
	m_players->m_next->m_prev = m_players;

	std::vector<HeroType> heroes;
	heroes.push_back(XIAHOUDUN);
	heroes.push_back(XUCHU);
	heroes.push_back(ZHENJI);
	heroes.push_back(GUOJIA);
	heroes.push_back(ZHANGLIAO);
	heroes.push_back(SIMAYI);

	heroes.push_back(GUANYU);
	heroes.push_back(ZHANGFEI);
	heroes.push_back(ZHAOYUN);
	heroes.push_back(MACHAO);
	heroes.push_back(LAOZHUGE);
	heroes.push_back(HUANGYUEYING);

	heroes.push_back(HUANGGAI);
	heroes.push_back(GANNING);
	heroes.push_back(LVMENG);
	heroes.push_back(DAQIAO);
	heroes.push_back(LUXUN);
	heroes.push_back(SUNSHANGXIANG);
	heroes.push_back(ZHOUYU);

	shuffleHero(heroes);

	heroes.push_back(CAOCAO);
	heroes.push_back(SUNQUAN);
	heroes.push_back(LIUBEI);

	begin->choosehero(heroes);
	shuffleHero(heroes);

	// 为主公创建AI
	begin->setinput(Interface::createInterface(begin->seat(), begin->type()));
	HeroType hero = begin->type();

	for (creator = begin->m_next; creator != begin; creator = creator->m_next)
	{
		creator->choosehero(heroes, hero);
		// 为其他电脑创建AI
		creator->setinput(Interface::createInterface(creator->seat(), creator->type()));
	}
	return begin;
}

int Manager::playernum() const
{
	return m_playersCount;
}

void shuffleHero(std::vector<sgs::ConstData::HeroType> &heroVec)
{
	int size = heroVec.size();
	sgs::ConstData::HeroType temp;
	for (int i = size - 1; i > 0; --i)
	{
		int random = rand() % (i + 1);
		temp = heroVec.at(i);
		heroVec.at(i) = heroVec.at(random);
		heroVec.at(random) = temp;
	}
}
