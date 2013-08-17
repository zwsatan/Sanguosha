#include "sgsui_MainWindow.h"
#include "sgsui_ZhuangbeiArea.h"
#include "sgsui_WujiangArea.h"
#include "sgsui_PlayerArea.h"
#include "sgsui_Shoupai.h"
#include "sgsui_MessageBox.h"
#include "sgsui_DialogBox.h"
#include "sgsui_CardContainer.h"
#include "sgsui_WujiangChooseBox.h"
#include "sgsui_GuanxingBox.h"
#include "sgsui_FanjianBox.h"
#include "sgsui_GameOverBox.h"
#include "sgsui_PixmapAnimationWidget.h"
#include "sgsui_LineAnimationWidget.h"
#include "sgsui_ZhuangbeiLabel.h"
#include "sgsui_WugufengdengBox.h"
#include "sgsui_BlockTimer.h"
#include "sgsui_ZhuangbeiPic.h"
#include "sgsui_AudioPlayer.h"
#include "sgsui_nameConvert.h"
#include "sgsui_output.h"
#include "sgsdata_enum.h"
#include <QPropertyAnimation>
#include <QCloseEvent>
#include <QKeyEvent>

namespace sgsui {

MainWindow * MainWindow::mainWindowPtr()
{
	return mainWindowPointer;
}

void MainWindow::appendHistory(const QString &textInput)
{
	if (!mainWindowPointer)
		return;

	mainWindowPointer->historyText->append(setColored(textInput, false));
}

void MainWindow::appendDebug(const QString &textInput)
{
	if (!mainWindowPointer)
		return;

	mainWindowPointer->debugText->append(textInput);
}

void MainWindow::exec()
{
	if (!m_blockLoop.isRunning())
	{
		emit blocked();
		printDebug("<font color=red><b>Notice: </b></font>"
				   "MainWindow::exec: MainWindow is blocked");
		m_blockLoop.exec();
		emit unblocked();
		printDebug("<font color=red><b>Notice: </b></font>"
				   "MainWindow::exec: MainWindow is unblocked");
	}
	else
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "MainWindow::exec: MainWindow is already blocking!");
	}
}

void MainWindow::sleepSomeTime(int sleepTime)
{
	if (m_blockTimer)
		m_blockTimer->block(sleepTime);
}

void MainWindow::unblock()
{
	if (m_blockLoop.isRunning())
		m_blockLoop.quit();
}

bool MainWindow::isSingleStep() const
{
	return singleCheckBox->isChecked();
}

void MainWindow::pause()
{
	pauseButton->click();
}

void MainWindow::pauseClicked()
{
	if (m_responseType != NotResponding || !m_blockingFrameList.empty())
	{
		pauseButton->setChecked(!pauseButton->isChecked());
		return;
	}

	if (pauseButton->isChecked())
	{
		showPauseMessage(singleCheckBox->isChecked() ? GUIStaticData::singleStep : GUIStaticData::gamePaused);
		exec();
		m_pauseBox->hide();
		statusbar->clearMessage();
	}
	else
	{
		unblock();
	}
}

bool MainWindow::isBlocked() const
{
	return m_blockLoop.isRunning();
}

bool MainWindow::isResponding() const
{
	if (!m_GUIresponding && m_blockingFrameMovingAnimation->state() != QAbstractAnimation::Running)
	{
		if (!m_blockingFrameList.empty())
		{
			QPoint position = m_blockingFrameList.back()->pos();
			QPoint translate(10, 0);
			m_blockingFrameMovingAnimation->setTargetObject(m_blockingFrameList.back());
			m_blockingFrameMovingAnimation->setStartValue(position);
			m_blockingFrameMovingAnimation->setKeyValueAt(0.25, position + translate);
			m_blockingFrameMovingAnimation->setKeyValueAt(0.75, position - translate);
			m_blockingFrameMovingAnimation->setEndValue(position);
			m_blockingFrameMovingAnimation->start();
		}
		else
		{
			printDebug("<font color=red><b>Warning: </b></font>"
					   "MainWindow::isResponding(): "
					   "MainWindow has been set NOT responding while no widget is blocking it");
		}
	}
	return m_GUIresponding && m_responseType != NotResponding;
}

bool MainWindow::isRespondingNoAnimation() const
{
	return (m_responseType != NotResponding) && m_GUIresponding;
}

void MainWindow::setResponding(bool responding, BlockingFrame *blockWidget)
{
	if (responding)
	{
		m_blockingFrameList.remove(blockWidget);
		m_GUIresponding = m_blockingFrameList.empty();
	}
	else
	{
		m_blockingFrameList.push_back(blockWidget);
		m_GUIresponding = false;
	}
}

bool MainWindow::isSoundOn() const
{
	return soundCheckBox->isChecked();
}

void MainWindow::addUsedCard(CardFrame * usedCard)
{
	if (usedCardLayout->count() > GUIStaticData::maxUsedCardContain)
		delete usedCardLayout->takeAt(1)->widget();
	usedCardLayout->addWidget(usedCard);
}

void MainWindow::addShoupai(const sgs::DataType::Card * card)
{
	Shoupai* addedShoupai = new Shoupai(card, this);
	shoupaiLayout->addWidget(addedShoupai);
	m_shoupaiList.push_back(addedShoupai);
	printDebug("MainWindow::addShoupai: adding card " + cardFullDisplayName(card, false));
}

void MainWindow::removeShoupai(Shoupai * shoupaiToRemove)
{
	shoupaiLayout->removeWidget(shoupaiToRemove);
	m_shoupaiList.remove(shoupaiToRemove);
	delete shoupaiToRemove;
}

void MainWindow::endResponse()
{
	// this function should only be called at the end of a response
	// it set every Shoupai and PlayerArea and ZhuangbeiPic selectable

	m_responseType = NotResponding;

	terminateCardSelect();
	terminatePlayerSelect();
	terminateZhuangbeiSelect();
	terminateSkillSelect();
	checkButtons();

	// clear data
	m_isGoingToAbandon = false;
	m_selectedZhuangbei = 0;
	m_selectedSkill = sgs::ConstData::heroSkillNone;
	m_cancelResponse = false;
	m_selectedShoupaiList.clear();
	m_selectedPlayerAreaList.clear();
	m_maxShoupaiSelect = 0;
	m_minShoupaiSelect = 0;
	m_maxPlayerSelect = 0;
	m_minPlayerSelect = 0;
	m_skillVerShaOrNot = false;
	m_tuxiList.clear();
	m_selectedSkillCardVec.clear();
	m_liuliAttacker = 0;
	printDebug("MainWindow::endResponse: response is over");
}

void MainWindow::terminatePlayerSelect()
{
	auto iter = m_otherPlayerAreaVector.begin();
	while (iter != m_otherPlayerAreaVector.end())
	{
		(*iter)->setSelectable();
		(*iter)->setUnselected();
		++iter;
	}

	m_wujiangArea->setSelectable();
	m_wujiangArea->setUnselected();
}

void MainWindow::terminateCardSelect()
{
	for (auto iter = m_shoupaiList.begin(); iter != m_shoupaiList.end(); ++iter)
	{
		(*iter)->setUnselected();
		(*iter)->setSelectable();
	}
}

void MainWindow::terminateZhuangbeiSelect()
{
	m_zhuangbeiArea->setSelectable(0, true, true);
	m_zhuangbeiArea->setSelectable(1, true, true);
	m_zhuangbeiArea->setSelectable(2, true, true);
	m_zhuangbeiArea->setSelectable(3, true, true);
}

void MainWindow::terminateSkillSelect()
{
	m_wujiangArea->disableAllSkillButtons();
}

void MainWindow::changeMusicState(bool musicOn)
{
	if (musicOn)
		m_audioPlayer->startBGM();
	else
		m_audioPlayer->pauseBGM();
}

void MainWindow::changeScreenState(bool fullscreen)
{
	if (fullscreen)
		setWindowState(Qt::WindowFullScreen);
	else
		setWindowState(Qt::WindowMaximized);
}

QPoint MainWindow::cardDeckPoint() const
{
	return realPos(cardDeckLabel);
}

QPoint MainWindow::lastShoupaiPoint() const
{
	int cardFrameWidth = 93, verticalMargin = 37;
	int maxHorizontalPos = shoupaiArea->width() - cardFrameWidth;
	int existingDeckWidth = static_cast<int>(cardFrameWidth * m_shoupaiList.size());
	int offsetX = existingDeckWidth > maxHorizontalPos ? maxHorizontalPos : existingDeckWidth;
	int offsetY = verticalMargin;
	return realPos(shoupaiArea) + QPoint(offsetX, offsetY);
}

QPoint MainWindow::shoupaiEndPoint() const
{
	int verticalMargin = 37;
	return realPos(shoupaiArea) + QPoint(shoupaiArea->width(), verticalMargin);
}

QPoint MainWindow::usedCardPoint() const
{
	return realPos(usedCardFrame);
}

QPoint MainWindow::zhuangbeiPoint() const
{
	QPoint zhuangbeiMargin(20, 20);
	return realPos(zhuangbeiArea) + zhuangbeiMargin;
}

QPoint MainWindow::shoupaiCenterPoint() const
{
	return realPos(shoupaiArea) + QPoint(shoupaiArea->size().width() / 2, 20);
}

QPoint MainWindow::wujiangCenterPoint() const
{
	int offsetX = wujiangArea->size().width() / 2;
	int offsetY = wujiangArea->size().height() / 2;
	return realPos(wujiangArea) + QPoint(offsetX, offsetY);
}

// 获取子部件的绝对位移
QPoint MainWindow::realPos(QWidget * widgetToCal)
{
	QPoint topLeftCoordinate = widgetToCal->pos();
	QWidget * thisWidget = widgetToCal->parentWidget();
	while (thisWidget != widgetToCal->window())
	{
		topLeftCoordinate += thisWidget->pos();
		thisWidget = thisWidget->parentWidget();
	}

	return topLeftCoordinate;
}

void MainWindow::showWugu(const std::vector<std::pair<const sgs::DataType::Card *, bool> >& wuguCards)
{
	m_wugufengdengBox->showContainer(wuguCards);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	raise();
	if (getDialogResponse(GUIStaticData::quitOrNot) == 0)
	{
		event->ignore();
		return;
	}

	QApplication::processEvents();
	GUIStaticData::quitGame();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	// PlayerArea
	if (event->modifiers() == Qt::AltModifier)
	{
		for (int i = Qt::Key_0; i < Qt::Key_7; ++i)
		{
			if (event->key() == i)
			{
				int playArea = i - Qt::Key_0;
				AbstractPlayerArea * clickedPlayer = goToAbstractPlayerArea(playArea);
				if (clickedPlayer)
					clickedPlayer->click();
				return;
			}
		}
	}

	// zhuangbei
	if (event->modifiers() == Qt::ControlModifier)
	{
		ZhuangbeiPic* clickedZhuangbei;
		if (event->key() == Qt::Key_1)
		{
			clickedZhuangbei = m_zhuangbeiArea->getWuqiPic();
			if (clickedZhuangbei)
				clickedZhuangbei->click();
			return;
		}
		if (event->key() == Qt::Key_2)
		{
			clickedZhuangbei = m_zhuangbeiArea->getFangjuPic();
			if (clickedZhuangbei)
				clickedZhuangbei->click();
			return;
		}
		if (event->key() == Qt::Key_3)
		{
			clickedZhuangbei = m_zhuangbeiArea->getJianmaPic();
			if (clickedZhuangbei)
				clickedZhuangbei->click();
			return;
		}
		if (event->key() == Qt::Key_4)
		{
			clickedZhuangbei = m_zhuangbeiArea->getJiamaPic();
			if (clickedZhuangbei)
				clickedZhuangbei->click();
			return;
		}
	}

	if ((event->key() == Qt::Key_F7 && event->modifiers() == Qt::NoModifier)
		|| (event->key() == Qt::Key_P && event->modifiers() == Qt::ControlModifier))
	{
		pauseButton->click();
		return;
	}

	if (event->key() == Qt::Key_F12 && event->modifiers() == Qt::NoModifier)
	{
		singleCheckBox->click();
		return;
	}

	if ((event->key() == Qt::Key_F1 && event->modifiers() == Qt::NoModifier)
		|| (event->key() == Qt::Key_H && event->modifiers() == Qt::ControlModifier))
	{
		helpButton->click();
		return;
	}

	if (event->key() == Qt::Key_M && event->modifiers() == Qt::ControlModifier)
	{
		musicCheckBox->click();
		return;
	}

	if (event->key() == Qt::Key_S && event->modifiers() == Qt::ControlModifier)
	{
		soundCheckBox->click();
		return;
	}

	if (event->key() == Qt::Key_F && event->modifiers() == Qt::ControlModifier)
	{
		fullscreenCheckBox->click();
		return;
	}

	if (event->key() == Qt::Key_T && event->modifiers() == Qt::ControlModifier)
	{
		AICheckBox->click();
		return;
	}

	if ((event->key() == Qt::Key_F11 && event->modifiers() == Qt::NoModifier)
		|| (event->key() == Qt::Key_D && event->modifiers() == Qt::ControlModifier))
	{
		debugCheckBox->click();
		if (debugCheckBox->isChecked())
			showTipMessage(GUIStaticData::debugTurnedOn);
		else
			showTipMessage(GUIStaticData::debugTurnedOff);
		return;
	}

	if (event->key() == Qt::Key_Q && event->modifiers() == Qt::ControlModifier)
	{
		close();
		return;
	}

	if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
		&& event->modifiers() == Qt::NoModifier)
	{
		if (!m_blockingFrameList.empty())
			m_blockingFrameList.back()->enterKeyPressedEvent();
		else
			okButton->click();
		return;
	}

	if (event->key() == Qt::Key_Escape && event->modifiers() == Qt::NoModifier)
	{
		if (!m_blockingFrameList.empty())
			m_blockingFrameList.back()->escapeKeyPressedEvent();
		else
			cancelButton->click();
		return;
	}

	if (event->key() == Qt::Key_Delete && event->modifiers() == Qt::NoModifier)
	{
		abandonButton->click();
		return;
	}

	// shoupai select
	for (int i = Qt::Key_1; i < Qt::Key_9; ++i)
	{
		if (event->key() == i && event->modifiers() == Qt::NoModifier)
		{
			int shouPaiNum = i - Qt::Key_1;
			CardButton * shoupai = goToShoupai(shouPaiNum);
			if (shoupai)
				shoupai->click();
			return;
		}
	}

	if (event->key() == Qt::Key_0 && event->modifiers() == Qt::NoModifier)
	{
		if (m_responseType == NotResponding)
			return;

		if (m_responseType == DiscardCard)
		{
			std::vector<Shoupai *> selected_copy(m_selectedAbandonList.begin(), m_selectedAbandonList.end());
			for (auto iter = selected_copy.begin(); iter != selected_copy.end(); ++iter)
				(*iter)->click();
		}
		else
		{
			std::vector<Shoupai *> selected_copy(m_selectedShoupaiList.begin(), m_selectedShoupaiList.end());
			for (auto iter = selected_copy.begin(); iter != selected_copy.end(); ++iter)
				(*iter)->click();
		}
		return;
	}
	QMainWindow::keyPressEvent(event);
}

CardButton* MainWindow::goToShoupai(unsigned shoupaiIndex)
{
	// shoupaiIndex starts from 0
	if (m_shoupaiList.size() > shoupaiIndex)
	{
		auto iter = m_shoupaiList.begin();
		for (unsigned i = 0; i < shoupaiIndex; ++i)
			++iter;
		return *iter;
	}

	return 0;
}

AbstractPlayerArea* MainWindow::goToAbstractPlayerArea(int pressedKey)
{
	if (pressedKey <= 0)
		return m_wujiangArea;
	else if (pressedKey < GUIStaticData::getPlayerCount())
		return otherPlayerAreaAtIndex(pressedKey + 1);
	return 0;
}

void MainWindow::runPixmapAnimation(int targetPlayerIndex, PixmapAnimationType animationType)
{
	if (targetPlayerIndex < 1 || targetPlayerIndex > m_playerCount)
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "MainWindow::runPixmapAnimation: invalid target index");
		return;
	}

	printDebug("MainWindow::runPixmapAnimation: start, target index (seat + 1) = " + QString::number(targetPlayerIndex));

	QPoint targetPoint = animationPointAtIndex(targetPlayerIndex);
	m_pixmapAnimationWidget->pixmapAnimation(targetPoint, animationType);

	if (animationType == DamageAnimation || animationType == LightningAnimation)
	{
		if (targetPlayerIndex != m_playerIndex)
		{
			QWidget* target = otherPlayerAreaAtIndex(targetPlayerIndex)->parentWidget();
			m_damageMovingAnimation->setTargetObject(target);
			QPoint originPos = target->pos();
			m_damageMovingAnimation->setStartValue(originPos);
			m_damageMovingAnimation->setKeyValueAt(0.5, originPos + QPoint(20, 0));
			m_damageMovingAnimation->setEndValue(originPos);
			m_damageMovingAnimation->start();
		}

		if (animationType == DamageAnimation)
			m_audioPlayer->playSound(DamageEffectSound, getSexAtIndex(targetPlayerIndex) == sgs::ConstData::MALE);
		else
			m_audioPlayer->playSound(LightningEffectSound);
	}
}

void MainWindow::showTipMessage(const QString &tip, bool richText/* = false*/, int duration/* = GUIStaticData::showTipDuration*/)
{
	if (duration <= 0)
		duration = GUIStaticData::showTipDuration;

	QPoint pos = mainFrame->geometry().center() - QPoint(m_tipBox->width() / 2, m_tipBox->height() / 2);
	QRect rect(pos, m_tipBox->size());
	m_tipBox->setGeometry(rect);
	m_tipBox->showTipMessage(tip, richText, duration);
	statusbar->showStatusBarMessage(tip, richText, duration);
}

void MainWindow::showPauseMessage(const QString &pause, bool richText)
{
	QPoint pos = mainFrame->geometry().center() - QPoint(m_pauseBox->width() / 2, m_pauseBox->height() / 2);
	QRect rect(pos, m_pauseBox->size());

	m_pauseBox->setGeometry(rect);
	m_pauseBox->showPauseMessage(pause);

	// -1 means that this statusbar message does not disappear automatically
	statusbar->showStatusBarMessage(pause, richText, -1);
}

void MainWindow::showStatusBarMessage(const QString &tip, bool richText, int duration)
{
	statusbar->showStatusBarMessage(tip, richText, duration);
}

int MainWindow::getDialogResponse(const QString &message)
{
	m_dialogBox = new SGS_DialogBox(this);
	QPoint pos = mainFrame->geometry().center() - QPoint(m_dialogBox->width() / 2, m_dialogBox->height() / 2);
	m_dialogBox->setGeometry(QRect(pos, m_dialogBox->size()));
	int returnValue = m_dialogBox->exec(message);
	delete m_dialogBox;
	m_dialogBox = 0;

	return returnValue;
}

sgs::ConstData::HeroType MainWindow::chooseWujiang(
		sgs::ConstData::HeroType zhugong,
		sgs::ConstData::PlayerRole shenfen,
		sgs::ConstData::HeroType avaliable1,
		sgs::ConstData::HeroType avaliable2,
		sgs::ConstData::HeroType avaliable3)
{
	sgs::ConstData::HeroType result = m_wujiangChooseBox->chooseWujiang(zhugong, shenfen, avaliable1, avaliable2, avaliable3);
	m_audioPlayer->playSound(PlayersCreated);
	return result;
}

sgs::ConstData::HeroType MainWindow::chooseZhugongWujiang(
		sgs::ConstData::HeroType avaliable1,
		sgs::ConstData::HeroType avaliable2,
		sgs::ConstData::HeroType avaliable3,
		sgs::ConstData::HeroType avaliable4,
		sgs::ConstData::HeroType avaliable5)
{
	sgs::ConstData::HeroType result = m_wujiangChooseBox->chooseZhugongWujiang(avaliable1, avaliable2, avaliable3, avaliable4, avaliable5);
	m_audioPlayer->playSound(PlayersCreated);
	return result;
}

void MainWindow::runGuanxing()
{
	// TODO complete it;
}

void MainWindow::runWugufengdeng()
{
	// TODO complete it;
}

sgs::ConstData::CardColor MainWindow::runFanjian()
{
	QPoint pos = mainFrame->geometry().center() - QPoint(m_fanjianBox->width() / 2, m_fanjianBox->height() / 2);
	m_fanjianBox->setGeometry(QRect(pos, m_fanjianBox->size()));
	return m_fanjianBox->getFanjianColor();
}

void MainWindow::runLineAnimation(
		int sourcePlayerIndex,
		int targetPlayerIndex1,
		int targetPlayerIndex2,
		int targetPlayerIndex3,
		int targetPlayerIndex4,
		int targetPlayerIndex5,
		int targetPlayerIndex6,
		int targetPlayerIndex7)
{
	int targetCandidates[7] =
	{
		targetPlayerIndex1,
		targetPlayerIndex2,
		targetPlayerIndex3,
		targetPlayerIndex4,
		targetPlayerIndex5,
		targetPlayerIndex6,
		targetPlayerIndex7
	};

	int verticalDifference = 50;
	m_lineAnimationWidget->setGeometry(mainFrame->geometry().adjusted(0, 0, 0, verticalDifference));

	QPoint startPoint = animationPointAtIndex(sourcePlayerIndex);

	std::vector<QPoint> targetPointVec;
	for (int i = 0; i < 7; ++i)
	{
		if (targetCandidates[i])
			targetPointVec.push_back(animationPointAtIndex(targetCandidates[i]));
	}
	m_lineAnimationWidget->lineAnimation(startPoint, targetPointVec);
}

PlayerArea* MainWindow::otherPlayerAreaAtIndex(int index) const
{
	if (index == m_playerIndex || index < 0 || index > m_playerCount)
	{
		printDebug("MainWindow::otherPlayerAreaAtIndex: index == playerIndex or index invalid");
		return 0;
	}

	int initiatePosition = (index > m_playerIndex) ? (index - m_playerIndex) : (m_playerCount + index - m_playerIndex);
	return *(m_otherPlayerAreaVector.begin() + (initiatePosition - 1));
}

QPoint MainWindow::cardPointAtIndex(int index) const
{
	if (index == m_playerIndex)
		return lastShoupaiPoint();

	QPoint cardPointMargin(10, 0);
	return otherPlayerAreaAtIndex(index)->topLeftPoint() + cardPointMargin;
}

QPoint MainWindow::animationPointAtIndex(int index) const
{
	if (index == m_playerIndex)
		return shoupaiCenterPoint();
	return otherPlayerAreaAtIndex(index)->realGeometry().center();
}

QPoint MainWindow::judgePointAtIndex(int index) const
{
	if (index == m_playerIndex)
	{
		QPoint judgeMargin(10, -20);
		return realPos(zhuangbeiArea) + judgeMargin;
	}

	QPoint judgeMargin(60, -5);
	return otherPlayerAreaAtIndex(index)->topLeftPoint() + judgeMargin;
}

QPoint MainWindow::zhuangbeiPointAtIndex(int index) const
{
	if (index == m_playerIndex)
		return zhuangbeiPoint();
	return otherPlayerAreaAtIndex(index)->realGeometry().center();
}

sgs::ConstData::HeroSex MainWindow::getSexAtIndex(int index) const
{
	if (index == m_playerIndex)
		return m_wujiangArea->getSex();
	return otherPlayerAreaAtIndex(index)->getSex();
}

sgs::ConstData::HeroType MainWindow::getHeroTypeAtIndex(int index) const
{
	if (index == m_playerIndex)
		return m_wujiangArea->getHeroType();
	return otherPlayerAreaAtIndex(index)->getHeroType();
}

sgs::ConstData::PlayerRole MainWindow::getRoleAtIndex(int index) const
{
	if (index == m_playerIndex)
		return m_wujiangArea->getRole();
	return otherPlayerAreaAtIndex(index)->getRole();
}

bool MainWindow::getIsDeadAtIndex(int index) const
{
	if (index == m_playerIndex)
		return m_wujiangArea->isDead();
	return otherPlayerAreaAtIndex(index)->isDead();
}

const CardFrame * MainWindow::getWuqiAtIndex(int index) const
{
	if (index == m_playerIndex)
		return m_zhuangbeiArea->getWuqi();
	return otherPlayerAreaAtIndex(index)->getWuqi();
}

const CardFrame * MainWindow::getFangjuAtIndex(int index) const
{
	if (index == m_playerIndex)
		return m_zhuangbeiArea->getFangju();
	return otherPlayerAreaAtIndex(index)->getFangju();
}

const CardFrame * MainWindow::getJianmaAtIndex(int index) const
{
	if (index == m_playerIndex)
		return m_zhuangbeiArea->getJianma();
	return otherPlayerAreaAtIndex(index)->getJianma();
}

const CardFrame * MainWindow::getJiamaAtIndex(int index) const
{
	if (index == m_playerIndex)
		return m_zhuangbeiArea->getJiama();
	return otherPlayerAreaAtIndex(index)->getJiama();
}

void MainWindow::addWuqiAtIndex(int index, CardFrame * newWuqiCard)
{
	if (index == m_playerIndex)
		m_zhuangbeiArea->addWuqi(newWuqiCard);
	else
		otherPlayerAreaAtIndex(index)->addWuqi(newWuqiCard);
}

void MainWindow::addFangjuAtIndex(int index, CardFrame * newFangjuCard)
{
	if (index == m_playerIndex)
		m_zhuangbeiArea->addFangju(newFangjuCard);
	else
		otherPlayerAreaAtIndex(index)->addFangju(newFangjuCard);
}

void MainWindow::addJianmaAtIndex(int index, CardFrame * newJianmaCard)
{
	if (index == m_playerIndex)
		m_zhuangbeiArea->addJianma(newJianmaCard);
	else
		otherPlayerAreaAtIndex(index)->addJianma(newJianmaCard);
}

void MainWindow::addJiamaAtIndex(int index, CardFrame * newJiamaCard)
{
	if (index == m_playerIndex)
		m_zhuangbeiArea->addJiama(newJiamaCard);
	else
		otherPlayerAreaAtIndex(index)->addJiama(newJiamaCard);
}

void MainWindow::removeWuqiAtIndex(int index)
{
	if (index == m_playerIndex)
		m_zhuangbeiArea->removeWuqi();
	else
		otherPlayerAreaAtIndex(index)->removeWuqi();
}

void MainWindow::removeFangjuAtIndex(int index)
{
	if (index == m_playerIndex)
		m_zhuangbeiArea->removeFangju();
	else
		otherPlayerAreaAtIndex(index)->removeFangju();
}

void MainWindow::removeJianmaAtIndex(int index)
{
	if (index == m_playerIndex)
		m_zhuangbeiArea->removeJianma();
	else
		otherPlayerAreaAtIndex(index)->removeJianma();
}

void MainWindow::removeJiamaAtIndex(int index)
{
	if (index == m_playerIndex)
		m_zhuangbeiArea->removeJiama();
	else
		otherPlayerAreaAtIndex(index)->removeJiama();
}

void MainWindow::addJudgeAtIndex(int index, sgs::ConstData::CardType judgeCard)
{
	if (index == m_playerIndex)
		m_zhuangbeiArea->addJudge(judgeCard);
	else
		otherPlayerAreaAtIndex(index)->addJudge(judgeCard);
}

void MainWindow::removeJudgeAtIndex(int index, sgs::ConstData::CardType judgeCard)
{
	if (index == m_playerIndex)
		m_zhuangbeiArea->removeJudge(judgeCard);
	else
		otherPlayerAreaAtIndex(index)->removeJudge(judgeCard);
}

void MainWindow::showSkillAtIndex(int index, sgs::ConstData::HeroSkill skill)
{
	if (index == m_playerIndex)
		m_wujiangArea->showSkill(skill);
	else
		otherPlayerAreaAtIndex(index)->showSkill(skill);
}

void MainWindow::showZhuangbeiAtIndex(int index, sgs::ConstData::CardType equipCard)
{
	if (index == m_playerIndex)
	{
		QPoint zhuangbeiLabelMargin(10, -60);
		m_zhuangbeiLabel->move(zhuangbeiPoint() + zhuangbeiLabelMargin);
		m_zhuangbeiLabel->showZhuangbei(equipCard);
	}
	else
	{
		otherPlayerAreaAtIndex(index)->showZhuangbei(equipCard);
	}
}

void MainWindow::gameOver(char result)
{
	m_gameOverBox->showResult(result);
}

AudioPlayer * MainWindow::getAudioPlayer() const
{
	return m_audioPlayer;
}

AudioPlayer * MainWindow::getSkillPlayer() const
{
	return m_skillPlayer;
}

void MainWindow::setDead(int targetPlayerIndex)
{
	if (targetPlayerIndex == m_playerIndex)
		m_wujiangArea->setDead();
	else
		otherPlayerAreaAtIndex(targetPlayerIndex)->setDead();
}

void MainWindow::setRespodingPhase(int targetPlayerIndex, bool responding)
{
	if (targetPlayerIndex == m_playerIndex)
	{
		/*DO NOTHING, as GUI doesn't has the phase of human player*/
	}
	else
	{
		otherPlayerAreaAtIndex(targetPlayerIndex)->setResponding(responding);
	}
}

void MainWindow::setDying(int targetPlayerIndex, bool dying)
{
	if (targetPlayerIndex == m_playerIndex)
		m_wujiangArea->setDying(dying);
	else
		otherPlayerAreaAtIndex(targetPlayerIndex)->setDying(dying);
}

int MainWindow::getHP(int targetPlayerIndex) const
{
	if (targetPlayerIndex == m_playerIndex)
		return m_wujiangArea->getHP();
	return otherPlayerAreaAtIndex(targetPlayerIndex)->getHP();
}

void MainWindow::setHP(int targetPlayerIndex, int HP)
{
	if (targetPlayerIndex == m_playerIndex)
		m_wujiangArea->setHP(HP);
	else
		otherPlayerAreaAtIndex(targetPlayerIndex)->setHP(HP);
}

void MainWindow::setHPDamage(int targetPlayerIndex, int HPDamage)
{
	if (targetPlayerIndex == m_playerIndex)
	{
		m_wujiangArea->setHPDamage(HPDamage);
	}
	else
	{
		PlayerArea * targetPlayerArea = otherPlayerAreaAtIndex(targetPlayerIndex);
		targetPlayerArea->setHPDamage(HPDamage);
	}
}

} /* namespace sgsui */
