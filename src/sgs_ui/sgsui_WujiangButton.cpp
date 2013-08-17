#include "sgsui_WujiangButton.h"
#include "sgsui_nameConvert.h"
#include <QPainter>
#include <QMouseEvent>

namespace sgsui {

const QPoint WujiangButton::ms_selectedMargin(4, 4);
const QPoint WujiangButton::ms_unselectedMargin(0, 0);

WujiangButton::WujiangButton(QWidget * parent/* = 0*/)
	: QLabel(parent)
	, m_clicked(false)
	, m_heroType(sgs::ConstData::heroTypeNone)
{
}

void WujiangButton::setWujiang(sgs::ConstData::HeroType heroType)
{
	m_heroType = heroType;
	m_wujiangCardPixmap.load(wujiangCardPixmapStr(m_heroType));
	setFixedSize(m_wujiangCardPixmap.size());
}

void WujiangButton::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	if (m_clicked)
		painter.drawPixmap(ms_selectedMargin, m_wujiangCardPixmap);
	else
		painter.drawPixmap(ms_unselectedMargin, m_wujiangCardPixmap);
}

void WujiangButton::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_clicked = true;
		update();
	}
}

void WujiangButton::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_clicked = false;
		if (QRect(QPoint(0, 0), size()).contains(event->pos()))
			emit choosen(m_heroType);
		update();
	}
}

} /* namespace sgsui */
