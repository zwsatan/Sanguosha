#ifndef SGSAI_AILIUBEI_H_
#define SGSAI_AILIUBEI_H_

#include "sgsai_AICommon.h"

namespace sgsai {

class AILiubei : public AICommon
{
public:
	explicit AILiubei(int seat);
	bool useSkillOrNot(sgs::ConstData::HeroSkill skillType, sgs::DataType::Player * player);
};

} /* namespace sgsai */
#endif /* SGSAI_AILIUBEI_H_ */
