#include "sgsui_ResultPlayerLabel.h"
#include "sgsui_nameConvert.h"
#include <QPainter>

namespace sgsui {

const QSize ResultPlayerLabel::ms_fixedSize(96, 98);
const QColor ResultPlayerLabel::ms_deadColor(0, 0, 0, 100);

ResultPlayerLabel::ResultPlayerLabel(QWidget * parent)
	: QLabel(parent)
	, m_heroType(sgs::ConstData::heroTypeNone)
	, m_isDead(false)
{
	setFixedSize(ms_fixedSize);
}

void ResultPlayerLabel::showResult(sgs::ConstData::HeroType heroType, bool isDead/* = false*/)
{
	m_heroType = heroType;
	m_isDead = isDead;
}

void ResultPlayerLabel::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, QPixmap("images/frame/result.png"));
	painter.drawPixmap(1, 1, wujiangSelfPixmapStr(m_heroType));

	if (m_isDead)
	{
		painter.fillRect(QRect(QPoint(0, 0), ms_fixedSize), ms_deadColor);
		painter.drawPixmap(5, 20, QPixmap("images/death/dead.png"));
	}
}

} /* namespace sgsui */
