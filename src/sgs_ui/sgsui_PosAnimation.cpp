#include "sgsui_PosAnimation.h"
#include "sgsui_CardFrame.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_output.h"
#include <QtGui>

namespace sgsui {

PosAnimation::PosAnimation(QFrame * usedCardFrame, QHBoxLayout * usedCardLayout, QObject * parent/* = 0*/)
	: QObject(parent)
	, m_usedCardFrame(usedCardFrame)
	, m_usedCardLayout(usedCardLayout)
	, m_abandonCount(0)
	, m_addCardIndex(0)
	, m_animationCount(0)
{
	m_usedCardFrameMovingAnimation = 0;

	m_cardMovingAnimation = new QParallelAnimationGroup(this);

	m_pauseAnimation = new QPauseAnimation(this);
	m_pauseAnimation->setDuration(GUIStaticData::useCardAnimationDuration / 3);

	m_fullAnimation = new QSequentialAnimationGroup(this);
	m_fullAnimation->addAnimation(m_cardMovingAnimation);

	connect(m_cardMovingAnimation, SIGNAL(finished()), this, SLOT(movingStopped()));
	connect(m_fullAnimation, SIGNAL(finished()), this, SLOT(animationStopped()));
}

PosAnimation::~PosAnimation()
{
	for (auto iter = m_cardAnimationVector.begin(); iter != m_cardAnimationVector.end(); ++iter)
		delete *iter;
}

void PosAnimation::addCard(const QPoint &startPoint, const QPoint &finishPoint, CardFrame * targetCard, bool abandonCard) {
	if (m_blockLoop.isRunning())
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "PosAnimation::runAnimation: "
				   "the previous animation is not finished yet");
		return;
	}

	m_abandonCount += abandonCard;
	m_cardVector.push_back(std::pair<CardFrame *, bool>(targetCard, abandonCard));
	QPoint translatedFinishPoint(finishPoint + QPoint(m_abandonCount * abandonCard * 15, 0));
	QPropertyAnimation * animation = 0;
	{
		animation = new QPropertyAnimation(targetCard, "pos", this);
		animation->setDuration(GUIStaticData::useCardAnimationDuration / 2);
		animation->setEasingCurve(QEasingCurve::OutQuad);

		animation->setStartValue(startPoint);
		animation->setEndValue(translatedFinishPoint);
		m_cardAnimationVector.push_back(animation);
		m_cardMovingAnimation->addAnimation(animation);
	}

	targetCard->show();
	++m_addCardIndex;
}

void PosAnimation::runAnimation()
{
	// add pause after game started, to make sure the fisrt card distribute is quick without pause
	if (m_animationCount++ == GUIStaticData::getPlayerCount())
		m_fullAnimation->addAnimation(m_pauseAnimation);

	if (m_blockLoop.isRunning())
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "PosAnimation::runAnimation: "
				   "the previous animation is not finished yet");
		return;
	}

	if (m_abandonCount > 0)
	{
		if (m_usedCardLayout->count() > GUIStaticData::maxUsedCardContain)
			delete m_usedCardLayout->takeAt(1)->widget();

		QPoint originPos = m_usedCardFrame->pos();
		m_usedCardFrameMovingAnimation = new QPropertyAnimation(m_usedCardFrame, "pos", this);
		m_usedCardFrameMovingAnimation->setDuration(GUIStaticData::useCardAnimationDuration / 2);
		m_usedCardFrameMovingAnimation->setStartValue(originPos);
		int cardFrameWidth = 93;
		m_usedCardFrameMovingAnimation->setKeyValueAt(0.999999, originPos + QPoint(cardFrameWidth, 0));
		m_usedCardFrameMovingAnimation->setEndValue(originPos);
		m_cardMovingAnimation->addAnimation(m_usedCardFrameMovingAnimation);
	}
	m_fullAnimation->start();
	m_blockLoop.exec();
}

void PosAnimation::movingStopped()
{
	for (auto iter = m_cardVector.begin(); iter != m_cardVector.end(); ++iter)
	{
		std::pair<CardFrame *, bool> cardInfo = *iter;
		if (cardInfo.second)
		{
			if (m_usedCardLayout->count() > GUIStaticData::maxUsedCardContain)
				delete m_usedCardLayout->takeAt(1)->widget();
			m_usedCardLayout->addWidget(cardInfo.first);
		}
		// if it is not to abandon, it will be handled below
	}
}

void PosAnimation::animationStopped()
{
	for (auto iter = m_cardVector.begin(); iter != m_cardVector.end(); ++iter)
	{
		std::pair<CardFrame *, bool> cardInfo = *iter;
		// if it is to abandon, it has been handled above
		if (!cardInfo.second)
			delete cardInfo.first;
	}
	delete m_usedCardFrameMovingAnimation;
	m_usedCardFrameMovingAnimation = 0;

	m_cardVector.clear();
	for (auto iter = m_cardAnimationVector.begin(); iter != m_cardAnimationVector.end(); ++iter)
		delete *iter;

	m_cardAnimationVector.clear();
	m_cardMovingAnimation->clear();
	m_abandonCount = 0;
	m_addCardIndex = 0;
	m_blockLoop.quit();
}

} /* namespace sgsui */
