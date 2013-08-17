#ifndef SGSUI_WUGUFENGDENGBOX_H_
#define SGSUI_WUGUFENGDENGBOX_H_

#include "ui_WugufengdengBox.h"
#include "sgsui_BlockingFrame.h"
#include "sgsui_AbstractCardContainer.h"
#include "sgsdata_card.h"
#include <vector>
#include <utility>

namespace sgsui {

class WugufengdengBox
		: public BlockingFrame
		, protected Ui::WugufengdengBox
		, protected AbstractCardContainer
{
public:
	explicit WugufengdengBox(QWidget *parent = 0);

	void showContainer(const std::vector<std::pair<const sgs::DataType::Card *, bool> > &wugoInfo);
	QPoint removeCard(int index);
	int chooseWugu(const std::vector<std::pair<const sgs::DataType::Card *, bool> > &wugoInfo);

private:
	virtual void cardClicked(CardButton* clickedCard);

	void collectData();
	bool isEmpty() const;
	void enterKeyPressedEvent();

private:
	CardButton *			m_buttons[8];
	QFrame *				m_bigFrame[8];
	const QPoint *			m_points[8];
	bool					m_isChoosing;
	int						m_returnValue;

	const static QSize		ms_fixedSize;
	const static QSize		ms_buttonSize;
	const static QPoint		ms_cardPoint0;
	const static QPoint		ms_cardPoint1;
	const static QPoint		ms_cardPoint2;
	const static QPoint		ms_cardPoint3;
	const static QPoint		ms_cardPoint4;
	const static QPoint		ms_cardPoint5;
	const static QPoint		ms_cardPoint6;
	const static QPoint		ms_cardPoint7;

};

} /* namespace sgsui */
#endif /* SGSUI_WUGUFENGDENGBOX_H_ */
