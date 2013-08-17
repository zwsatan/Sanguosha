#ifndef _SGSDATA_MSG_H
#define _SGSDATA_MSG_H
#include <vector>
#include <utility>
#include "chain.h"
#include "sgsdata_enum.h"
#include "sgsdata_namespace.h"
#include "sgsdata_skill.h"

// 基类，不用于生成对象
class sgs::DataType::Message
{
public:
	Message(sgs::ConstData::MessageType t)
		: m_type(t)
		, m_dissolved(false)
	{}

	// 访问类型
	sgs::ConstData::MessageType type() const;
	// 调查是否是拆分过的消息
	bool dissolved() const;
	// 修改拆分状况
	bool setdissolve(bool);
	virtual ~Message();

private:
	sgs::ConstData::MessageType		m_type;				// 消息类型
	bool							m_dissolved;		// 是否是拆分得到的消息（如南蛮入侵针对每个玩家的结算过程）

	// 关于拆分：
	// 由于有些卡牌是对于许多目标依次结算，因此需要单独处理每一个结算过程，故将它们拆分为多个并依次处理。
	// 详见pcsha，pcnanman等函数的内部消息沟道机制。

};

// CardMessage		卡牌的使用和打出信息
// Message::type	有两种可能的消息类型：USECARD和HITCARD，通常的卡牌使用为HITCARD，仅在使用已装备的装备牌或判定牌生效时时取USECARD类型。
// from				来源玩家，注意当这个消息描述的延时锦囊时这一项指的是被判定的玩家。
// to				目标玩家，可以有多个，由_target参数描述目标数量，从0开始计数。注意在借刀杀人中，第一目标为需要出杀的玩家，第二目标为被杀的玩家
// card				使用的卡片。需要注意的是生成卡牌使用或打出信息时应该将卡片从信息发出端弹出，即若某玩家生成了打出卡牌的信息时，应从他的手牌弹出该牌。
// pos				记录了游戏牌是在哪一个地方被打出的。

// 卡牌相关：使用，打出。（这是一个命令性的消息，需要外部处理）
class sgs::Derive::CardMessage : public sgs::DataType::Message
{
public:
	CardMessage(bool use, sgs::DataType::Player * from, sgs::ConstData::PlayerRegionType playerRegion, int pos, sgs::DataType::Player * to = 0);
	// 截取构造，截取第一个目标。用于拆分消息。
	CardMessage(CardMessage * cardMsg);
	CardMessage(bool use, sgs::DataType::Player * from, const sgs::DataType::Card *card, sgs::DataType::Player * to = 0);
	~CardMessage();

	// 访问来源玩家
	sgs::DataType::Player * from() const;
	// 访问目标玩家数组，从0开始计数
	sgs::DataType::Player * to(int = 0) const;
	// 访问使用的卡片
	const sgs::DataType::Card * card() const;
	// 访问使用卡片的位置
	std::pair<sgs::ConstData::PlayerRegionType, int> pos() const;
	// 访问目标数
	int targets() const;

	// 增加目标
	bool addto(sgs::DataType::Player * to);
	// 减少一个目标，默认第一个
	bool popto(int = 0);

protected:

	sgs::DataType::Player *								m_from;			// 注意，延时锦囊中这一项代表被判定的玩家
	chain<sgs::DataType::Player *>						m_to;			// 目标
	int													m_targets;		// 目标数
	const sgs::DataType::Card *							m_card;			// 卡牌
	std::pair<sgs::ConstData::PlayerRegionType, int>	m_pos;			// 打出游戏牌的位置
};

// ZhangBaMessage	专用于处理丈八蛇矛效果的消息
class sgs::Derive::ZhangBaMessage : public CardMessage
{
public:
	ZhangBaMessage(sgs::DataType::Player * from, sgs::DataType::Player * to);
	~ZhangBaMessage();

	// 访问弃牌
	std::pair<const sgs::DataType::Card *, int> cost(int pos) const;
	bool addcost(int pos);

protected:
	std::pair<const sgs::DataType::Card *, int>			m_cost[2];
};

class sgs::Derive::SkillMessage : public sgs::DataType::Message
{
public:
	SkillMessage(sgs::DataType::Player * from, sgs::ConstData::HeroSkill type);
	~SkillMessage();

	// 访问来源
	sgs::DataType::Player * from() const;
	// 访问技能类型
	sgs::ConstData::HeroSkill skill() const;

	// 技能对应处理函数
	sgs::DataType::Message * settle(sgs::DataType::Message *msg);

protected:
	sgs::DataType::Skill			m_skill;			// 使用的技能种类
	sgs::DataType::Player *			m_from;				// 来源
};

// TransCardMessage			描述卡牌转移的信息，包括摸牌、弃牌、给牌、偷牌
// fromtype					描述卡牌来自哪个区域，可用的枚举变量有：PLAYER（来自玩家），POOLTOP（来自牌堆），DESK（来自牌桌）
// from						描述卡牌来自哪个玩家，仅在fromtype是PLAYER时有效。
// frompos					描述卡牌来自哪个位置，可用的枚举变量有：PHAND（手牌），PEQUIP（装备），PJUDGE（判定）
// pos						描述卡牌更具体的位置，从0开始计数，如frompos为PHAND时pos为0即表示第一张手牌
//							同样注意生成该信息时转移的牌应该从发出端弹出，而在处理这个信息时再将牌插入目标端。
// 卡牌转移（这是一个命令性的消息，需要外部处理）
class sgs::Derive::TransCardMessage : public sgs::DataType::Message
{
public:
	TransCardMessage(sgs::ConstData::RegionType fromRegion,
			sgs::ConstData::RegionType toRegion,
			bool open,
			sgs::DataType::Player * from = 0,
			ConstData::PlayerRegionType playerRegion = ConstData::playerRegionTypeNone,
			sgs::DataType::Player * to = 0,
			bool = false);
	~TransCardMessage();

	// 访问来源种类
	sgs::ConstData::RegionType fromtype() const;
	// 访问目标种类
	sgs::ConstData::RegionType totype() const;
	// 如果是玩家，访问来源指针
	sgs::DataType::Player * from() const;
	// 如果是玩家，访问目标指针
	sgs::DataType::Player * to() const;
	// 如果是玩家的话，来自手牌、装备区或判定区
	sgs::ConstData::PlayerRegionType frompos() const;
	// 访问牌数
	int cards() const;
	// 访问转移的第n张牌，默认第一张
	const sgs::DataType::Card * trans(int = 0) const;
	// 转移的位置，从0开始计数，只在从玩家处转移时有效
	int pos(int) const;
	// 是否可见
	bool open() const;
	// 是否是转移到判定区
	bool tojudge() const;

	// 加入一张转移卡
	bool transpush(const sgs::DataType::Card *);
	// 加入转移的位置，仅在确定来自玩家时可以使用
	bool transpush(int fp);
	// 写入转移卡和位置，用于五谷丰登。
	bool transpush(int fromPos, const sgs::DataType::Card *card);
	// 弹出第一张转移卡，默认第一张
	bool transpop(int = 0);

private:
	//来源种类
	sgs::ConstData::RegionType						m_fromtype;
	sgs::ConstData::RegionType						m_totype;		// 目标
	bool											m_open;			// 是否公开可见
	sgs::DataType::Player *							m_from;			// 如果来源是玩家，则记录玩家指针
	sgs::DataType::Player *							m_to;			// 如果目标是玩家，则记录玩家指针
	sgs::ConstData::PlayerRegionType				m_frompos;		// 如果是玩家的话，来自手牌、装备区或判定区
	int												m_cards;		// 转移牌数
	bool											m_toJudge;		// 如果是转移给玩家，是否是转移到它的判定区
	chain<int>										m_pos;			// 如果是玩家的话，来自哪个位置号
	chain<const sgs::DataType::Card *>				m_trans;		// 转移的牌
};

// SwitchPhaseMessage		阶段切换信息
//							在发出这个信息之前，阶段应该已经修改完毕
// 阶段切换（这是一个通知性的消息，不需要外部处理，即内部已经处理完成）
class sgs::Derive::SwitchPhaseMessage : public sgs::DataType::Message
{
public:
	SwitchPhaseMessage(sgs::DataType::Player * from, sgs::ConstData::PlayerPhase before, sgs::ConstData::PlayerPhase after);
	~SwitchPhaseMessage();

	// 访问来源玩家指针
	sgs::DataType::Player * from() const;
	// 访问切换前阶段
	sgs::ConstData::PlayerPhase before() const;
	// 访问切换后阶段
	sgs::ConstData::PlayerPhase after() const;

private:
	sgs::DataType::Player *				m_from;			// 来源玩家指针
	sgs::ConstData::PlayerPhase			m_before;		// 原来的阶段
	sgs::ConstData::PlayerPhase			m_after;		// 转换到的阶段
};

// HurtMessage		伤害/回复信息
//					在发出这个信息之前，伤害与回复已经造成。
class sgs::Derive::HurtMessage : public sgs::DataType::Message
{
public:
	HurtMessage(sgs::DataType::Player * from, sgs::DataType::Player * to, int hurt);
	~HurtMessage();

	sgs::DataType::Player* from() const;
	sgs::DataType::Player* to() const;
	int hurt() const;

private:
	sgs::DataType::Player *				m_from;			// 伤害来源
	sgs::DataType::Player *				m_to;			// 受到伤害的人
	int									m_hurt;			// 正值表示损血，负值表示加血
};

//JudgeMessage		判定消息
//					在发出这个消息之前，判定已经结束并确定生效与否。
class sgs::Derive::JudgeMessage : public sgs::DataType::Message
{
public:
	JudgeMessage(sgs::DataType::Player * from, const sgs::DataType::Card * card, const sgs::DataType::Card * result, bool effect);
	JudgeMessage(sgs::DataType::Player * from, sgs::ConstData::HeroSkill skill, const sgs::DataType::Card * result, bool effect);
	~JudgeMessage();

	sgs::DataType::Player * from() const;
	const sgs::DataType::Card* card() const;
	const sgs::DataType::Card* result() const;
	bool effect() const;
	bool cardJudge() const;
	sgs::ConstData::HeroSkill skill() const;

private:
	sgs::DataType::Player *				m_from;			// 判定来源。即谁接受了判定。
	const sgs::DataType::Card *			m_card;			// 需要判定的牌。
	const sgs::DataType::Card *			m_result;		// 作为判定结果的牌。
	bool								m_effect;		// 判定是否生效，如乐不思蜀是否成功
	sgs::ConstData::HeroSkill			m_skill;		// 需要判定的技能。
	bool								m_cardJudge;	// 判定的是牌吗？
};

//MDying:濒死信息
//neg:恢复到1血所需要的桃个数
//murderer:自然死亡时该指针为空，如被闪电劈死
class sgs::Derive::DyingMessage : public sgs::DataType::Message
{
public:
	DyingMessage(sgs::DataType::Player * from, sgs::DataType::Player * murder, int tao);
	~DyingMessage();

	sgs::DataType::Player * from() const;
	sgs::DataType::Player * murderer() const;
	int neg() const;

private:
	sgs::DataType::Player *				m_from;			// 濒死的玩家
	sgs::DataType::Player *				m_murderer;		// 对他造成伤害的玩家
	int									m_needTao;		// 求桃数
};

// KillMessage		击杀消息
//					发出信息前死亡已经造成，但死亡引起的惩罚与结束判定尚未进行，死亡玩家的游戏牌处理也未完成。
class sgs::Derive::KillMessage : public sgs::DataType::Message
{

public:
	KillMessage(sgs::DataType::Player * from, sgs::DataType::Player * to);
	~KillMessage();

	sgs::DataType::Player * from() const;
	sgs::DataType::Player * to() const;

private:
	sgs::DataType::Player *				m_from;			// 造成最后伤害的人
	sgs::DataType::Player *				m_die;			// 被杀的人
};

// BeginMessage			事件开始标记
//						暂时闲置不用
// 事件开始标记
class sgs::Derive::BeginMessage : public sgs::DataType::Message
{
public:
	BeginMessage()
		: Message(sgs::ConstData::BEGIN)
	{}
	~BeginMessage()
	{}
};

// FinaleMessage		游戏结束标记
//						这个消息标志着游戏的结束，储存着游戏结果。
class sgs::Derive::FinaleMessage : public sgs::DataType::Message
{

public:
	FinaleMessage(char finale)
		: Message(sgs::ConstData::FINALE)
		, m_finale(finale)
	{}
	~FinaleMessage()
	{}

	//访问用函数头
	char finale() const
	{
		return m_finale;
	}

private:
	char			m_finale;		//游戏结果，用位存储玩家死亡信息
};

class sgs::Derive::WuShengMessage : public sgs::Derive::SkillMessage
{

public:
	WuShengMessage(sgs::DataType::Player * from, sgs::ConstData::PlayerRegionType playerRegion, int, sgs::DataType::Player * to = 0);
	~WuShengMessage();

	const sgs::DataType::Card * card() const;
	sgs::DataType::Player * to(int pos) const;
	int targets() const;
	std::pair<sgs::ConstData::PlayerRegionType, int> pos() const;
	void addto(sgs::DataType::Player *);
	void popto(int pos = 0);

protected:
	const sgs::DataType::Card *								m_card;
	chain<sgs::DataType::Player *>							m_to;
	int														m_targets;
	std::pair<sgs::ConstData::PlayerRegionType, int>		m_pos;			// 手牌的哪一张
};

class sgs::Derive::QingGuoMeesage : public sgs::Derive::SkillMessage
{
public:
	QingGuoMeesage(sgs::DataType::Player * from, int pos);
	~QingGuoMeesage();

	const sgs::DataType::Card * card() const;
	int pos() const;

protected:
	const sgs::DataType::Card *								m_card;
	int														m_pos;			// 在手牌的那个位置
};

class sgs::Derive::LongDanMessage : public sgs::Derive::SkillMessage
{
public:
	LongDanMessage(sgs::DataType::Player * from, int, sgs::DataType::Player * to = 0);
	~LongDanMessage();

	const sgs::DataType::Card * card() const;
	sgs::DataType::Player * to(int pos) const;
	int targets() const;
	int pos() const;
	void addto(sgs::DataType::Player *);
	void popto(int pos = 0);

protected:
	const sgs::DataType::Card *								m_card;
	chain<sgs::DataType::Player *>							m_to;
	int														m_targets;
	int														m_pos;			// 手牌的哪一张
};

class sgs::Derive::QiXiMessage : public sgs::Derive::SkillMessage
{
public:
	QiXiMessage(sgs::DataType::Player * from, sgs::ConstData::PlayerRegionType, int, sgs::DataType::Player * to);
	~QiXiMessage();

	const sgs::DataType::Card * card() const;
	sgs::DataType::Player * to() const;
	std::pair<sgs::ConstData::PlayerRegionType, int> pos() const;

protected:
	const sgs::DataType::Card *								m_card;
	sgs::DataType::Player *									m_to;
	std::pair<sgs::ConstData::PlayerRegionType, int>		m_pos;
};

class sgs::Derive::FanKuiMessage : public sgs::Derive::SkillMessage
{
public:
	FanKuiMessage(sgs::DataType::Player * from, sgs::DataType::Player * to);
	~FanKuiMessage();

	sgs::DataType::Player * to() const;

protected:
	sgs::DataType::Player *			m_to;
};

class sgs::Derive::GangLieMessage : public sgs::Derive::SkillMessage
{
public:
	GangLieMessage(sgs::DataType::Player * from, sgs::DataType::Player * to);
	~GangLieMessage();

	sgs::DataType::Player * to() const;

protected:
	sgs::DataType::Player *			m_to;
};

class sgs::Derive::QingNangMessage : public sgs::Derive::SkillMessage
{
public:
	QingNangMessage(sgs::DataType::Player * from, int, sgs::DataType::Player * to);
	~QingNangMessage();

	const sgs::DataType::Card * card() const;
	sgs::DataType::Player * to() const;
	int pos() const;

protected:
	const sgs::DataType::Card *		m_card;
	int								m_pos;
	sgs::DataType::Player *			m_to;
};

class sgs::Derive::JiJiuMessage : public sgs::Derive::SkillMessage
{
public:
	JiJiuMessage(sgs::DataType::Player * from, sgs::ConstData::PlayerRegionType playerRegion, int pos);
	~JiJiuMessage();

	const sgs::DataType::Card * card() const;
	std::pair<sgs::ConstData::PlayerRegionType, int> pos() const;

protected:
	const sgs::DataType::Card *							m_card;
	std::pair<sgs::ConstData::PlayerRegionType, int>	m_pos;
};

class sgs::Derive::RenDeMessage : public sgs::Derive::SkillMessage
{
public:
	RenDeMessage(sgs::DataType::Player * from, sgs::DataType::Player * to);
	~RenDeMessage();

	const sgs::DataType::Card * card(int pos) const;
	int pos(int index) const;
	sgs::DataType::Player * to() const;
	int cards() const;

	// 返回消息时按从大到小排序，即插入时按从小到大插入
	void transpush(int);

protected:
	sgs::DataType::Player *											m_to;
	std::vector<std::pair<int, const sgs::DataType::Card *> >		m_card;
	int																m_cards;
};

class sgs::Derive::JiJiangMessage : public sgs::Derive::SkillMessage
{
public:
	JiJiangMessage(sgs::DataType::Player * from, sgs::DataType::Player * to = 0);
	~JiJiangMessage();

	int targets() const;
	void addto(sgs::DataType::Player *);
	void popto(int = 0);
	sgs::DataType::Player * to(int = 0) const;

protected:
	chain<sgs::DataType::Player *>									m_to;
	int																m_targets;
};

class sgs::Derive::LiJianMessage : public sgs::Derive::SkillMessage
{
public:
	LiJianMessage(sgs::DataType::Player * from,
				  sgs::ConstData::PlayerRegionType playerRegion,
				  int pos,
				  sgs::DataType::Player * first,
				  sgs::DataType::Player * second);
	~LiJianMessage();

	sgs::DataType::Player * first() const;
	sgs::DataType::Player * second() const;
	std::pair<sgs::ConstData::PlayerRegionType, int> pos() const;
	const sgs::DataType::Card * card() const;

protected:
	sgs::DataType::Player *									m_toFirst;
	sgs::DataType::Player *									m_toSecond;
	std::pair<sgs::ConstData::PlayerRegionType, int>		m_pos;
	const sgs::DataType::Card *								m_card;
};

class sgs::Derive::TuXiMessage : public sgs::Derive::SkillMessage
{
public:
	TuXiMessage(sgs::DataType::Player * from, sgs::DataType::Player * target1,
				int pos1, sgs::DataType::Player * target2 = 0, int pos2 = -1);
	~TuXiMessage();

	std::pair<sgs::DataType::Player *, int> pos(int index) const;
	const sgs::DataType::Card * card(int index) const;
	int targets() const;

protected:
	std::pair<sgs::DataType::Player *, int>					m_pos[2];
	const sgs::DataType::Card *								m_card[2];
	int														m_targets;
};

class sgs::Derive::YiJiMessage : public sgs::Derive::SkillMessage
{
public:
	YiJiMessage(sgs::DataType::Player * from,
				sgs::DataType::Player * target1,
				const sgs::DataType::Card * card1,
				sgs::DataType::Player * target2,
				const sgs::DataType::Card * card2);
	~YiJiMessage();

	std::pair<sgs::DataType::Player *, const sgs::DataType::Card *> trans(int pos) const;

protected:
	std::pair<sgs::DataType::Player *, const sgs::DataType::Card *>		m_trans[2];
};

class sgs::Derive::ZhiHengMessage : public sgs::Derive::SkillMessage
{
public:
	ZhiHengMessage(sgs::DataType::Player *);
	~ZhiHengMessage();

	void transpush(sgs::ConstData::PlayerRegionType playerRegion, int pos);
	std::pair<sgs::ConstData::PlayerRegionType, int> pos(int index) const;
	const sgs::DataType::Card * card(int index) const;
	int cards() const;

protected:
	std::vector<std::pair<sgs::ConstData::PlayerRegionType, int> >		m_pos;
	std::vector<const sgs::DataType::Card *>							m_card;
	int																	m_cards;
};

class sgs::Derive::FanJianMessage : public sgs::Derive::SkillMessage
{
public:
	FanJianMessage(sgs::DataType::Player * from, sgs::DataType::Player * to);
	~FanJianMessage();

	sgs::DataType::Player * to() const;

protected:
	sgs::DataType::Player *			m_to;
};

class sgs::Derive::JieYinMessage : public sgs::Derive::SkillMessage
{
public:
	JieYinMessage(sgs::DataType::Player * from, sgs::DataType::Player * to, int pos1, int pos2);
	~JieYinMessage();

	sgs::DataType::Player * to() const;
	int pos(int) const;
	const sgs::DataType::Card * card(int) const;

protected:
	sgs::DataType::Player *				m_to;
	int									m_pos[2];
	const sgs::DataType::Card *			m_card[2];
};

class sgs::Derive::GuoSeMessage : public sgs::Derive::SkillMessage
{
public:
	GuoSeMessage(sgs::DataType::Player * from, sgs::ConstData::PlayerRegionType, int, sgs::DataType::Player * to);
	~GuoSeMessage();

	sgs::DataType::Player * to() const;
	std::pair<sgs::ConstData::PlayerRegionType, int> pos() const;
	const sgs::DataType::Card * card() const;

protected:
	sgs::DataType::Player *									m_to;
	std::pair<sgs::ConstData::PlayerRegionType, int>		m_pos;
	const sgs::DataType::Card *								m_card;
};

class sgs::Derive::LiuLiMessage : public sgs::Derive::SkillMessage
{
public:
	LiuLiMessage(sgs::DataType::Player * from, sgs::ConstData::PlayerRegionType playerRegion, int, sgs::DataType::Player * to);
	~LiuLiMessage();

	sgs::DataType::Player * to() const;
	std::pair<sgs::ConstData::PlayerRegionType, int> pos() const;
	const sgs::DataType::Card * card() const;

protected:
	sgs::DataType::Player *									m_to;
	std::pair<sgs::ConstData::PlayerRegionType, int>		m_pos;
	const sgs::DataType::Card *								m_card;
};

#endif /*SGSDATA_MSG_H*/
