#include "sgsui_GameOverBox.h"
#include "sgsui_MainWindow.h"
#include "sgsui_nameConvert.h"
#include "sgsui_AudioPlayer.h"
#include "sgsui_output.h"

namespace sgsui {

const QSize GameOverBox::ms_fixedSize(1024, 400);

GameOverBox::GameOverBox(QWidget * parent/* = 0*/)
	: BlockingFrame(parent)
{
	setupUi(this);
	setFixedSize(ms_fixedSize);
	setStyleSheet("QFrame#GameOverBox{background-image:url(images/frame/wujiangChooseBox.png)}");

	m_roleLabelVec.push_back(playerRole1);
	m_roleLabelVec.push_back(playerRole2);
	m_roleLabelVec.push_back(playerRole3);
	m_roleLabelVec.push_back(playerRole4);
	m_roleLabelVec.push_back(playerRole5);
	m_roleLabelVec.push_back(playerRole6);
	m_roleLabelVec.push_back(playerRole7);
	m_roleLabelVec.push_back(playerRole8);

	m_resultPixmapVec.push_back(playerPixmap1);
	m_resultPixmapVec.push_back(playerPixmap2);
	m_resultPixmapVec.push_back(playerPixmap3);
	m_resultPixmapVec.push_back(playerPixmap4);
	m_resultPixmapVec.push_back(playerPixmap5);
	m_resultPixmapVec.push_back(playerPixmap6);
	m_resultPixmapVec.push_back(playerPixmap7);
	m_resultPixmapVec.push_back(playerPixmap8);

	m_resultLabelVec.push_back(playerResult1);
	m_resultLabelVec.push_back(playerResult2);
	m_resultLabelVec.push_back(playerResult3);
	m_resultLabelVec.push_back(playerResult4);
	m_resultLabelVec.push_back(playerResult5);
	m_resultLabelVec.push_back(playerResult6);
	m_resultLabelVec.push_back(playerResult7);
	m_resultLabelVec.push_back(playerResult8);

	m_resultFrameVec.push_back(resultFrame1);
	m_resultFrameVec.push_back(resultFrame2);
	m_resultFrameVec.push_back(resultFrame3);
	m_resultFrameVec.push_back(resultFrame4);
	m_resultFrameVec.push_back(resultFrame5);
	m_resultFrameVec.push_back(resultFrame6);
	m_resultFrameVec.push_back(resultFrame7);
	m_resultFrameVec.push_back(resultFrame8);

	for (auto iter = m_roleLabelVec.begin(); iter != m_roleLabelVec.end(); ++iter)
		(*iter)->setFont(GUIStaticData::contentFont);

	for (auto iter = m_resultLabelVec.begin(); iter != m_resultLabelVec.end(); ++iter)
		(*iter)->setFont(GUIStaticData::contentFont);

	for (auto iter = m_resultFrameVec.begin(); iter != m_resultFrameVec.end(); ++iter)
		(*iter)->hide();

	wonLabel->hide();
	lostLabel->hide();

	okButton->setEnabledPixmap(QPixmap("images/button/ok.png"));
	okButton->setDisabledPixmap(QPixmap("images/button/ok-disabled.png"));
	connect(okButton, SIGNAL(clicked()), this, SLOT(unblock()));
}

void GameOverBox::enterKeyPressedEvent()
{
	unblock();
}

void GameOverBox::showResult(char result)
{
	GUIStaticData::setGameOver();
	MainWindow * mainWindowPtr = static_cast<MainWindow *>(window());
	m_result = result;

	for (int i = 0; i < mainWindowPtr->m_playerCount; ++i)
	{
		QString roleName = roleDisplayName(mainWindowPtr->getRoleAtIndex(i + 1));
		m_roleLabelVec.at(i)->setText(setColored(roleName, false));
		m_resultPixmapVec.at(i)->showResult(mainWindowPtr->getHeroTypeAtIndex(i + 1), mainWindowPtr->getIsDeadAtIndex(i + 1));
		m_resultLabelVec.at(i)->setText(setColored(resultDisplayStr(getResult(i + 1)), false));
		m_resultFrameVec.at(i)->show();
	}

	int x = window()->width() / 2 - width() / 2;
	int y = window()->height() / 2 - height() / 2;
	setGeometry(QRect(QPoint(x, y), size()));

	if (getResult(mainWindowPtr->m_playerIndex))
	{
		mainWindowPtr->m_audioPlayer->playEndingMusic(true);
		wonLabel->show();
	}
	else
	{
		mainWindowPtr->m_audioPlayer->playEndingMusic(false);
		lostLabel->show();
	}

	exec();
	// now InitiateBox will determinate whether to quit or to play again
}

bool GameOverBox::getResult(int index)
{
	--index;
	return (m_result >> index) & 1;
}

} /* namespace sgsui */
