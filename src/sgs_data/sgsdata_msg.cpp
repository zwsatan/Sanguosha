#include "sgsdata_msg.h"
#include "sgsdata_player.h"
#include "sgsui_output.h"

using namespace sgs;
using namespace ConstData;
using namespace DataType;
using namespace Derive;

MessageType Message::type() const
{
	return m_type;
}

bool Message::dissolved() const
{
	return m_dissolved;
}

bool Message::setdissolve(bool b)
{
	m_dissolved = b;
	return m_dissolved;
}

Message::~Message()
{
}

CardMessage::CardMessage(bool use, Player * from, PlayerRegionType playerRegion, int pos, Player * to/* = 0*/)
	: Message(use ? USECARD : HITCARD)
	, m_from(from)
	, m_targets(0)
{
	m_pos.first = playerRegion;
	m_pos.second = pos;

	switch (playerRegion)
	{
	case PHAND:
		m_card = from->hand(pos);
		break;
	case PEQUIP:
		switch (pos)
		{
		case 0:
			m_card = from->weapon();
			break;
		case 1:
			m_card = from->armor();
			break;
		case 2:
			m_card = from->atkhorse();
			break;
		case 3:
			m_card = from->dfdhorse();
			break;
		}
		break;
	case PJUDGE:
		m_card = from->judge(pos);
		break;
	default:
		break;
	}

	if (to != 0)
	{
		m_to.push(to);
		m_targets++;
	}
}

CardMessage::CardMessage(CardMessage * cardMsg)
	: Message(cardMsg->type())
	, m_from(cardMsg->from())
	, m_targets(1)
{
	m_pos.first = cardMsg->pos().first;
	m_pos.second = cardMsg->pos().second;
	m_card = cardMsg->card();
	m_to.push(cardMsg->to(0));
}

CardMessage::CardMessage(bool use, Player * from, const Card * card, Player * to/* = 0*/)
	: Message(use ? USECARD : HITCARD)
	, m_from(from)
	, m_targets(0)
{
	m_pos.first = playerRegionTypeNone;
	m_pos.second = -1;
	m_card = card;
	if (to != 0)
	{
		m_to.push(to);
		m_targets++;
	}
}

SkillMessage::SkillMessage(Player * from, HeroSkill type)
	: Message(USESKILL)
	, m_skill(type)
	, m_from(from)
{
}

SkillMessage::~SkillMessage()
{
}

Player * SkillMessage::from() const
{
	return m_from;
}

HeroSkill SkillMessage::skill() const
{
	return m_skill.skill();
}

Message * SkillMessage::settle(Message * msg)
{
	return m_skill.settle(msg);
}

WuShengMessage::WuShengMessage(Player * from, PlayerRegionType playerRegion, int pos, Player * to/* = 0*/)
	: SkillMessage(from, WUSHENG)
	, m_targets(0)
{
	m_pos = std::pair<PlayerRegionType, int>(playerRegion, pos);
	m_card = from->visitCard(m_pos);
	if (to != 0)
	{
		m_to.push(to);
		m_targets++;
	}
}

WuShengMessage::~WuShengMessage()
{
}

const Card * WuShengMessage::card() const
{
	return m_card;
}

Player * WuShengMessage::to(int pos) const
{
	return m_to.at(pos + 1);
}

int WuShengMessage::targets() const
{
	return m_targets;
}

std::pair<PlayerRegionType, int> WuShengMessage::pos() const
{
	return m_pos;
}

void WuShengMessage::addto(Player * to)
{
	if (to != 0)
	{
		m_to.push(to);
		m_targets++;
	}
}

void WuShengMessage::popto(int pos)
{
	if (0 <= pos && pos < m_targets)
	{
		m_to.pop(pos + 1);
		m_targets--;
	}
}

GangLieMessage::GangLieMessage(Player * from, Player * to)
	: SkillMessage(from, GANGLIE)
	, m_to(to)
{
}

GangLieMessage::~GangLieMessage()
{
}

Player * GangLieMessage::to() const
{
	return m_to;
}

QingGuoMeesage::QingGuoMeesage(Player * from, int pos)
	: SkillMessage(from, QINGGUO)
	, m_pos(pos)
{
	if (pos >= 0 && pos < from->handnum())
	{
		m_card = from->hand(pos);
	}
	else
	{
		m_card = 0;
		m_pos = -1;
	}
}

QingGuoMeesage::~QingGuoMeesage()
{
}

const Card * QingGuoMeesage::card() const
{
	return m_card;
}

int QingGuoMeesage::pos() const
{
	return m_pos;
}

LongDanMessage::LongDanMessage(Player * from, int pos, Player * to)
	: SkillMessage(from, LONGDAN)
	, m_targets(0)
	, m_pos(pos)
{
	m_card = from->hand(pos);
	if (to != 0)
	{
		m_to.push(to);
		m_targets++;
	}
}

LongDanMessage::~LongDanMessage()
{
}

const Card * LongDanMessage::card() const
{
	return m_card;
}

Player * LongDanMessage::to(int pos) const
{
	return m_to.at(pos + 1);
}

int LongDanMessage::targets() const
{
	return m_targets;
}

int LongDanMessage::pos() const
{
	return m_pos;
}

void LongDanMessage::addto(Player * to)
{
	if (to != 0)
	{
		m_to.push(to);
		m_targets++;
	}
}
void LongDanMessage::popto(int pos)
{
	if (0 <= pos && pos < m_targets)
	{
		m_to.pop(pos + 1);
		m_targets--;
	}
}

QiXiMessage::QiXiMessage(Player * from, PlayerRegionType prt, int pos, Player * to)
	: SkillMessage(from, QIXI)
	, m_to(to)
{
	m_pos = std::pair<PlayerRegionType, int>(prt, pos);
	m_card = from->visitCard(m_pos);
}

QiXiMessage::~QiXiMessage()
{
}

FanKuiMessage::FanKuiMessage(Player * from, Player * to)
	: SkillMessage(from, FANKUI)
	, m_to(to)
{
}

FanKuiMessage::~FanKuiMessage()
{
}

Player * FanKuiMessage::to() const
{
	return m_to;
}

const Card * QiXiMessage::card() const
{
	return m_card;
}

Player * QiXiMessage::to() const
{
	return m_to;
}

std::pair<PlayerRegionType, int> QiXiMessage::pos() const
{
	return m_pos;
}

QingNangMessage::QingNangMessage(Player * from, int pos, Player * to)
	: SkillMessage(from, QINGNANG)
	, m_pos(pos)
	, m_to(to)
{
	m_card = from->hand(pos);
}

QingNangMessage::~QingNangMessage()
{
}

const Card * QingNangMessage::card() const
{
	return m_card;
}

Player * QingNangMessage::to() const
{
	return m_to;
}

int QingNangMessage::pos() const
{
	return m_pos;
}

JiJiuMessage::JiJiuMessage(Player * from, PlayerRegionType playerRegion, int pos)
	: SkillMessage(from, JIJIU)
{
	m_pos = std::pair<PlayerRegionType, int>(playerRegion, pos);
	m_card = from->visitCard(m_pos);
}

JiJiuMessage::~JiJiuMessage()
{
}

const Card * JiJiuMessage::card() const
{
	return m_card;
}

std::pair<sgs::ConstData::PlayerRegionType, int> JiJiuMessage::pos() const
{
	return m_pos;
}

ZhangBaMessage::ZhangBaMessage(Player * from, Player * to)
	: CardMessage(true, from, PEQUIP, 0, to)
{
	m_cost[0].first = 0;
	m_cost[0].second = -1;
	m_cost[1].first = 0;
	m_cost[1].second = -1;
}

ZhangBaMessage::~ZhangBaMessage()
{
}

std::pair<const Card *, int> ZhangBaMessage::cost(int pos) const
{
	if (pos < 0 || pos > 2)
		return std::pair<const Card *, int>((const Card *) 0, -1);
	return m_cost[pos];
}
bool ZhangBaMessage::addcost(int pos)
{
	if (pos < 0 || pos > from()->handnum())
	{
		printDebug("MZhangBa::addcost: condition 1");
		return false;
	}
	else if (m_cost[0].first && m_cost[1].first)
	{
		printDebug("MZhangBa::addcost: condition 2");
		return false;
	}
	else if (m_cost[0].first)
	{
		printDebug("MZhangBa::addcost: condition 3");
		m_cost[1].second = pos;
		m_cost[1].first = from()->hand(pos);
		return true;
	}
	else
	{
		printDebug("MZhangBa::addcost: condition 4");
		m_cost[0].second = pos;
		m_cost[0].first = from()->hand(pos);
		return true;
	}
}

Player * CardMessage::from() const
{
	return m_from;
}

Player * CardMessage::to(int pos) const
{
	if (pos < 0 || pos >= m_targets)
		return 0;
	return m_to.at(pos + 1);
}

const Card * CardMessage::card() const
{
	return m_card;
}

int CardMessage::targets() const
{
	return m_targets;
}

bool CardMessage::addto(Player * to)
{
	if (to && m_to.push(to))
	{
		++m_targets;
		return true;
	}

	return false;
}

bool CardMessage::popto(int pos)
{
	if (pos < 0 || pos >= m_to.size())
		return false;

	if (m_to.pop(pos + 1))
	{
		--m_targets;
		return true;
	}

	return false;
}

std::pair<PlayerRegionType, int> CardMessage::pos() const
{
	return m_pos;
}

CardMessage::~CardMessage()
{
	m_to.clear();
}

TransCardMessage::TransCardMessage(
		RegionType fromRegion,
		RegionType toRegion,
		bool open,
		Player * from/* = 0*/,
		PlayerRegionType playerRegion/* = playerRegionTypeNone*/,
		Player * to/* = 0*/,
		bool tojudge/* = false*/)
	: Message(TRANSCARD)
	, m_fromtype(fromRegion)
	, m_totype(toRegion)
	, m_open(open)
	, m_from(from)
	, m_to(to)
	, m_frompos(playerRegion)
	, m_cards(0)
	, m_toJudge(tojudge)
{
}

TransCardMessage::~TransCardMessage()
{
	m_trans.clear();
	m_pos.clear();
}

RegionType TransCardMessage::fromtype() const
{
	return m_fromtype;
}

RegionType TransCardMessage::totype() const
{
	return m_totype;
}

Player * TransCardMessage::from() const
{
	return m_from;
}

Player * TransCardMessage::to() const
{
	return m_to;
}

PlayerRegionType TransCardMessage::frompos() const
{
	return m_frompos;
}

int TransCardMessage::cards() const
{
	return m_cards;
}

const Card * TransCardMessage::trans(int pos) const
{
	if (pos < 0 || pos >= m_cards)
		return 0;
	return m_trans.at(pos + 1);
}

int TransCardMessage::pos(int p) const
{
	if (p < 0 || p >= m_cards)
		return 0;
	return m_pos.at(p + 1);
}

bool TransCardMessage::open() const
{
	return m_open;
}

bool TransCardMessage::tojudge() const
{
	return m_toJudge;
}

bool TransCardMessage::transpush(const sgs::DataType::Card * c)
{
	if (c != 0 && m_trans.push(c))
	{
		++m_cards;
		return true;
	}

	return false;
}

bool TransCardMessage::transpush(int fp)
{
	switch (m_frompos)
	{
	case PHAND:
		if (fp < 0 || fp >= m_from->handnum())
			return false;
		m_pos.push(fp);
		m_trans.push(m_from->hand(fp));
		++m_cards;
		return true;

	case PEQUIP:
		switch (fp)
		{
		case 0:
			m_pos.push(fp);
			m_trans.push(m_from->weapon());
			++m_cards;
			return true;

		case 1:
			m_pos.push(fp);
			m_trans.push(m_from->armor());
			m_cards++;
			return true;

		case 2:
			m_pos.push(fp);
			m_trans.push(m_from->atkhorse());
			m_cards++;
			return true;

		case 3:
			m_pos.push(fp);
			m_trans.push(m_from->dfdhorse());
			m_cards++;
			return true;

		default:
			return false;
		}

	case PJUDGE:
		if (fp < 0 || fp >= m_from->judgenum())
			return false;
		m_pos.push(fp);
		m_trans.push(m_from->judge(fp));
		m_cards++;
		return true;

	default:
		return false;
	}
}

bool TransCardMessage::transpush(int fromPos, const Card * card)
{
	if (card == 0)
		return false;

	m_pos.push(fromPos);
	m_trans.push(card);
	++m_cards;
	return true;
}

bool TransCardMessage::transpop(int pos)
{
	bool isValid = 0 <= pos && pos < m_trans.size();
	if (isValid && m_trans.pop(pos + 1))
	{
		--m_cards;
		if (m_fromtype == PLAYER)
			m_pos.pop(pos + 1);
		return true;
	}

	return false;
}

SwitchPhaseMessage::SwitchPhaseMessage(Player * from, PlayerPhase before, PlayerPhase after)
	: Message(SWITCHPHASE)
	, m_from(from)
	, m_before(before)
	, m_after(after)
{
}

Player * SwitchPhaseMessage::from() const
{
	return m_from;
}

PlayerPhase SwitchPhaseMessage::before() const
{
	return m_before;
}

PlayerPhase SwitchPhaseMessage::after() const
{
	return m_after;
}

SwitchPhaseMessage::~SwitchPhaseMessage()
{
}

HurtMessage::HurtMessage(Player * from, Player * to, int hurt)
	: Message(HURT)
	, m_from(from)
	, m_to(to)
	, m_hurt(hurt)
{
}

HurtMessage::~HurtMessage()
{
}

Player * HurtMessage::from() const
{
	return m_from;
}

Player * HurtMessage::to() const
{
	return m_to;
}

int HurtMessage::hurt() const
{
	return m_hurt;
}

JudgeMessage::JudgeMessage(Player * from, const Card * card, const Card * result, bool effect)
	: Message(JUDGE)
	, m_from(from)
	, m_card(card)
	, m_result(result)
	, m_effect(effect)
	, m_skill(heroSkillNone)
	, m_cardJudge(true)
{
}

JudgeMessage::JudgeMessage(Player * from, HeroSkill skill, const Card * result, bool effect)
	: Message(JUDGE)
	, m_from(from)
	, m_card(0)
	, m_result(result)
	, m_effect(effect)
	, m_skill(skill)
	, m_cardJudge(false)
{
}

JudgeMessage::~JudgeMessage()
{
}

Player * JudgeMessage::from() const
{
	return m_from;
}

const Card * JudgeMessage::card() const
{
	return m_card;
}

const Card * JudgeMessage::result() const
{
	return m_result;
}

bool JudgeMessage::effect() const
{
	return m_effect;
}

bool JudgeMessage::cardJudge() const
{
	return m_cardJudge;
}

HeroSkill JudgeMessage::skill() const
{
	return m_skill;
}

DyingMessage::DyingMessage(Player * from, Player * murderer, int tao)
	: Message(DYING)
	, m_from(from)
	, m_murderer(murderer)
	, m_needTao(tao)
{
}

DyingMessage::~DyingMessage()
{
}

Player * DyingMessage::from() const
{
	return m_from;
}

Player * DyingMessage::murderer() const
{
	return m_murderer;
}

int DyingMessage::neg() const
{
	return m_needTao;
}

KillMessage::KillMessage(Player * from, Player * to)
	: Message(KILL)
	, m_from(from)
	, m_die(to)
{
}

KillMessage::~KillMessage()
{
}

Player * KillMessage::from() const
{
	return m_from;
}

Player * KillMessage::to() const
{
	return m_die;
}

RenDeMessage::RenDeMessage(Player * from, Player * to)
	: SkillMessage(from, RENDE)
	, m_to(to)
	, m_cards(0)
{
}

RenDeMessage::~RenDeMessage()
{
}

const Card * RenDeMessage::card(int pos) const
{
	return m_card[pos].second;
}

int RenDeMessage::pos(int index) const
{
	return m_card[index].first;
}

Player * RenDeMessage::to() const
{
	return m_to;
}

int RenDeMessage::cards() const
{
	return m_cards;
}

void RenDeMessage::transpush(int pos)
{
	m_card.push_back(std::pair<int, const Card *>(pos, m_from->hand(pos)));
	m_cards++;
}

JiJiangMessage::JiJiangMessage(Player * from, Player * to)
	: SkillMessage(from, JIJIANG)
	, m_targets(0)
{
	if (to != 0)
	{
		m_to.push(to);
		m_targets++;
	}
}

JiJiangMessage::~JiJiangMessage()
{
	m_to.clear();
}

int JiJiangMessage::targets() const
{
	return m_targets;
}

void JiJiangMessage::addto(Player * to)
{
	if (to != 0)
	{
		m_to.push(to);
		m_targets++;
	}
}

void JiJiangMessage::popto(int i)
{
	if (i < 0 || i >= m_targets)
		return;

	m_to.pop(i + 1);
	--m_targets;
}

Player * JiJiangMessage::to(int pos) const
{
	return m_to.at(pos + 1);
}

LiJianMessage::LiJianMessage(Player * from, PlayerRegionType playerRegion, int pos, Player * first, Player * second)
	: SkillMessage(from, LIJIAN)
	, m_toFirst(first)
	, m_toSecond(second)
{
	m_pos = std::pair<PlayerRegionType, int>(playerRegion, pos);
	m_card = from->visitCard(m_pos);
}

LiJianMessage::~LiJianMessage()
{
}

Player * LiJianMessage::first() const
{
	return m_toFirst;
}

Player * LiJianMessage::second() const
{
	return m_toSecond;
}

std::pair<PlayerRegionType, int> LiJianMessage::pos() const
{
	return m_pos;
}

const Card * LiJianMessage::card() const
{
	return m_card;
}

TuXiMessage::TuXiMessage(Player * from, Player * target1, int pos1, Player * target2, int pos2)
	: SkillMessage(from, TUXI)
	, m_targets(0)
{
	if (target1 != 0)
	{
		m_pos[0] = std::pair<Player *, int>(target1, pos1);
		m_card[0] = target1->hand(pos1);
		++m_targets;
	}

	if (target2 != 0)
	{
		m_pos[1] = std::pair<Player *, int>(target2, pos2);
		m_card[1] = target2->hand(pos2);
		++m_targets;
	}
}

TuXiMessage::~TuXiMessage()
{
}

std::pair<Player *, int> TuXiMessage::pos(int index) const
{
	return m_pos[index];
}

const Card * TuXiMessage::card(int index) const
{
	return m_card[index];
}

int TuXiMessage::targets() const
{
	return m_targets;
}

YiJiMessage::YiJiMessage(Player * from, Player * target1, const Card * card1, Player * target2, const Card * card2)
	: SkillMessage(from, YIJI)
{
	 m_trans[0] = std::pair<Player *, const Card *>(target1, card1);
	 m_trans[1] = std::pair<Player *, const Card *>(target2, card2);
}

YiJiMessage::~YiJiMessage()
{
}

std::pair<Player *, const Card *> YiJiMessage::trans(int pos) const
{
	return  m_trans[pos];
}

ZhiHengMessage::ZhiHengMessage(Player * from)
	: SkillMessage(from, ZHIHENG), m_cards(0)
{
}

ZhiHengMessage::~ZhiHengMessage()
{
}

void ZhiHengMessage::transpush(PlayerRegionType playerRegion, int pos)
{
	m_pos.push_back(std::pair<PlayerRegionType, int>(playerRegion, pos));
	m_card.push_back(from()->visitCard(std::pair<PlayerRegionType, int>(playerRegion, pos)));
	m_cards++;
}

std::pair<PlayerRegionType, int> ZhiHengMessage::pos(int index) const
{
	return m_pos[index];
}

const Card * ZhiHengMessage::card(int index) const
{
	return m_card[index];
}

int ZhiHengMessage::cards() const
{
	return m_cards;
}

FanJianMessage::FanJianMessage(Player * from, Player * to)
	: SkillMessage(from, FANJIAN)
	, m_to(to)
{
}

FanJianMessage::~FanJianMessage()
{
}

Player * FanJianMessage::to() const
{
	return m_to;
}

JieYinMessage::JieYinMessage(Player * from, Player * to, int pos1, int pos2)
	: SkillMessage(from, JIEYIN)
	, m_to(to)
{
	m_pos[0] = pos1;
	m_card[0] = from->hand(pos1);
	m_pos[1] = pos2;
	m_card[1] = from->hand(pos2);
}

JieYinMessage::~JieYinMessage()
{
}

Player * JieYinMessage::to() const
{
	return m_to;
}

int JieYinMessage::pos(int i) const
{
	return m_pos[i];
}

const Card * JieYinMessage::card(int i) const
{
	return m_card[i];
}

GuoSeMessage::GuoSeMessage(Player * from, PlayerRegionType prt, int pos, Player * to)
	: SkillMessage(from, GUOSE)
	, m_to(to)
{
	m_pos = std::pair<PlayerRegionType, int>(prt, pos);
	m_card = from->visitCard(m_pos);
}

GuoSeMessage::~GuoSeMessage()
{
}

Player * GuoSeMessage::to() const
{
	return m_to;
}

std::pair<sgs::ConstData::PlayerRegionType, int> GuoSeMessage::pos() const
{
	return m_pos;
}

const sgs::DataType::Card * GuoSeMessage::card() const
{
	return m_card;
}

LiuLiMessage::LiuLiMessage(Player * from, PlayerRegionType playerRegion, int pos, Player * to) :
		SkillMessage(from, LIULI)
	  , m_to(to)
{
	m_pos = std::pair<PlayerRegionType, int>(playerRegion, pos);
	m_card = from->visitCard(m_pos);
}

LiuLiMessage::~LiuLiMessage()
{
}

Player * LiuLiMessage::to() const
{
	return m_to;
}

std::pair<sgs::ConstData::PlayerRegionType, int> LiuLiMessage::pos() const
{
	return m_pos;
}

const sgs::DataType::Card * LiuLiMessage::card() const
{
	return m_card;
}
