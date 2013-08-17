#include "sgsui_FanjianBox.h"

namespace sgsui {

const QSize FanjianBox::ms_fixedSize(480, 200);

FanjianBox::FanjianBox(QWidget * parent/* = 0*/)
	: BlockingFrame(parent)
	, m_returnColor(sgs::ConstData::cardColorNone)
{
	setupUi(this);
	setFixedSize(ms_fixedSize);

	setStyleSheet("QFrame#FanjianBox{background-image:url(images/frame/dialog.png)}");

	connect(heartButton, SIGNAL(clicked()), this, SLOT(chooseHeart()));
	connect(spadeButton, SIGNAL(clicked()), this, SLOT(chooseSpade()));
	connect(diamondButton, SIGNAL(clicked()), this, SLOT(chooseDiamond()));
	connect(clubButton, SIGNAL(clicked()), this, SLOT(chooseClub()));
}

sgs::ConstData::CardColor FanjianBox::getFanjianColor()
{
	exec();
	return m_returnColor;
}

void FanjianBox::chooseHeart()
{
	m_returnColor = sgs::ConstData::HEART;
	unblock();
}

void FanjianBox::chooseSpade()
{
	m_returnColor = sgs::ConstData::SPADE;
	unblock();
}

void FanjianBox::chooseDiamond()
{
	m_returnColor = sgs::ConstData::DIAMOND;
	unblock();
}

void FanjianBox::chooseClub()
{
	m_returnColor = sgs::ConstData::CLUB;
	unblock();
}

} /* namespace sgsui */
