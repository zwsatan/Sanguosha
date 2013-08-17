#include "sgsui_CardButton.h"
#include "sgsui_CardFrame.h"
#include "sgsui_AbstractCardContainer.h"
#include "sgsui_output.h"
#include <QMouseEvent>

namespace sgsui {

const QSize CardButton::ms_fixedSize(93, 170);

CardButton::CardButton(const sgs::DataType::Card * platformCard,
					   AbstractCardContainer * container,
					   bool visible/* = true*/,
					   QWidget * parent/* = 0*/)
	: QFrame(parent)
	, m_cardContainer(container)
	, m_cardSelectable(true)
	, m_plaformCard(platformCard)
{
	setupUi(this);
	setFixedSize(ms_fixedSize);

	if (visible)
		m_cardFrame = new CardFrame(platformCard->type(), platformCard->color(), platformCard->number(), this);
	else
		m_cardFrame = new CardFrame(this);

	cardLayout->addWidget(m_cardFrame);
	lowerSpacerWidget->hide();
}

const sgs::DataType::Card * CardButton::platformCard()
{
	return m_plaformCard;
}

bool CardButton::isSelected() const
{
	return upperSpacerWidget->isHidden();
}

bool CardButton::isSelectable() const
{
	return m_cardSelectable;
}

const CardFrame* CardButton::card() const
{
	return m_cardFrame;
}

void CardButton::click()
{
	QMouseEvent temp(QEvent::MouseButtonRelease, QPoint(width() / 2, height() / 2), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	mousePressEvent(&temp);
	mouseReleaseEvent(&temp);
}

void CardButton::setSelected(bool selected/* = true*/)
{
	if (isSelectable() && (isSelected() != selected))
	{
		if (selected)
		{
			lowerSpacerWidget->show();
			upperSpacerWidget->hide();
		}
		else
		{
			upperSpacerWidget->show();
			lowerSpacerWidget->hide();
		}
	}
}

void CardButton::setUnselected(bool unselected/* = true*/)
{
	setSelected(!unselected);
}

void CardButton::setSelectable(bool selectable/* = true*/)
{
	if (!selectable)
		setUnselected();

	m_cardSelectable = selectable;
	m_cardFrame->setNotDimed(m_cardSelectable);
}

void CardButton::setUnselectable(bool unselectable) {
	setSelectable(!unselectable);
}

void CardButton::mousePressEvent(QMouseEvent * event)
{
	if (m_cardSelectable && event->button() == Qt::LeftButton)
		container()->cardClicked(this);
}

AbstractCardContainer* CardButton::container() const
{
	return m_cardContainer;
}

} /* namespace sgsui */
