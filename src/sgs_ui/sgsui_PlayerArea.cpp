#include "sgsui_PlayerArea.h"
#include "sgsui_MainWindow.h"
#include "sgsui_CardFrame.h"
#include "sgsui_SkillLabel.h"
#include "sgsui_ZhuangbeiLabel.h"
#include "sgsui_nameConvert.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_output.h"
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>

namespace sgsui {

const QSize PlayerArea::ms_fixedSize(144, 199);
const QPoint PlayerArea::ms_skillPosition(15, 60);
const QPoint PlayerArea::ms_zhuangbeiPosition(0, 40);
const QPoint PlayerArea::ms_playerFrameMargin(6, 6);
const QPoint PlayerArea::ms_playerPixmapMargin(11, 23);
const QPoint PlayerArea::ms_countryFrameMargin(9, 21);
const QPoint PlayerArea::ms_countrySmallMargin(2, 2);
const QPoint PlayerArea::ms_rolePixmapMargin(91, 23);
const QPoint PlayerArea::ms_shoupaiPixmapMargin(113, 75);

const QPoint PlayerArea::ms_wuqiColorMargin(1, 118);
const QPoint PlayerArea::ms_wuqiPixmapMargin(20, 124);
const QPoint PlayerArea::ms_fangjuColorMargin(1, 135);
const QPoint PlayerArea::ms_fangjuPixmapMargin(20, 141);
const QPoint PlayerArea::ms_jianmaColorMargin(1, 152);
const QPoint PlayerArea::ms_jianmaPixmapMargin(20, 158);
const QPoint PlayerArea::ms_jiamaColorMargin(1, 169);
const QPoint PlayerArea::ms_jiamaPixmapMargin(20, 175);
const QPoint PlayerArea::ms_chainPixmapMargin(0, 20);
const QPoint PlayerArea::ms_phasePixmapMargin(121, 126);
const QPoint PlayerArea::ms_dyingPixmapMargin(20, 80);
const QPoint PlayerArea::ms_deadPixmapMargin(-10, 40);
const QPoint PlayerArea::ms_statusPixmapMargin(30, 0);
const QPoint PlayerArea::ms_recoverLabelMargin(0, 50);
const int PlayerArea::ms_horizontalHPMargin = 11;
const int PlayerArea::ms_verticalHPMargin = 77;
const int PlayerArea::ms_maxHorizontalHPDifference = 14;
const int PlayerArea::ms_maxHPWidth = 100;

const QRect PlayerArea::ms_playerFrameRect(6, 6, 133, 185);
const QColor PlayerArea::ms_selectedColor(255, 0, 0, 100);
const QColor PlayerArea::ms_unselectableColor(0, 0, 0, 100);

PlayerArea::PlayerArea(int thisPlayerIndex, QWidget * parent)
	: QFrame(parent)
	, m_playerIndex(thisPlayerIndex)
	, m_role(GUIStaticData::getPlayerRole(m_playerIndex))
	, m_heroType(GUIStaticData::getPlayerType(m_playerIndex))
	, m_sex(GUIStaticData::getPlayerSex(m_playerIndex))
	, m_side(GUIStaticData::getPlayerSide(m_playerIndex))
	, m_framePixmap("images/frame/playerArea.png")
	, m_playerPixmap(wujiangOtherPixmapStr(m_heroType))						// 对应查找英雄图片
	, m_countryFramePixmap(countryFramePixmapStr(m_side))					// 对应国家的将卡的边框
	, m_countrySmallPixmap(countrySmallPixmapStr(m_side))					// 对应国家的字的图片,如群,魏蜀吴等
	, m_rolePixmap(rolePixmapStr(m_role, GUIStaticData::isDebugLogOn()))	// 身份图片
	, m_deadRolePixmap(deadRolePixmapStr(m_role))							// 身份死亡时图片
	, m_dyingPixmap("images/animation/save-me.png")							// 濒死时救我图片
	, m_shoupaiPixmap("images/system/handcard.png")							// 手牌图片
	, m_chainPixmap("images/status/chain.png")								// 铁锁连舟图片缺失
	, m_lebuPixmap("images/status/indulgence.png")							// 乐不思蜀图片
	, m_shandianPixmap("images/status/lightning.png")						// 闪电图片
	, m_bingliangPixmap("images/status/supply_shortage.png")				// 兵粮寸断图片确实
	, m_HP(GUIStaticData::getPlayerHP(m_playerIndex))
	, m_maxHP(m_HP)
	, m_shoupaiNumber(0)
	, m_isDead(false)
	, m_isDying(false)
	, m_isResponding(false)
	, m_isChained(false)
	, m_phase(sgs::ConstData::OTHERPHASE)
	, m_playerSelected(false)
	, m_playerSelectable(true)
	, m_wuqiCard(0)
	, m_fangjuCard(0)
	, m_jianmaCard(0)
	, m_jiamaCard(0)
{
	setupUi(this);
	setFixedSize(ms_fixedSize);

	m_skillLabel = new SkillLabel(this);
	m_skillLabel->move(ms_skillPosition);
	m_skillLabel->hide();

	m_zhuangbeiLabel = new ZhuangbeiLabel(this);
	m_zhuangbeiLabel->move(ms_zhuangbeiPosition);
	m_zhuangbeiLabel->hide();

	m_recoverLabel = new QLabel(this);
	m_recoverLabel->move(ms_recoverLabelMargin);
	m_recoverLabel->hide();
	m_recoverLabel->setPixmap(QPixmap("images/HP/recover.png"));
	m_recoverLabel->setFixedSize(m_recoverLabel->sizeHint());

	m_recoverTimer = new QTimer(this);
	m_recoverTimer->setSingleShot(true);
	connect(m_recoverTimer, SIGNAL(timeout()), m_recoverLabel, SLOT(hide()));

	shoupaiNumLabel->setFont(GUIStaticData::statusbarFont);
	shoupaiNumLabel->hide();
}

QPoint PlayerArea::topLeftPoint() const
{
	QPoint topLeftCoordinate = pos();
	QWidget *thisWidget = parentWidget();
	while (thisWidget != window())
	{
		topLeftCoordinate += thisWidget->pos();
		thisWidget = thisWidget->parentWidget();
	}
	return topLeftCoordinate;
}

QRect PlayerArea::realGeometry() const
{
	return QRect(topLeftPoint(), size());
}

int PlayerArea::playerIndex() const
{
	return m_playerIndex;
}

sgs::ConstData::HeroSex PlayerArea::getSex() const
{
	return m_sex;
}

sgs::ConstData::HeroType PlayerArea::getHeroType() const
{
	return m_heroType;
}

sgs::ConstData::PlayerRole PlayerArea::getRole() const
{
	return m_role;
}

int PlayerArea::getHP() const
{
	return m_HP;
}

int PlayerArea::getPhase() const
{
	return m_phase;
}

int PlayerArea::getShoupaiNumber() const
{
	return m_shoupaiNumber;
}

const CardFrame * PlayerArea::getWuqi() const
{
	return m_wuqiCard;
}

const CardFrame * PlayerArea::getFangju() const
{
	return m_fangjuCard;
}

const CardFrame * PlayerArea::getJianma() const
{
	return m_jianmaCard;
}

const CardFrame * PlayerArea::getJiama() const
{
	return m_jiamaCard;
}

void PlayerArea::addJudge(sgs::ConstData::CardType judgeCard)
{
	ms_judgeList.push_back(judgeCard);
	update();
}

void PlayerArea::removeJudge(sgs::ConstData::CardType judgeCard)
{
	ms_judgeList.remove(judgeCard);
	update();
}

bool PlayerArea::chained() const
{
	return m_isChained;
}

bool PlayerArea::isDead() const
{
	return m_isDead;
}

bool PlayerArea::isDying() const
{
	return m_isDying;
}

void PlayerArea::setDead()
{
	if (!m_isDead)
	{
		m_isDead = true;
		if (m_role != sgs::ConstData::ZHU)
			m_rolePixmap = QPixmap(rolePixmapStr(m_role, true));

		m_playerSelected = false;
		m_playerSelectable = false;
		shoupaiNumLabel->hide();
		m_isDying = false;
		m_isResponding = false;
		m_phase = sgs::ConstData::OTHERPHASE;
		update();
	}
}

void PlayerArea::setDying(bool dying)
{
	if (m_isDying != dying)
	{
		m_isDying = dying;
		update();
	}
}

void PlayerArea::setResponding(bool responding)
{
	if (m_isResponding != responding)
	{
		m_isResponding = responding;
		update();
	}
}

void PlayerArea::addWuqi(CardFrame * newWuqiCard)
{
	if (m_wuqiCard)
		delete m_wuqiCard;
	m_wuqiCard = newWuqiCard;
	update();
}

void PlayerArea::addFangju(CardFrame * newFangjuCard)
{
	if (m_fangjuCard)
		delete m_fangjuCard;
	m_fangjuCard = newFangjuCard;
	update();
}

void PlayerArea::addJianma(CardFrame * newJianmaCard)
{
	if (m_jianmaCard)
		delete m_jianmaCard;
	m_jianmaCard = newJianmaCard;
	update();
}

void PlayerArea::addJiama(CardFrame * newJiamaCard)
{
	if (m_jiamaCard)
		delete m_jiamaCard;
	m_jiamaCard = newJiamaCard;
	update();
}

void PlayerArea::removeWuqi()
{
	if (m_wuqiCard)
		delete m_wuqiCard;
	m_wuqiCard = 0;
	update();
}

void PlayerArea::removeFangju()
{
	if (m_fangjuCard)
		delete m_fangjuCard;
	m_fangjuCard = 0;
	update();
}

void PlayerArea::removeJianma()
{
	if (m_jianmaCard)
		delete m_jianmaCard;
	m_jianmaCard = 0;
	update();
}

void PlayerArea::removeJiama()
{
	if (m_jiamaCard)
		delete m_jiamaCard;
	m_jiamaCard = 0;
	update();
}

sgs::DataType::Player * PlayerArea::getPlayer()
{
	return GUIStaticData::getPlayer(m_playerIndex);
}

void PlayerArea::click()
{
	QMouseEvent temp(QEvent::MouseButtonRelease, QPoint(width() / 2, height() / 2), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	mousePressEvent(&temp);
	mouseReleaseEvent(&temp);
}

bool PlayerArea::isSelected()
{
	return m_playerSelected;
}

bool PlayerArea::isSelectable()
{
	return m_playerSelectable;
}

void PlayerArea::setSelected(bool selected/* = true*/)
{
	if (m_isDead)
		return;

	if (isSelectable() && (isSelected() != selected))
	{
		m_playerSelected = selected;
		update();
	}
}

void PlayerArea::setUnselected(bool unselected/* = true*/)
{
	setSelected(!unselected);
}

void PlayerArea::setSelectable(bool selectable/* = true*/)
{
	if (m_isDead)
		return;

	if (!selectable)
		setUnselected(true);

	if (m_playerSelectable != selectable)
	{
		m_playerSelectable = selectable;
		update();
	}
}

void PlayerArea::setUnselectable(bool unselectable/* = true*/)
{
	setSelectable(!unselectable);
}

void PlayerArea::setHP(int HP)
{
	if (m_HP != HP)
	{
		m_HP = HP;
		if (m_HP > 0)
			m_isDying = false;
		update();
	}
}

void PlayerArea::setHPDamage(int damage)
{
	m_HP -= damage;
	if (m_HP > 0)
		m_isDying = false;
	update();
	if (damage < 0)
		showRecover();
}

void PlayerArea::setPhase(sgs::ConstData::PlayerPhase phase)
{
	if (m_phase != phase)
	{
		m_phase = phase;
		update();
	}
}

void PlayerArea::setShoupaiNumber(int shoupaiNumber)
{
	if (m_shoupaiNumber != shoupaiNumber)
	{
		m_shoupaiNumber = shoupaiNumber;
		if (m_shoupaiNumber > 0)
		{
			shoupaiNumLabel->setText(setColored(QString::number(m_shoupaiNumber)));

			if (shoupaiNumLabel->isHidden())
			{
				shoupaiNumLabel->show();
				update();
			}
		}
		else
		{
			shoupaiNumLabel->hide();
			update();
		}
	}
}

void PlayerArea::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.drawPixmap(ms_playerFrameMargin, m_framePixmap);
	painter.drawPixmap(ms_playerPixmapMargin, m_playerPixmap);
	painter.drawPixmap(ms_countryFrameMargin, m_countryFramePixmap);
	painter.drawPixmap(ms_countrySmallMargin, m_countrySmallPixmap);
	painter.drawPixmap(ms_rolePixmapMargin, m_rolePixmap);

	if (m_shoupaiNumber > 0)
		painter.drawPixmap(ms_shoupaiPixmapMargin, m_shoupaiPixmap);

	int horizontalDifference = ms_maxHPWidth / m_maxHP;
	if (horizontalDifference > ms_maxHorizontalHPDifference)
		horizontalDifference = ms_maxHorizontalHPDifference;

	for (int i = 0, j = ms_horizontalHPMargin; i < m_maxHP; ++i, j += horizontalDifference)
	{
		int hp = (i < m_HP) ? m_HP : 0;
		painter.drawPixmap(j, ms_verticalHPMargin, (smallHPPixmapStr(hp)));;
	}

	// take all numbers on small zhuangbei pixmap as SPADE (black color)
	using sgs::ConstData::SPADE;
	if (m_wuqiCard)
	{
		painter.drawPixmap(ms_wuqiColorMargin, cardColorStr(m_wuqiCard->getCardColor()));
		painter.drawPixmap(ms_wuqiPixmapMargin, smallZhuangbeiPixmapStr(m_wuqiCard->getCardType()));
	}

	if (m_fangjuCard)
	{
		painter.drawPixmap(ms_fangjuColorMargin, cardColorStr(m_fangjuCard->getCardColor()));
		painter.drawPixmap(ms_fangjuPixmapMargin, smallZhuangbeiPixmapStr(m_fangjuCard->getCardType()));
	}

	if (m_jianmaCard)
	{
		painter.drawPixmap(ms_jianmaColorMargin, cardColorStr(m_jianmaCard->getCardColor()));
		painter.drawPixmap(ms_jianmaPixmapMargin, smallZhuangbeiPixmapStr(m_jianmaCard->getCardType()));
	}

	if (m_jiamaCard)
	{
		painter.drawPixmap(ms_jiamaColorMargin, cardColorStr(m_jiamaCard->getCardColor()));
		painter.drawPixmap(ms_jiamaPixmapMargin, smallZhuangbeiPixmapStr(m_jiamaCard->getCardType()));
	}

	int i = 0;
	for (auto iter = ms_judgeList.begin(); iter != ms_judgeList.end(); ++iter)
	{
		switch (*iter)
		{
		case sgs::ConstData::SHANDIAN:
			painter.drawPixmap(ms_statusPixmapMargin * (++i), m_shandianPixmap);
			break;
		case sgs::ConstData::SISHU:
			painter.drawPixmap(ms_statusPixmapMargin * (++i), m_lebuPixmap);
			break;
		case sgs::ConstData::BINGLIANG:
			painter.drawPixmap(ms_statusPixmapMargin * (++i), m_bingliangPixmap);
			break;
		default:
			break;
		}
	}

	if (m_isChained)
		painter.drawPixmap(ms_chainPixmapMargin, m_chainPixmap);

	if (m_phase != sgs::ConstData::OTHERPHASE || m_isDying || m_isResponding)
	{
		painter.drawPixmap(ms_phasePixmapMargin, QPixmap(phasePixmapStr(m_phase, m_isDying, m_isResponding)));
		painter.drawPixmap(0, 0, QPixmap(phaseFramePixmapStr(m_phase, m_isDying, m_isResponding)));
	}

	if (m_isDying)
		painter.drawPixmap(ms_dyingPixmapMargin, m_dyingPixmap);

	if (m_isDead)
		painter.drawPixmap(ms_deadPixmapMargin, m_deadRolePixmap);

	if (m_playerSelected)
		painter.fillRect(ms_playerFrameRect, ms_selectedColor);

	if (!m_playerSelectable)
		painter.fillRect(ms_playerFrameRect, ms_unselectableColor);
}

void PlayerArea::mousePressEvent(QMouseEvent * event)
{
	if (m_isDead)
		return;

	if (m_playerSelectable && event->button() == Qt::LeftButton && mainWindow()->isResponding())
		mainWindow()->playerClicked(this);
}

MainWindow* PlayerArea::mainWindow() const
{
	return static_cast<MainWindow*>(window());
}

void PlayerArea::showSkill(sgs::ConstData::HeroSkill skill)
{
	m_skillLabel->showSkill(skill);
}

void PlayerArea::showRecover()
{
	m_recoverLabel->show();
	m_recoverTimer->start(GUIStaticData::showRecoverDuration);
}

void PlayerArea::showZhuangbei(sgs::ConstData::CardType equipCard)
{
	m_zhuangbeiLabel->showZhuangbei(equipCard);
}

// 没有实现铁锁卡牌,因此这里不会被触发
void PlayerArea::resetChain()
{
	m_isChained = !m_isChained;
	update();
}

} /* namespace sgsui */
