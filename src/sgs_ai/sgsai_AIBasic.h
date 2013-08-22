#ifndef SGSAI_AIBASIC_H_
#define SGSAI_AIBASIC_H_

#include "sgs_Interface.h"
#include <QObject>

typedef std::pair<const sgs::DataType::Card *, int>			CardPair;
typedef std::vector<CardPair>								CardVec;
typedef std::vector<sgs::DataType::Player *>				PlayerVec;

namespace sgsai {

class AIBasic : public QObject, public sgs::Interface
{
public:
	explicit AIBasic(int seat);
	virtual std::vector<int> discardCard(unsigned int cardNumToDiscard);
	virtual sgs::DataType::Message * useCardResponse();
	virtual sgs::DataType::Message * taoOrNot(sgs::DataType::Player * from, int /*neededTaoNumber*/);
	virtual sgs::DataType::Message * shanOrNot(sgs::DataType::Player * /*from*/, const sgs::DataType::Card * /*card*/);
	virtual sgs::DataType::Message * shanOrNot(sgs::DataType::Player * from, sgs::ConstData::HeroSkill skillType);
	virtual sgs::DataType::Message * shaOrNot(sgs::DataType::Player * /*from*/, const sgs::DataType::Card * /*card*/);
	virtual sgs::DataType::Message * shaOrNot(sgs::DataType::Player * from, sgs::ConstData::HeroSkill skillType);
	virtual bool useSkillOrNot(sgs::ConstData::HeroSkill skillType, sgs::DataType::Message * message);
	virtual bool useSkillOrNot(sgs::ConstData::HeroSkill skillType, sgs::DataType::Player * player) = 0;

protected:
	typedef std::pair<sgs::DataType::Player *, sgs::DataType::Player *> JieDaoPair;

protected:
	// these functions are called by useCardResponse()
	virtual sgs::Derive::CardMessage * installEquip();
	virtual sgs::Derive::CardMessage * useTao();
	virtual sgs::Derive::CardMessage * useSha();
	virtual sgs::Derive::CardMessage * useJinnang();

	virtual sgs::Derive::CardMessage * useZhangba();

	// this method does not care whether the argument player is dead
	virtual bool isMyFriend(sgs::DataType::Player * player) const;
	// when lord is dead, this method returns 0
	sgs::DataType::Player * lord();
	const PlayerVec& shaTarget();
	// sorted
	const PlayerVec& sishuTarget();
	// sorted
	const PlayerVec& guochaiTarget();
	// sorted
	const PlayerVec& juedouTarget();
	// sorted
	PlayerVec shunqianTarget();
	PlayerVec playersHavingWeapon();
	PlayerVec jiedaoTarget(sgs::DataType::Player *weaponOwner);
	bool isAllFanzeiDead() const;
	// the first is the attacker and the second is the victim
	JieDaoPair makeJiedaoPair();

private:
	sgs::DataType::Player *		m_lord;

	PlayerVec					m_shaTargetVec;
	PlayerVec					m_sishuTargetVec;
	PlayerVec					m_guochaiTargetVec;
	PlayerVec					m_juedouTargetVec;

	const static JieDaoPair		ms_invalidJiedaoPair;
};

} /* namespace sgsai */
#endif /* SGSAI_AIBASIC_H_ */
