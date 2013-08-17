#include "sgsui_CardViewer.h"
#include "sgsdata_card.h"
#include "sgsui_nameConvert.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_CardFrame.h"

namespace sgsui {

const QSize CardViewer::ms_fixedSize(560, 290);

CardViewer::CardViewer(QWidget * parent/* = 0*/)
	: QFrame(parent)
	, m_card1(0)
	, m_card2(0)
	, m_cardFrame1(0)
	, m_cardFrame2(0)
{
	setupUi(this);
	setFixedSize(ms_fixedSize);
	setObjectName("CardViewer");
	setStyleSheet("QFrame#CardViewer{background-image:url(images/frame/cardViewer.png)}");

	messageLabel->setFont(GUIStaticData::headingFont);
}

void CardViewer::showViewer(const sgs::DataType::Card * card1, const sgs::DataType::Card * card2)
{
	if (m_cardFrame1)
	{
		delete m_cardFrame1;
		m_cardFrame1 = 0;
	}

	if (m_cardFrame2)
	{
		delete m_cardFrame2;
		m_cardFrame2 = 0;
	}

	m_card1 = card1;
	m_card2 = card2;

	if (!m_card1 || !m_card2)
		return;

	m_cardFrame1 = new CardFrame(m_card1->type(), m_card1->color(), m_card1->number(), cardLabel1);
	m_cardFrame2 = new CardFrame(m_card2->type(), m_card2->color(), m_card2->number(), cardLabel2);
	m_cardFrame2->setDimed(true);
	m_cardFrame1->show();
	m_cardFrame2->show();

	QString text;
	text += trUtf8("请选择要获得第一张卡牌 ");
	text += cardDisplayName(m_card1->type());
	text += trUtf8(" 的玩家");

	messageLabel->setText(setColored(text));
	show();
}

void CardViewer::showSecondViewer()
{
	if (!m_card1 || !m_card2)
		return;

	if (m_cardFrame1 && m_cardFrame2)
	{
		m_cardFrame1->setDimed(true);
		m_cardFrame2->setDimed(false);
	}

	QString text;
	text += trUtf8("请选择要获得第二张卡牌");
	text += cardDisplayName(m_card2->type());
	text += trUtf8(" 的玩家");
	messageLabel->setText(setColored(text));
}

} /* namespace sgsui */
