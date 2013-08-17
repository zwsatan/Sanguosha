#include "sgsui_SkillLabel.h"
#include "sgsui_nameConvert.h"
#include <QTimer>

namespace sgsui {

const QSize SkillLabel::ms_fixedSize(100, 50);

SkillLabel::SkillLabel(QWidget * parent/* = 0*/)
	: QLabel(parent)
{
	m_skillTimer = new QTimer(this);
	m_skillTimer->setSingleShot(true);
	connect(m_skillTimer, SIGNAL(timeout()), this, SLOT(hide()));
	setFont(GUIStaticData::skillFont);
	setFixedSize(ms_fixedSize);
}

void SkillLabel::showSkill(sgs::ConstData::HeroSkill skill, int skillDuration/* = GUIStaticData::showSkillDuration*/)
{
	if (skillDuration < 0)
		skillDuration = GUIStaticData::showSkillDuration;

	setText(setColored(skillDisplayName(skill), false));
	m_skillTimer->start(skillDuration);
	show();
}

} /* namespace sgsui */
