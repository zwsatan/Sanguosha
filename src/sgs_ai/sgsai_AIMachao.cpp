#include "sgsai_AIMachao.h"
#include "sgsui_output.h"

namespace sgsai {

AIMachao::AIMachao(int seat) :
		AICommon(seat) {
}

bool AIMachao::useSkillOrNot(sgs::ConstData::HeroSkill, sgs::DataType::Player *)
{
	printDebug("AIMachao::useSkillOrNot: start, AI's seat = " + QString::number(mySeat()));
	bool returnValue = true;
	printDebug("AIMachao::useSkillOrNot: over, return " + QString(returnValue ? "true" : "false"));
	return returnValue;
}

} /* namespace sgsai */
