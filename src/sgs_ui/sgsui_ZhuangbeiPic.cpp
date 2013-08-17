#include "sgsui_ZhuangbeiPic.h"
#include "sgsui_CardFrame.h"
#include "sgsui_MainWindow.h"
#include "sgsui_AbstractEquipProcessor.h"
#include "sgsui_nameConvert.h"
#include "sgsui_output.h"
#include <QMouseEvent>
#include <QPainter>

namespace sgsui {

const QSize ZhuangbeiPic::ms_fixedSize(146, 29);
const QPoint ZhuangbeiPic::ms_pixmapMargin(2, 2);
const QPoint ZhuangbeiPic::ms_numberMargin(110, 2);
const QPoint ZhuangbeiPic::ms_colorMargin(124, 2);
const QPoint ZhuangbeiPic::ms_pixmapMargin_selected(-2, -2);
const QPoint ZhuangbeiPic::ms_numberMargin_selected(106, -2);
const QPoint ZhuangbeiPic::ms_colorMargin_selected(120, -2);
const QColor ZhuangbeiPic::ms_unselectableColor(0, 0, 0, 150);

ZhuangbeiPic::ZhuangbeiPic(const CardFrame * newZhuangbeiCard, AbstractEquipProcessor * processor, QWidget * parent/* = 0*/)
	: QWidget(parent)
	, m_cardType(newZhuangbeiCard->getCardType())
	, m_cardColor(newZhuangbeiCard->getCardColor())
	, m_number(newZhuangbeiCard->getNumber())
	, m_processor(processor)
	, m_zhuangbeiPixmap(zhuangbeiPixmapStr(m_cardType))
	, m_colorPixmap(cardColorStr(m_cardColor))
	, m_numberPixmap(cardNumberStr(m_number, m_cardColor))
	, m_zhuangbeiSelected(false)
	, m_zhuangbeiSelectable(true)
{
	setFixedSize(ms_fixedSize);
	if (m_zhuangbeiPixmap.isNull())
	{
		printDebug( "<font color=red><b>Warning: </b></font>"
					"ZhuangbeiPic::ZhuangbeiPic: invalid Zhuangbei Pixmap "
					+ zhuangbeiPixmapStr(m_cardType)
					+ ", perhaps the cardType is not Zhuangbei Card");
	}
}

void ZhuangbeiPic::click()
{
	QMouseEvent temp(QEvent::MouseButtonRelease, QPoint(width() / 2, height() / 2), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	mousePressEvent(&temp);
	mouseReleaseEvent(&temp);
}

bool ZhuangbeiPic::isSelected() const
{
	return m_zhuangbeiSelected;
}

bool ZhuangbeiPic::isSelectable() const
{
	return m_zhuangbeiSelectable;
}

sgs::ConstData::CardType ZhuangbeiPic::getCardType() const
{
	return m_cardType;
}

sgs::ConstData::CardColor ZhuangbeiPic::getCardColor() const
{
	return m_cardColor;
}

int ZhuangbeiPic::getCardNumber() const
{
	return m_number;
}

void ZhuangbeiPic::setSelected(bool selected/* = true*/)
{
	if (isSelectable() && (isSelected() != selected))
	{
		m_zhuangbeiSelected = selected;
		update();
	}
}

void ZhuangbeiPic::setUnselected(bool unselected/* = true*/)
{
	setSelected(!unselected);
}

void ZhuangbeiPic::setSelectable(bool selectable/* = true*/)
{
	if (!selectable)
		setUnselected();

	if (m_zhuangbeiSelectable != selectable)
	{
		m_zhuangbeiSelectable = selectable;
		update();
	}
}

void ZhuangbeiPic::setUnselectable(bool unselectable/* = true*/)
{
	setSelectable(!unselectable);
}

void ZhuangbeiPic::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	if (m_zhuangbeiSelected)
	{
		painter.drawPixmap(ms_pixmapMargin, m_zhuangbeiPixmap);
		painter.drawPixmap(ms_numberMargin, m_numberPixmap);
		painter.drawPixmap(ms_colorMargin, m_colorPixmap);
	}
	else
	{
		painter.drawPixmap(ms_pixmapMargin_selected, m_zhuangbeiPixmap);
		painter.drawPixmap(ms_numberMargin_selected, m_numberPixmap);
		painter.drawPixmap(ms_colorMargin_selected, m_colorPixmap);
	}

	if (!m_zhuangbeiSelectable)
		painter.fillRect(rect(), ms_unselectableColor);
}

void ZhuangbeiPic::mousePressEvent(QMouseEvent * event)
{
	if (m_zhuangbeiSelectable && event->button() == Qt::LeftButton && static_cast<MainWindow*>(window())->isResponding())
		m_processor->zhuangbeiClicked(this);
}

} /* namespace sgsui */
