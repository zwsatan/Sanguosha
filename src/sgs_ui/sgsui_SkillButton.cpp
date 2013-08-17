#include "sgsui_SkillButton.h"

namespace sgsui {

SkillButton::SkillButton(QString Skilltext, QWidget * parent/* = 0*/)
	: BlockableButton(parent)
{
	setText(Skilltext);
}

SkillButton::SkillButton(QWidget *parent)
	: BlockableButton(parent)
{
}

} /* namespace sgsui */
