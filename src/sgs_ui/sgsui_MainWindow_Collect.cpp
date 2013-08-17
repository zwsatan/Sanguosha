#include "sgsui_MainWindow.h"
#include "sgsui_Shoupai.h"
#include "sgsui_ZhuangbeiArea.h"
#include "sgsui_WujiangArea.h"
#include "sgsui_PlayerArea.h"
#include "sgsui_WugufengdengBox.h"
#include "sgsui_AudioPlayer.h"
#include "sgsui_CardContainer.h"
#include "sgsui_ZhuangbeiPic.h"
#include "sgsui_output.h"
#include "sgsui_nameConvert.h"
#include "sgsui_PlayerSort.h"
#include "sgsai_AIBasic.h"
#include "sgsdata_enum.h"
#include "sgsdata_msg.h"
#include "sgsdata_player.h"

namespace sgsui {

void MainWindow::enableAllShoupai()
{
	for (auto iter = shoupaiList.begin(); iter != shoupaiList.end(); ++iter)
	{
		(*iter)->setSelectable();
		(*iter)->setUnselected();
	}
	m_selectedShoupaiList.clear();
}

void MainWindow::disableAllShoupai()
{
	for (auto iter = shoupaiList.begin(); iter != shoupaiList.end(); ++iter)
		(*iter)->setUnselectable();
	m_selectedShoupaiList.clear();
}

void MainWindow::disableAllPlayers()
{
	for (int i = 1; i <= m_playerCount; ++i)
	{
		if (i == m_playerIndex)
			m_wujiangArea->setUnselectable();
		else
			otherPlayerAreaAtIndex(i)->setUnselectable();
	}

	m_selectedPlayerAreaList.clear();
}

void MainWindow::disableAllZhuangbei()
{
	m_zhuangbeiArea->setSelectable(0, false, true);
	m_zhuangbeiArea->setSelectable(1, false, true);
	m_zhuangbeiArea->setSelectable(2, false, true);
	m_zhuangbeiArea->setSelectable(3, false, true);
	m_selectedZhuangbei = 0;
}

void MainWindow::clearCardPlayerEquipSelect()
{
	unselectAllShoupai();
	unselectAllPlayers();
	unselectAllZhuangbei();
}

void MainWindow::unselectAllShoupai()
{
	for (auto iter = shoupaiList.begin(); iter != shoupaiList.end(); ++iter)
		(*iter)->setUnselected();
	m_selectedShoupaiList.clear();
}

void MainWindow::unselectAllPlayers()
{
	for (int i = 1; i <= m_playerCount; ++i)
	{
		if (i == m_playerIndex)
			m_wujiangArea->setUnselected();
		else
			otherPlayerAreaAtIndex(i)->setUnselected();
	}
	m_selectedPlayerAreaList.clear();
}

void MainWindow::unselectAllZhuangbei()
{
	m_zhuangbeiArea->setSelected(0, false, true);
	m_zhuangbeiArea->setSelected(1, false, true);
	m_zhuangbeiArea->setSelected(2, false, true);
	m_zhuangbeiArea->setSelected(3, false, true);
	m_selectedZhuangbei = 0;
}

void MainWindow::enablePlayer(int targetPlayerIndex)
{
	if (targetPlayerIndex == m_playerIndex)
		m_wujiangArea->setSelectable();
	else
		otherPlayerAreaAtIndex(targetPlayerIndex)->setSelectable();
}

void MainWindow::enableShaTarget()
{
	sgs::DataType::Player * thisPlayer = GUIStaticData::getPlayer(m_playerIndex);

	// do it no matter the player is alive or dead
	for (int i = 1; i <= m_playerCount; ++i)
	{
		if (i == m_playerIndex)
		{
			m_wujiangArea->setUnselectable();
			continue;
		}

		sgs::DataType::Player * otherPlayer = GUIStaticData::getPlayer(i);
		PlayerArea * playerArea = otherPlayerAreaAtIndex(i);
		// 如果该选手的将卡为老诸葛,且手牌没有,则不能成为杀的目标
		if (otherPlayer->type() == sgs::ConstData::LAOZHUGE && otherPlayer->handnum() == 0)
		{
			playerArea->setUnselectable();
			continue;
		}

		// 获取双方的距离,包括加减马和马超的效果
		int distance = thisPlayer->distance(otherPlayer);
		// 获取武器可攻击的距离,默认为1
		int range = thisPlayer->range();
		playerArea->setSelectable(distance <= range);
		playerArea->setUnselected();
	}
}

void MainWindow::enableJiedaoVictim(int attackerIndex)
{
	// this function does not disable players that is already selected
	// kira is the Japaese version of English word "Killer"
	sgs::DataType::Player * kiraPlayer = GUIStaticData::getPlayer(attackerIndex);

	for (int i = 1; i <= m_playerCount; ++i)
	{
		if (i == m_playerIndex)
		{
			m_wujiangArea->setSelectable(kiraPlayer->distance(myPlayer()) <= kiraPlayer->range());
			m_wujiangArea->setUnselected();
			continue;
		}

		sgs::DataType::Player * otherPlayer = GUIStaticData::getPlayer(i);
		PlayerArea * playerArea = otherPlayerAreaAtIndex(i);

		// skip the attacker
		if (i == attackerIndex)
		{
			playerArea->setUnselectable();
			continue;
		}
		playerArea->setSelectable(kiraPlayer->distance(otherPlayer) <= kiraPlayer->range());
		playerArea->setUnselected();
	}
}

void MainWindow::enableShunqianTarget()
{
	sgs::DataType::Player * thisPlayer = GUIStaticData::getPlayer(m_playerIndex);

	// do it no matter the player is alive or dead
	for (int i = 1; i <= m_playerCount; ++i)
	{
		if (i == m_playerIndex)
		{
			m_wujiangArea->setUnselectable();
			continue;
		}

		sgs::DataType::Player * otherPlayer = GUIStaticData::getPlayer(i);
		PlayerArea * playerArea = otherPlayerAreaAtIndex(i);
		bool selectable = totalCardNum(otherPlayer) > 0;

		// 顺手牵羊,特别处理黄月英(无视距离),陆逊(不能成为指定目标)
		selectable &= (myPlayer()->type() == sgs::ConstData::HUANGYUEYING || thisPlayer->distance(otherPlayer) <= 1);
		selectable &= otherPlayer->type() != sgs::ConstData::LUXUN;

		playerArea->setSelectable(selectable);
	}
}

void MainWindow::enablePlayerWithWeapon(bool enableMyself/* = false*/)
{
	for (int i = 1; i <= m_playerCount; ++i)
	{
		if (i == m_playerIndex)
			m_wujiangArea->setSelectable(enableMyself);
		else
			otherPlayerAreaAtIndex(i)->setSelectable(hasWeapon(GUIStaticData::getPlayer(i)));
	}
}

void MainWindow::enableAllPlayers(bool hasCard/* = false*/, bool enableMyself/* = false*/)
{
	// do it no matter the player is alive or dead
	for (int i = 1; i <= m_playerCount; ++i)
	{
		if (i == m_playerIndex)
		{
			m_wujiangArea->setSelectable(enableMyself);
			continue;
		}

		// 设置为可以选,但是未选中状态
		otherPlayerAreaAtIndex(i)->setSelectable();
		otherPlayerAreaAtIndex(i)->setUnselected();

		// 检查卡牌数目,包括装备卡,判定区的卡
		if (hasCard && totalCardNum(GUIStaticData::getPlayer(i)) < 1)
			otherPlayerAreaAtIndex(i)->setUnselectable();
	}
}

void MainWindow::enableJuedouTarget()
{
	for (int i = 1; i <= m_playerCount; ++i)
	{
		if (i == m_playerIndex)
		{
			m_wujiangArea->setUnselectable();
			continue;
		}

		sgs::DataType::Player * otherPlayer = GUIStaticData::getPlayer(i);
		PlayerArea * playerArea = otherPlayerAreaAtIndex(i);

		// 老诸葛,手牌数为0,不能成为决斗目标
		if (otherPlayer->type() == sgs::ConstData::LAOZHUGE && otherPlayer->handnum() == 0)
		{
			playerArea->setUnselectable();
			continue;
		}

		playerArea->setSelectable();
	}
}

void MainWindow::enableSishuTarget()
{
	for (int i = 1; i <= m_playerCount; ++i)
	{
		if (i == m_playerIndex)
		{
			m_wujiangArea->setUnselectable();
			continue;
		}

		sgs::DataType::Player * otherPlayer = GUIStaticData::getPlayer(i);

		// 陆逊不能成为目标,同时在判定区不能再有乐不思蜀
		bool selectable = !hasSishuInJudge(GUIStaticData::getPlayer(i)) && otherPlayer->type() != sgs::ConstData::LUXUN;
		otherPlayerAreaAtIndex(i)->setSelectable(selectable);
	}
}

void MainWindow::enableQingnangTarget()
{
	for (int i = 1; i <= m_playerCount; ++i)
	{
		sgs::DataType::Player * otherPlayer = GUIStaticData::getPlayer(i);

		if (i == m_playerIndex)
		{
			m_wujiangArea->setSelectable(otherPlayer->hp() < otherPlayer->maxHP());
			continue;
		}
		// 只要对方费血,即可成为青囊的可选对象
		otherPlayerAreaAtIndex(i)->setSelectable(otherPlayer->hp() < otherPlayer->maxHP());
	}
}

void MainWindow::enableMalePlayers()
{
	for (int i = 1; i <= m_playerCount; ++i)
	{
		sgs::DataType::Player * otherPlayer = GUIStaticData::getPlayer(i);
		if (i == m_playerIndex)
		{
			m_wujiangArea->setUnselectable();
			continue;
		}

		otherPlayerAreaAtIndex(i)->setSelectable(otherPlayer->sex() == sgs::ConstData::MALE);
	}
}

void MainWindow::enableTuxiTarget()
{
	for (int i = 1; i <= m_playerCount; ++i)
	{
		sgs::DataType::Player * otherPlayer = GUIStaticData::getPlayer(i);
		if (i == m_playerIndex)
		{
			m_wujiangArea->setUnselectable();
			continue;
		}

		otherPlayerAreaAtIndex(i)->setSelectable(otherPlayer->handnum() > 0);
	}
}

void MainWindow::enableJieyinTarget()
{
	for (int i = 1; i <= m_playerCount; ++i)
	{
		sgs::DataType::Player * otherPlayer = GUIStaticData::getPlayer(i);
		if (i == m_playerIndex)
		{
			m_wujiangArea->setUnselectable();
			continue;
		}

		// 结姻对象必须为男性且费血
		bool selectable = otherPlayer->sex() == sgs::ConstData::MALE && otherPlayer->hp() < otherPlayer->maxHP();
		otherPlayerAreaAtIndex(i)->setSelectable(selectable);
	}
}

void MainWindow::enableLiuliTarget()
{
	sgs::DataType::Player * thisPlayer = GUIStaticData::getPlayer(m_playerIndex);

	// do it no matter the player is alive or dead
	for (int i = 1; i <= m_playerCount; ++i)
	{
		if (i == m_playerIndex)
		{
			m_wujiangArea->setUnselectable();
			continue;
		}

		sgs::DataType::Player *	otherPlayer = GUIStaticData::getPlayer(i);
		PlayerArea * playerArea = otherPlayerAreaAtIndex(i);

		// 老诸葛无手牌的情况下,依旧不受流离效果
		if (otherPlayer->type() == sgs::ConstData::LAOZHUGE && otherPlayer->handnum() == 0)
		{
			playerArea->setUnselectable();
			continue;
		}

		// 流离的攻击者不能成为流离的目标
		if (otherPlayer == m_liuliAttacker)
		{
			playerArea->setUnselectable();
			continue;
		}

		// 可被攻击的对象,只能是在攻击范围以内
		int distance = thisPlayer->distance(otherPlayer);
		int range = thisPlayer->range();
		bool selectAble = distance <= range;
		playerArea->setSelectable(selectAble);
		playerArea->setUnselected();
	}
}

void MainWindow::okClicked()
{
	// clearAbandonData() in discardCard() will do the following work
	unblock();
}

void MainWindow::cancelClicked()
{

	if (m_selectedSkill != sgs::ConstData::heroSkillNone)
	{
		printDebug("MainWindow::cancelClicked: handle skill");
		m_selectedSkill = sgs::ConstData::heroSkillNone;
		m_selectedSkillCardVec.clear();
		clearCardPlayerEquipSelect();
		cardNormalize();
		skillNormalize();
		checkButtons();
		return;
	}

	// unselect selected zhuangbei;
	if (m_selectedZhuangbei)
	{
		printDebug("MainWindow::cancelClicked: handle zhuangbei");
		zhuangbeiClicked(m_selectedZhuangbei);
		checkButtons();
		return;
	}

	if (m_responseType == DiscardCard)
	{
		// Ensure that all cards are available and unselected
		for (auto iter = shoupaiList.begin(); iter != shoupaiList.end(); ++iter)
			(*iter)->setUnselected();

		selectedAbandonList.clear();
		okButton->setDisabled();
	}
	else
	{
		switch (m_responseType)
		{
		case UseCard:
			for (unsigned int i = 0; i < m_selectedShoupaiList.size(); ++i)
			{
				newUnselectedCard(m_selectedShoupaiList.back());
				// Do not call popback here, newUnselectedCard will remove cards
			}
			break;
		default:
			m_cancelResponse = true;
			unblock();
			break;
		}
	}
}

void MainWindow::resetSkillSelect()
{
	printDebug("MainWindow::resetSkillSelect: reset");
	m_selectedSkill = sgs::ConstData::heroSkillNone;
	m_selectedSkillCardVec.clear();
	clearCardPlayerEquipSelect();
	cardNormalize();
	skillNormalize();
	checkButtons();
	return;
}

void MainWindow::abandonClicked()
{
	if (m_responseType == UseCard)
	{
		m_isGoingToAbandon = true;
		unblock();
	}
}

void MainWindow::skillClicked(sgs::ConstData::HeroSkill skill, int skillIndex)
{
	printDebug("MainWindow::skillClicked: skill: " + skillDisplayName(skill));

	// disable the skill button to avoid the player to click again
	m_wujiangArea->disableSkillButton(skillIndex);

	// when a skill button is clicked, first clear previous select
	clearCardPlayerEquipSelect();
	m_selectedSkill = skill;
	skillCardNormalize();
	skillPlayerChooseNormalize();
	checkButtons();
}

void MainWindow::cardClicked(CardButton * clickedCard)
{
	// newSelectedCard and newUnselectedCard will process skill choose card

	Shoupai * clickedShoupai = static_cast<Shoupai *>(clickedCard);

	if (m_responseType == DiscardCard)
	{
		if (clickedCard->isSelected())
		{
			clickedCard->setUnselected();
			selectedAbandonList.remove(clickedShoupai);
		}
		else
		{
			clickedCard->setSelected();
			selectedAbandonList.push_back(clickedShoupai);
			if (selectedAbandonList.size() > shoupaiNumToDiscard)
			{
				selectedAbandonList.front()->setUnselected();
				selectedAbandonList.pop_front();
			}
		}

		if (selectedAbandonList.size() == shoupaiNumToDiscard)
			okButton->setEnabled();
		else
			okButton->setDisabled();

	}
	else
	{
		if (clickedShoupai->isSelected())
			newUnselectedCard(clickedShoupai);
		else
			newSelectedCard(clickedShoupai);
	}
}

void MainWindow::zhuangbeiClicked(ZhuangbeiPic * clickedEquip)
{
	if (clickedEquip->isSelected())
	{
		clickedEquip->setUnselected();
		cardNormalize();
		m_selectedZhuangbei = 0;
		playerChooseNormalize();
		checkButtons();
	}
	else
	{
		clickedEquip->setSelected();
		disableAllShoupai();
		m_selectedZhuangbei = clickedEquip;
		playerChooseNormalize();
		checkButtons();
	}
}

void MainWindow::playerClicked(AbstractPlayerArea * clickedPlayerArea)
{
	if (clickedPlayerArea->isSelected())
		newUnselectedPlayer(clickedPlayerArea);
	else
		newSelectedPlayer(clickedPlayerArea);
}

int MainWindow::getIndex(Shoupai * clickedCard) const
{
	const sgs::DataType::Card * platformCard = clickedCard->platformCard();
	sgs::DataType::Player * player = GUIStaticData::getPlayer(m_playerIndex);
	for (int i = 0; i < player->handnum(); ++i)
	{
		if (player->hand(i) == platformCard)
			return i;
	}

	printDebug("<font color=red><b>Warning: </b></font>"
			   "MainWindow::getIndex: cannot find the card "
			   + cardFullDisplayName(platformCard, false)
			   + " in Player's hand, pause game to check bug");
	pauseButton->click();
	return -1;
}

Shoupai* MainWindow::goToCard(const sgs::DataType::Card * card) const
{
	for (auto iter = shoupaiList.begin(); iter != shoupaiList.end(); ++iter)
	{
		if ((*iter)->platformCard() == card)
			return *iter;
	}

	printDebug("<font color=red><b>Warning: </b></font>"
			   "MainWindow::goToCard: cannot find the certain card in shoupai, card: "
			   + cardFullDisplayName(card, false)
			   + ", pause game to check bug");
	pauseButton->click();
	return 0;
}

void MainWindow::newSelectedCard(Shoupai * selectedCard)
{
	if (!selectedCard)
		return;

	m_selectedShoupaiList.push_back(selectedCard);
	if (m_selectedShoupaiList.size() > m_maxShoupaiSelect)
		newUnselectedCard(m_selectedShoupaiList.front());
	// Do not call popfront here, newUnselectedCard will remove old card

	selectedCard->setSelected();
	if (m_selectedSkill != sgs::ConstData::heroSkillNone)
		skillPlayerChooseNormalize();
	else
		playerChooseNormalize();
	checkButtons();
}

void MainWindow::newUnselectedCard(Shoupai * unselectedCard)
{
	if (!unselectedCard)
		return;

	m_selectedShoupaiList.remove(unselectedCard);
	unselectedCard->setUnselected();

	if (m_selectedSkill != sgs::ConstData::heroSkillNone)
		skillPlayerChooseNormalize();
	else
		playerChooseNormalize();
	checkButtons();
}

void MainWindow::newSelectedPlayer(AbstractPlayerArea * playerArea)
{
	if (!playerArea)
		return;

	bool isSelectedShoupaiEmpty = m_selectedShoupaiList.empty();
	bool isJieDaoCard = !isSelectedShoupaiEmpty &&  m_selectedShoupaiList.back()->card()->getCardType() == sgs::ConstData::JIEDAO;
	if (isJieDaoCard && m_selectedSkill == sgs::ConstData::heroSkillNone)
	{
		// if first player (i.e. the weapon owner) is clicked
		if (m_selectedPlayerAreaList.empty())
		{
			showTipMessage(GUIStaticData::chooseJiedaoVictim);
			enableJiedaoVictim(playerArea->playerIndex());
		}
	}

	playerArea->setSelected();
	m_selectedPlayerAreaList.push_back(playerArea);
	if (m_selectedPlayerAreaList.size() > m_maxPlayerSelect)
	{
		printDebug("MainWindow::newSelectedPlayer: in");
		newUnselectedPlayer(m_selectedPlayerAreaList.front());
		// Do not call popfront here, newUnselectedPlayer will remove old player
		// Do not remove tuxi, newUnselectedPlayer will do it
	}

	if (m_responseType == GetTuxi)
	{
		QString text(trUtf8("选择目标的一张手牌"));
		sgs::DataType::Player * targetPlayer = playerArea->getPlayer();

		std::pair<sgs::ConstData::PlayerRegionType, int> temp1(cardContainer->chooseCard(text, targetPlayer, true, false, false, 1, sgs::ConstData::TUXI).back());

		std::pair<int, AbstractPlayerArea*> temp2(temp1.second, playerArea);
		m_tuxiList.push_back(temp2);
		printDebug("MainWindow::newSelectedPlayer: temp");
	}
	checkButtons();
}

void MainWindow::newUnselectedPlayer(AbstractPlayerArea * playerArea)
{
	if (!playerArea)
		return;

	// 处理借刀的情况
	bool isShoupaiEmpty = m_selectedShoupaiList.empty();
	bool isJieDaoCard = !isShoupaiEmpty && m_selectedShoupaiList.back()->card()->getCardType() == sgs::ConstData::JIEDAO;
	if (isJieDaoCard)
	{
		// if both attacker and victim are selected
		// and the unselected player is the attacker
		if (m_selectedPlayerAreaList.size() == 2 && m_selectedPlayerAreaList.front() == playerArea)
		{
			showTipMessage(GUIStaticData::chooseWeaponOwner);
			m_selectedPlayerAreaList.back()->setUnselectable();
			m_selectedPlayerAreaList.pop_back();
		}
		else
		{
			showTipMessage(GUIStaticData::chooseJiedaoVictim);
		}
	}

	m_selectedPlayerAreaList.remove(playerArea);

	if (m_responseType == GetTuxi)
	{
		if (m_tuxiList.front().second == playerArea)
			m_tuxiList.pop_front();
		else
			m_tuxiList.pop_back();
	}

	playerArea->setUnselected();
	checkButtons();
}

sgs::Derive::CardMessage * MainWindow::collectZhuangbei()
{
	sgs::Derive::CardMessage * returnMessage = 0;
	if (m_selectedZhuangbei)
	{
		returnMessage = new sgs::Derive::CardMessage(true, GUIStaticData::getPlayer(m_playerIndex), sgs::ConstData::PEQUIP, 0);

		std::vector<sgs::DataType::Player *> targetPlayerVec;
		for (auto iter = m_selectedPlayerAreaList.begin(); iter != m_selectedPlayerAreaList.end(); ++iter)
			targetPlayerVec.push_back((*iter)->getPlayer());

		// sort the selected players from small to big;
		std::sort(targetPlayerVec.begin(), targetPlayerVec.end(), PlayerSort(m_playerIndex - 1, BySeat));
		for (auto iter = targetPlayerVec.begin(); iter != targetPlayerVec.end(); ++iter)
			returnMessage->addto(*iter);

		printDebug("MainWindow::collectZhuangbei: using zhuangbei");
	}
	return returnMessage;
}

sgs::Derive::SkillMessage * MainWindow::collectSkill()
{
	using namespace sgs::ConstData;
	using namespace sgs::Derive;
	if (m_selectedSkill == heroSkillNone)
		return 0;

	SkillMessage * returnSkill = 0;

	switch (m_selectedSkill)
	{
	case GUOSE:
	{
		sgs::ConstData::PlayerRegionType playerRegion = m_selectedSkillCardVec.back().first;
		int pos = m_selectedSkillCardVec.back().second;
		sgs::DataType::Player *	player = m_selectedPlayerAreaList.back()->getPlayer();
		GuoSeMessage * returnGuose = new GuoSeMessage(myPlayer(), playerRegion, pos, player);
		returnSkill = returnGuose;
	}
		break;

	case JIEYIN:
	{
		int cardIndex1 = getIndex(m_selectedShoupaiList.front());
		int cardIndex2 = getIndex(m_selectedShoupaiList.back());

		// from big to small
		if (cardIndex1 < cardIndex2)
			std::swap(cardIndex1, cardIndex2);

		JieYinMessage * returnJieyin = new JieYinMessage(myPlayer(), m_selectedPlayerAreaList.back()->getPlayer(), cardIndex1, cardIndex2);
		returnSkill = returnJieyin;
	}
		break;

	case FANJIAN:
	{
		FanJianMessage * returnFanjian = new FanJianMessage(myPlayer(), m_selectedPlayerAreaList.back()->getPlayer());
		returnSkill = returnFanjian;
	}
		break;

	case ZHIHENG:
	{
		ZhiHengMessage * returnZhiheng = new ZhiHengMessage(myPlayer());
		for (auto citer = m_selectedSkillCardVec.begin(); citer != m_selectedSkillCardVec.end(); ++citer)
			returnZhiheng->transpush((*citer).first, (*citer).second);
		returnSkill = returnZhiheng;
	}
		break;

	case LIJIAN:
	{
		sgs::DataType::Player * juedou1 = (m_selectedPlayerAreaList.front()->getPlayer());
		sgs::DataType::Player * juedou2 = (m_selectedPlayerAreaList.back()->getPlayer());
		LiJianMessage * returnLijian = new LiJianMessage(myPlayer(), m_selectedSkillCardVec.back().first, m_selectedSkillCardVec.back().second, juedou1, juedou2);
		returnSkill = returnLijian;
	}
		break;

	case HUJIA:
	{
		SkillMessage * returnHujia = new SkillMessage(myPlayer(), HUJIA);
		returnSkill = returnHujia;
	}
		break;

	case JIJIANG:
	{
		JiJiangMessage * returnJijiang = new JiJiangMessage(myPlayer());
		std::vector<sgs::DataType::Player *> targetPlayerVec;
		for (auto iter = m_selectedPlayerAreaList.begin(); iter != m_selectedPlayerAreaList.end(); ++iter)
			targetPlayerVec.push_back((*iter)->getPlayer());

		// sort the selected players from small to big (by seat);
		std::sort(targetPlayerVec.begin(), targetPlayerVec.end(), PlayerSort(m_playerIndex - 1, BySeat));
		for (auto iter = targetPlayerVec.begin(); iter != targetPlayerVec.end(); ++iter)
			returnJijiang->addto(*iter);

		returnSkill = returnJijiang;
	}
		break;

	case RENDE:
	{
		RenDeMessage * returnRende = new RenDeMessage(myPlayer(), m_selectedPlayerAreaList.back()->getPlayer());
		std::vector<int> cardVec;
		for (auto iter = m_selectedShoupaiList.begin(); iter != m_selectedShoupaiList.end(); ++iter)
			cardVec.push_back(getIndex(*iter));

		// push the big one first
		// sort the selected cards from big to small
		std::sort(cardVec.rbegin(), cardVec.rend());
		for (auto iter = cardVec.begin(); iter != cardVec.end(); ++iter)
			returnRende->transpush(*iter);
		returnSkill = returnRende;
	}
		break;

	case QINGNANG:
	{
		printDebug("MainWindow::collectSkill: collect QINGNANG");
		int index = getIndex(m_selectedShoupaiList.back());
		sgs::DataType::Player * player = m_selectedPlayerAreaList.back()->getPlayer();
		QingNangMessage * returnQingnang = new QingNangMessage(myPlayer(), index, player);
		returnSkill = returnQingnang;
	}
		break;

	case JIJIU:
	{
		printDebug("MainWindow::collectSkill: collect JIJIU");
		JiJiuMessage * returnJijiu = new JiJiuMessage(myPlayer(), m_selectedSkillCardVec.back().first, m_selectedSkillCardVec.back().second);
		returnSkill = returnJijiu;
	}
		break;

	case QIXI:
	{
		printDebug("MainWindow::collectSkill: collect QIXI");
		QiXiMessage * returnQixi = new QiXiMessage(myPlayer(), m_selectedSkillCardVec.back().first, m_selectedSkillCardVec.back().second, m_selectedPlayerAreaList.back()->getPlayer());
		returnSkill = returnQixi;
	}
		break;

	case LONGDAN:
	{
		printDebug("MainWindow::collectSkill: collect LONGDAN");
		LongDanMessage * returnLongdan = new LongDanMessage(myPlayer(), getIndex(m_selectedShoupaiList.back()));
		std::vector<sgs::DataType::Player *> targetPlayerVec;
		for (auto iter = m_selectedPlayerAreaList.begin(); iter != m_selectedPlayerAreaList.end(); ++iter)
			targetPlayerVec.push_back((*iter)->getPlayer());

		// sort the selected players from small to big;
		std::sort(targetPlayerVec.begin(), targetPlayerVec.end(), PlayerSort(m_playerIndex - 1, BySeat));
		for (auto iter = targetPlayerVec.begin(); iter != targetPlayerVec.end(); ++iter)
			returnLongdan->addto(*iter);

		returnSkill = returnLongdan;
	}
		break;

	case WUSHENG:
	{
		printDebug("MainWindow::collectSkill: collect WUSHENG");
		WuShengMessage * returnWusheng = new WuShengMessage(myPlayer(), m_selectedSkillCardVec.back().first, m_selectedSkillCardVec.back().second);
		std::vector<sgs::DataType::Player *> targetPlayerVec;
		for (auto iter = m_selectedPlayerAreaList.begin(); iter != m_selectedPlayerAreaList.end(); ++iter)
			targetPlayerVec.push_back((*iter)->getPlayer());

		// sort the selected players from small to big;
		std::sort(targetPlayerVec.begin(), targetPlayerVec.end(), PlayerSort(m_playerIndex - 1, BySeat));
		for (auto iter = targetPlayerVec.begin(); iter != targetPlayerVec.end(); ++iter)
			returnWusheng->addto(*iter);

		returnSkill = returnWusheng;
	}
		break;

	case QINGGUO:
	{
		printDebug("MainWindow::collectSkill: collect Qingguo");
		QingGuoMeesage * returnQingguo = new QingGuoMeesage(myPlayer(), getIndex(m_selectedShoupaiList.back()));
		returnSkill = returnQingguo;
	}
		break;

	case KUROU:
	{
		printDebug("MainWindow::collectSkill: collect Kurou");
		SkillMessage * returnKurou = new SkillMessage(myPlayer(), KUROU);
		return returnKurou;
	}
		break;

	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "MainWindow::collectSkill: collection handled by default, skill "
				   + skillDisplayName(m_selectedSkill));
		pause();
		return returnSkill;
		break;
	}
	return returnSkill;
}

} /* namespace sgsui */
