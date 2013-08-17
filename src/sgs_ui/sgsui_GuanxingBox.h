#ifndef SGSUI_GUANXINGBOX_H_
#define SGSUI_GUANXINGBOX_H_

#include "ui_GuanxingBox.h"
#include "sgsui_BlockingFrame.h"
#include "sgsui_AbstractDragProcessor.h"
#include "sgsdata_card.h"

namespace sgsui {

class GuanxingBox
		: public BlockingFrame
		, protected Ui::GuanxingBox
		, public AbstractDragProcessor
{
public:
	explicit GuanxingBox(QWidget * parent);
	void enterKeyPressedEvent();
	void chooseGuanxing(std::vector<const sgs::DataType::Card *>& top, std::vector<const sgs::DataType::Card *>& buttom);

private:
	QPoint processDrag(DragableCardFrame * card);
	DragableCardFrame*& getPointer(const QPoint &currectPos);
	QPoint posOfPointer(DragableCardFrame * const & pointer) const;
	void clearData();

private:
	DragableCardFrame **		m_upperCard[5];
	DragableCardFrame **		m_lowerCard[5];
	DragableCardFrame *			m_upperCard0;
	DragableCardFrame *			m_upperCard1;
	DragableCardFrame *			m_upperCard2;
	DragableCardFrame *			m_upperCard3;
	DragableCardFrame *			m_upperCard4;
	DragableCardFrame *			m_lowerCard0;
	DragableCardFrame *			m_lowerCard1;
	DragableCardFrame *			m_lowerCard2;
	DragableCardFrame *			m_lowerCard3;
	DragableCardFrame *			m_lowerCard4;
	const QPoint *				m_upperPosition[5];

	const static QPoint			ms_upperPosition0;
	const static QPoint			ms_upperPosition1;
	const static QPoint			ms_upperPosition2;
	const static QPoint			ms_upperPosition3;
	const static QPoint			ms_upperPosition4;
	const static QSize			ms_fixedSize;
};

} /* namespace sgsui */
#endif /* SGSUI_GUANXINGBOX_H_ */
