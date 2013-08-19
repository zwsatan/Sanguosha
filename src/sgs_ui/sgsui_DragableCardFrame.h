#ifndef DRAGABLECARDFRAME_H_
#define DRAGABLECARDFRAME_H_

#include "sgsui_CardFrame.h"
#include "sgsui_AbstractDragProcessor.h"
#include "sgsdata_card.h"

namespace sgsui {

class DragableCardFrame : public CardFrame
{
public:
	DragableCardFrame(QWidget * dragBorderWidget,
					  AbstractDragProcessor * dragProcessor,
					  const sgs::DataType::Card * card,
					  const QPoint &pos);

	// NO pos version of constructor
	DragableCardFrame(QWidget* dragBorderWidget,
					  AbstractDragProcessor * dragProcessor,
					  const sgs::DataType::Card * card);

	const sgs::DataType::Card * card() const;
	const QPoint& getLastPos() const;

private:
	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	const QPoint& normalizePosition(QPoint &unnormalizedPosition);

private:
	QPoint									m_lastPos;
	QWidget * const							m_dragBorderWidget;
	AbstractDragProcessor * const			m_dragProcessor;
	QPoint									m_dragPoint;
	bool									m_pressed;
	const sgs::DataType::Card * const		m_platformCard;
};

} /* namespace sgsui */
#endif /* DRAGABLECARDFRAME_H_ */
