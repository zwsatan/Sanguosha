#ifndef _SGSDATA_HERO_H
#define _SGSDATA_HERO_H
#include "sgsdata_enum.h"
#include "sgsdata_namespace.h"
#include "sgsdata_skill.h"
#include "sgsdata_msg.h"
#include "sgsdata_player.h"

//武将基类的定义。武将以全局变量的形式存在，构造玩家时会决定对应的武将并通过指针来访问。
//其他武将在heroes.h中定义。
class sgs::DataType::Hero
{

public:
	Hero(int maxHP,
		 sgs::ConstData::HeroType type,
		 sgs::ConstData::HeroSex sex,
		 sgs::ConstData::HeroSide side,
		 sgs::ConstData::HeroSkill skill0 = sgs::ConstData::heroSkillNone,
		 sgs::ConstData::HeroSkill skill1 = sgs::ConstData::heroSkillNone,
		 sgs::ConstData::HeroSkill skill2 = sgs::ConstData::heroSkillNone,
		 sgs::ConstData::HeroSkill skill3 = sgs::ConstData::heroSkillNone);//构造函数
	virtual ~Hero();

	// 访问用函数头
	// 最大血量
	int maxHP() const;
	// 英雄种类
	sgs::ConstData::HeroType type() const;
	// 英雄性别
	sgs::ConstData::HeroSex sex() const;
	// 英雄势力
	sgs::ConstData::HeroSide side() const;
	// 英雄技能，从0开始计数
	sgs::DataType::Skill skill(int pos);
	// 英雄技能个数
	int skillnum() const;

	//阶段函数。锁定技会写死在这些函数的多态中。传入参数为对应的玩家。
	// 开始阶段
	virtual sgs::DataType::Message * startPhase(sgs::DataType::Player *);
	// 判定阶段
	virtual sgs::DataType::Message * judgePhase(sgs::DataType::Player *);
	// 摸牌阶段
	virtual sgs::DataType::Message * getPhase(sgs::DataType::Player *);
	// 主要阶段
	virtual sgs::DataType::Message * mainPhase(sgs::DataType::Player *);
	// 弃牌阶段
	virtual sgs::DataType::Message * abandonPhase(sgs::DataType::Player *);
	// 结束阶段
	virtual sgs::DataType::Message * endPhase(sgs::DataType::Player *);

private:
	int							m_maxHP;		// 最大血量
	sgs::ConstData::HeroType	m_type;			// 武将种类
	sgs::ConstData::HeroSex		m_sex;			// 武将性别
	sgs::ConstData::HeroSide	m_side;			// 武将势力
	sgs::DataType::Skill		m_skill[4];		// 武将技能
	int							m_skills;		// 武将技能个数
};

#endif /*_SGSDATA_HERO_H*/
