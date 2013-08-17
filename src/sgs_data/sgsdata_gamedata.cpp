#include "sgsdata_gamedata.h"
#include "sgsdata_player.h"
#include "sgsdata_platform.h"
#include "sgsdata_extern.h"
using namespace sgs;
using namespace Surface;
using namespace DataType;
 
GameData::GameData()
	: m_playernum(0)
	, m_players(0)
	, m_open(0)
{
}

GameData::~GameData()
{
	if (m_players != 0)
		delete [] m_players;
}

void GameData::initialize(Platform * platform)
{
	m_playernum = platform->playernum();
	if (m_playernum <=0)
		return;

	m_players = new Player * [m_playernum];
	if (m_players == 0)
		return;

	for (int i = 0; i < m_playernum; ++i)
		m_players[i] = platform->m_player->find(i);
	m_open = platform->m_player->find(0);//将第一个玩家设置为开放玩家
}

bool GameData::clear()
{
	m_playernum = 0;
	if (m_players != 0)
		delete [] m_players;
	return true;
}

int GameData::playernum() const
{
	return m_playernum;
}

Player *GameData::players(int seat) const
{
	return m_open->find(seat);
}

Player *GameData::open() const
{
	return m_open;
}
