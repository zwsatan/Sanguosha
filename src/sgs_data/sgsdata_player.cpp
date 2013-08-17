#include "sgsdata_extern.h"
#include "sgsdata_player.h"
#include "sgs_Interface.h"
#include "sgsdata_hero.h"
#include "sgsdata_heroes.h"
#include "sgsui_global.h"
#include "sgsdata_card.h"
#include "sgsdata_card_func.h"
#include "sgsui_GUIStaticData.h"
#include "sgsdata_history.h"
#include "sgsdata_platform.h"

using namespace sgs;
using namespace ConstData;
using namespace DataType;
using namespace Derive;
using namespace ExternData;

Player::Player(int seat, PlayerRole role)
	: m_seat(seat)
	, m_role(role)
{
	m_status = 0;
	m_shaCount = 0;
	m_phase = OTHERPHASE;
	m_equip.armor = 0;
	m_equip.atkhorse = 0;
	m_equip.dfdhorse = 0;
	m_equip.weapon = 0;
}

Player::~Player()
{
	m_hand.clear();
	m_judge.clear();
}

void Player::herotrans(HeroType heroType)
{
	switch (heroType)
	{
	case SUJIANG:
		m_hero = &ExternData::hsujiang;
		break;
	case NVSUJIANG:
		m_hero = &ExternData::hnvsujiang;
		break;
	case ZHANGFEI:
		m_hero = &ExternData::hzhangfei;
		break;
	case LVMENG:
		m_hero = &ExternData::hlvmeng;
		break;
	case MACHAO:
		m_hero = &ExternData::hmachao;
		break;
	case GUANYU:
		m_hero = &ExternData::hguanyu;
		break;
	case XIAHOUDUN:
		m_hero = &ExternData::hxiahoudun;
		break;
	case XUCHU:
		m_hero = &ExternData::hxuchu;
		break;
	case ZHENJI:
		m_hero = &ExternData::hzhenji;
		break;
	case LAOZHUGE:
		m_hero = &ExternData::hlaozhuge;
		break;
	case ZHAOYUN:
		m_hero = &ExternData::hzhaoyun;
		break;
	case GANNING:
		m_hero = &ExternData::hganning;
		break;
	case SIMAYI:
		m_hero = &ExternData::hsimayi;
		break;
	case HUANGGAI:
		m_hero = &ExternData::hhuanggai;
		break;
	case HUATUO:
		m_hero = &ExternData::hhuatuo;
		break;
	case LIUBEI:
		m_hero = &ExternData::hliubei;
		break;
	case CAOCAO:
		m_hero = &ExternData::hcaocao;
		break;
	case HUANGYUEYING:
		m_hero = &ExternData::hhuangyueying;
		break;
	case DIAOCHAN:
		m_hero = &ExternData::hdiaochan;
		break;
	case ZHANGLIAO:
		m_hero = &ExternData::hzhangliao;
		break;
	case GUOJIA:
		m_hero = &ExternData::hguojia;
		break;
	case SUNQUAN:
		m_hero = &ExternData::hsunquan;
		break;
	case LVBU:
		m_hero = &ExternData::hlvbu;
		break;
	case ZHOUYU:
		m_hero = &ExternData::hzhouyu;
		break;
	case SUNSHANGXIANG:
		m_hero = &ExternData::hsunshangxiang;
		break;
	case LUXUN:
		m_hero = &ExternData::hluxun;
		break;
	case DAQIAO:
		m_hero = &ExternData::hdaqiao;
		break;
	default:
		m_hero = 0;
		break;
	}
}

int Player::maxHP() const
{
	// 主公多加1滴血
	int zhuGongHp = (m_role == ZHU && sgsui::GUIStaticData::getPlayerCount() >= 4);
	return m_hero->maxHP() + zhuGongHp;
}

HeroType Player::type() const
{
	return m_hero->type();
}

HeroSex Player::sex() const
{
	return m_hero->sex();
}

HeroSide Player::side() const
{
	return m_hero->side();
}

int Player::skillnum() const
{
	return m_hero->skillnum();
}

Skill Player::skill(int pos)
{
	return m_hero->skill(pos);
}

Interface * Player::input() const
{
	return  m_input;
}

Player * Player::next() const
{
	return m_next;
}

Player * Player::prev() const
{
	return m_prev;
}

Player * Player::after() const
{
	Player * aft = m_next;
	while (aft->m_status & DEAD)
	{
		aft = aft->m_next;
		if (aft == this)
			return 0;
	}
	return aft;
}

Player * Player::before() const
{
	Player * bef = m_prev;
	while (bef->m_status & DEAD)
	{
		bef = bef->m_prev;
		if (bef == this)
			return 0;
	}
	return bef;
}

Player * Player::find(int n) const
{
	if (n < 0)
		return 0;
	else if (n == m_seat)
		return this->m_next->m_prev;
	else
	{
		Player * find = this->m_next->m_prev;
		for (int i = 0; find->m_seat != n && i < 10; i++)
			find = find->m_next;

		if (find->m_seat == n)
			return find;
		else
			return 0;
	}
}
int Player::distance(const Player * player) const
{
	if (player == NULL)
		return -1;

	if (player == this)
		return 0;

	int n = 1, m = 1;
	for (Player * start = m_next; start != player; start = start->m_next)
	{
		// 计算顺时针与对方的距离
		if (!(start->m_status & DEAD))
			n++;
	}

	for (Player * start = m_prev; start != player; start = start->m_prev)
	{
		// 计算逆时针与对方的距离
		if (!(start->m_status & DEAD))
			m++;
	}

	n = (n > m) ? m : n;

	// 自己攻击马,距离-1
	if (m_equip.atkhorse != 0)
		n--;

	// 对方防御马,距离+1
	if (player->m_equip.dfdhorse != 0)
		n++;

	// 马超,且距离大于0,距离减1
	if (type() == MACHAO && n > 0)
		n--;

	return n;
}

// 返回武器攻击范围
int Player::range() const
{
	if (m_equip.weapon == 0)
		return 1;

	switch (m_equip.weapon->mask())
	{
	case cardTypeNone:
		return 1;
	case ZHUGE:
		return 1;
	case QINGGANG:
		return 2;
	case HANBING:
		return 2;
	case CIXIONG:
		return 2;
	case GUANSHI:
		return 3;
	case YINYUE:
		return 3;
	case QINGLONG:
		return 3;
	case GUDING:
		return 2;
	case ZHANGBA:
		return 3;
	case FANGTIAN:
		return 4;
	case QILIN:
		return 5;
	case ZHUQUE:
		return 4;
	default:
		return 1;
	}
}

int Player::seat() const
{
	return m_seat;
}

PlayerRole Player::role() const
{
	return m_role;
}

PlayerPhase Player::phase() const
{
	return m_phase;
}

int Player::hp() const
{
	return m_hp;
}

char Player::status() const
{
	return m_status;
}

int Player::handnum()
{
	return m_hand.size();
}

const Card * Player::hand(int pos)
{
	if (pos < 0 || pos >= m_hand.size())
		return 0;
	return m_hand.at(pos + 1);
}

int Player::judgenum()
{
	return m_judge.size();
}

const Card * Player::judge(int pos)
{
	if (pos < 0 || pos >= m_judge.size())
		return 0;
	return m_judge.at(pos + 1);
}

const Card * Player::atkhorse() const
{
	return m_equip.atkhorse;
}

const Card * Player::dfdhorse() const
{
	return m_equip.dfdhorse;
}

const Card * Player::weapon() const
{
	return m_equip.weapon;
}

const Card * Player::armor() const
{
	return m_equip.armor;
}

const Card * Player::visitCard(std::pair<PlayerRegionType, int> pos)
{
	switch (pos.first)
	{
	case PHAND:
		return hand(pos.second);

	case PEQUIP:
		switch (pos.second)
		{
		case 0:
			return m_equip.weapon;
		case 1:
			return m_equip.armor;
		case 2:
			return m_equip.atkhorse;
		case 3:
			return m_equip.dfdhorse;
		default:
			return 0;
		}
		break;

	case PJUDGE:
		return judge(pos.second);

	default:
		return 0;
	}
}

Message * Player::round(Platform&)
{
	Message * m = 0;
	switch (m_phase)
	{
	case OTHERPHASE:
		m = new SwitchPhaseMessage(this, OTHERPHASE, STARTPHASE);
		m_phase = STARTPHASE;
		break;

	case STARTPHASE:
		m = m_hero->startPhase(this);
		break;

	case JUDGEMENTPHASE:
		m = m_hero->judgePhase(this);
		break;

	case GETCARDPHASE:
		m = m_hero->getPhase(this);
		break;

	case MAINPHASE:
		m = m_hero->mainPhase(this);
		break;

	case ABANDONCARDPHASE:
		m = m_hero->abandonPhase(this);
		break;

	case ENDPHASE:
		m = m_hero->endPhase(this);
		break;

	default:
		break;
	}
	return m;
}

char& Player::setstatus()
{
	return m_status;
}

int Player::shaCount() const
{
	return m_shaCount;
}

void Player::addShaCount()
{
	++m_shaCount;
}

void Player::clearShaCount()
{
	m_shaCount = 0;
}

void Player::setphase(PlayerPhase phase)
{
	m_phase = phase;
}

int& Player::sethp()
{
	return m_hp;
}

chain<const sgs::DataType::Card *>& Player::setjudge()
{
	return m_judge;
}

void Player::setatk(const sgs::DataType::Card * card)
{
	m_equip.atkhorse = card;
}

void Player::setdfd(const sgs::DataType::Card * card)
{
	m_equip.dfdhorse = card;
}

void Player::setweapon(const sgs::DataType::Card * card)
{
	m_equip.weapon = card;
}

void Player::setarmor(const sgs::DataType::Card * card)
{
	m_equip.armor = card;
}

void Player::choosehero(std::vector<HeroType> &herovec, HeroType lord/* = sgs::ConstData::heroTypeNone*/)
{
	if (lord == sgs::ConstData::heroTypeNone)
	{
		// 主公选将,这里逆序是因为,最后默认三个为刘备,曹操,孙权
		HeroType avaliable[5];
		std::vector<HeroType>::reverse_iterator riter = herovec.rbegin();
		for (int i = 0; i < 5; ++i)
			avaliable[i] = *riter++;

		HeroType choosenHero;
		if (m_seat == 0)
		{
			choosenHero = sgsui::lordChoose(avaliable[0], avaliable[1], avaliable[2], avaliable[3], avaliable[4]);
			herotrans(choosenHero);
		}
		else
		{
			choosenHero = avaliable[rand() % 5];
			herotrans(choosenHero);
		}

		// 将已选武将移除选将列表
		for (int i = 0; i < 5; ++i)
			herovec.pop_back();

		// 将未选的4个武将重新加入武将列表
		for (int i = 0; i < 5; ++i)
		{
			if (avaliable[i] != choosenHero)
				herovec.push_back(avaliable[i]);
		}

		// 主公生命值上限+1
		m_hp = m_hero->maxHP() + 1;
	}
	else
	{
		if (m_seat == 0)
			herotrans(sgsui::heroChoose(lord, m_role, *(herovec.rbegin() + 0), *(herovec.rbegin() + 1), *(herovec.rbegin() + 2)));
		else
			herotrans(*(herovec.rbegin() + rand() % 3));

		for (int i = 0; i < 3; ++i)
			herovec.pop_back();

		m_hp = m_hero->maxHP();
	}
}

void Player::popCard(std::pair<PlayerRegionType, int> pos)
{
	switch (pos.first)
	{
	case PHAND:
		m_hand.pop(pos.second + 1);
		if (type() == LUXUN && !(status() & DEAD) && m_hand.empty() && input()->useSkillOrNot(LIANYING))
		{
			// 处理陆逊手牌为空，且使用连营的情况
			SkillMessage * lianying = new SkillMessage(this, LIANYING);
			ExternData::sgsout << lianying;
			ExternData::history.push(lianying);

			TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER, m_seat == 0, 0, playerRegionTypeNone, this);
			trans->transpush(ExternData::platform.get());
			ExternData::platform.analyze(trans);
		}
		break;

	case PEQUIP:
		switch (pos.second)
		{
		case 0:
			m_equip.weapon = 0;
			break;
		case 1:
			m_equip.armor = 0;
			break;
		case 2:
			m_equip.atkhorse = 0;
			break;
		case 3:
			m_equip.dfdhorse = 0;
			break;
		default:
			return;
		}
		break;

	case PJUDGE:
		m_judge.pop(pos.second + 1);
		break;

	default:
		return;
	}
}

void Player::setinput(sgs::Interface * input)
{
	m_input = input;
}

void Player::pushHand(const Card * card)
{
	card->setmask();
	m_hand.push(card);
}

Message * Player::ShanOrNot(Message * msg)
{
	CardMessage * cardMsg = static_cast<CardMessage *>(msg);
	Message *response =  m_input->shanOrNot(cardMsg->from(), cardMsg->card());
	if (response == 0)
		return 0;

	CardMessage * res = 0;
	switch (response->type())
	{
	case USESKILL:
		response = static_cast<SkillMessage *>(response)->settle(response);
		if (response == 0)
			return 0;
		res = static_cast<CardMessage *>(response);
		break;

	default:
		res = static_cast<CardMessage *>(response);
		res->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, res->pos().second));
		break;
	}
	return res;
}

Message * Player::TaoOrNot(Message * msg)
{
	DyingMessage * dyingMsg = static_cast<DyingMessage *>(msg);
	Message *response = static_cast<CardMessage *>(m_input->taoOrNot(dyingMsg->from(), dyingMsg->neg()));
	if (response == 0)
		return 0;

	CardMessage * res = 0;
	switch (response->type())
	{
	case USESKILL:
		response = static_cast<SkillMessage *>(response)->settle(response);
		res = static_cast<CardMessage *>(response);
		res->addto(dyingMsg->from());
		break;

	default:
		res = static_cast<CardMessage *>(response);
		res->from()->popCard(
					std::pair<PlayerRegionType, int>(PHAND, res->pos().second));
		break;
	}
	return res;
}

Message * Player::ShaOrNot(Message * msg)
{
	if (msg->type() == USESKILL)
	{
		LiJianMessage * liJianMsg = static_cast<LiJianMessage *>(msg);
		Player * target = (this == liJianMsg->first()) ? liJianMsg->second() : liJianMsg->first();
		Message * response =  m_input->shaOrNot(target, LIJIAN);
		if (response == 0)
			return 0;

		MessageType type = response->type();
		if (type == USESKILL)
		{
			response = static_cast<SkillMessage *>(response)->settle(response);
			if (response == 0)
				return 0;

			CardMessage * res = static_cast<CardMessage *>(response);
			while (res->targets())
				res->popto();
			res->addto(target);
		}
		else if (type == HITCARD || type == USECARD)
		{
			CardMessage * res = static_cast<CardMessage *>(response);
			if (res->card()->type() == ZHANGBA)
			{
				ZhangBaMessage * zhangba = new ZhangBaMessage(res->from(), 0);
				delete res;
				res = func::pczhangba(zhangba);
			}
			else
			{
				res->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, res->pos().second));
			}
			while (res->targets())
				res->popto();
			res->addto(target);
		}

		return response;
	}
	else
	{
		CardMessage * cardMsg = static_cast<CardMessage *>(msg);
		Message * response =  m_input->shaOrNot(cardMsg->from(), cardMsg->card());
		if (response == 0)
			return 0;

		MessageType type = response->type();
		if (type == USESKILL)
		{
			response = static_cast<SkillMessage *>(response)->settle(response);
			if (response == 0)
				return 0;

			CardMessage * res = static_cast<CardMessage *>(response);
			while (res->targets())
				res->popto();

			switch (cardMsg->card()->mask())
			{
			case SHA:
				res->addto(cardMsg->to());
				break;
			case NANMAN:
				break;
			case JUEDOU:
				if (cardMsg->from() == this)
					res->addto(cardMsg->to());
				else
					res->addto(cardMsg->from());
				break;

			case JIEDAO:
				res->addto(cardMsg->to(1));
				break;

			default:
				break;
			}
		}
		else if (type == HITCARD || type == USECARD)
		{
			CardMessage * res = static_cast<CardMessage *>(response);
			if (res->card()->type() == ZHANGBA)
			{
				ZhangBaMessage * zhangba = new ZhangBaMessage(res->from(), 0);
				delete res;
				res = func::pczhangba(zhangba);
			}
			else
			{
				res->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, res->pos().second));
			}

			while (res->targets())
				res->popto();

			switch (cardMsg->card()->type())
			{
			case SHA:
				res->addto(cardMsg->to());
				break;
			case NANMAN:
				break;
			case JUEDOU:
				if (cardMsg->from() == this)
					res->addto(cardMsg->to());
				else
					res->addto(cardMsg->from());
				break;
			case JIEDAO:
				res->addto(cardMsg->to(1));
				break;
			default:
				break;
			}
		}

		return response;
	}
}

Message * Player::WuXieOrNot(Message * m, int times)
{
	CardMessage * res = static_cast<CardMessage *>(m_input->wuxieOrNot(m, times));
	if (res == 0)
		return 0;

	popCard(res->pos());
	res->addto(this);
	return res;
}
