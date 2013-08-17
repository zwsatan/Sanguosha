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
		TransCardMessage * m = 0;
		if (m_player->seat() == 0)
			m = new TransCardMessage(POOLTOP, PLAYER, true, 0, playerRegionTypeNone, m_player);
		else
			m = new TransCardMessage(POOLTOP, PLAYER, false, 0, playerRegionTypeNone, m_player);

		for (int j = 0; j < 4; j++)
		{
			const Card * c = get();
			m->transpush(c);
		}

		analyze(m);
		m_player = m_player->after();
	}
}

char Platform::run()
{
	bool end = false;
	Message * r = 0;
	while (!end)
	{
		msgInsert(m_player->round(*this));
		while (!msgEmpty())
		{
			r = msgRead();
			msgPop();
			r = analyze(r);
			if (r != 0 && r->type() == FINALE)
			{
				end = true;
				break;
			}

			msgInsert(r);
		}
	}
	FinaleMessage * f = static_cast<FinaleMessage *>(r);
	return f->finale();
}
void Platform::end() {
	printDebug("Platform::end: cleaning cards");
	clearcard();
	printDebug("Platform::end: cleaning players");
	clearplayer();
	printDebug("Platform::end: over");
	while (!m_groove.empty()) {
		delete m_groove.top();
		m_groove.pop();
	}
}
Message * Platform::analyze(Message * m)
{
	if (m == 0)
		return 0;

	Message * result = 0;
	switch (m->type())
	{
	case HITCARD:
	{
		CardMessage * message = static_cast<CardMessage*>(m);
		bool jinnang = false;

		switch (message->card()->mask())
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

		if (message->from()->type() == HUANGYUEYING && jinnang && message->from()->input()->useSkillOrNot(JIZHI))
		{
			SkillMessage * jizhi = new SkillMessage(message->from(), JIZHI);
			ExternData::sgsout << jizhi;
			ExternData::history.push(jizhi);

			bool isOpen = message->from()->seat() == 0;
			TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, message->from());
			trans->transpush(get());
			analyze(trans);
		}
		result = message->card()->settle(message);
		return result;
	}

	case USECARD:
	{
		CardMessage* message = static_cast<CardMessage*>(m);
		switch (message->card()->mask())
		{
		case SISHU:
			result = func::pcsishu(message);
			break;

		case SHANDIAN:
			result = func::pcshandian(message);
			break;

		case ZHANGBA:
			printDebug("Platform::analyze: In case USECARD/ZHANGBA");
			result = func::pcsha(static_cast<ZhangBaMessage *>(message));
			break;

		default:
			result = 0;
		}
		return result;
	}

	case USESKILL:
	{
		SkillMessage * message = static_cast<SkillMessage *>(m);
		result = message->settle(message);
		return result;
	}

	case SWITCHPHASE:
	{
		SwitchPhaseMessage * message = static_cast<SwitchPhaseMessage*>(m);
		if (message->from()->phase() == OTHERPHASE)
			m_player = m_player->after();

		ExternData::history.push(m);
		ExternData::sgsout << m;
		return 0;
	}

	case TRANSCARD:
	{
		TransCardMessage* message = static_cast<TransCardMessage*>(m);
		switch (message->totype())
		{
		case PLAYER:
		{
			if (message->tojudge())
			{
				int n = message->cards();
				for (int i = 0; i < n; i++)
					message->to()->setjudge().push(message->trans(i));
			}
			else
			{
				int n = message->cards();
				for (int i = 0; i < n; i++)
					message->to()->pushHand(message->trans(i));
			}
		}
			break;

		case POOLTOP:
		{
			int n = message->cards();
			for (int i = 0; i < n; i++)
				ExternData::platform.pooltop(message->trans(n - 1 - i));
		}
			break;

		case POOLBOTTOM:
		{
			int n = message->cards();
			for (int i = 0; i < n; i++)
				ExternData::platform.poolbottom(message->trans(n - 1 - i));
		}
			break;

		case DUST:
		{
			int n = message->cards();
			for (int i = 0; i < n; i++)
				ExternData::platform.abandon(message->trans(i));
		}
			break;

		default:
			break;
		}

		ExternData::sgsout << m;
		ExternData::history.push(m);

		// 孙尚香
		if (message->fromtype() == PLAYER
			&& message->from()->type() == SUNSHANGXIANG
			&& !(message->from()->status() & DEAD)
			&& message->frompos() == PEQUIP
			&& message->from()->input()->useSkillOrNot(XIAOJI))
		{
			SkillMessage * xiaoji = new SkillMessage(message->from(), XIAOJI);
			ExternData::sgsout << xiaoji;
			ExternData::history.push(xiaoji);

			bool isOpen = message->from()->seat() == 0;
			TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER, isOpen, 0, playerRegionTypeNone, message->from());
			for (int i = 0; i < 2; i++)
				trans->transpush(get());
			analyze(trans);
		}
		return 0;
	}

	case HURT:
	{
		printDebug("Platform::analyze: case HURT start");
		HurtMessage * h = static_cast<HurtMessage *>(m);
		ExternData::sgsout << m;
		ExternData::history.push(m);
		DyingMessage * d = 0;
		while (h->to()->hp() <= 0)
		{
			printDebug("Platform::analyze: case HURT ask for Tao (self)");
			d = new DyingMessage(h->to(), h->from(), 1 - h->to()->hp());
			ExternData::sgsout << d;
			ExternData::history.push(d);
			Message * rescue = d->from()->TaoOrNot(d);
			if (rescue == 0)
				break;

			CardMessage * c = static_cast<CardMessage *>(rescue);
			Message * res = c->card()->settle(c);
			ExternData::sgsout << res;
			ExternData::history.push(res);
		}

		if (h->to()->hp() <= 0)
		{
			for (Player * temp = d->from()->after(); temp != d->from(); temp = temp->after())
			{
				printDebug("Platform::analyze: case HURT in loop");
				if (h->to()->hp() > 0)
					break;

				printDebug("Platform::analyze: case HURT ask for Tao (others)");
				Message * rescue = temp->TaoOrNot(d);
				while (rescue != 0)
				{
					CardMessage * c = static_cast<CardMessage *>(rescue);
					Message * res = c->card()->settle(c);
					ExternData::sgsout << res;
					ExternData::history.push(res);

					if (h->to()->hp() > 0)
						break;

					d = new DyingMessage(h->to(), h->from(), 1 - h->to()->hp());
					ExternData::sgsout << d;
					ExternData::history.push(d);
					rescue = temp->TaoOrNot(d);
				}
			}
		}

		printDebug("Platform::analyze: case HURT over");
		if (h->to()->hp() <= 0)
		{
			result = new KillMessage(d->murderer(), d->from());
			return result;
		}
		else
		{
			if (h->to()->type() == XIAHOUDUN && h->from() != 0
					&& !(h->from()->status() & DEAD) && h->hurt() > 0
					&& h->to()->input()->useSkillOrNot(GANGLIE, h->from()))
			{
				GangLieMessage * m = new GangLieMessage(h->to(), h->from());
				return m;
			} else if (h->to()->type() == SIMAYI && h->from() != 0
					&& !(h->from()->status() & DEAD) && h->hurt() > 0
					&& h->to()->input()->useSkillOrNot(FANKUI, h->from()))
			{
				FanKuiMessage * m = new FanKuiMessage(h->to(), h->from());
				return m;

			} else if (h->to()->type() == GUOJIA)
			{
				for (int i = 0; i < h->hurt(); i++)
				{
					if (h->to()->input()->useSkillOrNot(YIJI))
					{
						YiJiMessage * yiji =
								dynamic_cast<YiJiMessage *>(h->to()->input()->getYiji(
										ExternData::platform.get(),
										ExternData::platform.get()));
						ExternData::sgsout << yiji;
						ExternData::history.push(yiji);

						for (int i = 0; i < 2; i++)
						{
							TransCardMessage * trans = new TransCardMessage(POOLTOP, PLAYER,
									yiji->from()->seat() == 0, 0, playerRegionTypeNone,
									yiji->trans(i).first);
							trans->transpush(yiji->trans(i).second);
							ExternData::platform.analyze(trans);
						}

					} else
						break;
				}
				return 0;
			} else
				return 0;
		}
	}

	case KILL: {
		printDebug("Platform::analyze: case KILL start");
		KillMessage * k = static_cast<KillMessage *>(m);
		ExternData::sgsout << m;
		ExternData::history.push(m);
		k->to()->setstatus() |= DEAD;
		if (m_player == k->to())
			m_player = k->to()->after();

		/*these code has been revised by Hu Ronghang, May 12*/
		char result = endOrNot();
		if (result != 0) {
			FinaleMessage * f = new FinaleMessage(result);
			return f;
		}

		// 被杀死的人弃掉所有的手牌和装备牌
		TransCardMessage * t = new TransCardMessage(PLAYER, DUST, true, k->to(), PHAND);
		while (k->to()->handnum()>0) {
			t->transpush(k->to()->handnum() - 1);
			k->to()->popCard(std::pair<PlayerRegionType,int>(PHAND,k->to()->handnum()-1));
		}
		if (t->cards()) {
			analyze(t);
		} else {
			delete t;
		}

		TransCardMessage * judge = new TransCardMessage(PLAYER, DUST, true, k->to(),
				PJUDGE);
		for (int i = 0; i < k->to()->judgenum(); ++i) {
			judge->transpush(i);
		}
		k->to()->setjudge().clear();
		if (judge->cards()) {
			analyze(judge);
		} else {
			delete judge;
		}

		t = new TransCardMessage(PLAYER, DUST, true, k->to(), PEQUIP);
		bool existEquip = 0;
		if (k->to()->weapon() != 0) {
			t->transpush(0);
			k->to()->setweapon(0);
			existEquip = true;
		}
		if (k->to()->armor()) {
			t->transpush(1);
			k->to()->setarmor(0);
			existEquip = true;
		}
		if (k->to()->atkhorse()) {
			t->transpush(2);
			k->to()->setatk(0);
			existEquip = true;
		}
		if (k->to()->dfdhorse()) {
			t->transpush(3);
			k->to()->setdfd(0);
			existEquip = true;
		}

		if (existEquip) {
			analyze(t);
		} else {
			delete t;
		}
		t = 0;

		// 别忘了还要弃装备和判定区的牌 TODO

		if (k->from() && !(k->from()->status() & DEAD)) {

			// 如果主公杀了忠臣，在内部发出消息t，然后返回主公弃牌的消息
			if (k->from() && k->from()->role() == ZHU
					&& k->to()->role() == ZHONG) {
				//			ExternData::sgsout << t;
				//			ExternData::history.push(t);

				printDebug(
						"Platform::analyze: case KILL: lord discards all hand and equip cards");
				Player * lord = k->from();

				TransCardMessage * lordDiscardAll = new TransCardMessage(PLAYER, DUST, true,
						lord, PHAND);
				while (lord->handnum() > 0) {
					lordDiscardAll->transpush(lord->handnum() - 1);
					lord->popCard(std::pair<PlayerRegionType,int>(PHAND,lord->handnum()-1));
				}
				if (lordDiscardAll->cards())
					analyze(lordDiscardAll);
				else
					delete lordDiscardAll;
				lordDiscardAll = new TransCardMessage(PLAYER, DUST, true, lord,
						PEQUIP);
				bool lordHasEquip = false;
				if (lord->weapon()) {
					lordDiscardAll->transpush(0);
					lordHasEquip = true;
					lord->setweapon(0);
				}
				if (lord->armor()) {
					lordDiscardAll->transpush(1);
					lordHasEquip = true;
					lord->setarmor(0);
				}
				if (lord->atkhorse()) {
					lordDiscardAll->transpush(2);
					lordHasEquip = true;
					lord->setatk(0);
				}
				if (lord->dfdhorse()) {
					lordDiscardAll->transpush(3);
					lordHasEquip = true;
					lord->setdfd(0);
				}
				if (lordHasEquip) {
					analyze(lordDiscardAll);
				} else {
					delete lordDiscardAll;
				}
				return 0;
			}

			// 如果杀了反贼，在内部发出消息t，然后返回凶手摸牌的消息
			if (k->from() && k->to()->role() == FAN) {
				printDebug(
						"Platform::analyze: case KILL: murderer gets 3 cards");
				Player * murderer = k->from();
				TransCardMessage * murdererGetCard = new TransCardMessage(POOLTOP, PLAYER,
						((murderer->seat() == 0) ? true : false), 0, playerRegionTypeNone,
						murderer);
				for (int i = 0; i < 3; i++) {
					const Card * temp = get();
					murdererGetCard->transpush(temp);
				}
				return murdererGetCard;
			}
			printDebug("Platform::analyze: case KILL start");
			return t;
		} else {
			return 0;
		}
		break;
	}
	case JUDGE: {
		ExternData::history.push(m);
		ExternData::sgsout << m;
		JudgeMessage * temp = static_cast<JudgeMessage *>(m);
		if (temp->from()->type() == GUOJIA
				&& temp->from()->input()->useSkillOrNot(TIANDU)) {
			SkillMessage * tiandu = new SkillMessage(temp->from(), TIANDU);
			ExternData::sgsout << tiandu;
			ExternData::history.push(tiandu);

			TransCardMessage* trans = new TransCardMessage(POOLTOP, PLAYER, true, 0,
					playerRegionTypeNone, temp->from());
			trans->transpush(temp->result());
			analyze(trans);
			return 0;
		} else {
			abandon(temp->result());
			return 0;
		}
	}

	default:
		delete m;
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
