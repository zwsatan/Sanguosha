#include "sgsui_LineAnimationWidget.h"
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QPainter>

namespace sgsui {

LineAnimationWidget::LineAnimationWidget(QWidget * parent/* = 0*/)
	: QWidget(parent)
	, m_linePen(Qt::white, 3, Qt::SolidLine, Qt::RoundCap)
	, m_sourcePoint(0, 0)
{
	m_fullTimer = new QTimer(this);
	m_fullTimer->setSingleShot(true);

	m_effect = new QGraphicsOpacityEffect(this);
	m_effect->setOpacity(.7);
	setGraphicsEffect(m_effect);

	connect(m_fullTimer, SIGNAL(timeout()), this, SLOT(hide()));
}

void LineAnimationWidget::lineAnimation(const QPoint &source, const std::vector<QPoint> &targetVec, int duration)
{
	m_sourcePoint = source;
	m_targetCount = 0;

	for (auto citer = targetVec.begin(); citer != targetVec.end(); ++citer)
		m_targetPoint[m_targetCount++] = *citer;

	m_fullDuration = (duration > 0) ? duration : GUIStaticData::lineAnimationDuration;
	update();
	m_fullTimer->start(m_fullDuration);
	show();
}

void LineAnimationWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(m_linePen);
	for (int i = 0; i < m_targetCount; ++i)
		painter.drawLine(m_sourcePoint, m_targetPoint[i]);
}

} /* namespace sgsui */
