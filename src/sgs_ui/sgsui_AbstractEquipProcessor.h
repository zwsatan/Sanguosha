#ifndef SGSUI_ABSTRACTEQUIPPROCESSOR_H_
#define SGSUI_ABSTRACTEQUIPPROCESSOR_H_

namespace sgsui {

class ZhuangbeiPic;

class AbstractEquipProcessor
{
public:
	virtual void zhuangbeiClicked(ZhuangbeiPic *clickEquip) = 0;
	virtual ~AbstractEquipProcessor()
	{
	}
};

} /* namespace sgsui */
#endif /* SGSUI_ABSTRACTEQUIPPROCESSOR_H_ */
