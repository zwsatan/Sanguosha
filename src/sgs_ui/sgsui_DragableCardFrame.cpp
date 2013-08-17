#include "sgsui_DragableCardFrame.h"
#include <QMouseEvent>

namespace sgsui {

DragableCardFrame::DragableCardFrame(QWidget * dragBorderWidget,
									 AbstractDragProcessor * dragProcessor,
									 const sgs::DataType::Card * card,
									 const QPoint &pos)
	: CardFrame(card->type(), card->color(), card->number(), dragBorderWidget)
	, m_lastPos(pos)
	, m_dragBorderWidget(dragBorderWidget)
	, m_dragProcessor(dragProcessor)
	, m_pressed(false)
	, m_platformCard(card)
{
	move(pos);
}

DragableCardFrame::DragableCardFrame(QWidget * dragBorderWidget,
									 AbstractDragProcessor * dragProcessor,
									 const sgs::DataType::Card * card)
	: CardFrame(card->type(), card->color(), card->number(), dragBorderWidget)
	, m_dragBorderWidget(dragBorderWidget)
	, m_dragProcessor(dragProcessor)
	, m_pressed(false)
	, m_platformCard(card)
{
	m_lastPos = pos();
}

const sgs::DataType::Card * DragableCardFrame::card() const
{
	return m_platformCard;
}

const QPoint& DragableCardFrame::getLastPos() const
{
	return m_lastPos;
}

void DragableCardFrame::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_lastPos = pos();
		m_dragPoint = event->pos();
		m_pressed = true;
	}
}

void DragableCardFrame::mouseMoveEvent(QMouseEvent *event)
{
	if (!m_pressed)
		return;

	raise();
	QPoint moveToPosition(event->pos() - m_dragPoint + pos());
	this->move(normalizePosition(moveToPosition));
}

void DragableCardFrame::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_pressed && event->button() == Qt::LeftButton)
	{
		move(m_dragProcessor->processDrag(this));
		m_lastPos = pos();
		m_pressed = false;
	}
}

const QPoint& DragableCardFrame::normalizePosition(QPoint &unnormalizedPosition)
{
	if (unnormalizedPosition.rx() < 0)
		unnormalizedPosition.rx() = 0;

	if (unnormalizedPosition.rx() > m_dragBorderWidget->width() - width())
		unnormalizedPosition.rx() = m_dragBorderWidget->width() - width();

	if (unnormalizedPosition.ry() < 0)
		unnormalizedPosition.ry() = 0;

	if (unnormalizedPosition.ry() > m_dragBorderWidget->height() - height())
		unnormalizedPosition.ry() = m_dragBorderWidget->height() - height();

	return unnormalizedPosition;
}

} /* namespace sgsui */
