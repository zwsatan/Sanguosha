#include "sgsui_GuanxingBox.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_nameConvert.h"
#include "sgsui_DragableCardFrame.h"
#include "sgsui_output.h"

namespace sgsui {

const QPoint GuanxingBox::ms_upperPosition0(1, 1);
const QPoint GuanxingBox::ms_upperPosition1(103, 1);
const QPoint GuanxingBox::ms_upperPosition2(205, 1);
const QPoint GuanxingBox::ms_upperPosition3(307, 1);
const QPoint GuanxingBox::ms_upperPosition4(409, 1);
const QSize GuanxingBox::ms_fixedSize(915, 400);

GuanxingBox::GuanxingBox(QWidget * parent)
	: BlockingFrame(parent)
	, m_upperCard0(0)
	, m_upperCard1(0)
	, m_upperCard2(0)
	, m_upperCard3(0)
	, m_upperCard4(0)
	, m_lowerCard0(0)
	, m_lowerCard1(0)
	, m_lowerCard2(0)
	, m_lowerCard3(0)
	, m_lowerCard4(0)
{
	setupUi(this);
	setFixedSize(ms_fixedSize);

	okButton->setEnabledPixmap(QPixmap("images/button/ok.png"));
	okButton->setDisabledPixmap(QPixmap("images/button/ok-disabled.png"));
	upHint1->setFont(GUIStaticData::contentFont);
	upHint2->setFont(GUIStaticData::contentFont);
	downHint1->setFont(GUIStaticData::contentFont);
	downHint2->setFont(GUIStaticData::contentFont);
	hintLabel1->setFont(GUIStaticData::contentFont);
	hintLabel2->setFont(GUIStaticData::contentFont);

	m_upperCard[0] = &m_upperCard0;
	m_upperCard[1] = &m_upperCard1;
	m_upperCard[2] = &m_upperCard2;
	m_upperCard[3] = &m_upperCard3;
	m_upperCard[4] = &m_upperCard4;

	m_lowerCard[0] = &m_lowerCard0;
	m_lowerCard[1] = &m_lowerCard1;
	m_lowerCard[2] = &m_lowerCard2;
	m_lowerCard[3] = &m_lowerCard3;
	m_lowerCard[4] = &m_lowerCard4;

	m_upperPosition[0] = &ms_upperPosition0;
	m_upperPosition[1] = &ms_upperPosition1;
	m_upperPosition[2] = &ms_upperPosition2;
	m_upperPosition[3] = &ms_upperPosition3;
	m_upperPosition[4] = &ms_upperPosition4;

	setStyleSheet("QFrame#GuanxingBox{background-image:url(images/frame/guanxingBox.png)}");

	zhugeliangLabel->showResult(sgs::ConstData::LAOZHUGE);

	connect(okButton, SIGNAL(clicked()), this, SLOT(unblock()));
}

void GuanxingBox::enterKeyPressedEvent()
{
	okButton->click();
}

void GuanxingBox::chooseGuanxing(std::vector<const sgs::DataType::Card *> &top, std::vector<const sgs::DataType::Card *> &buttom)
{
	printDebug("GuanxingBox::chooseGuanxing: start");
	int x = window()->width() / 2 - width() / 2;
	int y = window()->height() / 2 - height() / 2;
	setGeometry(QRect(QPoint(x, y), size()));

	int i = 0;
	for (auto iter = top.begin(); iter != top.end(); ++iter, ++i)
	{
		printDebug("GuanxingBox::chooseGuanxing: adding card");
		// Note: upperCard[i] is DragableCardFrame**
		// So *upperCard[i] is DragableCardFrame*
		*(m_upperCard[i]) = new DragableCardFrame(middleFrame, this, *iter, *(m_upperPosition[i]));
	}
	printDebug("GuanxingBox::chooseGuanxing: cards created");

	exec();
	top.clear();
	buttom.clear();

	for (int i = 0; i < 5; ++i)
	{
		DragableCardFrame * upper = *(m_upperCard[i]);
		DragableCardFrame * lower = *(m_lowerCard[i]);
		// Note: upperCard[i] is DragableCardFrame**
		// So *upperCard[i] is DragableCardFrame*
		if (upper)
			top.push_back(upper->card());
		if (lower)
			buttom.push_back(lower->card());
	}

	clearData();
}

QPoint GuanxingBox::processDrag(DragableCardFrame * card)
{
	DragableCardFrame *& currentPositionPointer = getPointer(card->pos());
	DragableCardFrame *& lastPositionPointer = getPointer(card->getLastPos());

	if (currentPositionPointer != lastPositionPointer)
		lastPositionPointer = 0;

	if (currentPositionPointer)
	{
		currentPositionPointer->move(card->getLastPos());
		getPointer(currentPositionPointer->pos()) = currentPositionPointer;
	}

	currentPositionPointer = card;
	return posOfPointer(getPointer(card->pos()));
}

DragableCardFrame *& GuanxingBox::getPointer(const QPoint &currectPos)
{
	if (currectPos.x() < 52)
		return (currectPos.y() < 73) ? m_upperCard0 : m_lowerCard0;
	else if (currectPos.x() < 154)
		return (currectPos.y() < 73) ? m_upperCard1 : m_lowerCard1;
	else if (currectPos.x() < 256)
		return (currectPos.y() < 73) ? m_upperCard2 : m_lowerCard2;
	else if (currectPos.x() < 358)
		return (currectPos.y() < 73) ? m_upperCard3 : m_lowerCard3;
	else
		return (currectPos.y() < 73) ? m_upperCard4 : m_lowerCard4;
}

QPoint GuanxingBox::posOfPointer(DragableCardFrame * const & pointer) const
{
	if (&pointer == &m_upperCard0)
		return QPoint(1, 1);
	if (&pointer == &m_upperCard1)
		return QPoint(103, 1);
	if (&pointer == &m_upperCard2)
		return QPoint(205, 1);
	if (&pointer == &m_upperCard3)
		return QPoint(307, 1);
	if (&pointer == &m_upperCard4)
		return QPoint(409, 1);
	if (&pointer == &m_lowerCard0)
		return QPoint(1, 145);
	if (&pointer == &m_lowerCard1)
		return QPoint(103, 145);
	if (&pointer == &m_lowerCard2)
		return QPoint(205, 145);
	if (&pointer == &m_lowerCard3)
		return QPoint(307, 145);
	if (&pointer == &m_lowerCard4)
		return QPoint(409, 145);

	printDebug("<font color=red><b>Warning: </b></font>"
			   "GuanxingBox::posOfPointer: "
			   "cannot trace to position from the pointer");
	return QPoint();
}

void GuanxingBox::clearData()
{
	if (m_upperCard0)
	{
		delete m_upperCard0;
		m_upperCard0 = 0;
	}
	if (m_upperCard1)
	{
		delete m_upperCard1;
		m_upperCard1 = 0;
	}
	if (m_upperCard2)
	{
		delete m_upperCard2;
		m_upperCard2 = 0;
	}
	if (m_upperCard3)
	{
		delete m_upperCard3;
		m_upperCard3 = 0;
	}
	if (m_upperCard4)
	{
		delete m_upperCard4;
		m_upperCard4 = 0;
	}
	if (m_lowerCard0) {
		delete m_lowerCard0;
		m_lowerCard0 = 0;
	}
	if (m_lowerCard1)
	{
		delete m_lowerCard1;
		m_lowerCard1 = 0;
	}
	if (m_lowerCard2)
	{
		delete m_lowerCard2;
		m_lowerCard2 = 0;
	}
	if (m_lowerCard3)
	{
		delete m_lowerCard3;
		m_lowerCard3 = 0;
	}
	if (m_lowerCard4)
	{
		delete m_lowerCard4;
		m_lowerCard4 = 0;
	}
}

} /* namespace sgsui */
