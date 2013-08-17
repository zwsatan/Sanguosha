#include "sgsai_AISujiang.h"
#include "sgsui_output.h"

namespace sgsai {

AISujiang::AISujiang(int seat)
	: AICommon(seat)
{
}

bool AISujiang::useSkillOrNot(sgs::ConstData::HeroSkill, sgs::DataType::Player *)
{
	printDebug("AISujiang::useSkillOrNot: start, AI's seat = " + QString::number(mySeat()));
	bool returnValue = true;
	printDebug("AISujiang::useSkillOrNot: over");
	return returnValue;
}

} /* namespace sgsai */
