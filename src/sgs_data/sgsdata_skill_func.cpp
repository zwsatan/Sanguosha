#include "sgsdata_skill.h"
#include "sgsdata_skill_func.h"
#include "sgsdata_msg.h"
#include "sgsdata_card.h"
#include "sgsdata_extern.h"
#include "sgsdata_history.h"
#include "sgsdata_player.h"
#include "sgs_Interface.h"
#include "sgsdata_platform.h"
#include "sgsui_output.h"

using namespace sgs;
using namespace ConstData;
using namespace DataType;
using namespace Derive;

Message * func::pcwusheng(Message * msg)
{
	WuShengMessage * message = static_cast<WuShengMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);

	CardMessage * result = new CardMessage(false, message->from(), message->pos().first, message->pos().second);
	for (int i = 0; i < message->targets(); ++i)
		result->addto(message->to(message->targets() - 1 - i));
	message->card()->setmask(SHA);
	message->from()->popCard(message->pos());
	return result;
}

Message * func::pcganglie(Message * msg)
{
	GangLieMessage * message = static_cast<GangLieMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);

	const Card * judge = ExternData::platform.getJudge();
	JudgeMessage * judgeResult = 0;
	if (judge->color() != HEART)
		judgeResult = new JudgeMessage(message->from(), GANGLIE, judge, true);
	else
		judgeResult = new JudgeMessage(message->from(), GANGLIE, judge, false);

	judgeResult = static_cast<JudgeMessage *>(func::pcguicai(judgeResult));
	ExternData::sgsout << judgeResult;
	ExternData::history.push(judgeResult);
	ExternData::platform.abandon(judgeResult->result());

	// 刚烈无效,直接返回
	if (!judgeResult->effect())
		return 0;

	// yesOrNot返回真时表示选择弃牌
	if (message->to()->handnum() >= 2 && message->to()->input()->yesOrNot(GANGLIE))
	{
		std::vector<int> discard = message->to()->input()->discardCard(2);
		std::sort(discard.begin(), discard.end());

		TransCardMessage * result = new TransCardMessage(PLAYER, DUST, true, message->to(), PHAND);
		result->transpush(discard[1]);
		message->to()->popCard(std::pair<PlayerRegionType, int>(PHAND, discard[1]));
		result->transpush(discard[0]);
		message->to()->popCard(std::pair<PlayerRegionType, int>(PHAND, discard[0]));
		return result;
	}
	else
	{
		message->to()->sethp()--;
		HurtMessage * result = new HurtMessage(message->from(), message->to(), 1);
		return result;
	}
}

Message * func::pcqingguo(Message * msg)
{
	QingGuoMeesage * message = static_cast<QingGuoMeesage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);

	CardMessage * result = new CardMessage(false, message->from(), PHAND, message->pos());
	message->card()->setmask(SHAN);
	message->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, message->pos()));
	return result;
}

Message * func::pclongdan(Message * msg)
{
	LongDanMessage * message = static_cast<LongDanMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);

	CardMessage * result = new CardMessage(false, message->from(), PHAND, message->pos());
	for (int i = 0; i < message->targets(); ++i)
		result->addto(message->to(message->targets() - 1 - i));

	switch (message->card()->type())
	{
	case SHA:
		message->card()->setmask(SHAN);
		break;
	case SHAN:
		message->card()->setmask(SHA);
		break;
	default:
		break;
	}

	message->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, message->pos()));
	return result;
}

Message * func::pcqixi(Message * msg)
{
	QiXiMessage * message = static_cast<QiXiMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);

	CardMessage * result = new CardMessage(false, message->from(), message->pos().first, message->pos().second);
	result->addto(message->to());
	message->card()->setmask(GUOCHAI);
	message->from()->popCard(message->pos());
	return result;
}

Message * func::pcfankui(Message * msg)
{
	FanKuiMessage * message = static_cast<FanKuiMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);

	std::vector < std::pair<PlayerRegionType, int> > choose = message->from()->input()->chooseCardResponse(message->to(), true, true, false, 1, FANKUI);
	bool isOpen = message->from()->seat() == 0;
	TransCardMessage * result = new TransCardMessage(PLAYER, PLAYER, isOpen, message->to(), choose[0].first, message->from());
	result->transpush(choose[0].second);
	message->to()->popCard(choose[0]);
	return result;
}

Message * func::pcguicai(Message * msg)
{
	JudgeMessage * message = static_cast<JudgeMessage *>(msg);
	Player * simayi = 0;
	if (message->from()->type() == SIMAYI)
	{
		simayi = message->from();
	}
	else
	{
		for (Player * temp = message->from()->after(); temp != message->from(); temp = temp->after())
		{
			if (temp->type() == SIMAYI)
			{
				simayi = temp;
				break;
			}
		}
	}

	if (simayi && !(simayi->status() & DEAD) && simayi->handnum() && simayi->input()->useSkillOrNot(GUICAI, message))
	{
		// 播放动画
		SkillMessage * guicaiMsg = new SkillMessage(simayi, GUICAI);
		ExternData::sgsout << guicaiMsg;
		ExternData::history.push(guicaiMsg);

		// 确定改判牌
		std::vector < std::pair<PlayerRegionType, int> > choose = simayi->input()->chooseCardResponse(simayi, true, false, false, 1, GUICAI);

		// 处理原判定情况
		ExternData::sgsout << message;
		ExternData::history.push(message);
		ExternData::platform.abandon(message->result());

		// 播放改判动画
		TransCardMessage * transMsg = new TransCardMessage(PLAYER, POOLTOP, true, simayi, PHAND);
		transMsg->transpush(choose[0].second);
		ExternData::sgsout << transMsg;
		delete transMsg;

		// 确定改判牌
		const Card * changeCard = simayi->visitCard(choose[0]);
		simayi->popCard(choose[0]);

		// 修改判定结果
		if (message->cardJudge())
		{
			CardType cardMask = message->card()->mask();

			if (cardMask == SISHU)
			{
				if (changeCard->color() != HEART)
					message = new JudgeMessage(message->from(), message->card(), changeCard, true);
				else
					message = new JudgeMessage(message->from(), message->card(), changeCard, false);
				return message;
			}
			else if (cardMask == BAGUA)
			{
				if (changeCard->color() == HEART || changeCard->color() == DIAMOND)
					message = new JudgeMessage(message->from(), message->card(), changeCard, true);
				else
					message = new JudgeMessage(message->from(), message->card(), changeCard, false);
				return message;
			}
			else if (cardMask == SHANDIAN)
			{
				if (changeCard->color() == SPADE && 2 <= changeCard->number() && changeCard->number() <= 9)
					message = new JudgeMessage(message->from(), message->card(), changeCard, true);
				else
					message = new JudgeMessage(message->from(), message->card(), changeCard, false);
				return message;
			}
		}
		else
		{
			HeroSkill skillType = message->skill();

			if (skillType == GANGLIE)
			{
				if (changeCard->color() != HEART)
					message = new JudgeMessage(message->from(), message->skill(), changeCard, true);
				else
					message = new JudgeMessage(message->from(), message->skill(), changeCard, false);
				return message;
			}
			else if (skillType == LUOSHEN)
			{
				if (changeCard->color() == SPADE || changeCard->color() == CLUB)
					message = new JudgeMessage(message->from(), message->skill(), changeCard, true);
				else
					message = new JudgeMessage(message->from(), message->skill(), changeCard, false);
				return message;
			}
			else if (skillType == TIEQI)
			{
				if (changeCard->color() == HEART || changeCard->color() == DIAMOND)
					message = new JudgeMessage(message->from(), message->skill(), changeCard, true);
				else
					message = new JudgeMessage(message->from(), message->skill(), changeCard, false);
				return message;
			}
		}
	}
	else
	{
		return msg;
	}

	return 0;
}

Message * func::pckurou(Message * msg)
{
	SkillMessage * message = static_cast<SkillMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);

	message->from()->sethp()--;
	Message * res = new HurtMessage(message->from(), message->from(), 1);
	while (res != 0)
	{
		if (res->type() == KILL)
			return res;
		res = ExternData::platform.analyze(res);
	}

	bool isOpen = message->from()->seat() == 0;
	TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, message->from());
	trans->transpush(ExternData::platform.get());
	trans->transpush(ExternData::platform.get());
	return trans;
}

Message * func::pcqingnang(Message * msg)
{
	QingNangMessage * message = static_cast<QingNangMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);
	message->from()->setstatus() |= SkillOneUsed;

	TransCardMessage * trans = new TransCardMessage(PLAYER, DUST, true, message->from(), PHAND);
	trans->transpush(message->pos());
	message->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, message->pos()));
	ExternData::platform.analyze(trans);

	message->to()->sethp()++;
	HurtMessage * hurt = new HurtMessage(message->from(), message->to(), -1);
	return hurt;
}

Message * func::pcjijiu(Message * msg)
{
	JiJiuMessage * message = static_cast<JiJiuMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);

	CardMessage * result = new CardMessage(false, message->from(), message->pos().first, message->pos().second);
	message->card()->setmask(TAO);
	message->from()->popCard(message->pos());
	return result;
}

Message * func::pcrende(Message * msg)
{
	RenDeMessage * message = static_cast<RenDeMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);

	int cardSize = message->cards();
	int skillStatus = message->from()->status();
	if (((cardSize == 1 && (skillStatus & SkillTwoUsed)) || cardSize >= 2) && !(skillStatus & SkillOneUsed))
	{
		message->from()->setstatus() |= SkillOneUsed;
		if (message->from()->hp() < message->from()->maxHP())
		{
			printDebug("func::pcrende: add HP, player:"
					   + QString::number(message->from()->seat())
					   + " current HP = "
					   + QString::number(message->from()->hp())
					   + " maxHP = "
					   + QString::number(message->from()->maxHP()));
			message->from()->sethp()++;
			HurtMessage * hurt = new HurtMessage(message->from(), message->from(), -1);
			ExternData::platform.analyze(hurt);
		}
	}
	else if (cardSize == 1 && !(skillStatus & SkillTwoUsed) && !(skillStatus & SkillOneUsed))
		message->from()->setstatus() |= SkillTwoUsed;

	bool isOpen = message->from()->seat() == 0 || message->to()->seat() == 0;
	TransCardMessage * trans = new TransCardMessage(PLAYER, PLAYER, isOpen, message->from(), PHAND, message->to());
	for (int i = 0; i < message->cards(); i++)
	{
		trans->transpush(message->pos(i));
		message->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, message->pos(i)));
	}
	return trans;
}

Message * func::pcjijiang(Message * msg)
{
	JiJiangMessage * message = static_cast<JiJiangMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);
	Message * result = 0;

	for (Player * temp = message->from()->after(); temp != message->from(); temp = temp->after())
	{
		if (temp->side() == SHU && temp->input()->yesOrNot(JIJIANG))
		{
			result = temp->input()->shaOrNot(message->from(), JIJIANG);

			if (result == 0)
				continue;

			CardMessage * card = 0;
			MessageType msgType = result->type();

			if (msgType == USESKILL)
			{
				card = static_cast<CardMessage *>(static_cast<SkillMessage *>(result)->settle(result));
			}
			else if (msgType == HITCARD)
			{
				card = static_cast<CardMessage *>(result);
				card->from()->popCard(card->pos());
			}

			TransCardMessage * trans = new TransCardMessage(PLAYER, PLAYER, true, card->from(), PHAND, message->from());
			trans->transpush(card->pos().second, card->card());
			ExternData::platform.analyze(trans);

			delete card;
			card = new CardMessage(false, message->from(), PHAND, 0);
			message->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, 0));
			for (int i = 0; i < message->targets(); i++)
				card->addto(message->to(message->targets() - 1 - i));
			return card;
		}
	}
	return 0;
}

Message * func::pchujia(Message * msg)
{
	SkillMessage * skillMsg = static_cast<SkillMessage *>(msg);
	ExternData::sgsout << skillMsg;
	ExternData::history.push(skillMsg);
	Message * result = 0;

	for (Player * temp = skillMsg->from()->after(); temp != skillMsg->from(); temp = temp->after())
	{
		bool isWei = temp->side() == WEI;
		if (isWei && temp->input()->yesOrNot(HUJIA))
		{
			result = temp->input()->shanOrNot(skillMsg->from(), HUJIA);
			if (result == 0)
				continue;

			CardMessage * cardMsg = 0;
			MessageType msgType = result->type();

			if (msgType == USESKILL)
			{
				cardMsg = static_cast<CardMessage *>(static_cast<SkillMessage *>(result)->settle(result));
			}
			else if (msgType == HITCARD)
			{
				cardMsg = static_cast<CardMessage *>(result);
				cardMsg->from()->popCard(cardMsg->pos());
			}

			TransCardMessage * trans = new TransCardMessage(PLAYER, PLAYER, true, cardMsg->from(), PHAND, skillMsg->from());
			trans->transpush(cardMsg->pos().second, cardMsg->card());
			ExternData::platform.analyze(trans);

			delete cardMsg;
			cardMsg = new CardMessage(false, skillMsg->from(), PHAND, 0);
			skillMsg->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, 0));
			return cardMsg;
		}
	}
	return 0;
}

Message * func::pclijian(Message * msg)
{
	LiJianMessage * message = static_cast<LiJianMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);
	message->from()->setstatus() |= SkillOneUsed;

	TransCardMessage * trans = new TransCardMessage(PLAYER, DUST, true, message->from(), message->pos().first);
	trans->transpush(message->pos().second);
	ExternData::platform.analyze(trans);

	Player * inTurn = message->second();
	Player * hurtFrom = message->first();
	while (1)
	{
		bool checkLvbu = hurtFrom->type() == LVBU; //检查是否是吕布
		for (int i = 0; i < 1 + checkLvbu; i++)
		{
			if (i == 1)
			{
				SkillMessage* wushuang = new SkillMessage(hurtFrom, WUSHUANG);
				ExternData::sgsout << wushuang;
				ExternData::history.push(wushuang);
			}

			Message * shaornot = inTurn->ShaOrNot(message);
			if (shaornot == 0)
			{
				inTurn->sethp() --;Message
				* hurt = new HurtMessage(hurtFrom, inTurn, 1);
				return hurt;
			}

			switch (shaornot->type())
			{
			case HITCARD:
				ExternData::sgsout << shaornot;
				ExternData::history.push(shaornot);
				ExternData::platform.abandon(static_cast<CardMessage *>(shaornot)->card());
				break;

			case USECARD:
			{
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

		Player * temp = inTurn;
		inTurn = hurtFrom;
		hurtFrom = temp;
	}
}

Message * func::pczhiheng(Message * msg)
{
	ZhiHengMessage * message = static_cast<ZhiHengMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);
	message->from()->setstatus() |= SkillOneUsed;

	for (int i = 0; i < message->cards(); ++i)
	{
		TransCardMessage * trans = new TransCardMessage(PLAYER, DUST, true, message->from(), message->pos(i).first);
		trans->transpush(message->pos(i).second);
		message->from()->popCard(message->pos(i));
		ExternData::platform.analyze(trans);
	}

	bool isOpen = message->from()->seat() == 0;
	TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, message->from());
	for (int i = 0; i < message->cards(); i++)
		trans->transpush(ExternData::platform.get());
	return trans;
}

Message * func::pcfanjian(Message * msg)
{
	FanJianMessage * message = static_cast<FanJianMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);
	message->from()->setstatus() |= SkillOneUsed;

	std::pair<CardColor, int> result = message->to()->input()->respondFanjian(message->from());
	const Card * choose = message->from()->hand(result.second);
	TransCardMessage * trans = new TransCardMessage(PLAYER, PLAYER, true, message->from(), PHAND, message->to());
	trans->transpush(result.second);
	message->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, result.second));
	ExternData::platform.analyze(trans);

	if (choose->color() != result.first)
	{
		message->to()->sethp()--;
		HurtMessage * hurt = new HurtMessage(message->from(), message->to(), 1);
		return hurt;
	}

	return 0;
}

Message * func::pcjieyin(Message * msg)
{
	JieYinMessage * message = static_cast<JieYinMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);
	message->from()->setstatus() |= SkillOneUsed;

	TransCardMessage * trans = new TransCardMessage(PLAYER, DUST, true, message->from(), PHAND);
	for (int i = 0; i < 2; i++)
	{
		trans->transpush(message->pos(i));
		message->from()->popCard(std::pair<PlayerRegionType, int>(PHAND, message->pos(i)));
	}
	ExternData::platform.analyze(trans);

	message->to()->sethp()++;
	HurtMessage * recover = new HurtMessage(message->from(), message->to(), -1);
	ExternData::platform.analyze(recover);

	if (message->from()->hp() < message->from()->maxHP())
	{
		message->from()->sethp()++;
		recover = new HurtMessage(message->from(), message->from(), -1);
		ExternData::platform.analyze(recover);
	}

	return 0;
}

Message * func::pcguose(Message * msg)
{
	GuoSeMessage * message = static_cast<GuoSeMessage *>(msg);
	ExternData::sgsout << message;
	ExternData::history.push(message);

	CardMessage * result = new CardMessage(false, message->from(), message->pos().first, message->pos().second, message->to());
	message->card()->setmask(SISHU);
	message->from()->popCard(message->pos());
	return result;
}
