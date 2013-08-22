#include "sgsai_AIBasic.h"
#include "sgsui_MainWindow.h"
#include "sgsui_PlayerSort.h"
#include "sgsui_nameConvert.h"
#include "sgsui_output.h"
#include "sgsdata_enum.h"
#include "sgsdata_msg.h"
#include "sgsdata_player.h"
#include "sgsdata_card.h"
#include "sgsdata_extern.h"
#include "sgsdata_gamedata.h"

using namespace sgs::ConstData;
using namespace sgs::Derive;
using namespace sgs::DataType;
using namespace std;

namespace sgsai {

const AIBasic::JieDaoPair AIBasic::ms_invalidJiedaoPair(NULL, NULL);

AIBasic::AIBasic(int seat)
	: QObject(sgsui::MainWindow::mainWindowPtr())
	, Interface(seat)
	, m_lord(0)
{
}

vector<int> AIBasic::discardCard(unsigned int cardNumToDiscard)
{
	printDebug("AIBasic::discardCard: start, AI's seat = " + QString::number(mySeat()));
	printDebug("AIBasic::useCardResponse: discard cards other than sha, shan, tao and wuxie");

	// 电脑弃牌的选择就是,手牌中,非杀,非闪,非桃且非无懈的就可以弃掉
	vector<int> returnValue;
	for (int i = 0; i < myPlayer()->handnum(); ++i)
	{
		CardType type = myPlayer()->hand(i)->type();
		if (type != SHA && type != SHAN && type != TAO && type != WUXIE)
		{
			returnValue.push_back(i);
			if (returnValue.size() >= cardNumToDiscard)
			{
				printDebug(sgsui::discardInterpret(returnValue, myPlayer()));
				printDebug("AIBasic::discardCard: over");
				return returnValue;
			}
		}
	}

	// 当迫不得已时,选择优先弃杀
	printDebug("AIBasic::useCardResponse: discard sha");
	for (int i = 0; i < myPlayer()->handnum(); ++i)
	{
		if (myPlayer()->hand(i)->type() == SHA)
		{
			returnValue.push_back(i);
			if (returnValue.size() >= cardNumToDiscard)
			{
				printDebug(sgsui::discardInterpret(returnValue, myPlayer()));
				printDebug("AIBasic::discardCard: over");
				return returnValue;
			}
		}
	}

	// 继续,迫不得已弃掉闪或无懈
	printDebug("AIBasic::useCardResponse: discard shan and wuxie");
	for (int i = 0; i < myPlayer()->handnum(); ++i)
	{
		if (myPlayer()->hand(i)->type() == SHAN || myPlayer()->hand(i)->type() == WUXIE)
		{
			returnValue.push_back(i);
			if (returnValue.size() >= cardNumToDiscard)
			{
				printDebug(sgsui::discardInterpret(returnValue, myPlayer()));
				printDebug("AIBasic::discardCard: over");
				return returnValue;
			}
		}
	}

	// 开始弃桃了
	printDebug("AIBasic::useCardResponse: discard tao");
	for (int i = 0; i < myPlayer()->handnum(); ++i)
	{
		if (myPlayer()->hand(i)->type() == TAO)
		{
			returnValue.push_back(i);
			if (returnValue.size() >= cardNumToDiscard)
			{
				printDebug(sgsui::discardInterpret(returnValue, myPlayer()));
				printDebug("AIBasic::discardCard: over");
				return returnValue;
			}
		}
	}

	printDebug(sgsui::discardInterpret(returnValue, myPlayer()));
	printDebug("AIBasic::discardCard: over");
	return returnValue;
}

Message * AIBasic::useCardResponse()
{
	printDebug("AIBasic::useCardResponse: start, AI's seat = " + QString::number(mySeat()));

	CardMessage * returnMessage = 0;

	// 卡牌使用阶段,优先使用丈八
	// 其实这个判定很傻逼,只要武器是丈八,而且手牌>2,就会直接使用丈八特效出杀
	if (!returnMessage)
	{
		printDebug("AIBasic::useCardResponse: trying to use zhangba");
		returnMessage = useZhangba();
	}

	// 没有丈八,直接查看手牌有装备没,有直接装备上
	if (!returnMessage)
	{
		printDebug("AIBasic::useCardResponse: trying to use equip");
		returnMessage = installEquip();
	}

	// 开始使用锦囊
	if (!returnMessage)
	{
		printDebug("AIBasic::useCardResponse: trying to use jinnang (not wuxie)");
		returnMessage = useJinnang();
	}

	// 开始判断使用桃
	if (!returnMessage)
	{
		printDebug("AIBasic::useCardResponse: trying to use tao");
		returnMessage = useTao();
	}

	// 开始判断使用杀
	if (!returnMessage)
	{
		printDebug("AIBasic::useCardResponse: trying to use sha");
		returnMessage = useSha();
	}

	if (!returnMessage)
		printDebug("AIBasic::useCardResponse: no suitable card to use");

	printDebug(sgsui::messageInterpret(returnMessage));
	printDebug("AIBasic::useCardResponse: over");
	return returnMessage;
}

Message * AIBasic::taoOrNot(Player * from, int /*neededTaoNumber*/)
{
	printDebug("AIBasic::taoOrNot: start, AI's seat = " + QString::number(mySeat()));

	if (from != myPlayer())
	{
		PlayerRole myRole = myPlayer()->role();
		PlayerRole fromRole = from->role();
		bool shouldNotSave = false;

		// 对方身份对立的时候,不救
		if ((myRole == ZHU || myRole == ZHONG) && fromRole == FAN)
			shouldNotSave = true;
		else if (myRole == FAN || fromRole == ZHONG)
			shouldNotSave = true;

		if (shouldNotSave || !isMyFriend(from))
		{
			printDebug(sgsui::messageInterpret(static_cast<CardMessage * >(0)));
			printDebug("AIBasic::taoOrNot: over");
			return NULL;
		}
	}

	for (int i = 0; i < myPlayer()->handnum(); ++i)
	{
		if (myPlayer()->hand(i)->type() == TAO)
		{
			CardMessage * returnMessage = new CardMessage(false, myPlayer(), PHAND, i, from);
			printDebug(sgsui::messageInterpret(returnMessage));
			printDebug("AIBasic::taoOrNot: over");
			return returnMessage;
		}
	}

	printDebug("AIBasic::taoOrNot: no tao to use. response cancelled");
	printDebug(sgsui::messageInterpret(static_cast<CardMessage * >(0)));
	printDebug("AIBasic::taoOrNot: over");
	return NULL;
}

Message * AIBasic::shanOrNot(Player * /*from*/, const Card * /*card*/)
{
	printDebug("AIBasic::shanOrNot: start, AI's seat = " + QString::number(mySeat()));
	for (int i = 0; i < myPlayer()->handnum(); ++i)
	{
		if (myPlayer()->hand(i)->type() == SHAN)
		{
			CardMessage * returnMessage = new CardMessage(false, myPlayer(), PHAND, i);
			printDebug(sgsui::messageInterpret(returnMessage));
			printDebug("AIBasic::shanOrNot: over");
			return returnMessage;
		}
	}

	printDebug("AIBasic::shanOrNot: no shan to use. response cancelled");
	printDebug(sgsui::messageInterpret(static_cast<CardMessage * >(0)));
	printDebug("AIBasic::shanOrNot: over");
	return NULL;
}

Message * AIBasic::shanOrNot(Player * from, HeroSkill skillType)
{
	printDebug("AIBasic::shanOrNot (skill version): start, AI's seat = " + QString::number(mySeat()));

	if (skillType != HUJIA)
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "AIBasic::shanOrNot (skill version): skill is not HUJIA");
	}

	if (!isMyFriend(from))
	{
		printDebug(sgsui::messageInterpret(static_cast<CardMessage * >(0)));
		printDebug("AIBasic::shanOrNot (skill version): from-player is not my friend");
		printDebug("AIBasic::shanOrNot (skill version): over");
		return NULL;
	}

	for (int i = 0; i < myPlayer()->handnum(); ++i)
	{
		if (myPlayer()->hand(i)->type() == SHAN)
		{
			CardMessage * returnMessage = new CardMessage(false, myPlayer(), PHAND, i);
			printDebug(sgsui::messageInterpret(returnMessage));
			printDebug("AIBasic::shanOrNot (skill version): over");
			return returnMessage;
		}
	}

	printDebug("AIBasic::shanOrNot (skill version): no shan to use. response cancelled");
	printDebug(sgsui::messageInterpret(static_cast<CardMessage * >(0)));
	printDebug("AIBasic::shanOrNot (skill version): over");
	return NULL;
}

Message * AIBasic::shaOrNot(Player * /*from*/, const Card * /*card*/)
{
	printDebug("AIBasic::shaOrNot: start, AI's seat = " + QString::number(mySeat()));
	CardMessage * returnMessage = 0;
	for (int i = 0; i < myPlayer()->handnum(); ++i)
	{
		if (myPlayer()->hand(i)->type() == SHA)
		{
			returnMessage = new CardMessage(false, myPlayer(), PHAND, i);
			printDebug(sgsui::messageInterpret(returnMessage));
			printDebug("AIBasic::shaOrNot: over");
			return returnMessage;
		}
	}

	if (!returnMessage)
	{
		printDebug("AIBasic::shaOrNot: trying to use zhangba");
		returnMessage = useZhangba();
	}

	printDebug("AIBasic::shaOrNot: no sha to use. response cancelled");
	returnMessage = 0;
	printDebug(sgsui::messageInterpret(returnMessage));
	printDebug("AIBasic::shaOrNot: over");
	return returnMessage;
}

Message * AIBasic::shaOrNot(Player * from, HeroSkill skillType)
{
	printDebug("AIBasic::shaOrNot (skill version): start, AI's seat = " + QString::number(mySeat()));

	if (skillType != RENDE)
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "AIBasic::shaOrNot (skill version): skill is not RENDE");
	}

	if (!isMyFriend(from))
	{
		printDebug(sgsui::messageInterpret(static_cast<CardMessage * >(0)));
		printDebug("AIBasic::shaOrNot (skill version): from-player is not my friend");
		printDebug("AIBasic::shaOrNot (skill version): over");
		return NULL;
	}

	for (int i = 0; i < myPlayer()->handnum(); ++i)
	{
		if (myPlayer()->hand(i)->type() == SHA)
		{
			CardMessage * returnMessage = new CardMessage(false, myPlayer(), PHAND, i);
			printDebug(sgsui::messageInterpret(returnMessage));
			printDebug("AIBasic::shaOrNot (skill version): over");
			return returnMessage;
		}
	}

	printDebug("AIBasic::shaOrNot (skill version): no sha to use. response cancelled");
	printDebug(sgsui::messageInterpret(static_cast<CardMessage * >(0)));
	printDebug("AIBasic::shaOrNot (skill version): over");
	return NULL;
}

bool AIBasic::useSkillOrNot(HeroSkill skillType, Message * message)
{
	QString text("<font color=red><b>Warning: </b></font>AIBasic::useSkillOrNot: this function should not be called. ");
	text.append(" skill: ");
	text.append(sgsui::skillDisplayName(skillType));
	text.append(" message: ");
	text.append(sgsui::allMessageInterpret(message));
	printDebug(text);
	return true;
}

// 仅仅是判断若有装备则构造返回
// 因此电脑每次都傻傻的不停替换完所有武器
// 还是按手牌顺序替换
CardMessage * AIBasic::installEquip()
{
	CardMessage * returnMessage = 0;
	for (int i = 0; i < myPlayer()->handnum(); ++i)
	{
		if (sgsui::getEquipType(myPlayer()->hand(i)->type()) == NotEquip)
			continue;

		// 如何选择武器是最好的呢
		returnMessage = new CardMessage(false, myPlayer(), PHAND, i);
		return returnMessage;
	}
	return returnMessage;
}

CardMessage * AIBasic::useTao()
{
	if (myPlayer()->hp() < myPlayer()->maxHP())
	{
		bool hasPreserved = false;	// 是否为主公保留桃
		for (int i = 0; i < myPlayer()->handnum(); ++i)
		{
			if (myPlayer()->hand(i)->type() == TAO)
			{
				// 若主公只剩下1-2滴血,且忠臣的生命大于2,则为主公保留桃1个
				// 若有2个桃,第二开始就不需要保留
				if (myPlayer()->role() == ZHONG && myPlayer()->hp() > 2 && lord()->hp() < 3 && !hasPreserved)
				{
					hasPreserved = true;
					printDebug("AIBasic::useTao: preserved Tao for lord");
					continue;
				}

				CardMessage * returnMessage = new CardMessage(false, myPlayer(), PHAND, i, myPlayer());
				return returnMessage;
			}
		}
	}

	return NULL;
}

CardMessage * AIBasic::useSha()
{
	if (!canUseSha())
		return NULL;

	const PlayerVec &attackTargetVec = shaTarget();
	if (!attackTargetVec.empty())
	{
		// use sha
		for (int i = 0; i < myPlayer()->handnum(); ++i)
		{
			if (myPlayer()->hand(i)->type() == SHA)
			{
				CardMessage * returnMessage = 0;
				if (canUseFangtian() && attackTargetVec.size() > 1)
				{
					// 使用方天画戟的情况
					printDebug("AIBasic::useSha: using fangtian");
					returnMessage = new CardMessage(false, myPlayer(), PHAND, i);

					// 最多攻击3个人
					PlayerVec arrangedTargets;
					int j = 0;
					auto iter = attackTargetVec.begin();
					while (iter != attackTargetVec.end() && j < 3)
					{
						arrangedTargets.push_back(*iter);
						++iter;
						++j;
					}

					// sort the targets by seat;
					sgsui::PlayerSort arrangeSeat(mySeat(), sgsui::BySeat);
					sort(arrangedTargets.begin(), arrangedTargets.end(), arrangeSeat);

					for (auto iter = arrangedTargets.begin(); iter != arrangedTargets.end(); ++iter)
						returnMessage->addto(*iter);
				}
				else
				{
					printDebug("AIBasic::useSha: not using fangtian");

					// 对方有仁王盾,不适用黑杀
					// try each player in range
					for (auto riter = attackTargetVec.rbegin(); riter != attackTargetVec.rend(); ++riter)
					{
						Player * target = *riter;

						CardColor color = myPlayer()->hand(i)->color();
						bool isBlackSha = (color == CLUB || color == SPADE);
						bool enemyHasRenwang = (target->armor() && (target->armor()->type() == RENWANG));
						const Card * myWeapon = myPlayer()->weapon();
						bool renwangEffective = !myWeapon || myWeapon->type() != QINGGANG;

						if (!isBlackSha || !enemyHasRenwang || !renwangEffective)
						{
							returnMessage = new CardMessage(false, myPlayer(), PHAND, i, target);
							printDebug("AIBasic::useSha: will attack player "
									   + QString::number(target->seat())
									   + ", break and stop searching for target");
							break;
						}
						else
						{
							printDebug("AIBasic::useSha: skip player "
									   + QString::number(target->seat())
									   + " due to renwang, going to try next player");
						}
					}
				}

				return returnMessage;
			}
		}
	}

	return NULL;
}

CardMessage * AIBasic::useJinnang()
{
	CardMessage * returnMessage = 0;
	for (int i = 0; i < myPlayer()->handnum(); ++i)
	{
		const CardType currentJinnang = myPlayer()->hand(i)->type();

		if (!sgsui::isJinnangCard(currentJinnang))
			continue;

		if (currentJinnang == WUXIE)
			continue;

		QString cardFullName = sgsui::cardFullDisplayName(myPlayer()->hand(i), false);
		printDebug("AIBasic::useJinnang: trying to use jinnang " + cardFullName);

		switch (currentJinnang)
		{
		case SHANDIAN:
			if (!hasShandianInJudge(myPlayer()))
				returnMessage = new CardMessage(false, myPlayer(), PHAND, i, myPlayer());
			break;

		case SHUNQIAN:
		{
			PlayerVec enemyVec(shunqianTarget());
			if (!enemyVec.empty())
				returnMessage = new CardMessage(false, myPlayer(), PHAND, i, enemyVec.back());
		}
			break;

		case GUOCHAI:
		{
			const PlayerVec &enemyVec = guochaiTarget();
			if (!enemyVec.empty())
				returnMessage = new CardMessage(false, myPlayer(), PHAND, i, enemyVec.back());
		}
			break;

		case JIEDAO:
		{
			pair<Player *, Player *> jiedaoPair = makeJiedaoPair();
			if (jiedaoPair != ms_invalidJiedaoPair)
			{
				returnMessage = new CardMessage(false, myPlayer(), PHAND, i);
				// the first is the attacker and the second is the victim
				returnMessage->addto(jiedaoPair.second);
				returnMessage->addto(jiedaoPair.first);
			}
		}
			break;

			// TODO reconsider whether to use this card
		case JUEDOU:
		{
			const PlayerVec &enemyVec = juedouTarget();
			if (!enemyVec.empty())
				returnMessage = new CardMessage(false, myPlayer(), PHAND, i, enemyVec.back());
		}
			break;

		case WANJIAN:
			// TODO reconsider whether to use this card
			returnMessage = new CardMessage(false, myPlayer(), PHAND, i);
			break;

		case NANMAN:
			// TODO reconsider whether to use this card
			returnMessage = new CardMessage(false, myPlayer(), PHAND, i);
			break;

		case TAOYUAN:
			// TODO reconsider whether to use this card
			returnMessage = new CardMessage(false, myPlayer(), PHAND, i);
			break;

		case WUGU:
			// TODO reconsider whether to use this card
			returnMessage = new CardMessage(false, myPlayer(), PHAND, i);
			break;

		case WUZHONG:
			returnMessage = new CardMessage(false, myPlayer(), PHAND, i);
			break;

		case SISHU:
		{
			const PlayerVec &enemyVec = sishuTarget();
			if (!enemyVec.empty())
				returnMessage = new CardMessage(false, myPlayer(), PHAND, i, enemyVec.back());
		}
			break;

		default:
			printDebug("<font color=red><b>Warning: </b></font>"
					   "AIBasic::useJinnang: jinnang type handled by default!");
			break;
		}

		// 锦囊如果被使用,跳出循环,并停止搜寻下一卡牌
		if (returnMessage)
			break;

		printDebug("AIBasic::useJinnang: cannot use " + sgsui::cardDisplayName(currentJinnang) + ", skip");
	}
	return returnMessage;
}

CardMessage * AIBasic::useZhangba()
{
	CardMessage * returnMessage = 0;
	if (canUseZhangba())
	{
		const PlayerVec &attackTargetVec = shaTarget();
		if (!attackTargetVec.empty())
			returnMessage = new CardMessage(true, myPlayer(), PEQUIP, 0, attackTargetVec.back());
	}
	return returnMessage;
}

} /* namespace sgsai */
