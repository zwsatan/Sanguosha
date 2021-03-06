#include "sgsui_MainWindow.h"
#include "sgsui_Shoupai.h"
#include "sgsui_PlayerArea.h"
#include "sgsui_WugufengdengBox.h"
#include "sgsui_AudioPlayer.h"
#include "sgsui_PosAnimation.h"
#include "sgsui_output.h"
#include "sgsui_nameConvert.h"
#include "sgsdata_msg.h"
#include "sgsdata_player.h"
#include "sgsdata_card.h"
#include "sgsdata_extern.h"

namespace sgsui {

void MainWindow::CardMsgReceived(const sgs::Derive::CardMessage * msg)
{
	// when maskType is ctNone, change it to cardType
	sgs::ConstData::CardType cardType = msg->card()->type();
	sgs::ConstData::CardType maskType = msg->card()->mask();
	if (maskType != cardType)
		printDebug("MainWindow::CardMsgReceived: card has mask");

	// Convert platform player Index to GUI player index
	// no matter whether the to is the from
	int sourcePlayerIndex = msg->from()->seat() + 1;

	int targetPlayerIndex[7] = {0};
	for (int i = 0, j = 0; i < msg->targets(); ++i, ++j)
		targetPlayerIndex[j] = msg->to(i)->seat() + 1;

	// only run line animation for HITCARD
	if (msg->type() == sgs::ConstData::HITCARD)
	{
		runLineAnimation(sourcePlayerIndex, targetPlayerIndex[0],
				targetPlayerIndex[1], targetPlayerIndex[2],
				targetPlayerIndex[3], targetPlayerIndex[4],
				targetPlayerIndex[5], targetPlayerIndex[6]);
	}

	// set PlayerArea's phase to responding (if it is responding)
	bool isResponse = msg->from()->phase() == sgs::ConstData::OTHERPHASE;
	if (isResponse)
		setRespodingPhase(sourcePlayerIndex, true);

	QString sourceAndTarget("MainWindow::MCardReceived: source and targets indexes (seat + 1) = ");
	sourceAndTarget.append(QString::number(sourcePlayerIndex) + ", ");
	for (int i = 0; i < 7; ++i)
		sourceAndTarget.append(QString::number(targetPlayerIndex[i]) + ", ");
	printDebug(sourceAndTarget);

	QPoint sourcePoint, targetPoint;
	bool abandon = true;
	bool useZhuangbei = false, hitZhuangbei = false;
	switch (msg->pos().first)
	{
	case sgs::ConstData::PHAND:
		if (sourcePlayerIndex == m_playerIndex)
		{
			printDebug("MainWindow::MCardReceived: source player is human player, trying to remove shoupai");
			ShoupaiButton * shoupai = goToCard(msg->card());
			sourcePoint = realPos(shoupai);
			printDebug("MainWindow::MCardReceived: remove shoupai " + cardFullDisplayName(shoupai->platformCard(), false));
			removeShoupai(shoupai);
		}
		else
		{
			sourcePoint = cardPointAtIndex(sourcePlayerIndex);
			otherPlayerAreaAtIndex(sourcePlayerIndex)->setShoupaiNumber(msg->from()->handnum());
		}
		break;

	case sgs::ConstData::PEQUIP:
		useZhuangbei = msg->type() == sgs::ConstData::USECARD;
		hitZhuangbei = msg->type() == sgs::ConstData::HITCARD;
		sourcePoint = zhuangbeiPointAtIndex(sourcePlayerIndex);
		break;

	case sgs::ConstData::PJUDGE:
		sourcePoint = judgePointAtIndex(sourcePlayerIndex);
		abandon = false;
		break;

	default:
		break;
	}

	// if a card is judge result card, play sound UseEquip
	bool isJudgeResult = isJudgeCard(maskType) && msg->type() == sgs::ConstData::USECARD;
	if (useZhuangbei || isJudgeResult)
		m_audioPlayer->playSound(UseEquip);
	else
		m_audioPlayer->playSound(maskType, (msg->from()->sex() == sgs::ConstData::MALE));

	if (isZhuangbeiCard(maskType))
	{
		targetPoint = zhuangbeiPointAtIndex(sourcePlayerIndex);
		abandon = false;
	}
	else if (isJudgeCard(maskType))
	{
		targetPoint = judgePointAtIndex(targetPlayerIndex[0]);
		abandon = false;
	}
	else
	{
		targetPoint = usedCardPoint();
	}

	sgs::ConstData::CardColor cardColor = msg->card()->color();
	if (maskType == sgs::ConstData::SHA)
	{
		bool isRedColor = cardColor == sgs::ConstData::SPADE || cardColor == sgs::ConstData::CLUB;
		PixmapAnimationType animationType = isRedColor ? BlackSlashAnimation : RedSlashAnimation;
		runPixmapAnimation(sourcePlayerIndex, animationType);
	}

	if (maskType == sgs::ConstData::WUXIE)
		runPixmapAnimation(sourcePlayerIndex, WuxieAnimation);

	if (maskType == sgs::ConstData::SHAN)
		runPixmapAnimation(sourcePlayerIndex, JinkAnimation);

	if (maskType == sgs::ConstData::TAO)
		runPixmapAnimation(targetPlayerIndex[0], PeachAnimation);

	if (maskType == sgs::ConstData::LEISHA)
		runPixmapAnimation(sourcePlayerIndex, ThunderSlashAnimation);

	if (maskType == sgs::ConstData::HUOSHA)
		runPixmapAnimation(sourcePlayerIndex, FireSlashAnimation);

	if (maskType == sgs::ConstData::JIU)
		runPixmapAnimation(sourcePlayerIndex, AnalepticAnimation);

	if (maskType == sgs::ConstData::TIESUO)
		runPixmapAnimation(targetPlayerIndex[0], ChainAnimation);

	if (maskType == sgs::ConstData::JUEDOU)
		runPixmapAnimation(sourcePlayerIndex, DuelAnimation);


	if (useZhuangbei)
		showZhuangbeiAtIndex(sourcePlayerIndex, maskType);

	if (hitZhuangbei)
	{
		switch (getEquipType(cardType))
		{
		case Wuqi:
			removeWuqiAtIndex(sourcePlayerIndex);
			break;
		case Fangju:
			removeFangjuAtIndex(sourcePlayerIndex);
			break;
		case Jianma:
			removeJianmaAtIndex(sourcePlayerIndex);
			break;
		case Jiama:
			removeJiamaAtIndex(sourcePlayerIndex);
			break;
		default:
			break;
		}
	}

	int cardNumber = msg->card()->number();
	m_cardAnimation->addCard(sourcePoint, targetPoint, new CardFrame(cardType, cardColor, cardNumber, this), abandon);
	m_cardAnimation->runAnimation();

	printDebug("MainWindow::MCardReceived: runAnimation over");

	EquipType equipType = getEquipType(maskType);
	switch (equipType)
	{
	case Wuqi:
		addWuqiAtIndex(sourcePlayerIndex, new CardFrame(maskType, cardColor, cardNumber));
		break;
	case Fangju:
		addFangjuAtIndex(sourcePlayerIndex, new CardFrame(maskType, cardColor, cardNumber));
		break;
	case Jianma:
		addJianmaAtIndex(sourcePlayerIndex, new CardFrame(maskType, cardColor, cardNumber));
		break;
	case Jiama:
		addJiamaAtIndex(sourcePlayerIndex, new CardFrame(maskType, cardColor, cardNumber));
		break;
	default:
		break;
	}

	bool toJudge = isJudgeCard(maskType) && msg->type() == sgs::ConstData::HITCARD;
	if (toJudge)
		addJudgeAtIndex(targetPlayerIndex[0], maskType);

	printDebug("MainWindow::MCardReceived: equip over");

	// set PlayerArea's phase to not responding
	if (isResponse)
		setRespodingPhase(sourcePlayerIndex, false);

	sgs::ConstData::HeroType heroType = msg->from()->type();
	QString historyString(wujiangDisplayName(heroType));

	if (maskType == sgs::ConstData::JIEDAO)
	{
		historyString.append(trUtf8("使用了"));
		historyString.append(cardFullDisplayName(msg->card()));
		historyString.append( trUtf8("，杀手是"));
		historyString.append(wujiangDisplayName(msg->to(0)->type()));
		historyString.append( trUtf8("，目标是"));
		historyString.append(wujiangDisplayName(msg->to(1)->type()));
	}
	else if (useZhuangbei)
	{
		historyString.append(trUtf8("使用了已经装备的"));
		historyString.append(cardFullDisplayName(msg->card()));
	}
	else
	{
		if (msg->targets() > 0)
		{
			historyString.append(trUtf8("对 "));
			for (int i = 0; i < msg->targets(); ++i)
			{
				heroType = msg->to(i)->type();
				historyString.append(wujiangDisplayName(heroType));
				historyString.append(' ');
			}
		}

		if (cardType == maskType)
		{
			historyString.append(trUtf8("使用了"));
			historyString.append(cardFullDisplayName(msg->card()));
		}
		else
		{
			historyString.append(trUtf8("把"));
			historyString.append(cardFullDisplayName(msg->card()));
			historyString.append(trUtf8("当作"));
			historyString.append(cardDisplayName(maskType));
			historyString.append(trUtf8("使用"));
		}
	}

	// the judge result will not be writen here
	// it will be writen in MJudgeReceived
	if (!isJudgeResult)
		printHistory(historyString);

	printDebug("MainWindow::MCardReceived: over");
}

void MainWindow::SkillMsgReceived(const sgs::Derive::SkillMessage * msg)
{
	int sourcePlayerIndex = msg->from()->seat() + 1;
	m_skillPlayer->playSound(msg->skill(), sourcePlayerIndex);
	showSkillAtIndex(sourcePlayerIndex, msg->skill());

	QString historyString(wujiangDisplayName(msg->from()->type()));
	historyString.append(trUtf8("使用了技能"));
	historyString.append(skillDisplayName(msg->skill()));
	printHistory(historyString);

	// if it is other's skill, pause some time
	if (sourcePlayerIndex != m_playerIndex)
		sleepSomeTime(GUIStaticData::skillBlockDuration);
}

void MainWindow::HurtMsgReceived(const sgs::Derive::HurtMessage * msg)
{
	bool fromGod = !(msg->from());
	sgs::ConstData::HeroType sourceHero = sgs::ConstData::heroTypeNone;
	sgs::ConstData::HeroType targetHero = msg->to()->type();
	if (!fromGod)
		sourceHero = msg->from()->type();

	int hurt = msg->hurt();
	int targetPlayer = msg->to()->seat() + 1;

	// DO NOT play sound here, runPixmapAnimation will do it
	if (hurt > 0)
	{
		runPixmapAnimation(targetPlayer, (fromGod ? LightningAnimation : DamageAnimation));
		sleepSomeTime(GUIStaticData::basicAnimationDuration);
	}
	setHPDamage(targetPlayer, hurt);

	bool fromMyself = (msg->from() == msg->to());
	if (hurt > 0)
	{
		QString historyString;
		if (fromGod)
			historyString = wujiangDisplayName(targetHero) + trUtf8("受到了无来源的");
		else
			historyString = wujiangDisplayName(sourceHero) + trUtf8("对") + (fromMyself ? trUtf8("自己") : wujiangDisplayName(targetHero));
		historyString.append(trUtf8("造成了") + QString::number(hurt)).append(trUtf8("点伤害"));
		printHistory(historyString);
	}
	else
	{
		QString historyString(wujiangDisplayName(targetHero));
		if (!fromGod && (msg->from() != msg->to()))
		{
			historyString.append(trUtf8("在 "));
			historyString.append(wujiangDisplayName(sourceHero));
			historyString.append(trUtf8(" 的救助下"));
		}
		historyString.append(trUtf8("回复了") + QString::number(-hurt) + trUtf8("点HP"));
		printHistory(historyString);
	}

	int platformHP = GUIStaticData::getPlayerHP(targetPlayer);
	int GUIHP = getHP(targetPlayer);
	if (platformHP != GUIHP)
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "MainWindow::HurtMsgReceived: HP mismatch, platform HP = "
				   + QString::number(platformHP) + ", GUI HP = "
				   + QString::number(GUIHP));
		pause();
	}
}

void MainWindow::TransCardMsgReceived(const sgs::Derive::TransCardMessage * msg)
{
	printDebug("MainWindow::MTransCardReceived: start");

	int cardCount = msg->cards();

	printDebug("MainWindow::MTransCardReceived: number = " + QString::number(cardCount) + " cards");

	bool removeCardsFromShoupai = false;
	bool fromPlayerArea = false, toPlayerArea = false;
	bool toJudge = msg->tojudge();
	int fromZuangbeiIndex = -1;
	QPoint sourcePoint(0, 0), targetPoint(0, 0);
	QPoint wuguPoint[8];
	bool fromWugu = false;

	// TODO confirm how to identify
	// and revise the following code
	using namespace sgs::ConstData;
	sgs::DataType::Player * fromPlayer = msg->from();
	if (fromPlayer)
	{
		int sourcePlayerIndex = fromPlayer->seat() + 1;
		if (sourcePlayerIndex == m_playerIndex)
		{
			switch (msg->frompos())
			{
			case PHAND:
				removeCardsFromShoupai = true;
				// the source point is handled below
				break;

			case PEQUIP:
				sourcePoint = zhuangbeiPointAtIndex(sourcePlayerIndex);
				for (int i = 0; i < msg->cards(); ++i)
				{
					fromZuangbeiIndex = msg->pos(i);
					switch (fromZuangbeiIndex)
					{
					case 0:
						printDebug("MainWindow::MTransCardReceived: remove weapon");
						removeWuqiAtIndex(sourcePlayerIndex);
						break;
					case 1:
						printDebug("MainWindow::MTransCardReceived: remove armor");
						removeFangjuAtIndex(sourcePlayerIndex);
						break;
					case 2:
						printDebug("MainWindow::MTransCardReceived: remove atkhorse");
						removeJianmaAtIndex(sourcePlayerIndex);
						break;
					case 3:
						printDebug("MainWindow::MTransCardReceived: remove dfdhorse");
						removeJiamaAtIndex(sourcePlayerIndex);
						break;
					default:
						break;
					}
				}
				break;

			case PJUDGE:
				sourcePoint = judgePointAtIndex(sourcePlayerIndex);
				for (int i = 0; i < msg->cards(); ++i)
					removeJudgeAtIndex(sourcePlayerIndex, msg->trans(i)->type());
				break;

			default:
				break;
			}
		}
		else
		{
			switch (msg->frompos())
			{
			case PHAND:
				sourcePoint = cardPointAtIndex(fromPlayer->seat() + 1);
				fromPlayerArea = true;
				otherPlayerAreaAtIndex(sourcePlayerIndex)->setShoupaiNumber(msg->from()->handnum());
				break;

			case PEQUIP:
				sourcePoint = zhuangbeiPointAtIndex(sourcePlayerIndex);
				for (int i = 0; i < msg->cards(); ++i)
				{
					fromZuangbeiIndex = msg->pos(i);
					switch (fromZuangbeiIndex)
					{
					case 0:
						printDebug("MainWindow::MTransCardReceived: remove weapon");
						removeWuqiAtIndex(sourcePlayerIndex);
						break;
					case 1:
						printDebug("MainWindow::MTransCardReceived: remove armor");
						removeFangjuAtIndex(sourcePlayerIndex);
						break;
					case 2:
						printDebug("MainWindow::MTransCardReceived: remove atkhorse");
						removeJianmaAtIndex(sourcePlayerIndex);
						break;
					case 3:
						printDebug("MainWindow::MTransCardReceived: remove dfdhorse");
						removeJiamaAtIndex(sourcePlayerIndex);
						break;
					default:
						break;
					}
				}
				break;

			case PJUDGE:
				sourcePoint = judgePointAtIndex(sourcePlayerIndex);
				for (int i = 0; i < msg->cards(); ++i)
					removeJudgeAtIndex(sourcePlayerIndex, msg->trans(i)->type());
				break;

			default:
				break;
			}
		}
	}
	else
	{
		if (msg->fromtype() == sgs::ConstData::DESK && m_wugufengdengBox->isVisible())
		{
			fromWugu = true;
			for (int i = 0; i < msg->cards(); ++i)
				wuguPoint[i] = m_wugufengdengBox->removeCard(msg->pos(i));
		}
		else
		{
			sourcePoint = cardDeckPoint();
		}
	}

	bool addCardsToShoupai = false;
	bool abandonCards = false;
	int targetPlayerIndex = 0;
	sgs::DataType::Player * toPlayer = msg->to();
	if (toPlayer)
	{
		targetPlayerIndex = toPlayer->seat() + 1;
		abandonCards = false;
		if (msg->tojudge())
		{
			targetPoint = judgePointAtIndex(targetPlayerIndex);
		}
		else if (targetPlayerIndex == m_playerIndex)
		{
			addCardsToShoupai = true;
			// the target point is handled below
		}
		else
		{
			targetPoint = cardPointAtIndex(targetPlayerIndex);
			toPlayerArea = true;
		}
	}
	else
	{
		if (msg->totype() == sgs::ConstData::POOLTOP)
		{
			targetPoint = cardDeckPoint();
			abandonCards = false;
		}
		else
		{
			targetPoint = usedCardPoint();
			abandonCards = true;
		}
	}

	int addCardDifference = 0, cardFrameWidth = 93;
	if (addCardsToShoupai)
	{
		addCardDifference = (shoupaiEndPoint().x() - lastShoupaiPoint().x()) / cardCount;
		if (addCardDifference > cardFrameWidth)
			addCardDifference = cardFrameWidth;
	}

	std::list<ShoupaiButton *> shoupaiToRemoveList;
	const sgs::DataType::Card * card = 0;
	ShoupaiButton * shoupaiToRemove = 0;
	for (int i = 0; i < cardCount; ++i)
	{
		card = msg->trans(i);
		if (removeCardsFromShoupai)
		{
			shoupaiToRemove = goToCard(card);
			shoupaiToRemoveList.push_back(shoupaiToRemove);
			sourcePoint = realPos(shoupaiToRemove);
		}

		if (addCardsToShoupai)
			targetPoint = lastShoupaiPoint() + QPoint(addCardDifference * i, 0);
		if (fromPlayerArea)
			sourcePoint += QPoint(15, 0);
		if (toPlayerArea)
			targetPoint += QPoint(15, 0);

		bool isOpen = msg->open();
		CardFrame * cardFrame = NULL;
		if (isOpen)
			cardFrame = new CardFrame(card->type(), card->color(), card->number(), this);
		else
			cardFrame = new CardFrame(this);

		if (fromWugu)
			m_cardAnimation->addCard(wuguPoint[i], targetPoint, cardFrame, abandonCards);
		else
			m_cardAnimation->addCard(sourcePoint, targetPoint, cardFrame, abandonCards);
	}

	// remove should be done before animation
	if (removeCardsFromShoupai)
	{
		for (auto iter = shoupaiToRemoveList.begin(); iter != shoupaiToRemoveList.end(); ++iter)
			removeShoupai(*iter);
	}

	m_cardAnimation->runAnimation();

	// add should be done after animation
	if (addCardsToShoupai)
	{
		for (int i = 0; i < cardCount; ++i)
		{
			card = msg->trans(i);
			addShoupai(card);
		}
	}

	if (toPlayerArea)
		otherPlayerAreaAtIndex(targetPlayerIndex)->setShoupaiNumber(msg->to()->handnum());

	if (toJudge)
		addJudgeAtIndex(targetPlayerIndex, msg->trans(0)->type());

// TODO change Zhuangbei
// TODO printHistory

	printDebug("MainWindow::MTransCardReceived: over");
}

void MainWindow::SwitchPhaseMsgReceived(const sgs::Derive::SwitchPhaseMessage * msg)
{
	int targetPlayerIndex = msg->from()->seat() + 1;
	if (m_currentPlayerSeat + 1 != targetPlayerIndex && m_currentPlayerSeat >= 0)
	{
		// currentPlayerSeat is set to -1 when initializing
		otherPlayerAreaAtIndex(m_currentPlayerSeat + 1)->setPhase(sgs::ConstData::OTHERPHASE);
	}

	if (targetPlayerIndex != m_playerIndex)
	{
		otherPlayerAreaAtIndex(targetPlayerIndex)->setPhase(msg->after());

		// sleep some time to make the phase change visible
		sleepSomeTime(GUIStaticData::switchPhaseDuration);
	}
}

void MainWindow::DyingMsgReceived(const sgs::Derive::DyingMessage * msg)
{
	using sgs::ExternData::gamedata;
	setDying(msg->from()->seat() + 1, true);
	sleepSomeTime(GUIStaticData::dyingDuration);
}

void MainWindow::KillMsgReceived(const sgs::Derive::KillMessage * msg)
{
	bool fromGod = !(msg->from());
	sgs::ConstData::HeroType sourceHero, targetHero = msg->to()->type();
	if (!fromGod)
		sourceHero = msg->from()->type();

	setDead(msg->to()->seat() + 1);
	roleFrame->setDead(msg->to()->role());
	m_audioPlayer->playSound(targetHero);

	QString historyMessage(wujiangDisplayName(targetHero)
						   + (fromGod ? trUtf8("死于非命")
									  : trUtf8("被 ") + wujiangDisplayName(sourceHero) + trUtf8(" 杀死了")));
	printHistory(historyMessage);
	sleepSomeTime(GUIStaticData::killDuration);
}

void MainWindow::JudgeMsgReceived(const sgs::Derive::JudgeMessage * msg)
{
	sgs::ConstData::CardType cardType = msg->result()->type();
	sgs::ConstData::CardColor cardColor = msg->result()->color();
	sgs::ConstData::HeroType heroType = msg->from()->type();
	int number = msg->result()->number();
	CardFrame * cardFrame = new CardFrame(heroType, cardType, cardColor, number, msg->effect(), this);
	m_cardAnimation->addCard(cardDeckPoint(), usedCardPoint(), cardFrame, true);
	m_cardAnimation->runAnimation();

	QString historyString(wujiangDisplayName(heroType));
	if (msg->cardJudge())
	{
		historyString.append(trUtf8("的判定牌"));
		historyString.append(cardFullDisplayName(msg->card()));
		historyString.append(trUtf8("判定结果为"));
	}
	else
	{
		historyString.append(skillDisplayName(msg->skill()));
		historyString.append(trUtf8("判定结果为"));
	}

	historyString.append(cardFullDisplayName(msg->result()));
	historyString.append(msg->effect() ? GUIStaticData::judgePositive : GUIStaticData::judgeNegative);
	printHistory(historyString);
}

} /* namespace sgsui */
