#include "sgsai_AILvbu.h"
#include "sgsui_output.h"

namespace sgsai {

AILvbu::AILvbu(int seat) :
		AICommon(seat) {
}

bool AILvbu::useSkillOrNot(sgs::ConstData::HeroSkill, sgs::DataType::Player *) {
	printDebug(
			"AILvbu::useSkillOrNot: start, AI's seat = "
					+ QString::number(mySeat()));
	printDebug(
			"<font color=red><b>Warning: </b></font>AILvbu::useSkillOrNot: shouldn't be called becase lvbu has no passive skill");

	bool returnValue = true;
	printDebug(
			"AILvbu::useSkillOrNot: over, return "
					+ QString(returnValue ? "true" : "false"));
	return returnValue;
}

} /* namespace sgsai */
