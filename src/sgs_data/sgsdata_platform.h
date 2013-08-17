#ifndef _SGSDATA_PLATFORM_H
#define _SGSDATA_PLATFORM_H
#include "sgsdata_enum.h"
#include "sgsdata_namespace.h"
#include "sgsdata_gamecard.h"
#include "sgsdata_manager.h"
#include <stack>

// 平台类的定义。平台类是GameCard类和Manager类的派生类。
// 平台是数据集散的中心和底层逻辑实现的平台。
// 在底层逻辑的主函数中只执行平台的函数。
// 平台也提供最主要的消息处理函数analyze，用在处理消息的各个地方。
// 平台还提供摸牌、弃牌等接口。
// 平台以全局变量的形式存在，在许多函数中被访问和调用成员函数。
class sgs::DataType::Platform
		: public sgs::DataType::GameCard
		, public sgs::DataType::Manager
{
public:
	// 界面可以通过数据堆访问玩家数据。
	friend class sgs::Surface::GameData;

	// 处理各种消息的核心函数。
	sgs::DataType::Message * analyze(sgs::DataType::Message *msg);
	// 获得一张判定牌
	const Card * getJudge();

	// 主函数接口
	// 游戏初始化
	void start(int playersCount);
	// 游戏运行
	char run();
	// 游戏结束
	void end();

	// 消息沟道相关函数
	// 向消息沟道插入一个新消息，如果传入空指针则不会插入
	void msgInsert(sgs::DataType::Message *msg);
	// 调查消息沟道是否为空
	bool msgEmpty();
	// 读取消息沟道的最顶层消息
	sgs::DataType::Message * msgRead();
	// 弹出消息沟道最顶层的消息
	void msgPop();

private:
	// 返回游戏结果（每一位0代表玩家失败，1代表玩家获胜，如果游戏没有结束则返回0）
	char endOrNot();

private:
	sgs::DataType::Player *					m_player;		// 当前动作的玩家
	int										m_playernum;	// 玩家个数
	std::stack<sgs::DataType::Message *>	m_groove;		// 待处理消息沟道
};

#endif /*_SGSDATA_PLATFORM_H*/
