#ifndef SGSUI_GLOBAL_H_
#define SGSUI_GLOBAL_H_

#include "sgsdata_enum.h"
#include <utility>

namespace sgsui {

// choose hero of ZHONG, FAN and NEI
sgs::ConstData::HeroType heroChoose(sgs::ConstData::HeroType lordHeroType,
									sgs::ConstData::PlayerRole selfRole,
									sgs::ConstData::HeroType avaliable1,
									sgs::ConstData::HeroType avaliable2,
									sgs::ConstData::HeroType avaliable3);

// choose hero of ZHU
sgs::ConstData::HeroType lordChoose(sgs::ConstData::HeroType avliable1,
									sgs::ConstData::HeroType avliable2,
									sgs::ConstData::HeroType avliable3,
									sgs::ConstData::HeroType avliable4,
									sgs::ConstData::HeroType avliable5);

} /* namespace sgsui */
#endif /* SGSUI_GLOBAL_H_ */
