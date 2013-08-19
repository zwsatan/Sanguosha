#include "sgsui_CardContainer.h"
#include "sgsui_MainWindow.h"
#include "sgsui_CardButton.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_output.h"
#include "sgsui_nameConvert.h"
#include <algorithm>

namespace sgsui {

QSize CardContainer::ms_fixedSize(700, 346);

CardContainer::CardContainer(QWidget *parent)
	: BlockingFrame(parent)
	, m_maxCardSelect(0)
	, m_minCardSelect(0)
	, m_targetPlayer(0)
	, m_handSelectable(0)
	, m_equipSelectable(0)
	, m_judgeSelectable(0)
	, m_skipGuanshi(false)
	, m_onlyChooseHorse(false)
	, m_canceled(0)
{
	setupUi(this);

	setFixedSize(ms_fixedSize);

	okButton->setEnabledPixmap(QPixmap("images/button/ok.png"));
	okButton->setDisabledPixmap(QPixmap("images/button/ok-disabled.png"));
	cancelButton->setEnabledPixmap(QPixmap("images/button/cancel.png"));
	cancelButton->setDisabledPixmap(QPixmap("images/button/cancel-disabled.png"));

	setStyleSheet("QFrame#CardContainer{background-image:url(images/frame/cardContainer.png)}");

	connect(okButton, SIGNAL(clicked()), this, SLOT(unblock()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
	okButton->setDisabled();
}

CardContainer::~CardContainer()
{
	// Normally the cardList is already destroyed by clearContainer()
	// However, in case the game is shutdown unexpectly, this destructor ensures that cards are destroyed
	auto iter = m_cardList.begin();
	while (iter != m_cardList.end())
	{
		delete (*iter);
		++iter;
	}
}

void CardContainer::enterKeyPressedEvent()
{
	okButton->click();
}

CardPosPairVec CardContainer::chooseCard(
		const QString &message,
		sgs::DataType::Player * targetPlayer,
		bool handSelectable,
		bool equipSelectable,
		bool judgeSelectable,
		int selectNum,
		sgs::ConstData::CardType reason)
{
	printDebug("CardContainer::chooseCard (card version): start");
	messageLabel->setText(setColored(message));

	int x = window()->width() / 2 - width() / 2;
	int y = window()->height() / 2 - height() / 2;
	setGeometry(QRect(QPoint(x, y), size()));

	m_targetPlayer = targetPlayer;
	m_handSelectable = handSelectable;
	m_equipSelectable = equipSelectable;
	m_judgeSelectable = judgeSelectable;
	m_maxCardSelect = selectNum;
	m_minCardSelect = selectNum;

	// 贯石斧
	m_skipGuanshi = (reason == sgs::ConstData::GUANSHI);
	// 麒麟弓
	m_onlyChooseHorse = (reason == sgs::ConstData::QILIN);

	constructContainer();
	exec();
	collectData();
	clearContainer(); // destroy cards and lists
	printDebug(selectInterpret(m_returnValue));
	printDebug("CardContainer::chooseCard (card version): over");

	return m_returnValue;
}

CardPosPairVec CardContainer::chooseCard(
		const QString &message,
		sgs::DataType::Player * targetPlayer,
		bool handSelectable,
		bool equipSelectable,
		bool judgeSelectable,
		int selectNum,
		sgs::ConstData::HeroSkill)
{
	printDebug("CardContainer::chooseCard (card version): start");
	messageLabel->setText(setColored(message));

	int x = window()->width() / 2 - width() / 2;
	int y = window()->height() / 2 - height() / 2;
	setGeometry(QRect(QPoint(x, y), size()));

	m_targetPlayer = targetPlayer;
	m_handSelectable = handSelectable;
	m_equipSelectable = equipSelectable;
	m_judgeSelectable = judgeSelectable;
	m_maxCardSelect = selectNum;
	m_minCardSelect = selectNum;

	constructContainer();
	exec();
	collectData();
	clearContainer(); // destroy cards and lists
	printDebug(selectInterpret(m_returnValue));
	printDebug("CardContainer::chooseCard (card version): over");

	return m_returnValue;
}

CardPosPairVec CardContainer::chooseSkillCard(sgs::ConstData::HeroSkill skill)
{
	cancelButton->show();
	QString message;
	using namespace sgs::ConstData;
	switch (skill)
	{
	case LIULI:
		message += trUtf8("选择一张要弃置的牌 ");
		break;

	case ZHIHENG:
		message += trUtf8("请选择要弃置的卡牌 ");
		break;

	case GUOSE:
		message += trUtf8("选择一张?花色的卡牌 ");
		break;

	case WUSHENG:
	case JIJIU:
		message += trUtf8("选择一张红色手牌或装备");
		break;

	case QIXI:
		message += trUtf8("选择一张黑色手牌或装备");
		break;

	case LIJIAN:
		message += trUtf8("选择要弃置的牌");
		break;

	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "CardContainer::chooseSkillCard: skill handled by default");
		break;
	}

	messageLabel->setText(setColored(message));
	int x = window()->width() / 2 - width() / 2;
	int y = window()->height() / 2 - height() / 2;
	setGeometry(QRect(QPoint(x, y), size()));

	m_targetPlayer = MainWindow::mainWindowPtr()->myPlayer();
	m_maxCardSelect = cardNum(skill);
	m_minCardSelect = 1;
	CardButton * newCardButton = 0;
	const sgs::DataType::Card * addingCard = 0;

	// 构造手牌
	printDebug("CardContainer::chooseSkillCard: constructing hand card");
	int shoupaiNum = m_targetPlayer->handnum();
	for (int i = shoupaiNum - 1; i >= 0; --i)
	{
		addingCard = m_targetPlayer->hand(i);
		// 判断此卡是否可以满足技能条件,例如国色需要方块,奇袭需要黑卡等条件
		if (addThisCardInSkill(skill, addingCard))
		{
			// 用此卡构造出另外一张卡,构造出来的卡的mask和type不一样
			newCardButton = new CardButton(addingCard, this, m_targetPlayer);
			m_cardList.push_back(newCardButton);
			cardLayout->addWidget(newCardButton);
		}
	}
	printDebug("CardContainer::chooseSkillCard: hand card constructed");

	// 构造装备卡
	printDebug("CardContainer::chooseSkillCard: constructing equip card");
	addingCard = m_targetPlayer->weapon();
	if (addingCard && addThisCardInSkill(skill, addingCard))
	{
		newCardButton = new CardButton(addingCard, this);
		m_cardList.push_back(newCardButton);
		cardLayout->addWidget(newCardButton);
	}

	addingCard = m_targetPlayer->armor();
	if (addingCard && addThisCardInSkill(skill, addingCard))
	{
		newCardButton = new CardButton(addingCard, this);
		m_cardList.push_back(newCardButton);
		cardLayout->addWidget(newCardButton);
	}

	addingCard = m_targetPlayer->atkhorse();
	if (addingCard && addThisCardInSkill(skill, addingCard))
	{
		newCardButton = new CardButton(addingCard, this);
		m_cardList.push_back(newCardButton);
		cardLayout->addWidget(newCardButton);
	}

	addingCard = m_targetPlayer->dfdhorse();
	if (addingCard && addThisCardInSkill(skill, addingCard))
	{
		newCardButton = new CardButton(addingCard, this);
		m_cardList.push_back(newCardButton);
		cardLayout->addWidget(newCardButton);
	}

	printDebug("CardContainer::chooseSkillCard: equip card constructed");

	exec();

	collectData();

	// destroy cards and lists
	clearContainer();

	printDebug(selectInterpret(m_returnValue));
	printDebug("CardContainer::chooseSkillCard: over");
	return m_returnValue;
}

void CardContainer::cancel()
{
	m_canceled = true;
	unblock();
}

void CardContainer::cardClicked(CardButton* clickedCard) {
	if (clickedCard->isSelected()) {
		clickedCard->setUnselected();
		m_selectedCardList.remove(clickedCard);
	} else {
		clickedCard->setSelected();
		m_selectedCardList.push_back(clickedCard);
		if (m_selectedCardList.size() > m_maxCardSelect) {
			m_selectedCardList.front()->setUnselected();
			m_selectedCardList.pop_front();
		}
	}

	if (m_selectedCardList.size() <= m_maxCardSelect
			&& m_selectedCardList.size() >= m_minCardSelect)
		okButton->setEnabled();
	else
		okButton->setDisabled();
}

void CardContainer::constructContainer()
{
	printDebug("CardContainer::constructContainer: start");
	CardButton * newCardButton = 0;
	const sgs::DataType::Card * addingCard = 0;

	// construct shoupai
	if (m_handSelectable)
	{
		printDebug("CardContainer::constructContainer: constructing hand card");
		int shoupaiNum = m_targetPlayer->handnum();
		for (int i = shoupaiNum - 1; i >= 0; --i)
		{
			addingCard = m_targetPlayer->hand(i);
			bool isHumanTarget = GUIStaticData::getPlayer(GUIStaticData::getHumanIndex()) == m_targetPlayer;
			newCardButton = new CardButton(addingCard, this, isHumanTarget);
			m_cardList.push_back(newCardButton);
			cardLayout->addWidget(newCardButton);
		}
		printDebug("CardContainer::constructContainer: hand card constructed");
	}

	// construct zhuangbei
	if (m_equipSelectable)
	{
		printDebug("CardContainer::constructContainer: constructing equip card");
		addingCard = m_targetPlayer->weapon();
		if (!m_onlyChooseHorse && addingCard && !m_skipGuanshi)
		{
			newCardButton = new CardButton(addingCard, this);
			m_cardList.push_back(newCardButton);
			cardLayout->addWidget(newCardButton);
		}

		addingCard = m_targetPlayer->armor();
		if (!m_onlyChooseHorse && addingCard)
		{
			newCardButton = new CardButton(addingCard, this);
			m_cardList.push_back(newCardButton);
			cardLayout->addWidget(newCardButton);
		}

		addingCard = m_targetPlayer->atkhorse();
		if (addingCard)
		{
			newCardButton = new CardButton(addingCard, this);
			m_cardList.push_back(newCardButton);
			cardLayout->addWidget(newCardButton);
		}

		addingCard = m_targetPlayer->dfdhorse();
		if (addingCard)
		{
			newCardButton = new CardButton(addingCard, this);
			m_cardList.push_back(newCardButton);
			cardLayout->addWidget(newCardButton);
		}
		printDebug("CardContainer::constructContainer: equip card constructed");
	}

	// construct judge
	if (m_judgeSelectable)
	{
		printDebug("CardContainer::constructContainer: constructing judge card");
		int selectablePandingNum = m_targetPlayer->judgenum();
		for (int i = 0; i < selectablePandingNum; ++i)
		{
			addingCard = m_targetPlayer->judge(i);
			newCardButton = new CardButton(addingCard, this);
			m_cardList.push_back(newCardButton);
			cardLayout->addWidget(newCardButton);
		}
		printDebug("CardContainer::constructContainer: judge card constructed");
	}

	printDebug("CardContainer::constructContainer: over");
}

void CardContainer::collectData()
{
	m_returnValue.clear();
	if (m_canceled)
		return;

	std::vector<int> selectedHandVec, selectedJudgeVec;
	int result;
	CardButton * card = 0;
	for (auto iter = m_selectedCardList.begin(); iter != m_selectedCardList.end(); ++iter)
	{
		card = (*iter);
		result = getIndexAtHand(card);
		if (result != -1)
		{
			selectedHandVec.push_back(result);
			continue;
		}

		result = getIndexAtEquip(card);
		if (result != -1)
		{
			selectedJudgeVec.push_back(result);
			continue;
		}

		if (m_targetPlayer->weapon() == card->platformCard())
		{
			m_returnValue.push_back(std::pair<sgs::ConstData::PlayerRegionType, int>(sgs::ConstData::PEQUIP, 0));
			continue;
		}

		if (m_targetPlayer->armor() == card->platformCard())
		{
			m_returnValue.push_back(std::pair<sgs::ConstData::PlayerRegionType, int>(sgs::ConstData::PEQUIP, 1));
			continue;
		}

		if (m_targetPlayer->atkhorse() == card->platformCard())
		{
			m_returnValue.push_back(std::pair<sgs::ConstData::PlayerRegionType, int>(sgs::ConstData::PEQUIP, 2));
			continue;
		}

		if (m_targetPlayer->dfdhorse() == card->platformCard())
		{
			m_returnValue.push_back(std::pair<sgs::ConstData::PlayerRegionType, int>(sgs::ConstData::PEQUIP, 3));
			continue;
		}
	}

	// sort them from big to small (use reverse iterator)
	std::sort(selectedHandVec.rbegin(), selectedHandVec.rend());
	std::sort(selectedJudgeVec.rbegin(), selectedJudgeVec.rend());

	for (auto iter = selectedHandVec.begin(); iter != selectedHandVec.end(); ++iter)
		m_returnValue.push_back(std::pair<sgs::ConstData::PlayerRegionType, int>(sgs::ConstData::PHAND, *iter));

	for (std::vector<int>::iterator iter = selectedJudgeVec.begin(); iter != selectedJudgeVec.end(); ++iter)
		m_returnValue.push_back(std::pair<sgs::ConstData::PlayerRegionType, int>(sgs::ConstData::PJUDGE, *iter));
}

void CardContainer::clearContainer()
{
	for (auto iter = m_cardList.begin(); iter != m_cardList.end(); ++iter)
	{
		cardLayout->removeWidget(*iter);
		delete *iter;
	}

	// DO NOT clean returnValue here, or you can only return an empty vector!
	m_cardList.clear();
	m_selectedCardList.clear();
	m_targetPlayer = 0;
	m_handSelectable = false;
	m_equipSelectable = false;
	m_judgeSelectable = false;
	m_skipGuanshi = false;
	m_onlyChooseHorse = false;
	m_maxCardSelect = 0;
	m_minCardSelect = 0;
	m_canceled = false;
	okButton->setDisabled();
	cancelButton->hide();
}

int CardContainer::getIndexAtHand(CardButton * clickedCard) const
{
	int shoupaiNum = m_targetPlayer->handnum();
	const sgs::DataType::Card * platCard = clickedCard->platformCard();
	for (int i = 0; i < shoupaiNum; ++i)
	{
		if (m_targetPlayer->hand(i) == platCard)
			return i;
	}
	return -1;
}

int CardContainer::getIndexAtEquip(CardButton * clickedCard) const
{
	int judgeNum = m_targetPlayer->judgenum();
	const sgs::DataType::Card * platCard = clickedCard->platformCard();
	for (int i = 0; i < judgeNum; ++i)
	{
		if (m_targetPlayer->judge(i) == platCard)
			return i;
	}
	return -1;
}

bool CardContainer::addThisCardInSkill(sgs::ConstData::HeroSkill skill, const sgs::DataType::Card * card) const
{
	using namespace sgs::ConstData;
	if (!card)
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "CardContainer::addThisCardInSkill: null pointer");
		return false;
	}

	switch (skill)
	{
	case GUOSE:
		return (card->color() == sgs::ConstData::DIAMOND);

	case WUSHENG:
	case JIJIU:
		return (card->color() == sgs::ConstData::HEART || card->color() == sgs::ConstData::DIAMOND);

	case QIXI:
		return (card->color() == sgs::ConstData::SPADE || card->color() == sgs::ConstData::CLUB);

	case LIULI:
	case ZHIHENG:
	case LIJIAN:
		return true;

	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "CardContainer::addThisCardInSkill: skill handled by default");
		return false;
	}
}

unsigned CardContainer::cardNum(sgs::ConstData::HeroSkill skill) const
{
	using namespace sgs::ConstData;
	switch (skill)
	{
	case ZHIHENG:
		return 0xFFFF;
	case LIULI:
	case GUOSE:
	case WUSHENG:
	case JIJIU:
	case QIXI:
	case LIJIAN:
		return 1;
	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "CardContainer::cardNum: skill handled by default");
		return 1;
	}
}

} /* namespace sgsui */
