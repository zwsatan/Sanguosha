#include "sgsui_MainWindow.h"
#include "sgsui_Shoupai.h"
#include "sgsui_ZhuangbeiArea.h"
#include "sgsui_WujiangArea.h"
#include "sgsui_PlayerArea.h"
#include "sgsui_WugufengdengBox.h"
#include "sgsui_GuanxingBox.h"
#include "sgsui_AudioPlayer.h"
#include "sgsui_CardContainer.h"
#include "sgsui_ZhuangbeiPic.h"
#include "sgsui_CardViewer.h"
#include "sgsui_output.h"
#include "sgsui_FanjianBox.h"
#include "sgsui_nameConvert.h"
#include "sgsui_PlayerSort.h"
#include "sgsai_AIBasic.h"
#include "sgsdata_enum.h"
#include "sgsdata_msg.h"
#include "sgsdata_player.h"

namespace sgsui {

std::vector<int> MainWindow::discardCard(unsigned int cardNumToDiscard)
{
	if (AICheckBox->isChecked())
		return m_innerAI->discardCard(cardNumToDiscard);

	printDebug("MainWindow::discardCard: start");

	m_responseType = DiscardCard;

	showTipMessage(trUtf8("弃") + QString::number(cardNumToDiscard) + trUtf8("张手牌"));
	m_shoupaiNumToDiscard = cardNumToDiscard;

	// Ensure that all cards are available and unselected
	for (auto iter = m_shoupaiList.begin(); iter != m_shoupaiList.end(); ++iter)
	{
		(*iter)->setUnselected();
		(*iter)->setSelectable();
	}
	m_selectedAbandonList.clear();

	okButton->setDisabled();
	cancelButton->setDisabled();
	abandonButton->setDisabled();

	disableAllPlayers();

	m_wujiangArea->disableAllSkillButtons();

	m_zhuangbeiArea->setSelectable(0, false, true);
	m_zhuangbeiArea->setSelectable(1, false, true);
	m_zhuangbeiArea->setSelectable(2, false, true);
	m_zhuangbeiArea->setSelectable(3, false, true);

	exec();

	m_discardIndexList.clear();
	collectDiscardedShoupai();
	clearAbandonData();

	printDebug("MainWindow::discardCard");
	printDebug(discardInterpret(m_discardIndexList, myPlayer()));
	endResponse();
	return m_discardIndexList;
}

sgs::DataType::Message * MainWindow::useCardResponse()
{
	if (AICheckBox->isChecked())
		return m_innerAI->useCardResponse();

	printDebug("MainWindow::useCardResponse: start");
	m_responseType = UseCard;
	cardNormalize();
	playerChooseNormalize();
	skillNormalize();
	checkButtons();

	m_audioPlayer->playSound(YourTurn);
	exec();

	sgs::Derive::CardMessage * useZhuangbei = collectZhuangbei();
	if (useZhuangbei)
	{
		printDebug("MainWindow::useCardResponse: ");
		printDebug(messageInterpret(useZhuangbei));
		endResponse();
		return useZhuangbei;
	}
	sgs::Derive::SkillMessage * useSkill = collectSkill();
	if (useSkill)
	{
		printDebug("MainWindow::useCardResponse: ");
		printDebug(messageInterpret(useSkill));
		endResponse();
		return useSkill;
	}

	sgs::Derive::CardMessage * returnMessage = collectZhuangbei();

	if (m_isGoingToAbandon)
	{
		returnMessage = 0;
	}
	else
	{
		returnMessage = new sgs::Derive::CardMessage(false,
													 GUIStaticData::getPlayer(m_playerIndex),
													 sgs::ConstData::PHAND,
													 getIndex(m_selectedShoupaiList.back()));

		// add targets
		if (m_selectedShoupaiList.back()->card()->getCardType() == sgs::ConstData::JIEDAO)
		{
			returnMessage->addto(m_selectedPlayerAreaList.back()->getPlayer());
			returnMessage->addto(m_selectedPlayerAreaList.front()->getPlayer());
		}
		else
		{
			std::vector<sgs::DataType::Player *> targetPlayerVec;
			for (auto iter = m_selectedPlayerAreaList.begin(); iter != m_selectedPlayerAreaList.end(); ++iter)
				targetPlayerVec.push_back((*iter)->getPlayer());

			// sort the selected players from small to big;
			std::sort(targetPlayerVec.begin(), targetPlayerVec.end(), PlayerSort(m_playerIndex - 1, BySeat));
			for (auto iter = targetPlayerVec.begin(); iter != targetPlayerVec.end(); ++iter)
				returnMessage->addto(*iter);
		}

		// add target for tao
		if (m_selectedShoupaiList.back()->card()->getCardType() == sgs::ConstData::TAO)
			returnMessage->addto(GUIStaticData::getPlayer(m_playerIndex));

		// add target for shandian
		if (m_selectedShoupaiList.back()->card()->getCardType() == sgs::ConstData::SHANDIAN)
			returnMessage->addto(GUIStaticData::getPlayer(m_playerIndex));
	}
	printDebug("MainWindow::useCardResponse: ");
	printDebug(messageInterpret(returnMessage));
	endResponse();
	return returnMessage;
}

sgs::DataType::Message * MainWindow::taoOrNot(sgs::DataType::Player * from, int neededTaoNumber)
{
	if (AICheckBox->isChecked())
		return m_innerAI->taoOrNot(from, neededTaoNumber);

	printDebug("MainWindow::taoOrNot: start");

	m_responseType = TaoOrNot;
	cardNormalize();
	playerChooseNormalize();
	skillNormalize();
	checkButtons();

	if (from->seat() + 1 != m_playerIndex)
	{
		QString str = wujiangDisplayName(from->type()) + trUtf8("濒死，共需要") + QString::number(neededTaoNumber) + trUtf8("个桃子，要不要救助？");
		if (getDialogResponse(str)  == 0)
		{
			printDebug("MainWindow::taoOrNot: response canceled by user");
			endResponse();
			return 0;
		}
	}
	else
	{
		showTipMessage(trUtf8("你需要") + QString::number(neededTaoNumber) + trUtf8("个桃子来救助自己"));
	}

	exec();
	sgs::Derive::SkillMessage * useSkill = collectSkill();
	if (useSkill)
	{
		printDebug("MainWindow::taoOrNot: (use skill)");
		printDebug(messageInterpret(useSkill));
		endResponse();
		return useSkill;
	}

	sgs::Derive::CardMessage * returnMessage = 0;
	if (m_cancelResponse)
	{
		printDebug("MainWindow::taoOrNot: cancelled");
		returnMessage = 0;
	}
	else
	{
		returnMessage = new sgs::Derive::CardMessage(false,
													 GUIStaticData::getPlayer(m_playerIndex),
													 sgs::ConstData::PHAND,
													 getIndex(m_selectedShoupaiList.back()), from);
	}
	printDebug("MainWindow::taoOrNot: ");
	printDebug(messageInterpret(returnMessage));
	endResponse();
	return returnMessage;
}

sgs::DataType::Message * MainWindow::shanOrNot(sgs::DataType::Player * from, const sgs::DataType::Card * card)
{
	if (AICheckBox->isChecked())
		return m_innerAI->shanOrNot(from, card);

	printDebug("MainWindow::shanOrNot: start");

	m_responseType = ShanOrNot;
	cardNormalize();
	playerChooseNormalize();
	skillNormalize();
	checkButtons();

	if (from && card)
		showTipMessage(wujiangDisplayName(from->type()) + trUtf8("对你使用了") + cardFullDisplayName(card, false) + trUtf8("，请用闪来响应"));
	else
		showTipMessage(trUtf8("，请用闪来响应"));

	exec();
	sgs::Derive::CardMessage * useZhuangbei = collectZhuangbei();
	if (useZhuangbei)
	{
		printDebug("MainWindow::shanOrNot: (use equip)");
		printDebug(messageInterpret(useZhuangbei));
		endResponse();
		return useZhuangbei;
	}

	sgs::Derive::SkillMessage * useSkill = collectSkill();
	if (useSkill)
	{
		printDebug("MainWindow::shanOrNot: (use skill)");
		printDebug(messageInterpret(useSkill));
		endResponse();
		return useSkill;
	}

	sgs::Derive::CardMessage * returnMessage = 0;
	if (m_cancelResponse)
	{
		returnMessage = 0;
	}
	else
	{
		returnMessage = new sgs::Derive::CardMessage(false,
													 GUIStaticData::getPlayer(m_playerIndex),
													 sgs::ConstData::PHAND,
													 getIndex(m_selectedShoupaiList.back()));
	}
	printDebug("MainWindow::shanOrNot: ");
	printDebug(messageInterpret(returnMessage));
	endResponse();
	return returnMessage;
}

sgs::DataType::Message * MainWindow::shanOrNot(sgs::DataType::Player * from, sgs::ConstData::HeroSkill skillType)
{
	if (AICheckBox->isChecked())
		return m_innerAI->shanOrNot(from, skillType);

	printDebug("MainWindow::shanOrNot (skill version): start");

	m_responseType = ShanOrNot;
	cardNormalize();
	playerChooseNormalize();
	skillNormalize();
	checkButtons();

	showTipMessage(wujiangDisplayName(from->type()) + trUtf8("使用了技能") + skillDisplayName(skillType) + trUtf8("，是否出闪？"));

	exec();
	sgs::Derive::SkillMessage * useSkill = collectSkill();
	if (useSkill)
	{
		printDebug("MainWindow::shanOrNot (skill version): (use skill)");
		printDebug(messageInterpret(useSkill));
		endResponse();
		return useSkill;
	}

	sgs::Derive::CardMessage * returnMessage = 0;
	if (m_cancelResponse)
	{
		returnMessage = 0;
	}
	else
	{
		// Do not add target here
		returnMessage = new sgs::Derive::CardMessage(false, GUIStaticData::getPlayer(m_playerIndex), sgs::ConstData::PHAND, getIndex(m_selectedShoupaiList.back()));
	}
	printDebug("MainWindow::shanOrNot (skill version): ");
	printDebug(messageInterpret(returnMessage));
	endResponse();
	return returnMessage;
}

sgs::DataType::Message * MainWindow::shaOrNot(sgs::DataType::Player * from,
											  const sgs::DataType::Card * card) {
	if (AICheckBox->isChecked()) {
		return m_innerAI->shaOrNot(from, card);
	}
	printDebug("MainWindow::shaOrNot: start");

	m_responseType = ShaOrNot;
	m_skillVerShaOrNot = true;
	cardNormalize();
	playerChooseNormalize();
	skillNormalize();
	checkButtons();

	if (from && card && from != myPlayer()) {
		showTipMessage(
					wujiangDisplayName(from->type())
					+ trUtf8("对你使用了")
					+ cardFullDisplayName(card, false)
					+ trUtf8("，请使用一张杀"));
	} else {
		showTipMessage(
					trUtf8("你现在可以使用一张杀"));
	}
	exec();
	sgs::Derive::CardMessage * useZhuangbei = collectZhuangbei();
	if (useZhuangbei) {
		printDebug("MainWindow::shaOrNot: (use equip)");
		printDebug(messageInterpret(useZhuangbei));
		endResponse();
		return useZhuangbei;
	}
	sgs::Derive::SkillMessage * useSkill = collectSkill();
	if (useSkill) {
		printDebug("MainWindow::shaOrNot: (use skill)");
		printDebug(messageInterpret(useSkill));
		endResponse();
		return useSkill;
	}

	sgs::Derive::CardMessage * returnMessage = 0;
	if (m_cancelResponse) {
		returnMessage = 0;
	} else {

		// Do not add target here
		returnMessage = new sgs::Derive::CardMessage(false,
											   GUIStaticData::getPlayer(m_playerIndex), sgs::ConstData::PHAND,
											   getIndex(m_selectedShoupaiList.back()));
	}
	printDebug("MainWindow::shaOrNot: ");
	printDebug(messageInterpret(returnMessage));
	endResponse();
	return returnMessage;
}

sgs::DataType::Message * MainWindow::shaOrNot(sgs::DataType::Player * from,
											  sgs::ConstData::HeroSkill skillType) {
	if (AICheckBox->isChecked()) {
		return m_innerAI->shaOrNot(from, skillType);
	}
	printDebug("MainWindow::shaOrNot (skill version): start");

	m_responseType = ShaOrNot;
	m_skillVerShaOrNot = true;
	cardNormalize();
	playerChooseNormalize();
	skillNormalize();
	checkButtons();

	showTipMessage(
				wujiangDisplayName(from->type())
				+ trUtf8("使用了技能")
				+ skillDisplayName(skillType)
				+ trUtf8("，是否出杀？"));
	exec();
	sgs::Derive::SkillMessage * useSkill = collectSkill();
	if (useSkill) {
		printDebug("MainWindow::shaOrNot (skill version): (use skill)");
		printDebug(messageInterpret(useSkill));
		endResponse();
		return useSkill;
	}

	sgs::Derive::CardMessage * returnMessage = 0;
	if (m_cancelResponse) {
		returnMessage = 0;
	} else {

		// Do not add target here
		returnMessage = new sgs::Derive::CardMessage(false,
											   GUIStaticData::getPlayer(m_playerIndex), sgs::ConstData::PHAND,
											   getIndex(m_selectedShoupaiList.back()));
	}
	printDebug("MainWindow::shaOrNot (skill version): ");
	printDebug(messageInterpret(returnMessage));
	endResponse();
	return returnMessage;
}

sgs::DataType::Message * MainWindow::wuxieOrNot(
		sgs::DataType::Message * originMsg, int count) {
	if (AICheckBox->isChecked()) {
		return m_innerAI->wuxieOrNot(originMsg, count);
	}
	printDebug("MainWindow::wuxieOrNot: start");

	if (!Interface::haveWuxie()) {
		printDebug("MainWindow::wuxieOrNot: don't have wuxie, return 0");
		printDebug("MainWindow::wuxieOrNot: over");
		return 0;
	}

	m_responseType = WuxieOrNot;
	cardNormalize();
	playerChooseNormalize();
	skillNormalize();
	checkButtons();

	sgs::Derive::CardMessage * messageToWixie =
			static_cast<sgs::Derive::CardMessage *>(originMsg);

	/*	if (messageToWixie->from() == myPlayer() && count == 0) {
	 printDebug(
	 "MainWindow::wuxieOrNot: from() == myPlayer() and count = 0, skip response, return 0");
	 printDebug("MainWindow::wuxieOrNot: over");
	 return 0;
	 }*/

	sgs::ConstData::CardType cardType = messageToWixie->card()->type();
	QString question = (wujiangDisplayName(messageToWixie->from()->type()));
	if (isJudgeCard(cardType)) {
		question.append(trUtf8("的判定牌"));
		question.append(cardDisplayName(messageToWixie->card()->type()));
		question.append(trUtf8("将要判定"));
	} else {
		question.append(trUtf8("使用了"));
		question.append(cardDisplayName(messageToWixie->card()->type()));
	}
	if (count > 0)
		question.append(trUtf8("，已经被无懈可击") + QString::number(count) + trUtf8("次"));

	question.append(trUtf8("，你可以使用无懈可击"));

	showTipMessage(question);

	exec();
	sgs::Derive::SkillMessage * useSkill = collectSkill();
	if (useSkill) {
		printDebug("MainWindow::wuxieOrNot: (use skill) ");
		printDebug(messageInterpret(useSkill));
		endResponse();
		return useSkill;
	}

	sgs::Derive::CardMessage * returnMessage = 0;
	if (m_cancelResponse) {
		returnMessage = 0;
	} else {

		// Do not add target here
		returnMessage = new sgs::Derive::CardMessage(false,
											   GUIStaticData::getPlayer(m_playerIndex), sgs::ConstData::PHAND,
											   getIndex(m_selectedShoupaiList.back()));
	}
	printDebug("MainWindow::wuxieOrNot: ");
	printDebug(messageInterpret(returnMessage));
	endResponse();
	return returnMessage;
}

bool MainWindow::useEquipOrNot(sgs::ConstData::CardType equipCardType,
							   sgs::DataType::Player *targetPlayer) {
	if (AICheckBox->isChecked()) {
		return m_innerAI->useEquipOrNot(equipCardType, targetPlayer);
	}
	printDebug("MainWindow::useEquipOrNot: start");
	QString question;
	if (targetPlayer) {
		question.append(trUtf8("是否要对") + wujiangDisplayName(targetPlayer->type()) + trUtf8("发动"));
	} else {
		question.append(trUtf8("是否要发动"));
	}
	question.append(cardDisplayName(equipCardType));
	question.append(trUtf8("的效果？"));
	bool result = getDialogResponse(question);
	printDebug(
				QString("MainWindow::useEquipOrNot: over, return ")
				+ QString(result ? "true" : "false"));
	return result;
}

bool MainWindow::useSkillOrNot(sgs::ConstData::HeroSkill skillType,
							   sgs::DataType::Player * targetPlayer) {
	if (AICheckBox->isChecked()) {
		return m_innerAI->useSkillOrNot(skillType, targetPlayer);
	}
	printDebug("MainWindow::useSkillOrNot: start");
	QString question(trUtf8("是否要"));
	if (targetPlayer) {
		question.append(trUtf8("对") + wujiangDisplayName(targetPlayer->type()));
	}
	question.append(trUtf8("发动技能"));
	question.append(skillDisplayName(skillType));
	question.append(trUtf8("？"));
	bool result = getDialogResponse(question);
	printDebug(
				QString("MainWindow::useSkillOrNot: over, return ")
				+ QString(result ? "true" : "false"));
	return result;
}

bool MainWindow::useSkillOrNot(sgs::ConstData::HeroSkill skillType,
							   sgs::DataType::Message * message) {
	if (AICheckBox->isChecked()) {
		return m_innerAI->useSkillOrNot(skillType, message);
	}
	printDebug("MainWindow::useSkillOrNot: start");

	sgs::Derive::JudgeMessage * judgeMessage =
			dynamic_cast<sgs::Derive::JudgeMessage *>(message);
	if (!judgeMessage) {
		printDebug(
					"<font color=red><b>Warning: </b></font>MainWindow::useSkillOrNot (message vesion): message type is not MJudge, message: "
					+ allMessageInterpret(message));
		pause();
	}

	QString question(wujiangDisplayName(judgeMessage->from()->type()));
	if (judgeMessage->cardJudge()) {
		question.append(trUtf8("的判定牌"));
		question.append(cardDisplayName(judgeMessage->card()->type()));
	} else {
		question.append(skillDisplayName(judgeMessage->skill()));
	}
	question.append(
				judgeMessage->effect() ?
					GUIStaticData::judgePositive :
					GUIStaticData::judgeNegative);
	question.append(trUtf8("是否改判？"));
	bool result = getDialogResponse(question);
	printDebug(
				QString("MainWindow::useSkillOrNot (message vesion): over, return ")
				+ QString(result ? "true" : "false"));
	return result;
}

std::vector<std::pair<sgs::ConstData::PlayerRegionType, int> > MainWindow::chooseCardResponse(
		sgs::DataType::Player *targetPlayer, bool isHandSelectable,
		bool isEquipSelectable, bool isJudgeSelectable, int num,
		sgs::ConstData::CardType reason) {
	if (AICheckBox->isChecked()) {
		return m_innerAI->chooseCardResponse(targetPlayer, isHandSelectable,
										   isEquipSelectable, isJudgeSelectable, num, reason);
	}

	QString text(trUtf8("选择"));
	text.append(QString::number(num));
	text.append(trUtf8("张卡牌"));
	return m_cardContainer->chooseCard(text, targetPlayer, isHandSelectable,
									 isEquipSelectable, isJudgeSelectable, num, reason);
}

std::vector<std::pair<sgs::ConstData::PlayerRegionType, int> > MainWindow::chooseCardResponse(
		sgs::DataType::Player *targetPlayer, bool isHandSelectable,
		bool isEquipSelectable, bool isJudgeSelectable, int num,
		sgs::ConstData::HeroSkill reason) {
	if (AICheckBox->isChecked()) {
		return m_innerAI->chooseCardResponse(targetPlayer, isHandSelectable,
										   isEquipSelectable, isJudgeSelectable, num, reason);
	}

	QString text(trUtf8("选择"));
	text.append(QString::number(num));
	text.append(trUtf8("张卡牌"));
	return m_cardContainer->chooseCard(text, targetPlayer, isHandSelectable,
									 isEquipSelectable, isJudgeSelectable, num, reason);
}

bool MainWindow::yesOrNot(sgs::ConstData::CardType reason)
{
	if (AICheckBox->isChecked())
		return m_innerAI->yesOrNot(reason);

	printDebug("MainWindow::yesOrNot(card version): start");
	QString question;
	if (reason == sgs::ConstData::CIXIONG)
	{
		// 处理雌雄剑的情况
		question.append(trUtf8("是否让对方摸一张牌（否则自己弃一张牌）？"));
	}

	bool result = getDialogResponse(question);
	printDebug(QString("MainWindow::yesOrNot(card version): over, return ") + QString(result ? "true" : "false"));
	return result;
}

bool MainWindow::yesOrNot(sgs::ConstData::HeroSkill reason)
{
	if (AICheckBox->isChecked())
		return m_innerAI->yesOrNot(reason);

	printDebug("MainWindow::yesOrNot(skill version): start");
	QString question;
	if (reason == sgs::ConstData::GANGLIE)
	{
		// 处理刚烈的情况
		question.append(trUtf8("是否弃2张手牌（否则受到1点伤害）"));
	}
	else if (reason == sgs::ConstData::HUJIA)
	{
		// 处理护驾的情况
		question.append(trUtf8("是否为曹操提供一张闪"));
	}

	bool result = getDialogResponse(question);
	printDebug(QString("MainWindow::yesOrNot(skill version): over, return ") + QString(result ? "true" : "false"));
	return result;
}

int MainWindow::chooseWugu(const std::vector<std::pair<const sgs::DataType::Card *, bool> >& wuguCards)
{
	if (AICheckBox->isChecked())
		return m_innerAI->chooseWugu(wuguCards);

	showWugu(wuguCards);
	return m_wugufengdengBox->chooseWugu(wuguCards);
}

void MainWindow::chooseGuanxing(std::vector<const sgs::DataType::Card *>& top, std::vector<const sgs::DataType::Card *>& buttom)
{
	if (AICheckBox->isChecked())
	{
		m_innerAI->chooseGuanxing(top, buttom);
		return;
	}

	m_guanxingBox->chooseGuanxing(top, buttom);
}

sgs::DataType::Message * MainWindow::getTuxi()
{
	if (AICheckBox->isChecked())
		return m_innerAI->getTuxi();

	m_responseType = GetTuxi;
	printDebug("MainWindow::getTuxi: start");

	if (getDialogResponse(trUtf8("是否要使用技能突袭？")) == 0)
	{
		printDebug("MainWindow::getTuxi: response canceled by user");
		endResponse();
		return 0;
	}

	showTipMessage(trUtf8("选择一至两名突袭目标"));

	cardNormalize();
	playerChooseNormalize();
	checkButtons();
	exec();

	sgs::Derive::SkillMessage * returnMessage = 0;
	if (m_cancelResponse)
	{
		printDebug("MainWindow::getTuxi: cancelled");
		returnMessage = 0;
	}
	else
	{
		if (m_tuxiList.size() == 1)
		{
			returnMessage = new sgs::Derive::TuXiMessage(myPlayer(),
												   m_tuxiList.front().second->getPlayer(),
												   m_tuxiList.front().first);
		}
		else
		{
			returnMessage = new sgs::Derive::TuXiMessage(myPlayer(),
												   m_tuxiList.front().second->getPlayer(),
												   m_tuxiList.front().first, m_tuxiList.back().second->getPlayer(),
												   m_tuxiList.back().first);
		}
	}
	printDebug("MainWindow::getTuxi: over");
	printDebug(messageInterpret(returnMessage));
	endResponse();
	return returnMessage;
}

sgs::DataType::Message * MainWindow::getYiji(const sgs::DataType::Card * card1,
											 const sgs::DataType::Card * card2) {
	if (AICheckBox->isChecked()) {
		return m_innerAI->getYiji(card1, card2);
	}
	printDebug("MainWindow::getYiji: start");
	m_responseType = GetYiji;
	sgs::DataType::Player * player1, *player2;

	m_cardViewer->setGeometry(
				QRect(
					mainFrame->geometry().center()
					- QPoint(m_cardViewer->width() / 2,
							 m_cardViewer->height() / 5),
					m_cardViewer->size()));
	m_cardViewer->showViewer(card1, card2);

	cardNormalize();
	playerChooseNormalize();
	checkButtons();
	exec();
	player1 = m_selectedPlayerAreaList.back()->getPlayer();

	m_cardViewer->setGeometry(
				QRect(
					mainFrame->geometry().center()
					- QPoint(m_cardViewer->width() / 2,
							 m_cardViewer->height() / 5),
					m_cardViewer->size()));
	m_cardViewer->showSecondViewer();

	cardNormalize();
	playerChooseNormalize();
	checkButtons();
	exec();
	m_cardViewer->hide();
	player2 = m_selectedPlayerAreaList.back()->getPlayer();
	sgs::Derive::SkillMessage * returnMessage = new sgs::Derive::YiJiMessage(myPlayer(),
																 player1, card1, player2, card2);
	printDebug("MainWindow::getYiji: over");
	printDebug(messageInterpret(returnMessage));
	endResponse();
	return returnMessage;
}

sgs::DataType::Message * MainWindow::getLiuli(
		sgs::DataType::Player * attacker) {
	if (AICheckBox->isChecked()) {
		return m_innerAI->getLiuli(attacker);
	}
	m_responseType = GetLiuli;
	m_liuliAttacker = attacker;
	printDebug("MainWindow::getLiuli: start");
	if (getDialogResponse(
			wujiangDisplayName(attacker->type()) + trUtf8("对你使用了杀，是否要使用技能流离？"))
		== 0) {
		printDebug("MainWindow::getLiuli: response canceled by user");
		endResponse();
		return 0;
	}
	cardNormalize();
	playerChooseNormalize();
	checkButtons();
	exec();

	sgs::Derive::SkillMessage * returnMessage = 0;
	if (m_cancelResponse) {
		printDebug("MainWindow::getLiuli: cancelled");
		returnMessage = 0;
	} else {
		CardPosPair liuliPair = m_selectedSkillCardVec.back();
		returnMessage = new sgs::Derive::LiuLiMessage(myPlayer(), liuliPair.first, liuliPair.second, m_selectedPlayerAreaList.back()->getPlayer());
	}
	printDebug("MainWindow::getLiuli: over");
	printDebug(messageInterpret(returnMessage));
	endResponse();
	return returnMessage;
}

std::pair<sgs::ConstData::CardColor, int> MainWindow::respondFanjian(
		sgs::DataType::Player * zhouyu) {
	sgs::ConstData::CardColor choosenColor = runFanjian();
	QString text(trUtf8("选择周瑜的一张手牌"));
	std::pair<sgs::ConstData::CardColor, int> choosen;
	choosen.first = choosenColor;
	choosen.second = m_cardContainer->chooseCard(text, zhouyu, true, false, false,
											   1, sgs::ConstData::heroSkillNone).back().second;
	return choosen;
}

void MainWindow::collectDiscardedShoupai()
{
	for (auto iter = m_selectedAbandonList.begin(); iter != m_selectedAbandonList.end(); ++iter)
		m_discardIndexList.push_back(getIndex(*iter));
}

void MainWindow::clearAbandonData()
{
	m_selectedAbandonList.clear();
	m_shoupaiNumToDiscard = 0;
	// DO NOT clear discardIndexList herer because it is return value!!
}

} /* namespace sgsui */
