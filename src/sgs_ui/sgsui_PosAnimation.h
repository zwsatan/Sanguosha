#ifndef SGSUI_POSANIMATION_H_
#define SGSUI_POSANIMATION_H_

#include <QObject>
#include <QEventLoop>
#include <QPoint>
#include <vector>
#include "sgsui_BlockLoop.h"

class QHBoxLayout;
class QFrame;
class QPropertyAnimation;
class QParallelAnimationGroup;
class QPauseAnimation;
class QSequentialAnimationGroup;

namespace sgsui {

class CardFrame;

class PosAnimation : public QObject
{
	Q_OBJECT
public:
	PosAnimation(QFrame * usedCardFrame, QHBoxLayout * usedCardLayout, QObject * parent = 0);
	~PosAnimation();
	void addCard(const QPoint &startPoint, const QPoint &finishPoint, CardFrame * targetCard, bool abandonCard);
	// the CardFrames are deleted or added into usedCardLayout when animation stopped
	void runAnimation();

private slots:
	void movingStopped();
	void animationStopped();

private:
	QFrame * const									m_usedCardFrame;
	QHBoxLayout * const								m_usedCardLayout;

	std::vector<std::pair<CardFrame *, bool> >		m_cardVector;
	std::vector<QPropertyAnimation *>				m_cardAnimationVector;
	QParallelAnimationGroup *						m_cardMovingAnimation;
	unsigned int									m_abandonCount;
	int												m_addCardIndex;
	QPropertyAnimation *							m_usedCardFrameMovingAnimation;
	QPauseAnimation *								m_pauseAnimation;
	QSequentialAnimationGroup *						m_fullAnimation;

	BlockLoop										m_blockLoop;

	int												m_animationCount;
};

} /* namespace sgsui */
#endif /* SGSUI_POSANIMATION_H_ */
