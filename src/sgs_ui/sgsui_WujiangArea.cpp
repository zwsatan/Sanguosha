#include "sgsui_WujiangArea.h"
#include "sgsui_MainWindow.h"
#include "sgsui_SkillLabel.h"
#include "sgsui_nameConvert.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_output.h"
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>

namespace sgsui {

const QSize WujiangArea::ms_fixedSize(190, 170);
const QRect WujiangArea::ms_clickableRect(0, 0, 140, 110);
const QRect WujiangArea::ms_coloredRect(25, 8, 94, 97);
const QColor WujiangArea::ms_selectedColor(255, 0, 0, 150);
const QColor WujiangArea::ms_unselectableColor(0, 0, 0, 100);

const QPoint WujiangArea::ms_skillPosition(15, 15);
const QPoint WujiangArea::ms_playerPixmapMargin(25, 8);
const QPoint WujiangArea::ms_countrySmallMargin(22, 5);
const QPoint WujiangArea::ms_rolePixmapMargin(77, 92);
const QPoint WujiangArea::ms_chainPixmapMargin(0, 5);
const QPoint WujiangArea::ms_dyingPixmapMargin(30, 0);
const QPoint WujiangArea::ms_deadPixmapMargin(0, 10);
const QPoint WujiangArea::ms_recoverLabelMargin(0, 0);

const int WujiangArea::ms_horizontalHPMargin = 155;
const int WujiangArea::ms_verticalHPMargin = 5;
const int WujiangArea::ms_maxVerticalHPDifference = 30;
const int WujiangArea::ms_maxHPHeight = 153;

WujiangArea::WujiangArea(int playerIndex, QWidget *parent)
	: QWidget(parent), m_playerIndex(playerIndex)
	, m_role(GUIStaticData::getPlayerRole(m_playerIndex))
	, m_heroType(GUIStaticData::getPlayerType(m_playerIndex))
	, m_sex(GUIStaticData::getPlayerSex(m_playerIndex))
	, m_side(GUIStaticData::getPlayerSide(m_playerIndex))
	, m_skillCount(GUIStaticData::getSkillCount(m_playerIndex))
	, m_playerPixmap(wujiangSelfPixmapStr(m_heroType))
	, m_countrySmallPixmap(countrySmallPixmapStr(m_side))
	, m_rolePixmap(rolePixmapStr(m_role))
	, m_deadRolePixmap(deadRolePixmapStr(m_role))
	, m_dyingPixmap("images/animation/save-me.png")
	, m_chainPixmap("images/status/chain.png")
	, m_HP(GUIStaticData::getPlayerHP(GUIStaticData::getHumanIndex()))
	, m_maxHP(GUIStaticData::getPlayerHP(GUIStaticData::getHumanIndex()))
	, m_isDead(false)
	, m_isDying(false)
	, m_isChained(false)
	, m_playerSelected(false)
	, m_playerSelectable(true)
{
	setupUi(this);
	setFixedSize(ms_fixedSize);

	m_skillLabel = new SkillLabel(this);
	m_skillLabel->move(ms_skillPosition);
	m_skillLabel->hide();

	m_skillButtons[0] = skill0;
	m_skillButtons[1] = skill1;
	m_skillButtons[2] = skill2;
	m_skillButtons[3] = skill3;

	skill0->hide();
	skill1->hide();
	skill2->hide();
	skill3->hide();

	spacer1->hide();
	spacer2->hide();

	skillWidget1->hide();
	skillWidget2->hide();
	noSkill->setFont(GUIStaticData::contentFont);

	// first disable all, then skill normalize will enable them
	disableAllSkillButtons();

	switch (m_skillCount)
	{
	case 4:
		skill3->show();
		spacer2->show();
		connect(m_skillButtons[3], SIGNAL(clicked()), this, SLOT(skillSlot3()));
		/* no break */
	case 3:
		skill2->show();
		skillWidget2->show();
		connect(m_skillButtons[2], SIGNAL(clicked()), this, SLOT(skillSlot2()));
		/* no break */
	case 2:
		skill1->show();
		spacer1->show();
		connect(m_skillButtons[1], SIGNAL(clicked()), this, SLOT(skillSlot1()));
		/* no break */
	case 1:
		skill0->show();
		skillWidget1->show();
		connect(m_skillButtons[0], SIGNAL(clicked()), this, SLOT(skillSlot0()));
		noSkill->hide();
		/* no break */
	}

	for (int i = 0; i < m_skillCount; ++i)
	{
		QString skillName = skillDisplayName(GUIStaticData::getPlayerSkill(m_playerIndex, i));
		m_skillButtons[i]->setText(setColored(skillName, false));
	}

	m_recoverLabel = new QLabel(this);
	m_recoverLabel->move(ms_recoverLabelMargin);
	m_recoverLabel->hide();
	m_recoverLabel->setPixmap(QPixmap("images/HP/recover.png"));
	m_recoverLabel->setFixedSize(m_recoverLabel->sizeHint());

	m_recoverTimer = new QTimer(this);
	m_recoverTimer->setSingleShot(true);
	connect(m_recoverTimer, SIGNAL(timeout()), m_recoverLabel, SLOT(hide()));
}

void WujiangArea::enableSkillButton(int skillButtonIndex, bool enable)
{
	if (0 <= skillButtonIndex && skillButtonIndex < m_skillCount)
		m_skillButtons[skillButtonIndex]->setEnabled(enable);
}

void WujiangArea::disableSkillButton(int skillButtonIndex, bool disable)
{
	if (0 <= skillButtonIndex && skillButtonIndex < m_skillCount)
		m_skillButtons[skillButtonIndex]->setDisabled(disable);
}

void WujiangArea::disableAllSkillButtons()
{
	for (int i = 0; i < m_skillCount; ++i)
		m_skillButtons[i]->setDisabled();
}

sgs::ConstData::HeroSex WujiangArea::getSex() const
{
	return m_sex;
}

sgs::ConstData::HeroType WujiangArea::getHeroType() const
{
	return m_heroType;
}

sgs::ConstData::PlayerRole WujiangArea::getRole() const
{
	return m_role;
}

int WujiangArea::getHP() const
{
	return m_HP;
}

int WujiangArea::playerIndex() const
{
	return m_playerIndex;
}

int WujiangArea::getMaxHP() const
{
	return m_maxHP;
}

bool WujiangArea::chained() const
{
	return m_isChained;
}

bool WujiangArea::isDead() const
{
	return m_isDead;
}

bool WujiangArea::isDying() const
{
	return m_isDying;
}

void WujiangArea::setDead()
{
	if (!m_isDead)
	{
		m_isDead = true;
		setDying(false);
		m_playerSelected = false;
		m_playerSelectable = false;
		update();
	}
}

void WujiangArea::setDying(bool dying)
{
	if (m_isDying != dying) {
		m_isDying = dying;
		update();
	}
}

void WujiangArea::showSkill(sgs::ConstData::HeroSkill skill)
{
	m_skillLabel->showSkill(skill);
}

void WujiangArea::showRecover()
{
	m_recoverLabel->show();
	m_recoverTimer->start(GUIStaticData::showRecoverDuration);
}

void WujiangArea::resetChain()
{
	m_isChained = !m_isChained;
	update();
}

sgs::DataType::Player * WujiangArea::getPlayer()
{
	return GUIStaticData::getPlayer(GUIStaticData::getHumanIndex());
}

sgs::ConstData::HeroSkill WujiangArea::getSkill(int skillIndex)
{
	return (skillIndex < m_skillCount) ?
			GUIStaticData::getPlayerSkill(m_playerIndex, skillIndex) :
			sgs::ConstData::heroSkillNone;
}

void WujiangArea::click()
{
	QMouseEvent temp(QEvent::MouseButtonRelease, ms_clickableRect.center(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	mousePressEvent(&temp);
	mouseReleaseEvent(&temp);
}

bool WujiangArea::isSelected()
{
	return m_playerSelected;
}

bool WujiangArea::isSelectable()
{
	return m_playerSelectable;
}

void WujiangArea::setSelected(bool selected/* = true*/)
{
	if (m_isDead)
		return;

	if (isSelectable() && (isSelected() != selected))
	{
		m_playerSelected = selected;
		update();
	}
}

void WujiangArea::setUnselected(bool unselected/* = true*/)
{
	setSelected(!unselected);
}

void WujiangArea::setSelectable(bool selectable/* = true*/)
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

void WujiangArea::setUnselectable(bool unselectable/* = true*/)
{
	setSelectable(!unselectable);
}

void WujiangArea::setHP(int HP)
{
	if (m_HP != HP)
	{
		m_HP = HP;
		if (m_HP > 0)
			m_isDying = false;
		update();
	}
}

void WujiangArea::setHPDamage(int damage)
{
	m_HP -= damage;
	printDebug("WujiangArea::setHPDamage: now HP = " + QString::number(m_HP));
	if (m_HP > 0)
		m_isDying = false;
	update();
	if (damage < 0)
		showRecover();
}

void WujiangArea::skillSlot0()
{
	emit skillClicked(GUIStaticData::getPlayerSkill(m_playerIndex, 0), 0);
}

void WujiangArea::skillSlot1()
{
	emit skillClicked(GUIStaticData::getPlayerSkill(m_playerIndex, 1), 1);
}

void WujiangArea::skillSlot2()
{
	emit skillClicked(GUIStaticData::getPlayerSkill(m_playerIndex, 2), 2);
}

void WujiangArea::skillSlot3()
{
	emit skillClicked(GUIStaticData::getPlayerSkill(m_playerIndex, 3), 3);
}

void WujiangArea::mousePressEvent(QMouseEvent * event)
{
	if (m_isDead || !ms_clickableRect.contains(event->pos()))
		return;

	if (m_playerSelectable && event->button() == Qt::LeftButton && mainWindow()->isResponding())
		static_cast<MainWindow *>(window())->playerClicked(this);
}

void WujiangArea::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.drawPixmap(ms_playerPixmapMargin, m_playerPixmap);
	painter.drawPixmap(ms_countrySmallMargin, m_countrySmallPixmap);
	painter.drawPixmap(ms_rolePixmapMargin, m_rolePixmap);

	int verticalDifference = ms_maxHPHeight / m_maxHP;
	if (verticalDifference > ms_maxVerticalHPDifference)
		verticalDifference = ms_maxVerticalHPDifference;
	for (int i = 0, j = ms_verticalHPMargin; i < m_maxHP; ++i, j += verticalDifference)
	{
		if (i < m_HP)
			painter.drawPixmap(ms_horizontalHPMargin, j, HPPixmapStr(m_HP));
		else
			painter.drawPixmap(ms_horizontalHPMargin, j, HPPixmapStr(0));
	}

	if (m_isChained)
		painter.drawPixmap(ms_chainPixmapMargin, m_chainPixmap);

	if (m_playerSelected)
		painter.fillRect(ms_coloredRect, ms_selectedColor);

	if (!m_playerSelectable)
		painter.fillRect(ms_coloredRect, ms_unselectableColor);

	if (m_isDead)
		painter.drawPixmap(ms_deadPixmapMargin, m_deadRolePixmap);

	if (m_isDying)
		painter.drawPixmap(ms_dyingPixmapMargin, m_dyingPixmap);
}

MainWindow * WujiangArea::mainWindow() const
{
	return static_cast<MainWindow *>(window());
}

} /* namespace sgsui */
