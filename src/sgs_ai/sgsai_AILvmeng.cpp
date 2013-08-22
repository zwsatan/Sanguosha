#include "sgsai_AILvmeng.h"
#include "sgsui_output.h"

namespace sgsai {

AILvmeng::AILvmeng(int seat) :
		AICommon(seat) {
}

bool AILvmeng::useSkillOrNot(sgs::ConstData::HeroSkill, sgs::DataType::Player *) {
	printDebug(
			"AILvmeng::useSkillOrNot: start, AI's seat = "
					+ QString::number(mySeat()));
	bool returnValue = true;
	printDebug(
			"AILvmeng::useSkillOrNot: over, return "
					+ QString(returnValue ? "true" : "false"));
	return returnValue;
}

} /* namespace sgsai */
