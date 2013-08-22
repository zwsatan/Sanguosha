#ifndef SGSAI_AIHUANGGAI_H_
#define SGSAI_AIHUANGGAI_H_

#include "sgsai_AICommon.h"

namespace sgsai {

class AIHuanggai : public AICommon
{
public:
	explicit AIHuanggai(int seat);
	bool useSkillOrNot(sgs::ConstData::HeroSkill skillType, sgs::DataType::Player * player);
	virtual sgs::DataType::Message * useCardResponse();

private:
	sgs::Derive::SkillMessage * useKurouInUsecard();
};

} /* namespace sgsai */
#endif /* SGSAI_AIHUANGGAI_H_ */
