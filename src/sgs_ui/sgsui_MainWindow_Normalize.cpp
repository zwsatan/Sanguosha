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
#include <algorithm>

namespace sgsui {

// 决定最小和最大的手牌可选数目
void MainWindow::cardNormalize()
{
	printDebug("MainWindow::cardNormalize: start");
	// Note: Discard card will not be handled here.
	// all the Discard card repsonse will be handled completely along

	// this function is called only at the beginning of a response, when all the cards are unselected
	// so all the players should be disabled since no card is selected
	// therefore, disable all the players first and
	// wait for the NormalizePlayer() to enable player select when a card is selected.
	disableAllPlayers();
	// if a response let user choose players without choosing any cards,
	// the following case of the response will enable player select

	if (m_selectedSkill != sgs::ConstData::heroSkillNone)
	{
		printDebug("<font color=red><b>Warning: </b></font>MainWindow::cardNormalize: selectedSkill not empty");
		skillCardNormalize();
		return;
	}

	// handle those case that are not using skills

	// handle equipCard
	disableAllZhuangbei();
	switch (m_responseType)
	{
	case UseCard:
		m_zhuangbeiArea->setSelectable(0, canUseZhangba(), true);
		break;
	case ShaOrNot:
		m_zhuangbeiArea->setSelectable(0, !m_skillVerShaOrNot && canUseZhangba(), true);
		break;
	default:
		break;
	}

	// handle handCard
	using namespace sgs::ConstData;
	switch (m_responseType)
	{
	case UseCard:
		m_maxShoupaiSelect = 1;
		m_minShoupaiSelect = 1;
		for (auto iter = m_shoupaiList.begin(); iter != m_shoupaiList.end(); ++iter)
		{
			sgs::ConstData::CardType cardType = (*iter)->card()->getCardType();
			switch (cardType)
			{
			case SHAN:
				(*iter)->setUnselectable();
				break;

			case SHA:
				(*iter)->setSelectable(canUseSha());
				break;

			case TAO:
				(*iter)->setSelectable(m_wujiangArea->getHP() < m_wujiangArea->getMaxHP());
				break;

				// 防具
			case BAGUA:
			case RENWANG:

				// 加减马
			case CHITU:
			case DAWAN:
			case ZICI:
			case DILU:
			case JUEYING:
			case ZHUAHUANG:
			case HUALIU:

				// 武器
			case ZHUGE:
			case QINGGANG:
			case HANBING:
			case CIXIONG:
			case GUANSHI:
			case QINGLONG:
			case GUDING:
			case ZHANGBA:
			case FANGTIAN:
			case QILIN:
				(*iter)->setSelectable();
				break;

				// 锦囊
			case SISHU:
			case GUOCHAI:
			case JIEDAO:
				// let User select this card, no matter whether it can use
				// if there's no weapon owner or no target in any weapon owner,
				// then User cannot use this card due to impossible player select
			case JUEDOU:
			case NANMAN:
			case SHUNQIAN:
			case TAOYUAN:
			case WANJIAN:
			case WUZHONG:
			case WUGU:
				(*iter)->setSelectable();
				break;

			case SHANDIAN:
				// 判定区没有闪电,则闪电锦囊可选
				(*iter)->setSelectable(!hasShandianInJudge(myPlayer()));
				break;

			case WUXIE:
				(*iter)->setUnselectable();
				break;

			default:
				printDebug("<font color=red><b>Warning: </b></font>"
						   "MainWindow::cardNormalize: "
						   "Usecard normalize handled by default, card: "
						   + cardDisplayName(cardType));
				(*iter)->setUnselectable();
				break;
			}
		}
		break;

	case ShanOrNot:
		m_maxShoupaiSelect = 1;
		m_minShoupaiSelect = 1;
		for (auto iter = m_shoupaiList.begin(); iter != m_shoupaiList.end(); ++iter)
		{
			if ((*iter)->card()->getCardType() == sgs::ConstData::SHAN)
				(*iter)->setSelectable();
			else
				(*iter)->setUnselectable();
		}
		break;

	case TaoOrNot:
		m_maxShoupaiSelect = 1;
		m_minShoupaiSelect = 1;
		for (auto iter = m_shoupaiList.begin(); iter != m_shoupaiList.end(); ++iter)
		{
			if ((*iter)->card()->getCardType() == sgs::ConstData::TAO)
				(*iter)->setSelectable();
			else
				(*iter)->setUnselectable();
		}
		break;

	case ShaOrNot:
		m_maxShoupaiSelect = 1;
		m_minShoupaiSelect = 1;
		for (auto iter = m_shoupaiList.begin(); iter != m_shoupaiList.end(); ++iter)
		{
			if ((*iter)->card()->getCardType() == sgs::ConstData::SHA)
				(*iter)->setSelectable();
			else
				(*iter)->setUnselectable();
		}
		break;

	case WuxieOrNot:
		m_maxShoupaiSelect = 1;
		m_minShoupaiSelect = 1;
		for (auto iter = m_shoupaiList.begin(); iter != m_shoupaiList.end(); ++iter)
		{
			if ((*iter)->card()->getCardType() == sgs::ConstData::WUXIE)
				(*iter)->setSelectable();
			else
				(*iter)->setUnselectable();
		}
		break;

	case GetTuxi:
		disableAllShoupai();
		m_maxShoupaiSelect = 0;
		m_minShoupaiSelect = 0;
		break;

	case GetYiji:
		disableAllShoupai();
		m_maxShoupaiSelect = 0;
		m_minShoupaiSelect = 0;
		break;

	case GetLiuli:
		disableAllShoupai();
		m_maxShoupaiSelect = 0;
		m_minShoupaiSelect = 0;
		m_selectedSkillCardVec = m_cardContainer->chooseSkillCard(LIULI);
		if (m_selectedSkillCardVec.empty())
		{
			m_cancelResponse = true;
			unblock();
			return;
		}
		break;

	default:
		break;
	}

	printDebug("MainWindow::cardNormalize: over");
}

// 处理player点击卡牌之后,包括技能或手牌或装备卡,确定最小和最大的可选目标数目
void MainWindow::playerChooseNormalize()
{
	printDebug("MainWindow::playerChooseNormalize: start");

	if (m_selectedSkill != sgs::ConstData::heroSkillNone)
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "MainWindow::playerChooseNormalize: selectedSkill not empty");
		skillPlayerChooseNormalize();
		return;
	}

	// unselect all players first
	unselectAllPlayers();

	using namespace sgs::ConstData;
	// 处理装备卡的情况
	if (m_selectedZhuangbei)
	{
		if (m_responseType == UseCard)
		{
			enableShaTarget();
			showTipMessage(GUIStaticData::chooseZhangbaTarget);
			m_maxPlayerSelect = 1;
			m_minPlayerSelect = 1;
		}
		else
		{
			disableAllPlayers();
			m_maxPlayerSelect = 0;
			m_minPlayerSelect = 0;
		}
		return;
	}

	// the case that shoupaiList is empty should be handled here
	// all the player select for TAO is not done by user;
	switch (m_responseType)
	{
	case UseCard:
		if (m_selectedShoupaiList.empty())
		{
			disableAllPlayers();
			m_maxPlayerSelect = 0;
			m_minPlayerSelect = 0;
		}
		else
		{
			CardType cardType = m_selectedShoupaiList.back()->card()->getCardType();
			switch (cardType)
			{
			case SHA:
				enableShaTarget();
				// 方天画戟特别处理
				if (canUseFangtian() && GUIStaticData::getPlayerCount() > 2)
				{
					if (GUIStaticData::getPlayerCount() > 3)
						showTipMessage(GUIStaticData::threeTargets);
					else
						showTipMessage(GUIStaticData::twoTargets);
					m_maxPlayerSelect = 3;
				}
				else
				{
					m_maxPlayerSelect = 1;
				}
				m_minPlayerSelect = 1;
				break;

			case TAO:
				disableAllPlayers();
				m_maxPlayerSelect = 0;
				m_minPlayerSelect = 0;
				break;

			// 装备
			case CHITU:
			case DAWAN:
			case ZICI:
			case DILU:
			case JUEYING:
			case ZHUAHUANG:
			case HUALIU:
			case BAIYIN:
			case TENGJIA:
			case BAGUA:
			case RENWANG:
			case ZHUGE:
			case QINGGANG:
			case HANBING:
			case CIXIONG:
			case GUANSHI:
			case YINYUE:
			case QINGLONG:
			case GUDING:
			case ZHANGBA:
			case FANGTIAN:
			case QILIN:
			case ZHUQUE:
				disableAllPlayers();
				m_maxPlayerSelect = 0;
				m_minPlayerSelect = 0;
				break;

			// 锦囊
			case SHANDIAN:
				disableAllPlayers();
				m_maxPlayerSelect = 0;
				m_minPlayerSelect = 0;
				break;

			case SHUNQIAN:
				enableShunqianTarget();
				m_maxPlayerSelect = 1;
				m_minPlayerSelect = 1;
				break;

			case GUOCHAI:
				// 只允许有牌的选手响应,包括装备卡,判断卡
				enableAllPlayers(true);
				m_maxPlayerSelect = 1;
				m_minPlayerSelect = 1;
				break;

			case JIEDAO:
				// this is called for the fisrt player select (the attacker)
				// other work will be done in newSelectedPlayer and newUnselectedPlayer
				enablePlayerWithWeapon();
				showTipMessage(GUIStaticData::chooseWeaponOwner);
				m_maxPlayerSelect = 2;
				m_minPlayerSelect = 2;
				break;

			case JUEDOU:
				enableJuedouTarget();
				m_maxPlayerSelect = 1;
				m_minPlayerSelect = 1;
				break;

			case WANJIAN:
			case NANMAN:
			case TAOYUAN:
			case WUGU:
			case WUZHONG:
				disableAllPlayers();
				m_minPlayerSelect = 0;
				m_minPlayerSelect = 0;
				break;

			case SISHU:
				enableSishuTarget();
				m_maxPlayerSelect = 1;
				m_minPlayerSelect = 1;
				break;

			default:
				printDebug("<font color=red><b>Warning: </b></font>"
						   "MainWindow::playerChooseNormalize: case handled by default");
				break;
			}
		}
		break;

	case TaoOrNot:
		disableAllPlayers();
		m_maxPlayerSelect = 0;
		m_minPlayerSelect = 0;
		break;

	case ShanOrNot:
		disableAllPlayers();
		m_maxPlayerSelect = 0;
		m_minPlayerSelect = 0;
		break;

	case ShaOrNot:
		disableAllPlayers();
		m_maxPlayerSelect = 0;
		m_minPlayerSelect = 0;
		break;

	case GetTuxi:
		enableTuxiTarget();
		m_maxPlayerSelect = 2;
		m_minPlayerSelect = 1;
		break;

	case GetYiji:
		// 郭嘉的遗计效果允许对自己使用
		enableAllPlayers(false, true);
		m_maxPlayerSelect = 1;
		m_minPlayerSelect = 1;
		break;

	case GetLiuli:
		showTipMessage(trUtf8("选择一名要转移到的角色（不能是杀的使用者）"));
		enableLiuliTarget();
		m_maxPlayerSelect = 1;
		m_minPlayerSelect = 1;
		break;

	default:
		break;
	}

	printDebug("MainWindow::playerChooseNormalize: over");
}

// 用来决定技能按钮是否可以点击
void MainWindow::skillNormalize()
{
	printDebug("MainWindow::skillNormalize: start");
	using namespace sgs::ConstData;

	// this function should be called at the beginning of a response (together with cardNormalize() )
	// almost all the active skill button (i.e skill button clicked by User) can be only clicked in main;
	sgs::ConstData::HeroSkill skill = sgs::ConstData::heroSkillNone;
	// 这里作出限定,一个英雄最多4个技能
	switch (m_responseType)
	{
	case UseCard:
		for (int i = 0; i < 4; ++i)
		{
			skill = m_wujiangArea->getSkill(i);
			if (skill == sgs::ConstData::heroSkillNone)
				break;

			switch (skill)
			{
			case GUOSE:
				m_wujiangArea->enableSkillButton(i, haveDiamondCard());
				break;

			case JIEYIN:
				m_wujiangArea->enableSkillButton(i, canUseJieyin());
				break;

			case FANJIAN:
				m_wujiangArea->enableSkillButton(i, myPlayer()->handnum() > 0 && !(myPlayer()->status() & sgs::ConstData::SkillOneUsed));
				break;

			case ZHIHENG:
				m_wujiangArea->enableSkillButton(i, !(myPlayer()->status() & sgs::ConstData::SkillOneUsed));
				break;

			case LIJIAN:
				m_wujiangArea->enableSkillButton(i, handEquipNum(myPlayer()) > 0 && !(myPlayer()->status() & sgs::ConstData::SkillOneUsed));
				break;

			case JIJIANG:
				m_wujiangArea->enableSkillButton(i, myPlayer()->role() == sgs::ConstData::ZHU && canUseSha());
				break;

			case RENDE:
				m_wujiangArea->enableSkillButton(i, myPlayer()->handnum() > 0);
				break;

			case QINGNANG:
				m_wujiangArea->enableSkillButton(i, !(myPlayer()->status() & sgs::ConstData::SkillOneUsed));
				break;

			case KUROU:
				m_wujiangArea->enableSkillButton(i);
				break;

			case QIXI:
				m_wujiangArea->enableSkillButton(i, haveBlackCard());
				break;

			case LONGDAN:
				m_wujiangArea->enableSkillButton(i, canUseSha());
				break;

			case WUSHENG:
				m_wujiangArea->enableSkillButton(i, canUseSha() && haveRedCard());
				break;

			default:
				m_wujiangArea->disableSkillButton(i);
				break;
			}
		}
		break;

	case ShaOrNot:
		for (int i = 0; i < 4; ++i)
		{
			skill = m_wujiangArea->getSkill(i);
			if (skill == sgs::ConstData::heroSkillNone)
				break;

			switch (skill)
			{
			case JIJIANG:
				m_wujiangArea->enableSkillButton(i, myPlayer()->role() == sgs::ConstData::ZHU);
				break;

			case LONGDAN:
			case WUSHENG:
				m_wujiangArea->enableSkillButton(i, haveRedCard());
				break;

			default:
				m_wujiangArea->disableSkillButton(i);
				break;
			}
		}
		break;

	case ShanOrNot:
		for (int i = 0; i < 4; ++i)
		{
			skill = m_wujiangArea->getSkill(i);
			if (skill == sgs::ConstData::heroSkillNone)
				break;

			switch (skill)
			{
			case HUJIA:
				m_wujiangArea->enableSkillButton(i, myPlayer()->role() == sgs::ConstData::ZHU);
				break;

			case LONGDAN:
				m_wujiangArea->enableSkillButton(i);
				break;

			case QINGGUO:
				m_wujiangArea->enableSkillButton(i);
				break;

			default:
				m_wujiangArea->disableSkillButton(i);
				break;
			}
		}
		break;

	case TaoOrNot:
		for (int i = 0; i < 4; ++i)
		{
			skill = m_wujiangArea->getSkill(i);
			if (skill == sgs::ConstData::heroSkillNone)
				break;

			switch (skill)
			{
			case JIJIU:
				m_wujiangArea->enableSkillButton(i, handEquipNum(myPlayer()) > 0);
				break;
			default:
				m_wujiangArea->disableSkillButton(i);
				break;
			}
		}
		break;

	default:
		m_wujiangArea->disableAllSkillButtons();
		break;
	}
	printDebug("MainWindow::skillNormalize: over");
}

// 选中技能之后,决定什么卡牌可以被点击
void MainWindow::skillCardNormalize()
{
	// All the skills that useds both skillCardNormalize and chooseSkillCard

	printDebug("MainWindow::skillCardNormalize: start");
	using namespace sgs::ConstData;
	switch (m_selectedSkill)
	{
	case heroSkillNone:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "MainWindow::skillChooseCardNormalize: no selected skill");
		disableAllShoupai();
		m_maxShoupaiSelect = 0;
		m_minShoupaiSelect = 0;
		break;

	case GUOSE:
		disableAllShoupai();
		m_maxShoupaiSelect = 0;
		m_minShoupaiSelect = 0;
		m_selectedSkillCardVec = m_cardContainer->chooseSkillCard(m_selectedSkill);
		if (m_selectedSkillCardVec.empty())
		{
			resetSkillSelect();
			return;
		}
		skillPlayerChooseNormalize();
		break;

	case JIEYIN:
		showTipMessage(trUtf8("选择要弃置的两张卡牌"));
		enableAllShoupai();
		m_maxShoupaiSelect = 2;
		m_minShoupaiSelect = 2;
		break;

	case FANJIAN:
		disableAllShoupai();
		m_maxShoupaiSelect = 0;
		m_minShoupaiSelect = 0;
		break;

	case ZHIHENG:
		disableAllShoupai();
		m_maxShoupaiSelect = 0;
		m_minShoupaiSelect = 0;
		m_selectedSkillCardVec = m_cardContainer->chooseSkillCard(m_selectedSkill);
		if (m_selectedSkillCardVec.empty())
		{
			printDebug("MainWindow::skillChooseCardNormalize: zhiheng canceled");
			resetSkillSelect();
			return;
		}
		unblock();
		break;

	case HUJIA:
		m_maxShoupaiSelect = 0;
		m_minShoupaiSelect = 0;
		unblock();
		break;

	case LIJIAN:
		disableAllShoupai();
		m_maxShoupaiSelect = 0;
		m_minShoupaiSelect = 0;
		m_selectedSkillCardVec = m_cardContainer->chooseSkillCard(m_selectedSkill);
		if (m_selectedSkillCardVec.empty())
		{
			resetSkillSelect();
			return;
		}
		skillPlayerChooseNormalize();
		break;

	case JIJIANG:
		switch (m_responseType)
		{
		case UseCard:
			disableAllShoupai();
			m_maxShoupaiSelect = 0;
			m_minShoupaiSelect = 0;
			break;

		case ShaOrNot:
			m_maxShoupaiSelect = 0;
			m_minShoupaiSelect = 0;
			unblock();
			break;

		default:
			break;
		}
		break;

	case RENDE:
		showTipMessage(trUtf8("选择任意数量的手牌"));
		enableAllShoupai();
		m_maxShoupaiSelect = 0xFFFF;
		m_minShoupaiSelect = 1;
		break;

	case QINGNANG:
		showTipMessage(trUtf8("选择一张手牌"));
		enableAllShoupai();
		m_maxShoupaiSelect = 1;
		m_minShoupaiSelect = 1;
		break;

	case JIJIU:
		m_maxShoupaiSelect = 0;
		m_minShoupaiSelect = 0;
		m_selectedSkillCardVec = m_cardContainer->chooseSkillCard(m_selectedSkill);
		if (m_selectedSkillCardVec.empty())
		{
			resetSkillSelect();
			return;
		}
		unblock();
		break;

	case KUROU:
		unblock();
		break;

	case QIXI:
	{
		disableAllShoupai();
		m_maxShoupaiSelect = 0;
		m_minShoupaiSelect = 0;
		m_selectedSkillCardVec = m_cardContainer->chooseSkillCard(m_selectedSkill);
		if (m_selectedSkillCardVec.empty())
		{
			resetSkillSelect();
			return;
		}
		skillPlayerChooseNormalize();
	}
		break;

	case LONGDAN:
	{
		switch (m_responseType)
		{
		case UseCard:
		case ShaOrNot:
			// enable all shan
			showTipMessage(trUtf8("选择一张闪"));
			for (auto iter = m_shoupaiList.begin(); iter != m_shoupaiList.end(); ++iter)
			{
				bool selectAble = (*iter)->card()->getCardType() == sgs::ConstData::SHAN;
				(*iter)->setSelectable(selectAble);
			}
			m_maxShoupaiSelect = 1;
			m_minShoupaiSelect = 1;
			break;

		case ShanOrNot:
			// enable all sha
			showTipMessage(trUtf8("选择一张杀"));
			for (auto iter = m_shoupaiList.begin(); iter != m_shoupaiList.end(); ++iter)
			{
				bool selectAble = (*iter)->card()->getCardType() == sgs::ConstData::SHA;
				(*iter)->setSelectable(selectAble);
			}
			m_maxShoupaiSelect = 1;
			m_minShoupaiSelect = 1;
			break;
		default:
			break;
		}
	}
		break;

	case WUSHENG:
		disableAllShoupai();
		m_maxShoupaiSelect = 0;
		m_minShoupaiSelect = 0;
		m_selectedSkillCardVec = m_cardContainer->chooseSkillCard(m_selectedSkill);
		if (m_selectedSkillCardVec.empty())
		{
			resetSkillSelect();
			return;
		}
		skillPlayerChooseNormalize();
		break;

	case QINGGUO:
		// enable all red shoupai
		showTipMessage(trUtf8("选择一张黑色手牌"));
		for (auto iter = m_shoupaiList.begin(); iter != m_shoupaiList.end(); ++iter)
		{
			sgs::ConstData::CardColor color = (*iter)->card()->getCardColor();
			bool selectAble = color == sgs::ConstData::SPADE || color == sgs::ConstData::CLUB;
			(*iter)->setSelectable(selectAble);
		}
		m_maxShoupaiSelect = 1;
		m_minShoupaiSelect = 1;
		break;

	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "MainWindow::skillChooseCardNormalize: "
				   "selected skill handled by default, check if the cases covers all possible skills");
		break;
	}
	printDebug("MainWindow::skillCardNormalize: over");
}

// 选中技能之后,决定什么的player可以被点击,同时确定最小和最大的可选目标数目
void MainWindow::skillPlayerChooseNormalize()
{
	printDebug("MainWindow::skillPlayerChooseNormalize: start");
	using namespace sgs::ConstData;
	switch (m_responseType)
	{
	case UseCard:
		if (m_selectedShoupaiList.empty())
		{
			switch (m_selectedSkill)
			{
			case GUOSE:
				showTipMessage(trUtf8("选择对一名玩家使用乐不思蜀"));
				enableSishuTarget();
				m_maxPlayerSelect = 1;
				m_minPlayerSelect = 1;
				break;

			case FANJIAN:
				showTipMessage(trUtf8("选择一个要反间的目标"));
				enableAllPlayers();
				m_maxPlayerSelect = 1;
				m_minPlayerSelect = 1;
				break;

			case LIJIAN:
				showTipMessage(trUtf8("分别选择两个男性进行决斗"));
				enableMalePlayers();
				m_maxPlayerSelect = 2;
				m_minPlayerSelect = 2;
				break;

			case JIJIANG:
				enableShaTarget();
				if (canUseFangtian() && GUIStaticData::getPlayerCount() > 2)
				{
					if (GUIStaticData::getPlayerCount() > 3)
						showTipMessage(GUIStaticData::threeTargets);
					else
						showTipMessage(GUIStaticData::twoTargets);
					m_maxPlayerSelect = 3;
				}
				else
				{
					showTipMessage(trUtf8("选择一个攻击目标"));
					m_maxPlayerSelect = 1;
				}
				m_minPlayerSelect = 1;
				break;

			case WUSHENG:
				enableShaTarget();
				if (canUseFangtian() && GUIStaticData::getPlayerCount() > 2)
				{
					if (GUIStaticData::getPlayerCount() == 3)
						showTipMessage(GUIStaticData::threeTargets);
					else
						showTipMessage(GUIStaticData::twoTargets);
					m_maxPlayerSelect = 3;
				}
				else
				{
					m_maxPlayerSelect = 1;
				}
				m_minPlayerSelect = 1;
				break;

			case QIXI:
				showTipMessage(trUtf8("选择一个奇袭的目标"));

				// only enable players with cards
				enableAllPlayers(true);
				m_maxPlayerSelect = 1;
				m_minPlayerSelect = 1;
				break;

			default:
				disableAllPlayers();
				break;
			}
		}
		else
		{
			switch (m_selectedSkill)
			{
			case heroSkillNone:
				printDebug("<font color=red><b>Warning: </b></font>"
						   "MainWindow::skillPlayerChooseNormalize: no selected skill");
				break;

			case JIEYIN:
				if (m_selectedShoupaiList.size() == 2)
				{
					// 要求选中的手牌有2张
					showTipMessage(trUtf8("选择一名受伤的男性角色"));
					enableJieyinTarget();
				}
				else
				{
					disableAllPlayers();
				}
				m_maxPlayerSelect = 1;
				m_minPlayerSelect = 1;
				break;

			case RENDE:
				showTipMessage(trUtf8("选择一个角色送出手牌"));
				enableAllPlayers();
				m_maxPlayerSelect = 1;
				m_minPlayerSelect = 1;
				break;

			case QINGNANG:
				showTipMessage(trUtf8("选择一个受伤的玩家（包括你自己）"));
				enableQingnangTarget();
				m_maxPlayerSelect = 1;
				m_minPlayerSelect = 1;
				break;

			case LONGDAN:
				enableShaTarget();
				if (canUseFangtian() && GUIStaticData::getPlayerCount() > 2)
				{
					if (GUIStaticData::getPlayerCount() == 3)
						showTipMessage(GUIStaticData::threeTargets);
					else
						showTipMessage(GUIStaticData::twoTargets);
					m_maxPlayerSelect = 3;
				}
				else
				{
					m_maxPlayerSelect = 1;
				}
				m_minPlayerSelect = 1;
				break;

			default:
				printDebug("<font color=red><b>Warning: </b></font>"
						   "MainWindow::skillPlayerChooseNormalize: "
						   "selected skill handled by default, check if the cases covers all possible skills "
						   + skillDisplayName(m_selectedSkill));
				break;
			}
		}
		break; /*case UseCard Over*/

	case ShaOrNot:
		if (m_selectedShoupaiList.empty())
		{
			disableAllPlayers();
		}
		else
		{
			switch (m_selectedSkill)
			{
			case heroSkillNone:
				printDebug("<font color=red><b>Warning: </b></font>"
						   "MainWindow::skillPlayerChooseNormalize: no selected skill");
				break;

			case LONGDAN:
			case WUSHENG:
				disableAllPlayers();
				m_maxPlayerSelect = 0;
				m_minPlayerSelect = 0;
				break;

			default:
				printDebug("<font color=red><b>Warning: </b></font>"
						   "MainWindow::skillPlayerChooseNormalize: "
						   "selected skill handled by default, check if the cases covers all possible skills "
						   + skillDisplayName(m_selectedSkill));
				disableAllPlayers();
				m_maxPlayerSelect = 0;
				m_minPlayerSelect = 0;
				break;
			}
		}
		break; /*case ShaOrNot Over*/

	case ShanOrNot:
		if (m_selectedShoupaiList.empty())
		{
			disableAllPlayers();
		}
		else
		{
			switch (m_selectedSkill)
			{
			case heroSkillNone:
				printDebug("<font color=red><b>Warning: </b></font>"
						   "MainWindow::skillPlayerChooseNormalize: no selected skill");
				break;
			case LONGDAN:
			case QINGGUO:
				disableAllPlayers();
				m_maxPlayerSelect = 0;
				m_minPlayerSelect = 0;
				break;

			default:
				printDebug("<font color=red><b>Warning: </b></font>"
						   "MainWindow::skillPlayerChooseNormalize: "
						   "selected skill handled by default, check if the cases covers all possible skills "
						   + skillDisplayName(m_selectedSkill));
				disableAllPlayers();
				m_maxPlayerSelect = 0;
				m_minPlayerSelect = 0;
				break;
			}
		}
		break; /*case ShanOrNot Over*/

	default:
		break; /*other ResponseType Over*/
	}

	printDebug("MainWindow::skillPlayerChooseNormalize: over");
}

void MainWindow::checkButtons()
{
	// if is Not Responding
	if (m_responseType == NotResponding)
	{
		okButton->setDisabled();
		cancelButton->setDisabled();
		abandonButton->setDisabled();
		return;
	}

	if (m_selectedZhuangbei)
	{
		okButton->setEnabled();
		cancelButton->setEnabled();
		abandonButton->setDisabled();
		return;
	}

	// handle OK Button
	unsigned shoupaiSize = m_selectedShoupaiList.size();
	unsigned selectedAreaSize = m_selectedPlayerAreaList.size();
	bool isShoupaiSizeValid = m_minShoupaiSelect <= shoupaiSize && shoupaiSize <= m_maxShoupaiSelect;
	bool isSelectedAreaSizeValid = m_minPlayerSelect <= selectedAreaSize && selectedAreaSize <= m_maxPlayerSelect;

	if (isShoupaiSizeValid && isSelectedAreaSizeValid)
		okButton->setEnabled();
	else
		okButton->setDisabled();

	// handle Cancel Button
	if (m_selectedSkill == sgs::ConstData::heroSkillNone)
	{
		// if not using skill
		switch (m_responseType)
		{
		case GetLiuli:
		case GetTuxi:
		case TaoOrNot:
		case ShanOrNot:
		case ShaOrNot:
		case WuxieOrNot:
			cancelButton->setEnabled();
			break;

		case UseCard:
		case GetYiji:
		case DiscardCard:
			cancelButton->setDisabled();
			break;

		default:
			cancelButton->setDisabled();
			break;
		}
	}
	else
	{
		cancelButton->setEnabled();
	}

	switch (m_responseType)
	{
	case UseCard:
		abandonButton->setEnabled();
		break;
	default:
		abandonButton->setDisabled();
		break;
	}
}

} /* namespace sgsui */
