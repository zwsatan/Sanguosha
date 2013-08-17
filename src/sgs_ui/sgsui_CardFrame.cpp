#include "sgsui_CardFrame.h"
#include "sgsui_nameConvert.h"
#include "sgsui_GUIStaticData.h"
#include <QPainter>

using namespace sgs::ConstData;

namespace sgsui {

const QSize CardFrame::ms_fixedSize(93, 130);
const QPoint CardFrame::ms_numberMargin(0, 3);
const QPoint CardFrame::ms_colorMargin(0, 18);
const QPoint CardFrame::ms_wujiangMiniMargin(51, 0);
const QPoint CardFrame::ms_judgePixmapMargin(-40, 15);

const QColor CardFrame::ms_unselectableColor(0, 0, 0, 100);
const QRect CardFrame::ms_judgeTextRect(0, 50, 93, 130);

CardFrame::CardFrame(CardType cardType, CardColor cardColor, int number, QWidget * parent/* = 0*/)
	: QWidget(parent)
	, m_judge(false)
	, m_wujiangMiniPixmap()
	, m_judgeString()
	, m_cardType(cardType)
	, m_cardColor(cardColor)
	, m_number(number)
	, m_cardPixmap(cardPixmapStr(m_cardType))
	, m_colorPixmap(cardColorStr(m_cardColor))
	, m_numberPixmap(cardNumberStr(m_number, m_cardColor))
	, m_dimed(false)
	, m_judgeResult(false)
{
	setFixedSize(ms_fixedSize);
}

CardFrame::CardFrame(HeroType heroType, CardType cardType, CardColor cardColor, int number, bool result, QWidget * parent/* = 0*/)
	: QWidget(parent)
	, m_judge(true)
	, m_wujiangMiniPixmap(wujiangMiniPixmapStr(heroType))
	, m_judgeString(wujiangDisplayName(heroType).append(GUIStaticData::judgeCardText))
	, m_cardType(cardType)
	, m_cardColor(cardColor)
	, m_number(number)
	, m_cardPixmap(cardPixmapStr(m_cardType))
	, m_colorPixmap(cardColorStr(m_cardColor))
	, m_numberPixmap(cardNumberStr(m_number, m_cardColor))
	, m_dimed(false)
	, m_judgeResult(result)
	, m_judgePixmap(m_judgeResult ? QString("images/system/positive.png") : QString("images/system/negative.png"))
	, m_judgeTextColor(isZhuangbeiCard(cardType) || isJinnangCard(cardType) ? QColor(255, 0, 0) : QColor(255, 255, 0))
{
	setFixedSize(ms_fixedSize);
}

CardFrame::CardFrame(QWidget * parent/* = 0*/)
	: QWidget(parent)
	, m_judge(false)
	, m_cardType(cardTypeNone)
	, m_cardColor(cardColorNone)
	, m_number(0)
	, m_cardPixmap("images/card/card-back.jpg")
	, m_dimed(false)
	, m_judgeResult(false)
{
	setFixedSize(ms_fixedSize);
}

QPoint CardFrame::topLeftPoint() const
{
	QPoint topLeftCoordinate = pos();
	QWidget *thisWidget = parentWidget();
	while (thisWidget != window())
	{
		topLeftCoordinate += thisWidget->pos();
		thisWidget = thisWidget->parentWidget();
	}

	return topLeftCoordinate;
}

bool CardFrame::isDimed() const
{
	return m_dimed;
}

void CardFrame::setDimed(bool dim)
{
	m_dimed = dim;
	update();
}

void CardFrame::setNotDimed(bool notDim)
{
	setDimed(!notDim);
}

void CardFrame::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, m_cardPixmap);
	painter.drawPixmap(ms_numberMargin, m_numberPixmap);
	painter.drawPixmap(ms_colorMargin, m_colorPixmap);

	if (m_dimed)
		painter.fillRect(QRect(QPoint(0, 0), ms_fixedSize), ms_unselectableColor);

	if (m_judge)
	{
		painter.setPen(m_judgeTextColor);
		painter.drawPixmap(ms_judgePixmapMargin, m_judgePixmap);
		painter.drawPixmap(ms_wujiangMiniMargin, m_wujiangMiniPixmap);
		painter.setFont(GUIStaticData::statusbarFont);
		painter.drawText(ms_judgeTextRect, Qt::AlignHCenter | Qt::AlignVCenter, m_judgeString);
	}
}

CardType CardFrame::getCardType() const
{
	return m_cardType;
}

CardColor CardFrame::getCardColor() const
{
	return m_cardColor;
}

int CardFrame::getNumber() const
{
	return m_number;
}

} /* namespace sgsui */
