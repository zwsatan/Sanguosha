#include "sgsdata_skill.h"
#include "sgsdata_enum.h"
#include "sgsdata_skill_func.h"

using namespace sgs;
using namespace DataType;
using namespace ConstData;

Skill::Skill()
{
	m_skill = heroSkillNone;
	m_processor = 0;
}

Skill::Skill(HeroSkill heroSkill)
	: m_skill(heroSkill)
{
	protrans(heroSkill);
}

HeroSkill Skill::skill() const
{
	return m_skill;
}

Message* Skill::settle(Message * msg) const
{
	return (*m_processor)(msg);
}

void Skill::protrans(HeroSkill skill)
{
	switch (skill)
	{
	// 观星,空城,洛神,裸衣,马术,铁骑,克己,咆哮,不进行函数回调处理
	// 应该是有单独处理
	case GUANXING:
	case KONGCHENG:
	case LUOSHEN:
	case LUOYI:
	case MASHU:
	case TIEQI:
	case KEJI:
	case PAOXIAO:
		m_processor = 0;
		break;

	case WUSHENG:
		m_processor = func::pcwusheng;
		break;

	case GANGLIE:
		m_processor = func::pcganglie;
		break;

	case QINGGUO:
		m_processor = func::pcqingguo;
		break;

	case LONGDAN:
		m_processor = func::pclongdan;
		break;

	case QIXI:
		m_processor = func::pcqixi;
		break;

	case FANKUI:
		m_processor = func::pcfankui;
		break;

	case GUICAI:
		m_processor = func::pcguicai;
		break;

	case KUROU:
		m_processor = func::pckurou;
		break;
	case QINGNANG:
		m_processor = func::pcqingnang;
		break;

	case JIJIU:
		m_processor = func::pcjijiu;
		break;

	case RENDE:
		m_processor = func::pcrende;
		break;

	case JIJIANG:
		m_processor = func::pcjijiang;
		break;

	case HUJIA:
		m_processor = func::pchujia;
		break;

	case LIJIAN:
		m_processor = func::pclijian;
		break;

	case ZHIHENG:
		m_processor = func::pczhiheng;
		break;

	case FANJIAN:
		m_processor = func::pcfanjian;
		break;

	case JIEYIN:
		m_processor = func::pcjieyin;
		break;

	case GUOSE:
		m_processor = func::pcguose;
		break;

	default:
		m_processor = 0;
		break;
	}
}
