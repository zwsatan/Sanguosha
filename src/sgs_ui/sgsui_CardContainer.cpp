#include "sgsui_CardContainer.h"
#include "sgsui_MainWindow.h"
#include "sgsui_CardButton.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_output.h"
#include "sgsui_nameConvert.h"
#include <algorithm>

namespace sgsui {

QSize CardContainer::ms_fixedSize(700, 346);

CardContainer::CardContainer(QWidget * parent/* = 0*/)
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
	// 正常情况下,由clearContainer销毁
	// 然后,防止在游戏意外结束,确保析构释放资源
	for (auto iter = m_cardList.begin(); iter != m_cardList.end(); ++iter)
		delete (*iter);
}

void CardContainer::enterKeyPressedEvent()
{
	okButton->click();
}

CardPosPairVec CardContainer::chooseCard(const QString &message,
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

	// 是否为贯石斧
	m_skipGuanshi = (reason == sgs::ConstData::GUANSHI);
	// 是否为麒麟弓
	m_onlyChooseHorse = (reason == sgs::ConstData::QILIN);

	constructContainer();
	exec();
	collectData();
	clearContainer();

	printDebug(selectInterpret(m_returnValue));
	printDebug("CardContainer::chooseCard (card version): over");

	return m_returnValue;
}

CardPosPairVec CardContainer::chooseCard(const QString &message,
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
	clearContainer();

	printDebug(selectInterpret(m_returnValue));
	printDebug("CardContainer::chooseCard (card version): over");

	return m_returnValue;
}

// 依据所选技能返回响应字符串
QString CardContainer::getMessageBySkill(sgs::ConstData::HeroSkill skill)
{
	using namespace sgs::ConstData;

	switch (skill)
	{
	case LIULI:
		return trUtf8("选择一张要弃置的牌 ");

	case ZHIHENG:
		return trUtf8("请选择要弃置的卡牌 ");

	case GUOSE:
		return trUtf8("选择一张♢花色的卡牌 ");

	case WUSHENG:
	case JIJIU:
		return trUtf8("选择一张红色手牌或装备");

	case QIXI:
		return trUtf8("选择一张黑色手牌或装备");

	case LIJIAN:
		return trUtf8("选择要弃置的牌");
		break;

	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "CardContainer::chooseSkillCard: skill handled by default");
		return "";
	}
}

CardPosPairVec CardContainer::chooseSkillCard(sgs::ConstData::HeroSkill skill)
{
	cancelButton->show();

	QString message = getMessageBySkill(skill);
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
			// 用此卡构造出CardButton可以用于显示
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

// 实现cardClicked方法,当卡牌被点击时
// 依据卡牌点击状态,设置卡牌进入或移除m_selectedCardList
void CardContainer::cardClicked(CardButton * clickedCard)
{
	if (clickedCard->isSelected())
	{
		// 若卡牌原先为已被点击状态,则重置,并移除m_selectedCardList
		clickedCard->setUnselected();
		m_selectedCardList.remove(clickedCard);
	}
	else
	{
		// 设置卡牌为被点击状态
		// 移入m_selectedCardList
		// 若已超过点击上限,则将已选卡牌的第一张弹出
		clickedCard->setSelected();
		m_selectedCardList.push_back(clickedCard);
		if (m_selectedCardList.size() > m_maxCardSelect)
		{
			m_selectedCardList.front()->setUnselected();
			m_selectedCardList.pop_front();
		}
	}

	int selectedSize = m_selectedCardList.size();
	if (m_minCardSelect <= selectedSize && selectedSize <= m_maxCardSelect)
		okButton->setEnabled();
	else
		okButton->setDisabled();
}

// 调用Helper函数构造卡牌容器
void CardContainer::constructContainer()
{
	printDebug("CardContainer::constructContainer: start");

	// 构造手牌
	constructHandHelper();
	// 构造装备卡
	constructEquipHelper();
	// 构造判定卡
	constructJudgeHelper();

	printDebug("CardContainer::constructContainer: over");
}

// 辅助构造手牌CardButton
void CardContainer::constructHandHelper()
{
	if (m_handSelectable)
	{
		printDebug("CardContainer::constructContainer: constructing hand card");
		int shoupaiNum = m_targetPlayer->handnum();
		for (int i = shoupaiNum - 1; i >= 0; --i)
		{
			// 视情况构造CardButton,若为目标为玩家,则构造可见手牌,否则构造不可见卡牌
			const sgs::DataType::Card * addingCard = m_targetPlayer->hand(i);
			bool isHumanTarget = GUIStaticData::getPlayer(GUIStaticData::getHumanIndex()) == m_targetPlayer;
			CardButton * newCardButton = new CardButton(addingCard, this, isHumanTarget);
			m_cardList.push_back(newCardButton);
			cardLayout->addWidget(newCardButton);
		}
		printDebug("CardContainer::constructContainer: hand card constructed");
	}
}

// 辅助构造装备卡牌CardButton
void CardContainer::constructEquipHelper()
{
	if (m_equipSelectable)
	{
		CardButton * newCardButton = 0;
		const sgs::DataType::Card * addingCard = 0;

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
}

// 辅助构造判定卡牌CardButton
void CardContainer::constructJudgeHelper()
{
	if (m_judgeSelectable)
	{
		printDebug("CardContainer::constructContainer: constructing judge card");
		int selectablePandingNum = m_targetPlayer->judgenum();
		for (int i = 0; i < selectablePandingNum; ++i)
		{
			const sgs::DataType::Card * addingCard = m_targetPlayer->judge(i);
			CardButton *newCardButton = new CardButton(addingCard, this);
			m_cardList.push_back(newCardButton);
			cardLayout->addWidget(newCardButton);
		}
		printDebug("CardContainer::constructContainer: judge card constructed");
	}
}


// 遍历所有已选卡牌
// 将其构建成 PlayerRegionType - index 方式,存入 m_returnValue 中,用于结算试用
// 如 武器就是 PEQUIP - 0
// 如 手牌就是 PHAND - 1
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

		result = getIndexAtJudge(card);
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

	for (auto iter = selectedJudgeVec.begin(); iter != selectedJudgeVec.end(); ++iter)
		m_returnValue.push_back(std::pair<sgs::ConstData::PlayerRegionType, int>(sgs::ConstData::PJUDGE, *iter));
}

// 清理卡牌容器
void CardContainer::clearContainer()
{
	for (auto iter = m_cardList.begin(); iter != m_cardList.end(); ++iter)
	{
		cardLayout->removeWidget(*iter);
		delete *iter;
	}
	m_cardList.clear();
	m_selectedCardList.clear();

	m_maxCardSelect = 0;
	m_minCardSelect = 0;
	m_targetPlayer = NULL;
	m_handSelectable = false;
	m_equipSelectable = false;
	m_judgeSelectable = false;
	m_skipGuanshi = false;
	m_onlyChooseHorse = false;
	m_canceled = false;

	okButton->setDisabled();
	cancelButton->hide();

	// m_returnValue 不必要clear,除非只想要得到一个空的vector
}

// 获取该卡牌在手牌中的index
int CardContainer::getIndexAtHand(CardButton * clickedCard) const
{
	int shoupaiNum = m_targetPlayer->handnum();
	const sgs::DataType::Card * platCard = clickedCard->platformCard();
	for (int index = 0; index < shoupaiNum; ++index)
	{
		if (m_targetPlayer->hand(index) == platCard)
			return index;
	}
	return -1;
}

// 获取该卡牌位于判定区的index,不过判定区和装备区在一起
int CardContainer::getIndexAtJudge(CardButton * clickedCard) const
{
	int judgeNum = m_targetPlayer->judgenum();
	const sgs::DataType::Card * platCard = clickedCard->platformCard();
	for (int index = 0; index < judgeNum; ++index)
	{
		if (m_targetPlayer->judge(index) == platCard)
			return index;
	}
	return -1;
}

// 判断该卡牌是否满则该技能对其的要求,这里主要就是花色要求
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

// 获取该技能下,能使用最大的卡牌数目
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
