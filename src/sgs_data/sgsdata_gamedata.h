#ifndef _SGSDATA_GAMEDATA_H
#define _SGSDATA_GAMEDATA_H
#include "sgsdata_enum.h"
#include "sgsdata_namespace.h"

//显示用数据堆，主要作用是为界面提供专门的访问接口。
class sgs::Surface::GameData
{
public:
	GameData();
	~GameData();

	// 初始化
	void initialize(sgs::DataType::Platform * platform);
	// 清空所有内容
	bool clear();

	int playernum() const;
	// 通过座位号访问玩家，从0开始
	sgs::DataType::Player * players(int seat) const;
	// 访问开放玩家
	sgs::DataType::Player * open() const;

private:
	int							m_playernum;	// 玩家个数
	sgs::DataType::Player **	m_players;		// 指针数组，角标对应座位
	sgs::DataType::Player *		m_open;			// 开放玩家（显示这个玩家的内部讯息）
};
 
#endif /*_SGSDATA_GAMEDATA_H*/
