#include "sgsai_AIHuanggai.h"
#include "sgsui_output.h"
#include "sgsdata_card.h"
#include "sgsdata_skill.h"
#include "sgsdata_player.h"
#include "sgsdata_msg.h"
#include "sgsui_PlayerSort.h"
#include "sgsui_output.h"
#include "sgsui_nameConvert.h"

namespace sgsai {

AIHuanggai::AIHuanggai(int seat) :
		AICommon(seat) {
}

bool AIHuanggai::useSkillOrNot(sgs::ConstData::HeroSkill /*skillType*/,
		sgs::DataType::Player * /*player*/) {
	printDebug(
			"AIHuanggai::useSkillOrNot: start, AI's seat = "
					+ QString::number(mySeat()));
	printDebug(
			"<font color=red><b>Warning: </b></font>AIHuanggai::useSkillOrNot: shouldn't be called becase huanggai has no passive skill");

	bool returnValue = true;
	printDebug(
			"AIHuanggai::useSkillOrNot: over, return "
					+ QString(returnValue ? "true" : "false"));
	return returnValue;
}

sgs::DataType::Message * AIHuanggai::useCardResponse() {
	sgs::Derive::SkillMessage * kurou = useKurouInUsecard();
	if (kurou) {
		printDebug(sgsui::messageInterpret(kurou));
		printDebug(
				"AIHuanggai::useCardResponse: use kurou, AI's seat = "
						+ QString::number(mySeat()));
		return kurou;
	}
	return AICommon::useCardResponse();
}

sgs::Derive::SkillMessage * AIHuanggai::useKurouInUsecard() {
	sgs::Derive::SkillMessage * returnKurou = 0;
	if (myPlayer()->hp() == myPlayer()->maxHP()) {
		returnKurou = new sgs::Derive::SkillMessage(myPlayer(),
				sgs::ConstData::KUROU);
	}
	return returnKurou;
}

} /* namespace sgsai */
