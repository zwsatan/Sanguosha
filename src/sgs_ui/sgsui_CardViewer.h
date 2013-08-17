#ifndef SGSUI_CARDVIEWER_H_
#define SGSUI_CARDVIEWER_H_

#include "ui_CardViewer.h"

namespace sgs {
namespace DataType {
class Card;
}
}

namespace sgsui {

class CardFrame;

class CardViewer
		: public QFrame
		, protected Ui::CardViewer
{
public:
	CardViewer(QWidget * parent = 0);
	void showViewer(const sgs::DataType::Card * _card1, const sgs::DataType::Card * card2);
	void showSecondViewer();

private:
	const sgs::DataType::Card *		m_card1;
	const sgs::DataType::Card *		m_card2;
	CardFrame *						m_cardFrame1;
	CardFrame *						m_cardFrame2;

	const static QSize				ms_fixedSize;
};

} /* namespace sgsui */
#endif /* SGSUI_CARDVIEWER_H_ */
