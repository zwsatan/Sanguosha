#ifndef _SGSDATA_CARD_H
#define _SGSDATA_CARD_H

#include "sgsdata_enum.h"
#include "sgsdata_namespace.h"

class sgs::DataType::Card
{
public:
	Card();
	Card(sgs::ConstData::CardColor cardColor, int cardNum, sgs::ConstData::CardType cardType);
	~Card();

	//访问用函数头
	int number() const; //访问点数
	sgs::ConstData::CardColor color() const; //访问花色
	sgs::ConstData::CardType type() const; //访问实体卡类型
	sgs::ConstData::CardType mask() const; //访问效果类型
	sgs::DataType::Message * settle(sgs::DataType::Message *msg) const; //调用处理器函数

	// 修改用函数头
	// 设置效果，同时修改处理器指向
	void setmask(sgs::ConstData::CardType mask) const;
	// 将效果改回实体卡效果，同时修改处理器指向
	void setmask() const;

private:
	void protrans(sgs::ConstData::CardType cardType) const; //根据卡牌种类为处理器函数指针赋值

private:
	int									m_number;		// 点数
	sgs::ConstData::CardColor			m_color;		// 花色
	sgs::ConstData::CardType			m_type;			// 实体卡种类
	mutable sgs::ConstData::CardType	m_mask;			// 执行效果种类（如大乔国色时实际效果卡即与实体卡种类无关）

	typedef sgs::DataType::Message * (*ProcessorHandler)(sgs::DataType::Message*);
	mutable ProcessorHandler			m_processor;	// 对应的处理器函数

};

#endif /*_SGSDATA_CARD_H*/
