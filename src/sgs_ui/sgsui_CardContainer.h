#ifndef SGSUI_CARDCONTAINER_H_
#define SGSUI_CARDCONTAINER_H_

#include "ui_CardContainer.h"
#include "sgsui_BlockingFrame.h"
#include "sgsui_AbstractCardContainer.h"
#include "sgsdata_enum.h"
#include "sgsdata_player.h"
#include "sgsdata_card.h"
#include <list>

namespace sgsui {

class CardContainer
		: public BlockingFrame
		, protected Ui::CardContainer
		, protected AbstractCardContainer
{
	Q_OBJECT


public:
	explicit CardContainer(QWidget * parent = 0);
	virtual ~CardContainer();

	void enterKeyPressedEvent();

	CardPosPairVec chooseCard(const QString &message,
							  sgs::DataType::Player * targetPlayer,
							  bool handSelectable,
							  bool equipSelectable,
							  bool judgeSelectable,
							  int selectNum,
							  sgs::ConstData::CardType reason);

	CardPosPairVec chooseCard(const QString &message,
							  sgs::DataType::Player * targetPlayer,
							  bool handSelectable,
							  bool equipSelectable,
							  bool judgeSelectable,
							  int selectNum,
							  sgs::ConstData::HeroSkill reason);

	CardPosPairVec chooseSkillCard(sgs::ConstData::HeroSkill skill);

private slots:
	void cancel();

private:
	virtual void cardClicked(CardButton * clickedCard);
	void constructContainer();		// 调用Helper函数构造卡牌容器
	void constructHandHelper();		// 辅助构造手牌CardButton
	void constructEquipHelper();	// 辅助构造装备卡牌CardButton
	void constructJudgeHelper();	// 辅助构造判定卡牌CardButton

	void collectData();
	void clearContainer();

	int getIndexAtHand(CardButton * clickedCard) const;
	int getIndexAtJudge(CardButton * clickedCard) const;

	QString getMessageBySkill(sgs::ConstData::HeroSkill skill);
	bool addThisCardInSkill(sgs::ConstData::HeroSkill skill, const sgs::DataType::Card * card) const;
	unsigned cardNum(sgs::ConstData::HeroSkill skill) const;


private:
	unsigned						m_maxCardSelect;
	unsigned						m_minCardSelect;
	std::list<CardButton *>			m_selectedCardList;
	std::list<CardButton *>			m_cardList;

	sgs::DataType::Player *			m_targetPlayer;
	bool							m_handSelectable;
	bool							m_equipSelectable;
	bool							m_judgeSelectable;
	CardPosPairVec					m_returnValue;
	static QSize					ms_fixedSize;

	bool							m_skipGuanshi;
	bool							m_onlyChooseHorse;
	bool							m_canceled;
};

} /* namespace sgsui */
#endif /* SGSUI_CARDCONTAINER_H_ */
