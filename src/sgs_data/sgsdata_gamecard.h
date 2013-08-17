#ifndef _SGSDATA_GAMECARD_H
#define _SGSDATA_GAMECARD_H
#include "sgsdata_enum.h"
#include "sgsdata_namespace.h"

class sgs::DataType::GameCard
{
public:
	GameCard();
	~GameCard();

	// 读入牌组（将全局常变量的指针存入牌堆）
	void load();
	// 清理牌堆，即清空牌堆与弃牌堆
	void clearcard();

	// 执行过程用函数
	// 从牌堆顶取出一张牌，执行完毕后牌堆里即无此牌
	const sgs::DataType::Card * get();
	// 向牌堆顶插入一张牌
	void pooltop(const sgs::DataType::Card *card);
	// 向牌堆底插入一张牌
	void poolbottom(const sgs::DataType::Card *card);
	// 弃一张牌
	void abandon(const sgs::DataType::Card *card);

	// 调试用函数
	// 观察牌堆从顶向底数的第n张牌，从0开始计数
	const sgs::DataType::Card * peekpool(int n) const;
	// 观察弃牌堆从顶向底数的第n张牌，从0开始计数
	const sgs::DataType::Card * peekdustbin(int n) const;
	int poolsize() const;
	int dustbinsize() const;
	// 修改卡牌，用于调试
	void setpool(int n, const sgs::DataType::Card *card);
	// 修改卡牌，用于调试
	void setdustbin(int n, const sgs::DataType::Card *card);

private:
	// 洗牌
	void refresh();

private:
	chain <const sgs::DataType::Card *>		m_pool;		// 牌堆
	chain <const sgs::DataType::Card *>		m_dustbin;	// 弃牌堆
};

#endif /*_SGSDATA_GAMECARD_H*/
