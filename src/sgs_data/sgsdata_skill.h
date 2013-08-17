#ifndef _SGSDATA_SKILL_H
#define _SGSDATA_SKILL_H
#include "sgsdata_enum.h"
#include "sgsdata_namespace.h"

class sgs::DataType::Skill
{

public:
	Skill();
	// 转换构造
	Skill(sgs::ConstData::HeroSkill heroSkill);

	// 访问技能类型
	sgs::ConstData::HeroSkill skill() const;
	// 调用处理器函数
	sgs::DataType::Message* settle(sgs::DataType::Message * msg) const;

private:
	// 由技能类型找到函数
	void protrans(sgs::ConstData::HeroSkill skill);

private:
	typedef sgs::DataType::Message* (*SkillProcessHandle)(sgs::DataType::Message*);

	sgs::ConstData::HeroSkill		m_skill;		// 技能类型
	SkillProcessHandle				m_processor;	// 处理器


};

#endif /*_SGSDATA_SKILL_H*/
