#include "sgsdata_card.h"
#include "sgsdata_card_func.h"

using namespace sgs;
using namespace ConstData;
using namespace DataType;

Card::Card()
	: m_number(0)
	, m_color(cardColorNone)
	, m_type(cardTypeNone)
	, m_mask(cardTypeNone)
	, m_processor(0)
{
}

Card::Card(CardColor cardColor, int cardNum, CardType cardType)
	: m_number(cardNum)
	, m_color(cardColor)
	, m_type(cardType)
	, m_mask(cardType)
{
	protrans(cardType);
}

Card::~Card()
{
}

CardColor Card::color() const
{
	return m_color;
}

int Card::number() const
{
	return m_number;
}

CardType Card::type() const
{
	return m_type;
}

CardType Card::mask() const
{
	return m_mask;
}

Message* Card::settle(Message * msg) const
{
	return (*m_processor)(msg);
}

void Card::setmask(CardType mask) const
{
	m_mask = mask;
	protrans(mask);
}

void Card::setmask() const
{
	m_mask = m_type;
	protrans(m_type);
}

void Card::protrans(CardType cardType) const
{
	switch (cardType)
	{
	case SHA:
		m_processor = func::pcsha;
		break;

	case SHAN:
		m_processor = func::pcshan;
		break;

	case TAO:
		m_processor = func::pctao;
		break;

	case CHITU:
	case DAWAN:
	case ZICI:
		m_processor = func::pcatk;
		break;

	case DILU:
	case JUEYING:
	case ZHUAHUANG:
	case HUALIU:
		m_processor = func::pcdfd;
		break;

	case ZHUGE:
	case QINGGANG:
	case HANBING:
	case CIXIONG:
	case GUANSHI:
	case YINYUE:
	case QINGLONG:
	case GUDING:
	case ZHANGBA:
	case FANGTIAN:
	case QILIN:
	case ZHUQUE:
		m_processor = func::pcweapon;
		break;

	case RENWANG:
	case BAGUA:
		m_processor = func::pcarmor;
		break;

	case SHUNQIAN:
		m_processor = func::pcshunqian;
		break;

	case GUOCHAI:
		m_processor = func::pcguochai;
		break;

	case WUZHONG:
		m_processor = func::pcwuzhong;
		break;

	case JUEDOU:
		m_processor = func::pcjuedou;
		break;

	case TAOYUAN:
		m_processor = func::pctaoyuan;
		break;
	case WANJIAN:
		m_processor = func::pcwanjian;
		break;

	case NANMAN:
		m_processor = func::pcnanman;
		break;

	case SISHU:
	case SHANDIAN:
		m_processor = func::pcyanshi;
		break;

	case WUGU:
		m_processor = func::pcwugu;
		break;

	case JIEDAO:
		m_processor = func::pcjiedao;
		break;

	default:
		break;
	}
}
