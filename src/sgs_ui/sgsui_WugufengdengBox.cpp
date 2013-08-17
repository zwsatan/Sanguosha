#include "sgsui_WugufengdengBox.h"
#include "sgsui_MainWindow.h"
#include "sgsui_CardButton.h"
#include "sgsui_nameConvert.h"
#include "sgsui_output.h"

namespace sgsui {

const QSize WugufengdengBox::ms_fixedSize(460, 360);
const QSize WugufengdengBox::ms_buttonSize(93, 170);
const QPoint WugufengdengBox::ms_cardPoint0(31, -4);
const QPoint WugufengdengBox::ms_cardPoint1(133, -4);
const QPoint WugufengdengBox::ms_cardPoint2(235, -4);
const QPoint WugufengdengBox::ms_cardPoint3(337, -4);
const QPoint WugufengdengBox::ms_cardPoint4(31, 134);
const QPoint WugufengdengBox::ms_cardPoint5(133, 134);
const QPoint WugufengdengBox::ms_cardPoint6(235, 134);
const QPoint WugufengdengBox::ms_cardPoint7(337, 134);

WugufengdengBox::WugufengdengBox(QWidget * parent)
	: BlockingFrame(parent)
	, m_isChoosing(false)
	, m_returnValue(-1)
{
	// 这里的五谷丰登最多处理8张牌,游戏设定的人数也最大是8个人
	setupUi(this);
	setFixedSize(ms_fixedSize);
	setStyleSheet("QFrame#WugufengdengBox{background-image:url(images/frame/wuguBox.png)}");

	m_points[0] = &ms_cardPoint0;
	m_points[1] = &ms_cardPoint1;
	m_points[2] = &ms_cardPoint2;
	m_points[3] = &ms_cardPoint3;
	m_points[4] = &ms_cardPoint4;
	m_points[5] = &ms_cardPoint5;
	m_points[6] = &ms_cardPoint6;
	m_points[7] = &ms_cardPoint7;

	okButton->setEnabledPixmap(QPixmap("images/button/ok.png"));
	okButton->setDisabledPixmap(QPixmap("images/button/ok-disabled.png"));
	okButton->setDisabled();

	for (int i = 0; i < 8; ++i)
	{
		m_bigFrame[i] = new QFrame(this);
		m_bigFrame[i]->setFixedSize(ms_buttonSize);
		m_bigFrame[i]->move(*m_points[i]);
		m_buttons[i] = 0;
	}
	connect(okButton, SIGNAL(clicked()), this, SLOT(unblock()));
}

void WugufengdengBox::showContainer(const std::vector<std::pair<const sgs::DataType::Card *, bool> > &wugoInfo)
{
	printDebug("WugufengdengBox::showContainer: start");
	if (isVisible())
	{
		printDebug("WugufengdengBox::showContainer: the window is already visible, quit");
		printDebug("WugufengdengBox::showContainer: over");
		return;
	}

	int x = window()->width() / 2 - width() / 2;
	int y = window()->height() / 2 - height() / 2;
	setGeometry(QRect(QPoint(x, y), size()));

	int i = 0;
	CardButton *newButton = 0;
	for (auto citer = wugoInfo.begin(); citer != wugoInfo.end(); ++citer, ++i)
	{
		newButton = new CardButton(citer->first, this, true, m_bigFrame[i]);
		m_buttons[i] = newButton;
		newButton->show();
	}
	show();
	printDebug("WugufengdengBox::showContainer: over");
}

QPoint WugufengdengBox::removeCard(int index)
{
	printDebug("WugufengdengBox::removeCard: start");
	QPoint position;
	if (m_buttons[index])
	{
		position = (MainWindow::realPos(m_buttons[index]));
		delete m_buttons[index];
		m_buttons[index] = 0;
	}
	else
	{
		printDebug("<font color=red><b>Warning: </b></font>WugufengdengBox::removeCard: removing a non-exist card, index = " + QString::number(index));
	}

	if (isEmpty())
	{
		printDebug("WugufengdengBox::removeCard: all cards removed, close");
		close();
	}

	printDebug("WugufengdengBox::removeCard: over");
	return position;
}

int WugufengdengBox::chooseWugu(const std::vector<std::pair<const sgs::DataType::Card *, bool> > &wugoInfo)
{
	printDebug("WugufengdengBox::chooseWugu: start");
	m_returnValue = -1;
	m_isChoosing = true;

	// don't hide after response
	exec(false);

	collectData();
	m_isChoosing = false;
	okButton->setDisabled();

	printDebug("WugufengdengBox::chooseWugu: chosen card pos = " + QString::number(m_returnValue));

	if (wugoInfo.at(m_returnValue).second)
		printDebug("<font color=red><b>Warning: </b></font>WugufengdengBox::chooseWugu: choosing a card which is already chosen, check bug");

	printDebug("WugufengdengBox::chooseWugu: over");
	return m_returnValue;
}

void WugufengdengBox::collectData()
{
	// pick out the selected card
	for (int i = 0; i < 8; ++i)
	{
		if (m_buttons[i] && m_buttons[i]->isSelected())
		{
			m_returnValue = i;
			break;
		}
	}

	// reset all other cards
	for (int i = 0; i < 8; ++i)
	{
		if (m_buttons[i])
		{
			m_buttons[i]->setUnselected();
			m_buttons[i]->setSelectable();
		}
	}
}

void WugufengdengBox::cardClicked(CardButton * clickedCard)
{
	if (!m_isChoosing)
		return;

	if (!clickedCard->isSelected())
	{
		// if the clicked card is not selected before click
		// set this card selected and others unselected (but still selectable)
		// set ok button enabled
		for (int i = 0; i < 8; ++i)
		{
			if (m_buttons[i])
			{
				if (m_buttons[i] == clickedCard)
				{
					m_buttons[i]->setSelected();
					okButton->setEnabled();
				}
				else
				{
					m_buttons[i]->setUnselectable();
				}
			}
		}
	}
	else
	{
		// if the clicked card is already selected before click
		// set this card unselected and all cards selectable
		// set ok button disabled
		clickedCard->setUnselected();
		for (int i = 0; i < 8; ++i)
		{
			if (m_buttons[i])
				m_buttons[i]->setSelectable();
		}
		okButton->setDisabled();
	}
}

bool WugufengdengBox::isEmpty() const
{
	for (int i = 0; i < 8; ++i)
	{
		if (m_buttons[i])
			return false;
	}
	return true;
}

void WugufengdengBox::enterKeyPressedEvent()
{
	okButton->click();
}

} /* namespace sgsui */
