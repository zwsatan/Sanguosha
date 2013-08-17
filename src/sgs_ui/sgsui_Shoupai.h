#ifndef SGSUI_SHOUPAI_H_
#define SGSUI_SHOUPAI_H_

#include "sgsui_CardButton.h"

namespace sgs {
namespace DataType {
class Card;
}
}

namespace sgsui {

class ShoupaiButton: public CardButton
{
public:
	ShoupaiButton(const sgs::DataType::Card * card, AbstractCardContainer * container, QWidget *parent = 0);

private:
	void mousePressEvent(QMouseEvent*);
	void mouseDoubleClickEvent(QMouseEvent*);
};

} /* namespace sgsui */
#endif /* SGSUI_SHOUPAI_H_ */
