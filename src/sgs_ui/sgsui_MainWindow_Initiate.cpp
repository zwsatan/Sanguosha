#include "sgsui_MainWindow.h"
#include "sgsui_ZhuangbeiArea.h"
#include "sgsui_WujiangArea.h"
#include "sgsui_PlayerArea.h"
#include "sgsui_MessageBox.h"
#include "sgsui_DialogBox.h"
#include "sgsui_CardContainer.h"
#include "sgsui_WujiangChooseBox.h"
#include "sgsui_GuanxingBox.h"
#include "sgsui_WugufengdengBox.h"
#include "sgsui_FanjianBox.h"
#include "sgsui_GameOverBox.h"
#include "sgsui_AboutFrame.h"
#include "sgsui_HelpBox.h"
#include "sgsui_PixmapAnimationWidget.h"
#include "sgsui_LineAnimationWidget.h"
#include "sgsui_PosAnimation.h"
#include "sgsui_BlockTimer.h"
#include "sgsui_ZhuangbeiLabel.h"
#include "sgsui_CardViewer.h"
#include "sgsui_AudioPlayer.h"
#include "sgsui_Shoupai.h"
#include "sgsui_output.h"
#include "sgsui_nameConvert.h"
#include <QSplashScreen>
#include <QPropertyAnimation>

namespace sgsui {

MainWindow * MainWindow::mainWindowPointer = 0;
const QSize MainWindow::ms_minSize(1024, 650);

MainWindow::MainWindow()
	: Interface(GUIStaticData::getHumanIndex() - 1)
	, m_playerCount(GUIStaticData::getPlayerCount())
	, m_playerIndex(GUIStaticData::getHumanIndex())
	, m_GUIresponding(true)
	, m_currentPlayerSeat(-1)
	, m_responseType(NotResponding)
	, m_skillVerShaOrNot(false)
	, m_isGoingToAbandon(false)
	, m_cancelResponse(false)
	, m_shoupaiNumToDiscard(0)
	, m_maxShoupaiSelect(0)
	, m_minShoupaiSelect(0)
	, m_maxPlayerSelect(0)
	, m_minPlayerSelect(0)
	, m_selectedZhuangbei(0)
	, m_selectedSkill(sgs::ConstData::heroSkillNone)
	, m_liuliAttacker(0)
{
	printDebug("MainWindow::MainWindow: start");

	// create splash
	QSplashScreen * splash = new QSplashScreen();
	splash->setPixmap(splashStr());
	splash->show();
	setWindowIcon(QIcon("images/icon.png"));
	splash->showMessage(GUIStaticData::startingGame, Qt::AlignRight | Qt::AlignTop, Qt::white);

	// create GUI objects
	setupUi(this);
	setMinimumSize(ms_minSize);
	GUIStaticData::resetData();
	setOptions(GUIStaticData::getOptions());

	splash->showMessage(GUIStaticData::initWindowWidgets, Qt::AlignRight | Qt::AlignTop, Qt::white);
	initiateWindowWidgets();

	splash->showMessage(GUIStaticData::initStyleSheet, Qt::AlignRight | Qt::AlignTop, Qt::white);
	initiateStyleSheet();

	splash->showMessage(GUIStaticData::initAnimation, Qt::AlignRight | Qt::AlignTop, Qt::white);
	initiateAnimation();

	splash->showMessage(GUIStaticData::initMusic, Qt::AlignRight | Qt::AlignTop, Qt::white);
	initiateMusic();

	splash->showMessage(GUIStaticData::initDebug, Qt::AlignRight | Qt::AlignTop, Qt::white);
	initiateDebug();

	splash->showMessage(GUIStaticData::initOver, Qt::AlignRight | Qt::AlignTop, Qt::white);

	QApplication::processEvents();

	splash->finish(this);
	delete splash;
	mainWindowPointer = this;
}

void MainWindow::setOptions(int options)
{
	//from lower to higher
	musicCheckBox->setChecked(options & 1);
	soundCheckBox->setChecked((options >> 1) & 1);
	fullscreenCheckBox->setChecked((options >> 2) & 1);
	AICheckBox->setChecked((options >> 3) & 1);
	changeScreenState(fullscreenCheckBox->isChecked());
	connect(AICheckBox, SIGNAL(clicked(bool)), this, SLOT(trustClicked(bool)));
}

MainWindow::~MainWindow()
{
	mainWindowPointer = 0;

	// Shoupai are card buttons which have no parent
	// they need to be deleted here
	for (auto iter = m_shoupaiList.begin(); iter != m_shoupaiList.end(); ++iter)
		delete *iter;
}

void MainWindow::trustClicked(bool trust)
{
	if (trust)
		showTipMessage(GUIStaticData::trustTurnedOn);
	else
		showTipMessage(GUIStaticData::trustTurnedOff);
}

void MainWindow::initiateWindowWidgets()
{
	okButton->setEnabledPixmap(QPixmap("images/button/ok.png"));
	okButton->setDisabledPixmap(QPixmap("images/button/ok-disabled.png"));
	connect(okButton, SIGNAL(clicked()), this, SLOT(okClicked()));

	cancelButton->setEnabledPixmap(QPixmap("images/button/cancel.png"));
	cancelButton->setDisabledPixmap(QPixmap("images/button/cancel-disabled.png"));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

	abandonButton->setEnabledPixmap(QPixmap("images/button/discard.png"));
	abandonButton->setDisabledPixmap(QPixmap("images/button/discard-disabled.png"));
	connect(abandonButton, SIGNAL(clicked()), this, SLOT(abandonClicked()));

	quitButton->setEnabledPixmap(QPixmap("images/button/close.png"));
	quitButton->setDisabledPixmap(QPixmap("images/button/close.png"));
	quitButton->setPressedPixmap(QPixmap("images/button/close-pressed.png"));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

	aboutButton->setEnabledPixmap(QPixmap("images/button/enabled-big.png"));
	aboutButton->setDisabledPixmap(QPixmap("images/button/disabled-big.png"));
	aboutButton->setPressedPixmap(QPixmap("images/button/pressed-big.png"));

	bigPauseButton->setEnabledPixmap(QPixmap("images/button/enabled-large.png"));
	bigPauseButton->setDisabledPixmap(QPixmap("images/button/disabled-large.png"));
	bigPauseButton->setPressedPixmap(QPixmap("images/button/pressed-large.png"));
	connect(bigPauseButton, SIGNAL(clicked()), pauseButton, SLOT(click()));

	helpButton->setEnabledPixmap(QPixmap("images/button/enabled-large.png"));
	helpButton->setDisabledPixmap(QPixmap("images/button/disabled-large.png"));
	helpButton->setPressedPixmap(QPixmap("images/button/pressed-large.png"));

	m_tipBox = new SGSMessageBox(this);
	m_tipBox->hide();
	m_pauseBox = new SGSMessageBox(this);
	m_pauseBox->hide();
	m_cardContainer = new CardContainer(this);
	m_cardContainer->hide();
	m_wujiangChooseBox = new WujiangChooseBox(this);
	m_wujiangChooseBox->hide();
	m_guanxingBox = new GuanxingBox(this);
	m_guanxingBox->hide();

	m_zhuangbeiLabel = new ZhuangbeiLabel(this);
	m_zhuangbeiLabel->hide();

	m_aboutFrame = new AboutFrame(this);
	m_aboutFrame->hide();
	connect(aboutButton, SIGNAL(clicked()), m_aboutFrame, SLOT(showAboutInfo()));

	m_helpBox = new HelpBox(this);
	m_helpBox->hide();
	connect(helpButton, SIGNAL(clicked()), m_helpBox, SLOT(displayHelp()));

	m_wugufengdengBox = new WugufengdengBox(this);
	m_wugufengdengBox->hide();

	m_fanjianBox = new FanjianBox(this);
	m_fanjianBox->hide();

	m_cardViewer = new CardViewer(this);
	m_cardViewer->hide();

	m_gameOverBox = new GameOverBox(this);
	m_gameOverBox->hide();

	cardDeckLabel->hide();

	m_blockingFrameMovingAnimation = new QPropertyAnimation(this);
	m_blockingFrameMovingAnimation->setPropertyName("pos");
	m_blockingFrameMovingAnimation->setDuration(GUIStaticData::blockingFrameMovingDuration);

	debugCheckBox->hide();
	debugTabWidget->hide();
	testWidget->hide();
	connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseClicked()));

	checkButtons();
}

void MainWindow::createPlayers()
{
	printDebug("MainWindow::createPlayers: start");

	// initiate battlefield infomation
	m_playerCount = GUIStaticData::getPlayerCount();
	m_playerIndex = GUIStaticData::getHumanIndex();

	// initiate myself
	m_zhuangbeiArea = new ZhuangbeiArea(this, zhuangbeiArea);
	m_wujiangArea = new WujiangArea(m_playerIndex, wujiangArea);
	m_innerAI = Interface::createInterface(m_playerIndex - 1, m_wujiangArea->getHeroType(), true);

	// initiate others
	switch (m_playerCount - 1)
	{
	case 1:
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 0) % m_playerCount + 1, playerFrame4));
		break;
	case 2:
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 0) % m_playerCount + 1, playerFrame3));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 1) % m_playerCount + 1, playerFrame4));
		centralPlayerLayout->removeItem(player5spacer);
		playerFrame5->hide();
		break;
	case 3:
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 0) % m_playerCount + 1, playerFrame3));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 1) % m_playerCount + 1, playerFrame4));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 2) % m_playerCount + 1, playerFrame5));
		break;
	case 4:
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 0) % m_playerCount + 1, playerFrame2));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 1) % m_playerCount + 1, playerFrame3));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 2) % m_playerCount + 1, playerFrame4));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 3) % m_playerCount + 1, playerFrame6));
		centralPlayerLayout->removeItem(player5spacer);
		playerFrame5->hide();
		break;
	case 5:
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 0) % m_playerCount + 1, playerFrame2));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 1) % m_playerCount + 1, playerFrame3));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 2) % m_playerCount + 1, playerFrame4));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 3) % m_playerCount + 1, playerFrame5));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 4) % m_playerCount + 1, playerFrame6));
		break;
	case 6:
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 0) % m_playerCount + 1, playerFrame1));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 1) % m_playerCount + 1, playerFrame2));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 2) % m_playerCount + 1, playerFrame3));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 3) % m_playerCount + 1, playerFrame4));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 4) % m_playerCount + 1, playerFrame6));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 5) % m_playerCount + 1, playerFrame7));
		centralPlayerLayout->removeItem(player5spacer);
		playerFrame5->hide();
		break;
	case 7:
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 0) % m_playerCount + 1, playerFrame1));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 1) % m_playerCount + 1, playerFrame2));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 2) % m_playerCount + 1, playerFrame3));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 3) % m_playerCount + 1, playerFrame4));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 4) % m_playerCount + 1, playerFrame5));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 5) % m_playerCount + 1, playerFrame6));
		m_otherPlayerAreaVector.push_back(new PlayerArea((m_playerIndex + 6) % m_playerCount + 1, playerFrame7));
		break;
	}

	// display role label
	int zhugongNum = 0, zhongchenNum = 0, fanzeiNum = 0, neijianNum = 0;
	for (int i = 1; i <= m_playerCount; ++i)
	{
		switch (GUIStaticData::getPlayerRole(i))
		{
		case sgs::ConstData::ZHU:
			++zhugongNum;
			break;

		case sgs::ConstData::ZHONG:
			++zhongchenNum;
			break;

		case sgs::ConstData::FAN:
			++fanzeiNum;
			break;

		case sgs::ConstData::NEI:
			++neijianNum;
			break;

		default:
			break;
		}
	}

	connect(m_wujiangArea, SIGNAL(skillClicked(sgs::ConstData::HeroSkill, int)), this, SLOT(skillClicked(sgs::ConstData::HeroSkill, int)));

	QApplication::processEvents();

	sleepSomeTime(GUIStaticData::createPlayerDuration);

	for (auto iter = m_otherPlayerAreaVector.begin(); iter != m_otherPlayerAreaVector.end(); ++iter)
		(*iter)->show();

	m_zhuangbeiArea->show();
	m_wujiangArea->show();
	roleFrame->setupRole(zhugongNum, zhongchenNum, fanzeiNum, neijianNum);
	cardDeckLabel->show();

	GUIStaticData::setPlayersCreated();

	printDebug("MainWindow::createPlayers: over");
}

void MainWindow::initiateStyleSheet()
{
	mainFrame->setStyleSheet("QWidget#mainFrame{border-image:url(images/backdrop/1.jpg)}");
	rightFrame->setStyleSheet("QWidget#rightFrame{border-image:url(images/frame/rightFrame.png)}");
	zhuangbeiArea->setStyleSheet("QWidget#zhuangbeiArea{background-image:url(images/frame/zhuangbeiArea.png)}");
	shoupaiArea->setStyleSheet("QFrame#shoupaiArea{background-image:url(images/frame/shoupaiArea.png)}");
	wujiangArea->setStyleSheet("QWidget#wujiangArea{background-image:url(images/frame/wujiangArea.png)}");
	statusbar->setStyleSheet("QLabel#statusbar{background-image:url(images/frame/statusbar.png)}");
	messageTopFrame->setStyleSheet("QFrame#messageTopFrame{background-image:url(images/frame/messagetop.png)}");
	messageMiddleFrame->setStyleSheet("QFrame#messageMiddleFrame{background-image:url(images/frame/messagemiddle.png)}");
	messageButtomFrame->setStyleSheet("QFrame#messageButtomFrame{background-image:url(images/frame/messagebuttom.png)}");
	cardDeckLabel->setStyleSheet("QLabel#cardDeckLabel{background-image:url(images/system/cardDeck.png)}");
	historyText->setStyleSheet(
				"QTextBrowser#historyText{background-image:url(images/frame/textbackground.png);}"
				"QScrollBar:vertical {margin: 13px 0px 13px 0px; border: 0px; width: 12px;}"
				".QScrollBar::handle:vertical {border-image:url(images/frame/scroll-area.png);}"
				".QScrollBar::add-page:vertical {background-color: rgb(85, 67, 53)}"
				".QScrollBar::sub-page:vertical {background-color: rgb(85, 67, 53)}"
				".QScrollBar::sub-line:vertical {subcontrol-origin: margin; border: 0px; height:12px}"
				".QScrollBar::up-arrow:vertical {border-image:url(images/frame/scroll-up-arrow.png); height:12px}"
				".QScrollBar::down-arrow:vertical {border-image:url(images/frame/scroll-down-arrow.png); height:12px}"
				".QScrollBar::add-line:vertical {subcontrol-origin: margin; border: 0px; height:12px}");
	optionFrame->setStyleSheet("QFrame#optionFrame{background-image:url(images/frame/optionFrame.png)}");
}

void MainWindow::initiateAnimation()
{
	m_cardAnimation = new PosAnimation(usedCardFrame, usedCardLayout, this);

	m_damageMovingAnimation = new QPropertyAnimation(this);
	m_damageMovingAnimation->setPropertyName("pos");
	m_damageMovingAnimation->setDuration(GUIStaticData::damageAnimationDuration);

	m_lineAnimationWidget = new LineAnimationWidget(this);
	m_lineAnimationWidget->hide();

	m_pixmapAnimationWidget = new PixmapAnimationWidget(this);
	m_pixmapAnimationWidget->hide();

	m_blockTimer = new BlockTimer(this);
}

void MainWindow::initiateMusic()
{
	printDebug("MainWindow::initiateMusic: start");

	m_audioPlayer = new AudioPlayer(this);
	printDebug("MainWindow::initiateMusic: audioPlayer created");

	m_skillPlayer = new AudioPlayer(this);
	printDebug("MainWindow::initiateMusic: skillplayer created");

	connect(musicCheckBox, SIGNAL(clicked(bool)), this, SLOT(changeMusicState(bool)));
	connect(fullscreenCheckBox, SIGNAL(clicked(bool)), this, SLOT(changeScreenState(bool)));

	if (musicCheckBox->isChecked())
	{
		printDebug("MainWindow::initiateMusic: starting background music");
		m_audioPlayer->startBGM();
		printDebug("MainWindow::initiateMusic: background music started");
	}

	printDebug("MainWindow::initiateMusic: over");
}

} /* namespace sgsui */
