#ifndef SGSAI_AILVMENG_H_
#define SGSAI_AILVMENG_H_

#include "sgsai_AICommon.h"

namespace sgsai {

#include "sgsai_AICommon.h"

class AILvmeng : public AICommon
{
public:
	explicit AILvmeng(int seat);
	bool useSkillOrNot(sgs::ConstData::HeroSkill skillType, sgs::DataType::Player * player);
};

} /* namespace sgsai */
#endif /* SGSAI_AILVMENG_H_ */
