#ifndef SGSAI_AIZHAOYUN_H_
#define SGSAI_AIZHAOYUN_H_

#include "sgsai_AICommon.h"

namespace sgsai {

class AIZhaoyun : public AICommon
{
public:
	explicit AIZhaoyun(int seat);
	bool useSkillOrNot(sgs::ConstData::HeroSkill skillType, sgs::DataType::Player * player);

private:
	virtual sgs::DataType::Message * useCardResponse();
	virtual sgs::DataType::Message * shaOrNot(sgs::DataType::Player * /*from*/, const sgs::DataType::Card * /*card*/);
	virtual sgs::DataType::Message * shanOrNot(sgs::DataType::Player * /*from*/, const sgs::DataType::Card * /*card*/);
	sgs::Derive::SkillMessage * useLongdanInUsecard();
	sgs::Derive::SkillMessage * useLongdanInShaOrNot();
	sgs::Derive::SkillMessage * useLongdanInShanOrNot();
	const CardVec& shaCard();
	const CardVec& shanCard();
	CardVec shaVec;
	CardVec shanVec;
};

} /* namespace sgsai */
#endif /* SGSAI_AIZHAOYUN_H_ */
