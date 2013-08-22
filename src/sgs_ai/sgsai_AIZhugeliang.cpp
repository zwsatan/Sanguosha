#include "sgsai_AIZhugeliang.h"
#include "sgsui_output.h"
#include "sgsdata_card.h"

namespace sgsai {

AIZhugeliang::AIZhugeliang(int seat) :
		AICommon(seat) {
}

bool AIZhugeliang::useSkillOrNot(sgs::ConstData::HeroSkill,
		sgs::DataType::Player *) {
	printDebug(
			"AIZhugeliang::useSkillOrNot: start, AI's seat = "
					+ QString::number(mySeat()));

	bool returnValue = true;
	printDebug(
			"AIZhugeliang::useSkillOrNot: over, return "
					+ QString(returnValue ? "true" : "false"));
	return returnValue;
}

void AIZhugeliang::chooseGuanxing(std::vector<const sgs::DataType::Card *>&,
		std::vector<const sgs::DataType::Card *>&) {
	return;
}

} /* namespace sgsai */
