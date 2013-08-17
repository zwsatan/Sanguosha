#ifndef _SGSDATA_MANAGER_H
#define _SGSDATA_MANAGER_H
#include "sgsdata_enum.h"
#include "sgsdata_namespace.h"

// 玩家管理员类的定义
// 负责玩家的构造、身份分配、武将选择等功能的实现。
// 是平台类的一个基类。
class sgs::DataType::Manager
{
public:
	Manager();
	~Manager();

	// 清空玩家
	void clearplayer();
	// 创造n(2~8)个玩家，分配身份并选定武将。返回主公（开始游戏的玩家）指针。
	sgs::DataType::Player * create(int playersCount);

	// 访问玩家个数
	int playernum() const;
	// 访问用户对应的玩家
	sgs::DataType::Player * player();

private:
	sgs::DataType::Player *			m_players;				// 玩家指针，指向用户对应的玩家
	int								m_playersCount;			// 玩家个数
};

#endif /*_SGSDATA_MANAGER_H*/
