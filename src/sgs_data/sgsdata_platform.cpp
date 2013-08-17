#include "sgsdata_platform.h"
#include "sgsdata_extern.h"
#include "sgsdata_gamedata.h"
#include "sgsdata_msg.h"
#include "sgsdata_player.h"
#include "sgsdata_card.h"
#include "sgsdata_card_func.h"
#include "sgsdata_history.h"
#include "sgsui_output.h"
#include "sgsui_nameConvert.h"
#include "sgs_Interface.h"

using namespace sgs;
using namespace ConstData;
using namespace DataType;
using namespace Derive;

void Platform::start(int playersCount)
{
	load();
	m_playernum = playersCount;
	m_player = create(playersCount);

	ExternData::gamedata.initialize(this);

	ExternData::sgsout << 1;
	for (int i = 0; i < playersCount; ++i)
	{
		bool isOpen = m_player->seat() == 0;
		TransCardMessage * transMsg = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, m_player);

		for (int j = 0; j < 4; j++)
		{
			const Card * card = get();	// 从牌堆顶部获取卡牌
			transMsg->transpush(card);	// 将卡牌压入转移信息中
		}

		analyze(transMsg);				// 传递转移信息,进行发牌
		m_player = m_player->after();
	}
}

char Platform::run()
{
	bool end = false;
	Message * readMsg = 0;
	while (!end)
	{
		msgInsert(m_player->round());
		while (!msgEmpty())
		{
			readMsg = msgRead();
			msgPop();

			readMsg = analyze(readMsg);
			if (readMsg != 0 && readMsg->type() == FINALE)
			{
				end = true;
				break;
			}

			msgInsert(readMsg);
		}
	}

	FinaleMessage * finaleMsg = static_cast<FinaleMessage *>(readMsg);
	return finaleMsg->finale();
}
void Platform::end()
{
	printDebug("Platform::end: cleaning cards");
	clearcard();
	printDebug("Platform::end: cleaning players");
	clearplayer();
	printDebug("Platform::end: over");
	while (!m_groove.empty())
	{
		delete m_groove.top();
		m_groove.pop();
	}
}
Message * Platform::analyze(Message * msg)
{
	if (msg == 0)
		return 0;

	Message * result = 0;
	switch (msg->type())
	{
	case HITCARD:
	{
		CardMessage * cardMsg = static_cast<CardMessage*>(msg);
		bool jinnang = false;

		switch (cardMsg->card()->mask())
		{
		case SHUNQIAN:
		case GUOCHAI:
		case JUEDOU:
		case JIEDAO:
		case NANMAN:
		case WANJIAN:
		case TAOYUAN:
		case WUGU:
		case WUZHONG:
			jinnang = true;
			break;
		default:
			break;
		}

		if (cardMsg->from()->type() == HUANGYUEYING && jinnang && cardMsg->from()->input()->useSkillOrNot(JIZHI))
		{
			SkillMessage * jizhi = new SkillMessage(cardMsg->from(), JIZHI);
			ExternData::sgsout << jizhi;
			ExternData::history.push(jizhi);

			bool isOpen = cardMsg->from()->seat() == 0;
			TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, cardMsg->from());
			trans->transpush(get());
			analyze(trans);
		}
		result = cardMsg->card()->settle(cardMsg);
		return result;
	}

	case USECARD:
	{
		CardMessage * cardMsg = static_cast<CardMessage*>(msg);
		switch (cardMsg->card()->mask())
		{
		case SISHU:
			result = func::pcsishu(cardMsg);
			break;

		case SHANDIAN:
			result = func::pcshandian(cardMsg);
			break;

		case ZHANGBA:
			printDebug("Platform::analyze: In case USECARD/ZHANGBA");
			result = func::pcsha(static_cast<ZhangBaMessage *>(cardMsg));
			break;

		default:
			result = 0;
		}
		return result;
	}

	case USESKILL:
	{
		SkillMessage * skillMsg = static_cast<SkillMessage *>(msg);
		result = skillMsg->settle(skillMsg);
		return result;
	}

	case SWITCHPHASE:
	{
		SwitchPhaseMessage * switchPhaseMsg = static_cast<SwitchPhaseMessage*>(msg);
		if (switchPhaseMsg->from()->phase() == OTHERPHASE)
			m_player = m_player->after();

		ExternData::history.push(msg);
		ExternData::sgsout << msg;
		return 0;
	}

	case TRANSCARD:
	{
		TransCardMessage * transCardMsg = static_cast<TransCardMessage*>(msg);
		switch (transCardMsg->totype())
		{
		case PLAYER:
		{
			if (transCardMsg->tojudge())
			{
				int n = transCardMsg->cards();
				for (int i = 0; i < n; i++)
					transCardMsg->to()->setjudge().push(transCardMsg->trans(i));
			}
			else
			{
				int n = transCardMsg->cards();
				for (int i = 0; i < n; i++)
					transCardMsg->to()->pushHand(transCardMsg->trans(i));
			}
		}
			break;

		case POOLTOP:
		{
			int n = transCardMsg->cards();
			for (int i = 0; i < n; i++)
				ExternData::platform.pooltop(transCardMsg->trans(n - 1 - i));
		}
			break;

		case POOLBOTTOM:
		{
			int n = transCardMsg->cards();
			for (int i = 0; i < n; i++)
				ExternData::platform.poolbottom(transCardMsg->trans(n - 1 - i));
		}
			break;

		case DUST:
		{
			int n = transCardMsg->cards();
			for (int i = 0; i < n; i++)
				ExternData::platform.abandon(transCardMsg->trans(i));
		}
			break;

		default:
			break;
		}

		ExternData::sgsout << msg;
		ExternData::history.push(msg);

		// 孙尚香
		if (transCardMsg->fromtype() == PLAYER
			&& transCardMsg->from()->type() == SUNSHANGXIANG
			&& !(transCardMsg->from()->status() & DEAD)
			&& transCardMsg->frompos() == PEQUIP
			&& transCardMsg->from()->input()->useSkillOrNot(XIAOJI))
		{
			SkillMessage * xiaoji = new SkillMessage(transCardMsg->from(), XIAOJI);
			ExternData::sgsout << xiaoji;
			ExternData::history.push(xiaoji);

			bool isOpen = transCardMsg->from()->seat() == 0;
			TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, transCardMsg->from());
			for (int i = 0; i < 2; i++)
				trans->transpush(get());
			analyze(trans);
		}
		return 0;
	}

	case HURT:
	{
		printDebug("Platform::analyze: case HURT start");
		HurtMessage * hurtMsg = static_cast<HurtMessage *>(msg);
		ExternData::sgsout << msg;
		ExternData::history.push(msg);
		DyingMessage * dyingMsg = 0;
		while (hurtMsg->to()->hp() <= 0)
		{
			printDebug("Platform::analyze: case HURT ask for Tao (self)");
			dyingMsg = new DyingMessage(hurtMsg->to(), hurtMsg->from(), 1 - hurtMsg->to()->hp());
			ExternData::sgsout << dyingMsg;
			ExternData::history.push(dyingMsg);
			Message * rescue = dyingMsg->from()->TaoOrNot(dyingMsg);
			if (rescue == 0)
				break;

			CardMessage * cardMsg = static_cast<CardMessage *>(rescue);
			Message * res = cardMsg->card()->settle(cardMsg);
			ExternData::sgsout << res;
			ExternData::history.push(res);
		}

		if (hurtMsg->to()->hp() <= 0)
		{
			for (Player * temp = dyingMsg->from()->after(); temp != dyingMsg->from(); temp = temp->after())
			{
				printDebug("Platform::analyze: case HURT in loop");
				if (hurtMsg->to()->hp() > 0)
					break;

				printDebug("Platform::analyze: case HURT ask for Tao (others)");
				Message * rescue = temp->TaoOrNot(dyingMsg);
				while (rescue != 0)
				{
					CardMessage * cardMsg = static_cast<CardMessage *>(rescue);
					Message * res = cardMsg->card()->settle(cardMsg);
					ExternData::sgsout << res;
					ExternData::history.push(res);

					if (hurtMsg->to()->hp() > 0)
						break;

					dyingMsg = new DyingMessage(hurtMsg->to(), hurtMsg->from(), 1 - hurtMsg->to()->hp());
					ExternData::sgsout << dyingMsg;
					ExternData::history.push(dyingMsg);
					rescue = temp->TaoOrNot(dyingMsg);
				}
			}
		}

		printDebug("Platform::analyze: case HURT over");
		if (hurtMsg->to()->hp() <= 0)
		{
			result = new KillMessage(dyingMsg->murderer(), dyingMsg->from());
			return result;
		}

		HeroType heroType = hurtMsg->to()->type();
		bool isFromPlayerDead = hurtMsg->from()->status() & DEAD;
		Interface * ai = hurtMsg->to()->input();
		if (heroType == XIAHOUDUN && hurtMsg->from() != 0 && !isFromPlayerDead && hurtMsg->hurt() > 0 && ai->useSkillOrNot(GANGLIE, hurtMsg->from()))
		{
			GangLieMessage * ganglieMsg = new GangLieMessage(hurtMsg->to(), hurtMsg->from());
			return ganglieMsg;
		}
		else if (heroType == SIMAYI && hurtMsg->from() != 0 && !isFromPlayerDead && hurtMsg->hurt() > 0 && ai->useSkillOrNot(FANKUI, hurtMsg->from()))
		{
			FanKuiMessage * fankuiMsg = new FanKuiMessage(hurtMsg->to(), hurtMsg->from());
			return fankuiMsg;

		}
		else if (heroType == GUOJIA)
		{
			for (int i = 0; i < hurtMsg->hurt(); i++)
			{
				bool isUseYiJi = hurtMsg->to()->input()->useSkillOrNot(YIJI);
				if (!isUseYiJi)
					break;

				YiJiMessage * yijiMsg = dynamic_cast<YiJiMessage *>(hurtMsg->to()->input()->getYiji(
																	 ExternData::platform.get(),
																	 ExternData::platform.get()));
				ExternData::sgsout << yijiMsg;
				ExternData::history.push(yijiMsg);

				for (int i = 0; i < 2; i++)
				{
					bool isOpen = yijiMsg->from()->seat() == 0;
					TransCardMessage * transMsg = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, yijiMsg->trans(i).first);
					transMsg->transpush(yijiMsg->trans(i).second);
					analyze(transMsg);
				}
			}

			return 0;
		}
		else
		{
			return 0;
		}
	}

	case KILL:
	{
		printDebug("Platform::analyze: case KILL start");
		KillMessage * killMsg = static_cast<KillMessage *>(msg);
		ExternData::sgsout << msg;
		ExternData::history.push(msg);
		killMsg->to()->setstatus() |= DEAD;
		if (m_player == killMsg->to())
			m_player = killMsg->to()->after();

		char result = endOrNot();
		if (result != 0)
		{
			FinaleMessage * finaleMsg = new FinaleMessage(result);
			return finaleMsg;
		}

		// 被杀死的人弃掉所有的手牌和装备牌
		TransCardMessage * transMsg = new TransCardMessage(PLAYER, DUST, true, killMsg->to(), PHAND);
		while (killMsg->to()->handnum() > 0)
		{
			transMsg->transpush(killMsg->to()->handnum() - 1);
			killMsg->to()->popCard(std::pair<PlayerRegionType, int>(PHAND, killMsg->to()->handnum() - 1));
		}

		if (transMsg->cards())
			analyze(transMsg);
		else
			delete transMsg;

		TransCardMessage * judge = new TransCardMessage(PLAYER, DUST, true, killMsg->to(), PJUDGE);
		for (int i = 0; i < killMsg->to()->judgenum(); ++i)
			judge->transpush(i);

		killMsg->to()->setjudge().clear();
		if (judge->cards())
			analyze(judge);
		else
			delete judge;

		transMsg = new TransCardMessage(PLAYER, DUST, true, killMsg->to(), PEQUIP);
		bool existEquip = 0;
		if (killMsg->to()->weapon() != 0)
		{
			transMsg->transpush(0);
			killMsg->to()->setweapon(0);
			existEquip = true;
		}

		if (killMsg->to()->armor())
		{
			transMsg->transpush(1);
			killMsg->to()->setarmor(0);
			existEquip = true;
		}

		if (killMsg->to()->atkhorse())
		{
			transMsg->transpush(2);
			killMsg->to()->setatk(0);
			existEquip = true;
		}

		if (killMsg->to()->dfdhorse())
		{
			transMsg->transpush(3);
			killMsg->to()->setdfd(0);
			existEquip = true;
		}

		if (existEquip)
			analyze(transMsg);
		else
			delete transMsg;

		transMsg = 0;

		// 别忘了还要弃装备和判定区的牌 TODO
		bool isFromKillerDead = killMsg->from() && !(killMsg->from()->status() & DEAD);
		if (!isFromKillerDead)
			return 0;

		// 如果主公杀了忠臣，在内部发出消息t，然后返回主公弃牌的消息
		if (killMsg->from() && killMsg->from()->role() == ZHU && killMsg->to()->role() == ZHONG)
		{
			printDebug("Platform::analyze: case KILL: lord discards all hand and equip cards");
			Player * lord = killMsg->from();

			TransCardMessage * lordDiscardAll = new TransCardMessage(PLAYER, DUST, true, lord, PHAND);
			while (lord->handnum() > 0)
			{
				lordDiscardAll->transpush(lord->handnum() - 1);
				lord->popCard(std::pair<PlayerRegionType,int>(PHAND,lord->handnum()-1));
			}

			if (lordDiscardAll->cards())
				analyze(lordDiscardAll);
			else
				delete lordDiscardAll;

			lordDiscardAll = new TransCardMessage(PLAYER, DUST, true, lord, PEQUIP);
			bool lordHasEquip = false;
			if (lord->weapon())
			{
				lordDiscardAll->transpush(0);
				lordHasEquip = true;
				lord->setweapon(0);
			}
			if (lord->armor())
			{
				lordDiscardAll->transpush(1);
				lordHasEquip = true;
				lord->setarmor(0);
			}
			if (lord->atkhorse())
			{
				lordDiscardAll->transpush(2);
				lordHasEquip = true;
				lord->setatk(0);
			}
			if (lord->dfdhorse())
			{
				lordDiscardAll->transpush(3);
				lordHasEquip = true;
				lord->setdfd(0);
			}

			if (lordHasEquip)
				analyze(lordDiscardAll);
			else
				delete lordDiscardAll;

			return 0;
		}

		// 如果杀了反贼，在内部发出消息t，然后返回凶手摸牌的消息
		if (killMsg->from() && killMsg->to()->role() == FAN)
		{
			printDebug("Platform::analyze: case KILL: murderer gets 3 cards");
			Player * murderer = killMsg->from();
			bool isOpen = ((murderer->seat() == 0) ? true : false);
			TransCardMessage * murdererGetCard = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, murderer);
			for (int i = 0; i < 3; i++)
			{
				const Card * temp = get();
				murdererGetCard->transpush(temp);
			}
			return murdererGetCard;
		}
		printDebug("Platform::analyze: case KILL start");
		return transMsg;
	}

	case JUDGE:
	{
		ExternData::history.push(msg);
		ExternData::sgsout << msg;
		JudgeMessage * temp = static_cast<JudgeMessage *>(msg);
		if (temp->from()->type() == GUOJIA && temp->from()->input()->useSkillOrNot(TIANDU))
		{
			SkillMessage * tiandu = new SkillMessage(temp->from(), TIANDU);
			ExternData::sgsout << tiandu;
			ExternData::history.push(tiandu);

			TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER, true, 0, playerRegionTypeNone, temp->from());
			trans->transpush(temp->result());
			analyze(trans);
			return 0;
		}

		abandon(temp->result());
		return 0;
	}

	default:
		delete msg;
		return 0;
	}
}

char Platform::endOrNot()
{
	int zhugongNum(0), zhongchenNum(0), fanzeiNum(0), neijianNum(0);
	char result = 0;
	for (int i = 0; i < m_playernum; i++)
	{
		if (!(m_player->find(i)->status() & DEAD))
		{
			switch (m_player->find(i)->role())
			{
			case ZHU:
				zhugongNum++;
				break;
			case ZHONG:
				zhongchenNum++;
				break;
			case FAN:
				fanzeiNum++;
				break;
			case NEI:
				neijianNum++;
				break;
			default:
				break;
			}
		}
	}

	if (!(zhugongNum == 0 || (fanzeiNum + neijianNum) == 0))
	{
		// 此情况表示游戏未结束
		return 0;
	}

	if (zhugongNum == 0)
	{
		if (zhongchenNum == 0 && fanzeiNum == 0)
		{
			// 内奸胜利情况
			for (int i = 0; i < m_playernum; ++i)
			{
				PlayerRole role = m_player->find(i)->role();
				if (role == NEI)
					result |= 1 << i;
			}

			return result;
		}
		else
		{
			// 反贼胜利
			for (int i = 0; i < m_playernum; i++)
			{
				PlayerRole role = m_player->find(i)->role();
				if (role == FAN)
					result |= 1 << i;
			}

			return result;
		}
	}

	if ((fanzeiNum + neijianNum) == 0)
	{
		// 主公和忠臣胜利
		for (int i = 0; i < m_playernum; ++i)
		{
			PlayerRole role = m_player->find(i)->role();
			if (role == ZHU || role == ZHONG)
				result |= 1 << i;
		}
		return result;
	}

	return result;
}

void Platform::msgInsert(Message * msg)
{
	if (msg != 0)
		m_groove.push(msg);
}

bool Platform::msgEmpty()
{
	return m_groove.empty();
}

Message * Platform::msgRead()
{
	return m_groove.top();
}

void Platform::msgPop()
{
	m_groove.pop();
}

const Card * Platform::getJudge()
{
	//暂时不询问司马懿。
	return get();
}
