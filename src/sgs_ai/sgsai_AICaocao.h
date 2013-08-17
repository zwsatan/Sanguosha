#ifndef SGSAI_AICAOCAO_H_
#define SGSAI_AICAOCAO_H_

#include "sgsai_AICommon.h"

namespace sgsai {

class AICaocao: public AICommon
{
public:
	explicit AICaocao(int seat);
	bool useSkillOrNot(sgs::ConstData::HeroSkill skillType, sgs::DataType::Player * player);
	sgs::DataType::Message * shanOrNot(sgs::DataType::Player * from, const sgs::DataType::Card * card);

private:
	sgs::Derive::SkillMessage * useHujiaInShanOrNot();
};

} /* namespace sgsai */
#endif /* SGSAI_AICAOCAO_H_ */
