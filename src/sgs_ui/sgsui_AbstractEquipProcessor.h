#ifndef SGSUI_ABSTRACTEQUIPPROCESSOR_H_
#define SGSUI_ABSTRACTEQUIPPROCESSOR_H_

namespace sgsui {

class ZhuangbeiPic;

class AbstractEquipProcessor
{
public:
	virtual ~AbstractEquipProcessor() {}

	virtual void zhuangbeiClicked(ZhuangbeiPic *clickEquip) = 0;
};

} /* namespace sgsui */
#endif /* SGSUI_ABSTRACTEQUIPPROCESSOR_H_ */
