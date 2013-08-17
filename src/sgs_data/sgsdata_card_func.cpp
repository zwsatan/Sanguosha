#include "sgsdata_card_func.h"
#include "sgsdata_skill_func.h"
#include "sgsdata_msg.h"
#include "sgsdata_card.h"
#include "sgsdata_extern.h"
#include "sgsdata_history.h"
#include "sgsdata_player.h"
#include "sgs_Interface.h"
#include "sgsdata_platform.h"
#include "sgsui_output.h"
#include "sgsui_nameConvert.h"
#include <stack>
using namespace sgs;
using namespace ConstData;
using namespace DataType;
using namespace Derive;

Message * func::pcsha(Message * msg)
{
	printDebug("func::pcsha: start");
	ExternData::sgsout << msg;
	ExternData::history.push(msg);
	std::stack<Message *> groove; //杀处理函数的内部消息沟道。
	CardMessage * message = static_cast<CardMessage *>(msg);

	bool checkLvbu = message->from()->type() == LVBU;
	bool checkCaocao = false; //是否已经被曹操把牌拿走
	bool checkMachao = false; //是否已经检查过马超的效果
	bool checkXuchu = (message->from()->type() == XUCHU) && (message->from()->status() & SkillOneUsed); //是否满足许褚裸衣的条件

	static bool qingLong = false; //是否因为青龙的追杀效果
	if (!qingLong)
	{
		message->from()->setstatus() |= ATTACKED;
		message->from()->addShaCount();
	}

	if (message->targets() > 1)
	{
		CardMessage * useEquip = new CardMessage(true, message->from(), PEQUIP, 0);
		for (int i = 0; i < message->targets(); i++)
			useEquip->addto(message->to(i));

		ExternData::sgsout << useEquip;
		ExternData::history.push(useEquip);
	}
	while (message->targets())
	{
		CardMessage * temp = new CardMessage(message);
		temp->setdissolve(true);
		message->popto();
		groove.push(temp);
	}

	while (!groove.empty())
	{
		Message * grooveTop = groove.top();
		groove.pop();
		if (grooveTop->dissolved() == true)
		{
			CardMessage * temp = static_cast<CardMessage *>(grooveTop);

			// LIULI
			if (temp->to()->type() == DAQIAO)
			{
				LiuLiMessage * liuli = static_cast<LiuLiMessage *>(temp->to()->input()->getLiuli(temp->from()));
				if (liuli != 0)
				{
					ExternData::sgsout << liuli;
					ExternData::history.push(liuli);

					TransCardMessage * trans = new TransCardMessage(PLAYER, DUST, true, liuli->from(), liuli->pos().first);
					trans->transpush(liuli->pos().second);
					liuli->from()->popCard(liuli->pos());
					ExternData::platform.analyze(trans);

					temp->popto();
					temp->addto(liuli->to());
				}
			}

			func::pccixiong(temp);
			if (!temp->from()->weapon() || temp->from()->weapon()->type() != QINGGANG)
			{
				printDebug("func::pcsha: in condition, going to process hangbing and renwang");
				if (func::pcrenwang(temp))
				{
					printDebug("func::pcsha: over, renwang used");
					delete grooveTop;
					continue;
				}
			}

			//检查马超的铁骑
			if (temp->from()->type() == MACHAO)
			{
				checkMachao = true;
				bool useTieQi = temp->from()->input()->useSkillOrNot(TIEQI, temp->to());
				if (useTieQi)
				{
					SkillMessage * tieqi = new SkillMessage(temp->from(), TIEQI);
					ExternData::sgsout << tieqi;
					ExternData::history.push(tieqi);
					const Card * judge = ExternData::platform.getJudge();
					JudgeMessage * judgeResult = 0;
					if (judge->color() == HEART || judge->color() == DIAMOND)
						judgeResult = new JudgeMessage(temp->from(), TIEQI, judge, true);
					else
						judgeResult = new JudgeMessage(temp->from(), TIEQI, judge, false);

					judgeResult = static_cast<JudgeMessage *>(func::pcguicai( judgeResult));
					ExternData::sgsout << judgeResult;
					ExternData::history.push(judgeResult);
					ExternData::platform.abandon(judgeResult->result());
					if (judgeResult->effect())
					{
						temp->to()->sethp()--;
						Message * hurt = new HurtMessage(temp->from(), temp->to(), 1);
						groove.push(hurt);
					}
					delete grooveTop;
					continue;
				}
			}
			//检查马超的铁骑完毕

			bool avoided = true;
			for (int i = 0; i < 1 + checkLvbu; i++)
			{
				if (i == 1)
				{
					SkillMessage* wushuang = new SkillMessage(temp->from(), WUSHUANG);
					ExternData::sgsout << wushuang;
					ExternData::history.push(wushuang);
				}

				if ((!temp->from()->weapon() || temp->from()->weapon()->type() != QINGGANG) && func::pcbagua(temp))
				{
					continue;
				}
				else
				{
					Message * shanornot = temp->to()->ShanOrNot(temp);
					if (shanornot == 0) {
						avoided = false;
						if (!func::pchanbing(temp))
						{
							qingLong = false;
							temp->to()->sethp() -= 1 + checkXuchu;
							Message * hurt = new HurtMessage(temp->from(), temp->to(), 1 + checkXuchu);
							groove.push(hurt);
						}
						break;
					}
					else
					{
						while (shanornot != 0)
							shanornot = ExternData::platform.analyze(shanornot);
					}
				}
			}
			if (avoided)
			{
				if (func::pcqinglong(temp))
				{
					Message * shaornot = temp->from()->ShaOrNot(temp);
					if (shaornot != 0)
					{
						qingLong = true;
						delete grooveTop;
						return shaornot;
					}
				}
				else if (func::pcguanshi(temp))
				{
					temp->to()->sethp() -= 1 + checkXuchu;
					Message * hurt = new HurtMessage(temp->from(), temp->to(), 1 + checkXuchu);
					groove.push(hurt);
				}
			}
			delete grooveTop;
			continue;
		}
		else
		{
			if (grooveTop->type() == HURT && static_cast<HurtMessage *>(grooveTop)->to()->type() == CAOCAO)
			{
				Message * res = ExternData::platform.analyze(grooveTop);
				if (res == 0 && static_cast<HurtMessage *>(grooveTop)->to()->input()->useSkillOrNot(JIANXIONG))
				{
					SkillMessage * jianxiong = new SkillMessage(static_cast<HurtMessage *>(grooveTop)->to(), JIANXIONG);
					ExternData::sgsout << jianxiong;
					ExternData::history.push(jianxiong);

					checkCaocao = true;
					TransCardMessage * trans = new TransCardMessage(DESK, PLAYER, true, 0, playerRegionTypeNone, static_cast<HurtMessage *>(grooveTop)->to());
					trans->transpush(message->card());
					message->card()->setmask();
					groove.push(trans);
				}
				else
				{
					groove.push(res);
				}
			}
			else
			{
				Message * res = ExternData::platform.analyze(grooveTop);
				if (res != 0)
				{
					if (res->type() == FINALE)
						return res;
					else
						groove.push(res);
				}
			}
		}
	}
	message->card()->setmask();
	if (!checkCaocao)
		ExternData::platform.abandon(message->card());
	printDebug("func::pcsha: over");
	return 0;
}

Message * func::pcsha(ZhangBaMessage * msg)
{
	printDebug("func::pcsha(zhangba version): start");

	//发出消息
	ExternData::sgsout << msg;
	ExternData::history.push(msg);

	bool checkLvbu = msg->from()->type() == LVBU;
	bool checkCaocao = false;
	bool checkXuchu = (msg->from()->type() == XUCHU) && (msg->from()->status() & SkillOneUsed); //是否满足许褚裸衣的条件

	printDebug("func::pcsha(zhangba version): message 2 sent");

	//结算杀的效果
	msg->from()->setstatus() |= ATTACKED;
	msg->from()->addShaCount(); // added by Hu Ronghang
	printDebug("func::pcsha(zhangba version): check point 1");

	//debug
	{
		printDebug("func::pcsha(zhangba version): Cost 0 is " + sgsui::cardFullDisplayName(msg->cost(0).first, false));

		printDebug("func::pcsha(zhangba version): Cost 1 is " + sgsui::cardFullDisplayName(msg->cost(1).first, false));
	}

	CardColor cardColor = cardColorNone;
	if (msg->cost(0).first->color() == msg->cost(1).first->color())
		cardColor = msg->cost(0).first->color();

	printDebug("func::pcsha(zhangba version): check point 2");
	const Card * fakeCard = new Card(cardColor, 0, SHA);
	printDebug("func::pcsha(zhangba version): fake card created");

	CardMessage * temp = new CardMessage(false, msg->from(), fakeCard, msg->to());

	// LIULI
	if (temp->to()->type() == DAQIAO)
	{
		LiuLiMessage * liuli = static_cast<LiuLiMessage *>(temp->to()->input()->getLiuli(temp->from()));
		if (liuli != 0)
		{
			ExternData::sgsout << liuli;
			ExternData::history.push(liuli);

			TransCardMessage * trans = new TransCardMessage(PLAYER, DUST, true, liuli->from(), liuli->pos().first);
			trans->transpush(liuli->pos().second);
			liuli->from()->popCard(liuli->pos());
			ExternData::platform.analyze(trans);

			temp->popto();
			temp->addto(liuli->to());
		}
	}

	if (!func::pcrenwang(temp))
	{
		printDebug("func::pcsha(zhangba version): in condition");

		if (temp->from()->type() == MACHAO)
		{
			bool useTieQi = temp->from()->input()->useSkillOrNot(TIEQI, temp->to());
			if (useTieQi)
			{
				SkillMessage * tieqi = new SkillMessage(temp->from(), TIEQI);
				ExternData::sgsout << tieqi;
				ExternData::history.push(tieqi);
				const Card * judge = ExternData::platform.getJudge();
				JudgeMessage * judgeResult = 0;
				if (judge->color() == HEART || judge->color() == DIAMOND)
					judgeResult = new JudgeMessage(temp->from(), TIEQI, judge, true);
				else
					judgeResult = new JudgeMessage(temp->from(), TIEQI, judge, false);
				judgeResult = static_cast<JudgeMessage *>(func::pcguicai(judgeResult));

				ExternData::sgsout << judgeResult;
				ExternData::history.push(judgeResult);
				if (judgeResult->from()->type() == GUOJIA && judgeResult->from()->input()->useSkillOrNot(TIANDU))
				{
					SkillMessage * tiandu = new SkillMessage(judgeResult->from(), TIANDU);
					ExternData::sgsout << tiandu;
					ExternData::history.push(tiandu);

					TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER, true, 0, playerRegionTypeNone, judgeResult->from());
					trans->transpush(judgeResult->result());
					ExternData::platform.analyze(trans);
				}
				else
				{
					ExternData::platform.abandon(judgeResult->result());
				}

				if (judgeResult->effect())
				{
					temp->to()->sethp()--;
					HurtMessage * hurt = new HurtMessage(temp->from(), temp->to(), 1);
					Message * res = hurt;
					if (hurt->to()->type() == CAOCAO && (res = ExternData::platform.analyze(hurt)) == 0 && temp->to()->input()->useSkillOrNot(JIANXIONG))
					{
						SkillMessage * jianxiong = new SkillMessage(temp->to(), JIANXIONG);
						ExternData::sgsout << jianxiong;
						ExternData::history.push(jianxiong);

						checkCaocao = true;
						TransCardMessage * trans = new TransCardMessage(DESK, PLAYER, true, 0, playerRegionTypeNone, temp->to());
						trans->transpush(msg->cost(0).first);
						trans->transpush(msg->cost(1).first);
						delete fakeCard;
						return trans;
					}
					else
					{
						ExternData::platform.abandon(msg->cost(0).first);
						ExternData::platform.abandon(msg->cost(1).first);
						delete fakeCard;
						return res;
					}
				}
			}
		}

		for (int i = 0; i < 1 + checkLvbu; i++) {
			if (i == 1) {
				SkillMessage * wushuang = new SkillMessage(msg->from(), WUSHUANG);
				ExternData::sgsout << wushuang;
				ExternData::history.push(wushuang);
			}
			if (!func::pcbagua(temp)) {
				Message * shanornot = temp->to()->ShanOrNot(temp);
				if (shanornot == 0) {
					temp->to()->sethp() -= 1 + checkXuchu;
					Message * res = new HurtMessage(temp->from(), temp->to(),
							1 + checkXuchu);
					if (static_cast<HurtMessage *>(res)->to()->type() == CAOCAO
							&& (res = ExternData::platform.analyze(res)) == 0
							&& temp->to()->input()->useSkillOrNot(JIANXIONG)) {
						SkillMessage * jianxiong = new SkillMessage(temp->to(), JIANXIONG);
						ExternData::sgsout << jianxiong;
						ExternData::history.push(jianxiong);

						checkCaocao = true;
						TransCardMessage * trans = new TransCardMessage(DESK, PLAYER, true,
								0, playerRegionTypeNone, temp->to());
						trans->transpush(msg->cost(0).first);
						trans->transpush(msg->cost(1).first);
						delete fakeCard;
						return trans;
					}
					while (res != 0) {
						if (res->type() == FINALE)
							return res;
						else
							res = ExternData::platform.analyze(res);
					}
					break;
				} else
					while (shanornot != 0)
						shanornot = ExternData::platform.analyze(shanornot);
			}
		}
	}

	//处理卡牌
	ExternData::platform.abandon(msg->cost(0).first);
	ExternData::platform.abandon(msg->cost(1).first);
	delete fakeCard;
	return 0;
}

Message * func::pcshan(Message * m) {
	printDebug("func::pcshan: start");
	CardMessage * message = static_cast<CardMessage *>(m);
	ExternData::sgsout << m;
	ExternData::history.push(m);
	ExternData::platform.abandon(message->card());
	printDebug("func::pcshan: over");
	return 0;
}
Message * func::pctao(Message * m) {
	printDebug("func::pctao: start");
	CardMessage * message = static_cast<CardMessage *>(m);
	ExternData::sgsout << m;
	ExternData::history.push(m);
	Message * result = 0;
	if (message->to()->hp() <= 0 && message->to()->type() == SUNQUAN
			&& message->to()->role() == ZHU && message->from()->side() == WU) {
		SkillMessage * jiuyuan = new SkillMessage(message->to(), JIUYUAN);
		ExternData::sgsout << jiuyuan;
		ExternData::history.push(jiuyuan);
		message->to()->sethp() += 2;
		result = new HurtMessage(message->to(), message->to(), -2);
	} else {
		message->to()->sethp()++;result
		= new HurtMessage(message->to(), message->to(), -1);
	}
	//	ExternData::sgsout<<result;
	//	ExternData::history.push(result);
	ExternData::platform.abandon(message->card());
	printDebug("func::pctao: over");
	return result;
}

Message * func::pcatk(Message * m) {
	printDebug("func::pcatk: start");
	CardMessage * message = static_cast<CardMessage *>(m);
	if (message->from()->atkhorse() != 0) {
		TransCardMessage * abandon = new TransCardMessage(PLAYER, DUST, true,
				message->from(), PEQUIP);
		abandon->transpush(2);
		message->from()->setatk(0);
		ExternData::platform.analyze(abandon);
	}
	ExternData::sgsout << m;
	ExternData::history.push(m);
	message->from()->setatk(message->card());
	printDebug("func::pcatk: over");
	return 0;
}
Message * func::pcdfd(Message * m) {
	printDebug("func::pcdfd: start");
	CardMessage * message = static_cast<CardMessage *>(m);
	if (message->from()->dfdhorse() != 0) {
		TransCardMessage * abandon = new TransCardMessage(PLAYER, DUST, true,
				message->from(), PEQUIP);
		abandon->transpush(3);
		ExternData::platform.analyze(abandon);
	}
	ExternData::sgsout << m;
	ExternData::history.push(m);
	message->from()->setdfd(message->card());
	printDebug("func::pcdfd: over");
	return 0;
}
Message * func::pcweapon(Message * m) {
	printDebug("func::pcweapon: start");
	CardMessage * message = static_cast<CardMessage *>(m);

	// the following code has been revised by Hu Ronghang (order reversed)
	if (message->from()->weapon() != 0) {
		printDebug("func::pcarmor: abandon old weapon");
		TransCardMessage * abandon = new TransCardMessage(PLAYER, DUST, true,
				message->from(), PEQUIP);
		abandon->transpush(0);
		message->from()->setweapon(0);
		ExternData::platform.analyze(abandon);
	}
	ExternData::sgsout << m;
	ExternData::history.push(m);

	message->from()->setweapon(message->card());
	printDebug(
			"func::pcweapon: current weapon: "
					+ sgsui::cardFullDisplayName(message->card()));
	printDebug("func::pcweapon: over");
	return 0;
}
Message * func::pcarmor(Message * m) {
	printDebug("func::pcarmor: start");
	CardMessage * message = static_cast<CardMessage *>(m);

	// the following code has been revised by Hu Ronghang (order reversed)
	if (message->from()->armor() != 0) {
		printDebug("func::pcarmor: abandon old armor");
		TransCardMessage * abandon = new TransCardMessage(PLAYER, DUST, true,
				message->from(), PEQUIP);
		abandon->transpush(1);
		message->from()->setarmor(0);
		ExternData::platform.analyze(abandon);
	}
	ExternData::sgsout << m;
	ExternData::history.push(m);

	message->from()->setarmor(message->card());
	printDebug(
			"func::pcarmor: current armor: "
					+ sgsui::cardFullDisplayName(message->card()));
	printDebug("func::pcarmor: over");
	return 0;
}
bool func::pchanbing(Message * m) {
	printDebug("func::pchanbing: start");
	CardMessage * message = static_cast<CardMessage *>(m);
	int targetHasCards = 0;
	targetHasCards += message->to()->handnum();
	if (message->to()->weapon())
		targetHasCards++;
	if (message->to()->armor())
		targetHasCards++;
	if (message->to()->atkhorse())
		targetHasCards++;
	if (message->to()->dfdhorse())
		targetHasCards++;
	if (message->from()->weapon()
			&& message->from()->weapon()->type() == HANBING && targetHasCards
			&& message->from()->input()->useEquipOrNot(HANBING, message->to())) {
		std::vector < std::pair<PlayerRegionType, int> > selected =
				message->from()->input()->chooseCardResponse(message->to(),
						true, true, false,
						(targetHasCards > 2) ? 2 : targetHasCards, HANBING);
		if (selected.empty())
			return false;
		else {
			CardMessage * useEquip = new CardMessage(true, message->from(), PEQUIP, 0,
					message->to());
			ExternData::sgsout << useEquip;
			ExternData::history.push(useEquip);
			for (unsigned i = 0; i < selected.size(); i++) {
				TransCardMessage * chooseCardResult = new TransCardMessage(PLAYER, DUST,
						true, message->to(), selected[i].first);
				chooseCardResult->transpush(selected[i].second);
				message->to()->popCard(selected[i]);
				ExternData::platform.analyze(chooseCardResult);
			}
		}
	} else {
		printDebug("func::pchanbing: over, return false");
		return false;
	}
	printDebug("func::pchanbing: over, return true");
	return true;
}
bool func::pcrenwang(Message *m) {
	printDebug("func::pcrenwang: start");
	CardMessage * message = static_cast<CardMessage *>(m);
	if ((message->card()->color() == SPADE || message->card()->color() == CLUB)
			&& message->to()->armor()
			&& message->to()->armor()->type() == RENWANG) {
		CardMessage * useEquip = new CardMessage(true, message->to(), PEQUIP, 1, 0);
		ExternData::sgsout << useEquip;
		ExternData::history.push(useEquip);
		printDebug("func::pcrenwang: over, return true");
		return true;
	} else {
		printDebug("func::pcrenwang: over, return false");
		return false;
	}
}
bool func::pcbagua(Message * m) {
	printDebug("func::pcbagua: start");
	CardMessage * message = static_cast<CardMessage *>(m);
	if (message->to()->armor() && message->to()->armor()->type() == BAGUA
			&& message->to()->input()->useEquipOrNot(BAGUA, message->from())) {
		CardMessage * useEquip = new CardMessage(true, message->to(), PEQUIP, 1, 0);
		ExternData::sgsout << useEquip;
		ExternData::history.push(useEquip);
		printDebug("func::pcbagua: trying to get");
		const Card * judgeCard = ExternData::platform.getJudge();
		printDebug("func::pcbagua: got");
		JudgeMessage *judgeResult = 0;
		if (judgeCard->color() == HEART || judgeCard->color() == DIAMOND)
			judgeResult = new JudgeMessage(message->to(), message->to()->armor(),
					judgeCard, true);
		else
			judgeResult = new JudgeMessage(message->to(), message->to()->armor(),
					judgeCard, false);
		judgeResult = static_cast<JudgeMessage *>(func::pcguicai(judgeResult));
		ExternData::platform.analyze(judgeResult);
		if (judgeResult->effect()) {
			printDebug("func::pcbagua: over, return true");
			return true;
		} else {
			printDebug("func::pcbagua: over, return false");
			return false;
		}
	}

	// added by Hu Ronghang
	// this function is usually called by pcsha
	// you missed the case that there is no bagua, or there will be no return value (which is undefined)
	else {
		printDebug("func::pcbagua: over, return false");
		return false;
	}
}
bool func::pcqinglong(Message *m) {
	printDebug("func::pcqinglong: start");
	CardMessage * message = static_cast<CardMessage *>(m);
	bool attackerHasSha = false;
	for (int i = 0; i < message->from()->handnum(); i++) {
		if (message->from()->hand(i)->type() == SHA) {
			attackerHasSha = true;
			break;
		}
	}
	if (message->from()->weapon()
			&& message->from()->weapon()->type() == QINGLONG && attackerHasSha
			&& message->from()->input()->useEquipOrNot(QINGLONG, message->to())) {
		CardMessage * useEquip = new CardMessage(true, message->from(), PEQUIP, 0,
				message->to());
		ExternData::sgsout << useEquip;
		ExternData::history.push(useEquip);
		printDebug("func::pcqinglong: over, return true");
		return true;
	} else {
		printDebug("func::pcqinglong: over, return false");
		return false;
	}
}
bool func::pcguanshi(Message * m) {
	printDebug("func::pcguanshi: start");
	CardMessage * message = static_cast<CardMessage *>(m);
	int cardNum = 0;
	cardNum += message->from()->handnum();
	if (message->from()->armor())
		cardNum++;
	if (message->from()->atkhorse())
		cardNum++;
	if (message->from()->dfdhorse())
		cardNum++;
	if (message->from()->weapon()
			&& message->from()->weapon()->type() == GUANSHI && cardNum >= 2
			&& message->from()->input()->useEquipOrNot(GUANSHI, message->to())) {
		std::vector < std::pair<PlayerRegionType, int> > chooseCardResult =
				message->from()->input()->chooseCardResponse(message->from(),
						true, true, false, 2, GUANSHI);
		if (chooseCardResult.empty())
			return false;
		else {
			CardMessage * useEquip = new CardMessage(true, message->from(), PEQUIP, 0,
					message->to());
			ExternData::sgsout << useEquip;
			ExternData::history.push(useEquip);
			for (int i = 0; i < 2; i++) {
				TransCardMessage * abandon = new TransCardMessage(PLAYER, DUST, true,
						message->from(), chooseCardResult[i].first);
				abandon->transpush(chooseCardResult[i].second);
				message->from()->popCard(chooseCardResult[i]);
				ExternData::platform.analyze(abandon);
			}
		}
		printDebug("func::pcguanshi: over, return true");
		return true;
	} else {
		printDebug("func::pcguanshi: over, return false");
		return false;
	}
}
bool func::pcqilin(Message * m) {
	printDebug("func::pcqilin: start");
	CardMessage * message = static_cast<CardMessage *>(m);
	if (message->from()->weapon() && message->from()->weapon()->type() == QILIN
			&& (message->to()->atkhorse() || message->to()->dfdhorse())
			&& message->from()->input()->useEquipOrNot(QILIN, message->to())) {
		if (message->to()->atkhorse() && message->to()->dfdhorse()) { //如果有两匹马，则选择其中一匹
			std::vector < std::pair<PlayerRegionType, int> > selected =
					message->from()->input()->chooseCardResponse(message->to(),
							false, true, false, 1, QILIN);
			if (selected.empty())
				return false;
			else {
				CardMessage * useEquip = new CardMessage(true, message->from(), PEQUIP, 0,
						message->to());
				ExternData::sgsout << useEquip;
				ExternData::history.push(useEquip);
				TransCardMessage * abandon = new TransCardMessage(PLAYER, DUST, true,
						message->to(), selected[0].first);
				abandon->transpush(selected[0].second);
				message->to()->popCard(selected[0]);
				ExternData::platform.analyze(abandon);
				printDebug("func::pcqilin: over, return true");
				return true;
			}
		} else {
			CardMessage * useEquip = new CardMessage(true, message->from(), PEQUIP, 0,
					message->to());
			ExternData::sgsout << useEquip;
			ExternData::history.push(useEquip);
			TransCardMessage * abandon = new TransCardMessage(PLAYER, DUST, true,
					message->to(), PEQUIP);
			if (message->to()->atkhorse()) {
				abandon->transpush(2);
				message->to()->setatk(0);
			} else {
				abandon->transpush(3);
				message->to()->setdfd(0);
			}
			ExternData::platform.analyze(abandon);
			printDebug("func::pcqilin: over, return true");
			return true;
		}
	} else {
		printDebug("func::pcqilin: over, return false");
		return false;
	}
}
bool func::pccixiong(Message * m) {
	printDebug("func::pccixiong: start");
	CardMessage * message = static_cast<CardMessage *>(m);
	if (message->from()->weapon()
			&& message->from()->weapon()->type() == CIXIONG
			&& message->from()->sex() != message->to()->sex()
			&& message->from()->input()->useEquipOrNot(CIXIONG, message->to())) {
		CardMessage * useEquip = new CardMessage(true, message->from(), PEQUIP, 0,
				message->to());
		ExternData::sgsout << useEquip;
		ExternData::history.push(useEquip);
		if (message->to()->handnum() == 0
				|| message->to()->input()->yesOrNot(CIXIONG)) { //yesOrNot返回true是让打杀的人摸牌
			TransCardMessage * ciXiongAward = new TransCardMessage(POOLTOP, PLAYER,
					(message->from()->seat() == 0) ? true : false, 0, playerRegionTypeNone,
					message->from());
			ciXiongAward->transpush(ExternData::platform.get());
			ExternData::platform.analyze(ciXiongAward);
		} else {
			std::vector<int> abandon = message->to()->input()->discardCard(1);
			TransCardMessage * ciXiongPunish = new TransCardMessage(PLAYER, DUST, true,
					message->to(), PHAND);
			ciXiongPunish->transpush(abandon[0]);
			message->to()->popCard(
					std::pair<PlayerRegionType, int>(PHAND, abandon[0]));
			ExternData::platform.analyze(ciXiongPunish);
		}
		printDebug("func::pccixiong: over, return true");
		return true;
	} else {
		printDebug("func::pccixiong: over, return false");
		return false;
	}
}
Message * func::pcshunqian(Message * m) {
	printDebug("func::pcshunqian: start");
	CardMessage * message = static_cast<CardMessage *>(m);
	ExternData::sgsout << m;
	ExternData::history.push(m);
	if (func::pcwuxie(m)) {
		printDebug("func::pcshunqian: wuxie succeeded");
		ExternData::platform.abandon(message->card());
		return 0;
	} else {
		printDebug("func::pcshunqian: wuxie failed");
		std::vector < std::pair<PlayerRegionType, int> > selected =
				message->from()->input()->chooseCardResponse(message->to(),
						true, true, true, 1, SHUNQIAN);
		if (selected.empty()) {
			printDebug("func::pcshunqian: over");
			return 0;
		} else {
			TransCardMessage * trans = new TransCardMessage(PLAYER, PLAYER,
					(message->from()->seat() == 0) ? true : false,
					message->to(), selected[0].first, message->from());
			trans->transpush(selected[0].second);
			message->to()->popCard(selected[0]);
			ExternData::platform.abandon(message->card());
			printDebug("func::pcshunqian: over");
			return trans;
		}
	}
}
Message * func::pcguochai(Message * m) {
	CardMessage * message = static_cast<CardMessage *>(m);
	ExternData::sgsout << m;
	ExternData::history.push(m);
	if (func::pcwuxie(m)) {
		ExternData::platform.abandon(message->card());
		return 0;
	} else {
		std::vector < std::pair<PlayerRegionType, int> > selected =
				message->from()->input()->chooseCardResponse(message->to(),
						true, true, true, 1, SHUNQIAN);
		if (selected.empty())
			return 0;
		else {
			TransCardMessage * trans = new TransCardMessage(PLAYER, DUST, true,
					message->to(), selected[0].first);
			trans->transpush(selected[0].second);
			message->to()->popCard(selected[0]);
			ExternData::platform.abandon(message->card());
			return trans;
		}
	}
}
bool func::pcwuxie(Message * m, Message * o, int times) {
	bool result = false;
	Message * origin = (o == 0) ? m : o;
	CardMessage * message = static_cast<CardMessage *>(m);
	CardMessage * answer = 0;
	if (o == 0)
		answer = static_cast<CardMessage *>(message->to()->WuXieOrNot(origin, times));
	if (answer != 0) {
		if (answer->from()->type() == HUANGYUEYING
				&& answer->from()->input()->useSkillOrNot(JIZHI)) {
			SkillMessage * jizhi = new SkillMessage(answer->from(), JIZHI);
			ExternData::sgsout << jizhi;
			ExternData::history.push(jizhi);
			TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER,
					answer->from()->seat() == 0, 0, playerRegionTypeNone, answer->from());
			trans->transpush(ExternData::platform.get());
			ExternData::platform.analyze(trans);
		}
		ExternData::sgsout << answer;
		ExternData::history.push(answer);
		if (pcwuxie(answer, origin, ++times)) {
			result = false;
		} else {
			result = true;
		}
		ExternData::platform.abandon(answer->card());
	} else
		for (Player * temp = message->to()->after(); temp != message->to();
				temp = temp->after()) {
			answer = static_cast<CardMessage *>(temp->WuXieOrNot(origin, times));
			if (answer != 0) {
				if (answer->from()->type() == HUANGYUEYING
						&& answer->from()->input()->useSkillOrNot(JIZHI)) {
					SkillMessage * jizhi = new SkillMessage(answer->from(), JIZHI);
					ExternData::sgsout << jizhi;
					ExternData::history.push(jizhi);
					TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER,
							answer->from()->seat() == 0, 0, playerRegionTypeNone,
							answer->from());
					trans->transpush(ExternData::platform.get());
					ExternData::platform.analyze(trans);
				}
				ExternData::sgsout << answer;
				ExternData::history.push(answer);
				if (pcwuxie(answer, origin, ++times)) {
					result = false;
				} else
					result = true;
				ExternData::platform.abandon(answer->card());
				break;
			}
		}
	return result;
}

Message * func::pcwuzhong(Message * m) {
	CardMessage * message = static_cast<CardMessage *>(m);
	message->addto(message->from());
	ExternData::sgsout << m;
	ExternData::history.push(m);
	if (pcwuxie(m)) {
		ExternData::platform.abandon(message->card());
		return 0;
	} else {
		TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER,
				message->from()->seat() == 0, 0, playerRegionTypeNone, message->from());
		for (int i = 0; i < 2; i++)
			trans->transpush(ExternData::platform.get());
		ExternData::platform.abandon(message->card());
		ExternData::platform.msgInsert(trans);
		return 0;
	}
}
Message * func::pctaoyuan(Message * m) {
	CardMessage * message = static_cast<CardMessage *>(m);
	std::stack<Message *> groove; //内部消息沟道

	//对初始消息添加目标。
	while (message->targets() > 0)
		message->popto();
	message->addto(message->from());
	for (Player * temp = message->from()->after(); temp != message->from();
			temp = temp->after()) {
		message->addto(temp);
	}
	ExternData::sgsout << m;
	ExternData::history.push(m);

	//解构初始消息
	while (message->targets()) {
		CardMessage * temp = new CardMessage(message); //截取构造
		temp->setdissolve(true); //设置为解构消息
		message->popto();
		groove.push(temp);
	}

	//沟道处理
	while (!groove.empty()) {
		CardMessage * grooveTop = static_cast<CardMessage *>(groove.top());
		groove.pop();
		if (grooveTop->dissolved()) {
			if (grooveTop->to()->hp() < grooveTop->to()->maxHP()) {
				if (func::pcwuxie(grooveTop)) {
					continue;
				} else {
					grooveTop->to()->sethp()++;HurtMessage
					* hurt = new HurtMessage(grooveTop->from(), grooveTop->to(), -1);
					groove.push(hurt);
					continue;
				}
			} else
				continue;
		} else {
			Message * res = ExternData::platform.analyze(grooveTop);
			if (res != 0) {
				if (res->type() == FINALE)
					return res;
				else
					groove.push(res);
			}
		}
	}

	//卡牌处理
	ExternData::platform.abandon(message->card());
	return 0;
}
Message * func::pcwanjian(Message * m) {
	CardMessage * message = static_cast<CardMessage *>(m);
	std::stack<Message *> groove; //内部消息沟道

	bool checkCaocao = false;

	//对初始消息添加目标。
	while (message->targets() > 0)
		message->popto();
	for (Player * temp = message->from()->after(); temp != message->from();
			temp = temp->after()) {
		message->addto(temp);
	}
	ExternData::sgsout << m;
	ExternData::history.push(m);

	//解构初始消息
	while (message->targets()) {
		CardMessage * temp = new CardMessage(message); //截取构造
		temp->setdissolve(true); //设置为解构消息
		message->popto();
		groove.push(temp);
	}

	//沟道处理
	while (!groove.empty()) {
		CardMessage * grooveTop = static_cast<CardMessage *>(groove.top());
		groove.pop();
		if (grooveTop->dissolved()) {
			if (func::pcwuxie(grooveTop)) {
				continue;
			} else {
				Message * shanornot = grooveTop->to()->ShanOrNot(grooveTop);
				if (shanornot == 0) {
					grooveTop->to()->sethp()--;Message
					* hurt = new HurtMessage(grooveTop->from(), grooveTop->to(), 1);
					if (static_cast<HurtMessage *>(hurt)->to()->type() == CAOCAO
							&& (hurt = ExternData::platform.analyze(hurt)) == 0
							&& grooveTop->to()->input()->useSkillOrNot(
									JIANXIONG)) {
						SkillMessage * jianxiong = new SkillMessage(grooveTop->to(),
								JIANXIONG);
						ExternData::sgsout << jianxiong;
						ExternData::history.push(jianxiong);

						checkCaocao = true;
						TransCardMessage * trans = new TransCardMessage(DESK, PLAYER, true,
								0, playerRegionTypeNone, grooveTop->to());
						trans->transpush(message->card());
						hurt = trans;
					}
					if (hurt != 0)
						groove.push(hurt);
					continue;
				} else
					groove.push(shanornot);
			}
		} else {
			Message * res = ExternData::platform.analyze(grooveTop);
			if (res != 0) {
				if (res->type() == FINALE)
					return res;
				else
					groove.push(res);
			}
		}
	}

	//卡牌处理
	message->card()->setmask();
	if (!checkCaocao)
		ExternData::platform.abandon(message->card());
	return 0;
}
Message * func::pcnanman(Message * m) {
	CardMessage * message = static_cast<CardMessage *>(m);
	std::stack<Message *> groove; //内部消息沟道

	bool checkCaocao = false;

	//对初始消息添加目标。
	while (message->targets() > 0)
		message->popto();
	for (Player * temp = message->from()->after(); temp != message->from();
			temp = temp->after()) {
		message->addto(temp);
	}
	ExternData::sgsout << m;
	ExternData::history.push(m);

	//解构初始消息
	while (message->targets()) {
		CardMessage * temp = new CardMessage(message); //截取构造
		temp->setdissolve(true); //设置为解构消息
		message->popto();
		groove.push(temp);
	}

	//沟道处理
	while (!groove.empty()) {
		CardMessage * grooveTop = static_cast<CardMessage *>(groove.top());
		groove.pop();
		if (grooveTop->dissolved()) {
			if (func::pcwuxie(grooveTop)) {
				continue;
			} else {
				Message * shaornot = grooveTop->to()->ShaOrNot(grooveTop);
				if (shaornot == 0) {
					grooveTop->to()->sethp()--;Message
					* hurt = new HurtMessage(grooveTop->from(), grooveTop->to(), 1);
					if (static_cast<HurtMessage *>(hurt)->to()->type() == CAOCAO
							&& (hurt = ExternData::platform.analyze(hurt)) == 0
							&& grooveTop->to()->input()->useSkillOrNot(
									JIANXIONG)) {
						SkillMessage * jianxiong = new SkillMessage(grooveTop->to(),
								JIANXIONG);
						ExternData::sgsout << jianxiong;
						ExternData::history.push(jianxiong);

						checkCaocao = true;
						TransCardMessage * trans = new TransCardMessage(DESK, PLAYER, true,
								0, playerRegionTypeNone, grooveTop->to());
						trans->transpush(message->card());
						hurt = trans;
					}
					if (hurt != 0)
						groove.push(hurt);
					continue;
				} else {
					switch (shaornot->type()) {
					case HITCARD:
						ExternData::sgsout << shaornot;
						ExternData::history.push(shaornot);
						ExternData::platform.abandon(
								static_cast<CardMessage *>(shaornot)->card());
						continue;
					case USECARD: {
						ZhangBaMessage * zhangba = static_cast<ZhangBaMessage *>(shaornot);
						ExternData::sgsout << shaornot;
						ExternData::history.push(shaornot);
						ExternData::platform.abandon(zhangba->cost(0).first);
						ExternData::platform.abandon(zhangba->cost(1).first);
					}
						continue;
					default:
						break;
					}
				}
			}
		} else {
			Message * res = ExternData::platform.analyze(grooveTop);
			if (res != 0) {
				if (res->type() == FINALE)
					return res;
				else
					groove.push(res);
			}
		}
	}

	//卡牌处理
	message->card()->setmask();
	if (!checkCaocao)
		ExternData::platform.abandon(message->card());
	return 0;
}
Message * func::pcyanshi(Message * m) {
	CardMessage * message = static_cast<CardMessage *>(m);
	message->to()->setjudge().push(message->card());
	ExternData::sgsout << m;
	ExternData::history.push(m);
	return 0;
}
Message * func::pcshandian(Message * m) {
	CardMessage * message = static_cast<CardMessage *>(m);
	ExternData::sgsout << m;
	ExternData::history.push(m);

	bool checkCaocao = false;

	bool afterHasShanDian = false;
	for (int i = 0; i < message->to()->after()->judgenum(); i++) {
		if (message->to()->after()->judge(i)->type() == SHANDIAN) {
			afterHasShanDian = true;
			break;
		}
	}
	if (!func::pcwuxie(m)) {
		const Card * judgeCard = ExternData::platform.getJudge();
		JudgeMessage * judgeResult = 0;
		if (judgeCard->color() == SPADE && judgeCard->number() >= 2
				&& judgeCard->number() <= 9) {
			judgeResult = new JudgeMessage(message->from(), message->card(),
					judgeCard, true);
		} else {
			judgeResult = new JudgeMessage(message->from(), message->card(),
					judgeCard, false);
		}
		judgeResult = static_cast<JudgeMessage *>(func::pcguicai(judgeResult));
		ExternData::platform.analyze(judgeResult);
		if (judgeResult->effect()) {
			message->from()->sethp() -= 3;
			Message * hurt = new HurtMessage(0, message->from(), 3);
			if (static_cast<HurtMessage *>(hurt)->to()->type() == CAOCAO && (hurt =
					ExternData::platform.analyze(hurt)) == 0
					&& message->from()->input()->useSkillOrNot(JIANXIONG)) {
				SkillMessage * jianxiong = new SkillMessage(message->from(), JIANXIONG);
				ExternData::sgsout << jianxiong;
				ExternData::history.push(jianxiong);

				checkCaocao = true;
				TransCardMessage* trans = new TransCardMessage(PLAYER, DUST, true,
						message->from(), PJUDGE);
				trans->transpush(message->card());
				ExternData::sgsout << trans;
				delete trans;

				trans = new TransCardMessage(DESK, PLAYER, true, 0, playerRegionTypeNone,
						message->from());
				trans->transpush(message->card());
				message->card()->setmask();
				return trans;
			}
			message->card()->setmask();
			TransCardMessage* trans = new TransCardMessage(PLAYER, DUST, true,
					message->from(), PJUDGE);
			trans->transpush(message->card());
			ExternData::platform.analyze(trans);
			return hurt;
		} else {
			TransCardMessage* trans = new TransCardMessage(
					PLAYER,
					PLAYER,
					true,
					message->from(),
					PJUDGE,
					afterHasShanDian ?
							message->from()->after()->after() :
							message->from()->after(), true);
			trans->transpush(message->card());
			ExternData::platform.analyze(trans);
			return 0;
		}
	} else {
		TransCardMessage* trans = new TransCardMessage(
				PLAYER,
				PLAYER,
				true,
				message->from(),
				PJUDGE,
				afterHasShanDian ?
						message->from()->after()->after() :
						message->from()->after(), true);
		trans->transpush(message->card());
		ExternData::platform.analyze(trans);
		return 0;
	}
}
Message * func::pcsishu(Message * m) {
	CardMessage * message = static_cast<CardMessage *>(m);
	ExternData::sgsout << m;
	ExternData::history.push(m);
	if (!func::pcwuxie(m)) {
		const Card * judgeCard = ExternData::platform.getJudge();
		JudgeMessage * judgeResult = 0;
		if (judgeCard->color() != HEART) {
			judgeResult = new JudgeMessage(message->from(), message->card(),
					judgeCard, true);
		} else {
			judgeResult = new JudgeMessage(message->from(), message->card(),
					judgeCard, false);
		}
		judgeResult = static_cast<JudgeMessage *>(func::pcguicai(judgeResult));
		ExternData::platform.analyze(judgeResult);
		if (judgeResult->effect())
			message->from()->setstatus() = message->from()->setstatus() | LE;
	}
	message->card()->setmask();
	TransCardMessage* trans = new TransCardMessage(PLAYER, DUST, true, message->from(),
			PJUDGE);
	trans->transpush(message->card());
	ExternData::platform.analyze(trans);
	return 0;
}

Message * func::pcwugu(Message * m) {
	//发送收到的消息
	ExternData::sgsout << m;
	ExternData::history.push(m);

	//做好各项准备
	CardMessage * message = static_cast<CardMessage *>(m);
	std::stack<Message *> groove; //内部消息沟道
	int playerAlive = 1;
	for (Player * temp = message->from()->after(); temp != message->from();
			temp = temp->after())
		playerAlive++;
	std::vector < std::pair<const Card *, bool> > candidate; //待选的牌，栈中记录着可选的牌和有没有被选走。
	for (int i = 0; i < playerAlive; i++) {
		std::pair<const Card *, bool> temp(ExternData::platform.get(), false);
		candidate.push_back(temp);
	}

	//对初始消息添加目标
	while (message->targets() > 0)
		message->popto();
	message->addto(message->from());
	for (Player * temp = message->from()->after(); temp != message->from();
			temp = temp->after()) {
		message->addto(temp);
	}

	//解构初始消息
	while (message->targets()) {
		CardMessage * temp = new CardMessage(message); //截取构造
		temp->setdissolve(true); //设置为解构消息
		message->popto();
		groove.push(temp);
	}

	//沟道处理
	while (!groove.empty()) {
		CardMessage * grooveTop = static_cast<CardMessage *>(groove.top());
		groove.pop();
		if (grooveTop->dissolved()) {
			if (func::pcwuxie(grooveTop)) {
				continue;
			} else {
				int pos = grooveTop->to()->input()->chooseWugu(candidate);
				candidate[pos].second = true;
				TransCardMessage * trans = new TransCardMessage(DESK, PLAYER, true, 0,
						playerRegionTypeNone, grooveTop->to());
				trans->transpush(pos, candidate[pos].first);
				groove.push(trans);
				continue;
			}
		} else {
			Message * res = ExternData::platform.analyze(grooveTop);
			if (res != 0) {
				if (res->type() == FINALE)
					return res;
				else
					groove.push(res);
			}
		}
	}

	//后续处理
	TransCardMessage * trans = new TransCardMessage(DESK, DUST, true);
	for (unsigned i = 0; i < candidate.size(); i++) {
		if (candidate[i].second == false) {
			trans->transpush(i, candidate[i].first);
			candidate[i].second = true;
		}
	}
	if (trans->cards() == 0)
		delete trans;
	else
		ExternData::platform.analyze(trans);
	ExternData::platform.abandon(message->card());
	return 0;
}

ZhangBaMessage * func::pczhangba(ZhangBaMessage * m)
{
	std::vector<int> discard = m->from()->input()->discardCard(2);
	if (discard.empty())
	{
		printDebug("func::pczhangba: condition if");
		delete m;
		return 0;
	}
	else
	{
		printDebug("func::pczhangba: condition else");
		std::sort(discard.begin(), discard.end());
		for (int i = 1; i >= 0; --i)
			m->addcost(discard[i]);

		//显示弃牌的过程
		TransCardMessage * trans = new TransCardMessage(PLAYER, DUST, true, m->from(), PHAND);
		trans->transpush(m->cost(0).second);
		trans->transpush(m->cost(1).second);
		ExternData::sgsout << trans;
		printDebug("func::pcsha(zhangba version): message 1 sent");
		delete trans;
		for (int i = 1; i >= 0; i--)
			m->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, discard[i]));

		return m;
	}
}

Message * func::pcjiedao(Message * m) {
	CardMessage * message = static_cast<CardMessage *>(m);
	ExternData::sgsout << m;
	ExternData::history.push(m);
	Message * res = message->to(0)->ShaOrNot(m);
	if (res != 0) {
		ExternData::platform.abandon(message->card());
		return res;
	} else {
		TransCardMessage * trans = new TransCardMessage(PLAYER, PLAYER, true, message->to(),
				PEQUIP, message->from());
		trans->transpush(0, message->to()->weapon());
		message->to()->setweapon(0);
		ExternData::platform.abandon(message->card());
		return trans;
	}
}

Message * func::pcjuedou(Message * m) {
	CardMessage * message = static_cast<CardMessage *>(m);
	bool checkCaocao = false;
	bool checkXuchu = (message->from()->type() == XUCHU)
			&& (message->from()->status() & SkillOneUsed);
	ExternData::sgsout << m;
	ExternData::history.push(m);
	if (pcwuxie(m)) {
		ExternData::platform.abandon(message->card());
		return 0;
	} else {
		Player * inTurn = message->to();
		Player * hurtFrom = message->from();
		while (1) {
			bool checkLvbu = hurtFrom->type() == LVBU; //检查是否是吕布
			for (int i = 0; i < 1 + checkLvbu; i++) {
				if (i == 1) {
					SkillMessage* wushuang = new SkillMessage(hurtFrom, WUSHUANG);
					ExternData::sgsout << wushuang;
					ExternData::history.push(wushuang);
				}
				Message * shaornot = inTurn->ShaOrNot(m);
				if (shaornot == 0) {
					inTurn->sethp() -= 1 + checkXuchu;
					Message * hurt = new HurtMessage(hurtFrom, inTurn,
							1 + checkXuchu);
					message->card()->setmask();
					if (inTurn->type() == CAOCAO
							&& (hurt = ExternData::platform.analyze(hurt)) == 0
							&& inTurn->input()->useSkillOrNot(JIANXIONG)) {
						SkillMessage * jianxiong = new SkillMessage(inTurn, JIANXIONG);
						ExternData::sgsout << jianxiong;
						ExternData::history.push(jianxiong);

						checkCaocao = true;
						TransCardMessage * trans = new TransCardMessage(DESK, PLAYER, true,
								0, playerRegionTypeNone, inTurn);
						trans->transpush(message->card());
						message->card()->setmask();
						ExternData::platform.msgInsert(trans);
					} else {
						ExternData::platform.abandon(message->card());
						if (hurt != 0)
							ExternData::platform.msgInsert(hurt);
					}
					return 0;
				} else {
					switch (shaornot->type()) {
					case HITCARD:
						ExternData::sgsout << shaornot;
						ExternData::history.push(shaornot);
						ExternData::platform.abandon(
								static_cast<CardMessage *>(shaornot)->card());
						break;
					case USECARD: {
						ZhangBaMessage * zhangba = static_cast<ZhangBaMessage *>(shaornot);
						ExternData::sgsout << shaornot;
						ExternData::history.push(shaornot);
						ExternData::platform.abandon(zhangba->cost(0).first);
						ExternData::platform.abandon(zhangba->cost(1).first);
						break;
					}
						break;
					default:
						break;
					}
				}
			}
			Player * temp = inTurn;
			inTurn = hurtFrom;
			hurtFrom = temp;
		}
	}

	// the function will not reach this point
	// it should have returned somewhere above
	return 0;
}
