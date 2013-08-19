#ifndef SGSUI_LINEANIMATIONWIDGET_H_
#define SGSUI_LINEANIMATIONWIDGET_H_

#include "sgsui_GUIStaticData.h"
#include <QFrame>
#include <QPen>
#include <vector>

class QTimer;
class QGraphicsOpacityEffect;

namespace sgsui {

class LineAnimationWidget : public QWidget
{
	Q_OBJECT

public:
	explicit LineAnimationWidget(QWidget * parent = 0);

public slots:
	void lineAnimation(const QPoint &source,
					   const std::vector<QPoint> &targetVec,
					   int duration/* = GUIStaticData::lineAnimationDuration*/);

private:
	void paintEvent(QPaintEvent*);

private:
	QPen							m_linePen;
	QPoint							m_sourcePoint;
	QPoint							m_targetPoint[7];
	int								m_fullDuration;
	int								m_updateCount;
	int								m_targetCount;
	QTimer *						m_fullTimer;

	QGraphicsOpacityEffect *		m_effect;
};

} /* namespace sgsui */
#endif /* SGSUI_LINEANIMATIONWIDGET_H_ */
