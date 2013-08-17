/*
 * sgsui_MainWindow_Display.cpp
 *
 *  Created on: Jun 20, 2012
 *      Author: latios
 */

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

void MainWindow::MCardReceived(const sgs::Derive::CardMessage * message)
{
	bool isResponse = message->from()->phase() == sgs::ConstData::OTHERPHASE;
	bool abandon = true;

	QPoint sourcePoint, targetPoint;

	// when maskType is ctNone, change it to cardType
	sgs::ConstData::CardType cardType = message->card()->type();
	sgs::ConstData::CardType maskType = message->card()->mask();
	if (maskType != cardType)
		printDebug("MainWindow::MCardReceived: card has mask");

	sgs::ConstData::CardColor cardColor = message->card()->color();
	int number = message->card()->number();
	EquipType equipType = getEquipType(maskType);
	bool toJudge = isJudgeCard(maskType) && message->type() == sgs::ConstData::HITCARD;
	bool isJudgeResult = isJudgeCard(maskType) && message->type() == sgs::ConstData::USECARD;

	// Convert platform player Index to GUI player index
	// no matter whether the to is the from
	int sourcePlayerIndex = message->from()->seat() + 1;

	int targetPlayerIndex[7] = {0};
	for (int i = 0, j = 0; i < message->targets(); ++i)
		targetPlayerIndex[j++] = message->to(i)->seat() + 1;

	// only run line animation for HITCARD
	if (message->type() == sgs::ConstData::HITCARD)
	{
		runLineAnimation(sourcePlayerIndex, targetPlayerIndex[0],
				targetPlayerIndex[1], targetPlayerIndex[2],
				targetPlayerIndex[3], targetPlayerIndex[4],
				targetPlayerIndex[5], targetPlayerIndex[6]);
	}

	// set PlayerArea's phase to responding (if it is responding)
	if (isResponse)
		setRespodingPhase(sourcePlayerIndex, true);

	QString sourceAndTarget("MainWindow::MCardReceived: source and targets indexes (seat + 1) = ");
	sourceAndTarget.append(QString::number(sourcePlayerIndex) + ", ");
	for (int i = 0; i < 7; ++i)
		sourceAndTarget.append(QString::number(targetPlayerIndex[i]) + ", ");
	printDebug(sourceAndTarget);

	Shoupai * shoupai = 0;
	bool useZhuangbei = false, hitZhuangbei = false;

	switch (message->pos().first)
	{
	case sgs::ConstData::PHAND:
		if (sourcePlayerIndex == m_playerIndex)
		{
			printDebug("MainWindow::MCardReceived: source player is human player, trying to remove shoupai");
			shoupai = goToCard(message->card());
			sourcePoint = realPos(shoupai);
			printDebug("MainWindow::MCardReceived: remove shoupai "
					   + cardFullDisplayName(shoupai->platformCard(), false));
			removeShoupai(shoupai);
		}
		else
		{
			sourcePoint = cardPointAtIndex(sourcePlayerIndex);
			otherPlayerAreaAtIndex(sourcePlayerIndex)->setShoupaiNumber(
					message->from()->handnum());
		}
		break;

	case sgs::ConstData::PEQUIP:
		useZhuangbei = message->type() == sgs::ConstData::USECARD;
		hitZhuangbei = message->type() == sgs::ConstData::HITCARD;
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
	if (useZhuangbei || isJudgeResult)
		m_audioPlayer->playSound(UseEquip);
	else
		m_audioPlayer->playSound(maskType, (message->from()->sex() == sgs::ConstData::MALE));

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

	if (maskType == sgs::ConstData::SHA)
	{
		if (cardColor == sgs::ConstData::SPADE || cardColor == sgs::ConstData::CLUB)
			runPixmapAnimation(sourcePlayerIndex, BlackSlashAnimation);
		else
			runPixmapAnimation(sourcePlayerIndex, RedSlashAnimation);
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

	m_cardAnimation->addCard(sourcePoint, targetPoint, new CardFrame(cardType, cardColor, number, this), abandon);
	m_cardAnimation->runAnimation();

	printDebug("MainWindow::MCardReceived: runAnimation over");

	switch (equipType)
	{
	case Wuqi:
		addWuqiAtIndex(sourcePlayerIndex, new CardFrame(maskType, cardColor, number));
		break;
	case Fangju:
		addFangjuAtIndex(sourcePlayerIndex, new CardFrame(maskType, cardColor, number));
		break;
	case Jianma:
		addJianmaAtIndex(sourcePlayerIndex, new CardFrame(maskType, cardColor, number));
		break;
	case Jiama:
		addJiamaAtIndex(sourcePlayerIndex, new CardFrame(maskType, cardColor, number));
		break;
	default:
		break;
	}

	if (toJudge)
		addJudgeAtIndex(targetPlayerIndex[0], maskType);

	printDebug("MainWindow::MCardReceived: equip over");

	// set PlayerArea's phase to not responding
	if (isResponse)
		setRespodingPhase(sourcePlayerIndex, false);

	sgs::ConstData::HeroType heroType = message->from()->type();
	QString historyString(wujiangDisplayName(heroType));

	if (maskType == sgs::ConstData::JIEDAO)
	{
		historyString.append(trUtf8("使用了"));
		historyString.append(cardFullDisplayName(message->card()));
		historyString.append( trUtf8("，杀手是"));
		historyString.append(wujiangDisplayName(message->to(0)->type()));
		historyString.append( trUtf8("，目标是"));
		historyString.append(wujiangDisplayName(message->to(1)->type()));
	}
	else if (useZhuangbei)
	{
		historyString.append(trUtf8("使用了已经装备的"));
		historyString.append(cardFullDisplayName(message->card()));
	}
	else
	{
		if (message->targets() > 0)
		{
			historyString.append(trUtf8("对 "));
			for (int i = 0; i < message->targets(); ++i)
			{
				heroType = message->to(i)->type();
				historyString.append(wujiangDisplayName(heroType));
				historyString.append(' ');
			}
		}

		if (cardType == maskType)
		{
			historyString.append(trUtf8("使用了"));
			historyString.append(cardFullDisplayName(message->card()));
		}
		else
		{
			historyString.append(trUtf8("把"));
			historyString.append(cardFullDisplayName(message->card()));
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

void MainWindow::MSkillReceived(const sgs::Derive::SkillMessage * message)
{
	int sourcePlayerIndex = message->from()->seat() + 1;
	m_skillPlayer->playSound(message->skill(), sourcePlayerIndex);
	showSkillAtIndex(sourcePlayerIndex, message->skill());

	QString historyString(wujiangDisplayName(message->from()->type()));
	historyString.append(trUtf8("使用了技能"));
	historyString.append(skillDisplayName(message->skill()));
	printHistory(historyString);

	// if it is other's skill, pause some time
	if (sourcePlayerIndex != m_playerIndex)
		sleepSomeTime(GUIStaticData::skillBlockDuration);
}

void MainWindow::MHurtReceived(const sgs::Derive::HurtMessage * message)
{
	bool fromGod = !(message->from());
	sgs::ConstData::HeroType sourceHero = sgs::ConstData::heroTypeNone;
	sgs::ConstData::HeroType targetHero = message->to()->type();
	if (!fromGod)
		sourceHero = message->from()->type();

	int hurt = message->hurt();
	int targetPlayer = message->to()->seat() + 1;

	// DO NOT play sound here, runPixmapAnimation will do it
	if (hurt > 0)
	{
		runPixmapAnimation(targetPlayer, (fromGod ? LightningAnimation : DamageAnimation));
		sleepSomeTime(GUIStaticData::basicAnimationDuration);
	}
	setHPDamage(targetPlayer, hurt);

	bool fromMyself = (message->from() == message->to());
	if (hurt > 0)
	{
		QString historyString(
				(fromGod ? wujiangDisplayName(targetHero) + trUtf8("受到了无来源的")
						 : wujiangDisplayName(sourceHero) + trUtf8("对") + (fromMyself ? trUtf8("自己") : wujiangDisplayName(targetHero)) + trUtf8("造成了")));
		historyString.append(QString::number(hurt)).append(trUtf8("点伤害"));
		printHistory(historyString);
	}
	else
	{
		QString historyString(wujiangDisplayName(targetHero));
		if (!fromGod && (message->from() != message->to()))
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
				   "MainWindow::MHurtReceived: HP mismatch, platform HP = "
				   + QString::number(platformHP) + ", GUI HP = "
				   + QString::number(GUIHP));
		pause();
	}
}

void MainWindow::MTransCardReceived(const sgs::Derive::TransCardMessage * message)
{
	printDebug("MainWindow::MTransCardReceived: start");

	int cardCount = message->cards();

	printDebug("MainWindow::MTransCardReceived: number = " + QString::number(cardCount) + " cards");

	bool open = message->open(), abandonCards = false;
	bool addCardsToShoupai = false, removeCardsFromShoupai = false;
	bool fromPlayerArea = false, toPlayerArea = false;
	bool toJudge = message->tojudge();
	int fromZuangbeiIndex = -1;
	QPoint sourcePoint(0, 0), targetPoint(0, 0);
	QPoint wuguPoint[8];
	bool fromWugu = false;
	int sourcePlayerIndex = 0;

	// TODO confirm how to identify
	// and revise the following code
	using namespace sgs::ConstData;
	sgs::DataType::Player * fromPlayer = message->from();
	sgs::DataType::Player * toPlayer = message->to();
	if (fromPlayer)
	{
		sourcePlayerIndex = fromPlayer->seat() + 1;
		if (sourcePlayerIndex == m_playerIndex)
		{
			switch (message->frompos())
			{
			case PHAND:
				removeCardsFromShoupai = true;
				// the source point is handled below
				break;
			case PEQUIP:
				sourcePoint = zhuangbeiPointAtIndex(sourcePlayerIndex);
				for (int i = 0; i < message->cards(); ++i)
				{
					fromZuangbeiIndex = message->pos(i);
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
				for (int i = 0; i < message->cards(); ++i)
					removeJudgeAtIndex(sourcePlayerIndex, message->trans(i)->type());
				break;
			default:
				break;
			}
		}
		else
		{
			switch (message->frompos())
			{
			case PHAND:
				sourcePoint = cardPointAtIndex(fromPlayer->seat() + 1);
				fromPlayerArea = true;
				otherPlayerAreaAtIndex(sourcePlayerIndex)->setShoupaiNumber(message->from()->handnum());
				break;
			case PEQUIP:
				sourcePoint = zhuangbeiPointAtIndex(sourcePlayerIndex);
				for (int i = 0; i < message->cards(); ++i)
				{
					fromZuangbeiIndex = message->pos(i);
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
				for (int i = 0; i < message->cards(); ++i)
					removeJudgeAtIndex(sourcePlayerIndex, message->trans(i)->type());
				break;
			default:
				break;
			}
		}
	}
	else
	{
		if (message->fromtype() == sgs::ConstData::DESK && m_wugufengdengBox->isVisible())
		{
			fromWugu = true;
			for (int i = 0; i < message->cards(); ++i)
				wuguPoint[i] = m_wugufengdengBox->removeCard(message->pos(i));
		}
		else
		{
			sourcePoint = cardDeckPoint();
		}
	}

	int targetPlayerIndex = 0;
	if (toPlayer)
	{
		targetPlayerIndex = toPlayer->seat() + 1;
		abandonCards = false;
		if (message->tojudge())
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
		if (message->totype() == sgs::ConstData::POOLTOP)
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

	std::list<Shoupai *> shoupaiToRemoveList;
	const sgs::DataType::Card * card = 0;
	Shoupai * shoupaiToRemove = 0;
	for (int i = 0; i < cardCount; ++i)
	{
		card = message->trans(i);
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

		if (fromWugu)
		{
			m_cardAnimation->addCard(wuguPoint[i], targetPoint,
								   (open ? new CardFrame(card->type(), card->color(), card->number(), this)
										 : new CardFrame(this)), abandonCards);
		}
		else
		{
			m_cardAnimation->addCard(sourcePoint, targetPoint,
								   (open ? new CardFrame(card->type(), card->color(), card->number(), this)
										 : new CardFrame(this)), abandonCards);
		}
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
			card = message->trans(i);
			addShoupai(card);
		}
	}

	if (toPlayerArea)
		otherPlayerAreaAtIndex(targetPlayerIndex)->setShoupaiNumber(message->to()->handnum());

	if (toJudge)
		addJudgeAtIndex(toPlayer->seat() + 1, message->trans(0)->type());

// TODO change Zhuangbei
// TODO printHistory

	printDebug("MainWindow::MTransCardReceived: over");
}

void MainWindow::MSwitchPhaseReceived(const sgs::Derive::SwitchPhaseMessage * message)
{
	int targetPlayerIndex = message->from()->seat() + 1;
	if (m_currentPlayerSeat + 1 != targetPlayerIndex && m_currentPlayerSeat >= 0)
	{
		// currentPlayerSeat is set to -1 when initializing
		otherPlayerAreaAtIndex(m_currentPlayerSeat + 1)->setPhase(sgs::ConstData::OTHERPHASE);
	}

	if (targetPlayerIndex != m_playerIndex)
	{
		otherPlayerAreaAtIndex(targetPlayerIndex)->setPhase(message->after());

		// sleep some time to make the phase change visible
		sleepSomeTime(GUIStaticData::switchPhaseDuration);
	}
}

void MainWindow::MDyingReceived(const sgs::Derive::DyingMessage * message)
{
	using sgs::ExternData::gamedata;
	setDying(message->from()->seat() + 1, true);
	sleepSomeTime(GUIStaticData::dyingDuration);
}

void MainWindow::MKillReceived(const sgs::Derive::KillMessage * message)
{
	bool fromGod = !(message->from());
	sgs::ConstData::HeroType sourceHero, targetHero = message->to()->type();
	if (!fromGod)
		sourceHero = message->from()->type();

	setDead(message->to()->seat() + 1);
	roleFrame->setDead(message->to()->role());
	m_audioPlayer->playSound(targetHero);

	QString historyMessage(wujiangDisplayName(targetHero)
						   + (fromGod ? trUtf8("死于非命")
									  : trUtf8("被 ") + wujiangDisplayName(sourceHero) + trUtf8(" 杀死了")));
	printHistory(historyMessage);
	sleepSomeTime(GUIStaticData::killDuration);
}

void MainWindow::MJudgeReceived(const sgs::Derive::JudgeMessage * message)
{
	sgs::ConstData::HeroType heroType = message->from()->type();
	sgs::ConstData::CardType cardType = message->result()->type();
	sgs::ConstData::CardColor cardColor = message->result()->color();
	int number = message->result()->number();
	m_cardAnimation->addCard(cardDeckPoint(), usedCardPoint(), new CardFrame(heroType, cardType, cardColor, number, message->effect(), this), true);
	m_cardAnimation->runAnimation();

	QString historyString(wujiangDisplayName(heroType));
	if (message->cardJudge())
	{
		historyString.append(trUtf8("的判定牌"));
		historyString.append(cardFullDisplayName(message->card()));
		historyString.append(trUtf8("判定结果为"));
	}
	else
	{
		historyString.append(skillDisplayName(message->skill()));
		historyString.append(trUtf8("判定结果为"));
	}

	historyString.append(cardFullDisplayName(message->result()));
	historyString.append(message->effect() ? GUIStaticData::judgePositive : GUIStaticData::judgeNegative);
	printHistory(historyString);
}

} /* namespace sgsui */
