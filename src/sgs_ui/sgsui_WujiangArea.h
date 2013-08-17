#ifndef SGSUI_WUJIANGAREA_H_
#define SGSUI_WUJIANGAREA_H_

#include "ui_WujiangArea.h"
#include "sgsui_AbstractPlayerArea.h"
#include "sgsdata_enum.h"

namespace sgsui {

class MainWindow;
class SkillLabel;

class WujiangArea
		: public QWidget
		, protected Ui::WujiangArea
		, public AbstractPlayerArea
{
Q_OBJECT

public:
	WujiangArea(int playerIndex, QWidget * parent);

	sgs::DataType::Player * getPlayer();
	sgs::ConstData::HeroSkill getSkill(int skillIndex);
	void click();
	bool isSelected();
	bool isSelectable();
	sgs::ConstData::HeroSex getSex() const;
	sgs::ConstData::HeroType getHeroType() const;
	sgs::ConstData::PlayerRole getRole() const;
	int getHP() const;
	int playerIndex() const;
	int getMaxHP() const;
	bool chained() const;
	bool isDead() const;
	bool isDying() const;
	void setDead();
	void setDying(bool dying);
	void showSkill(sgs::ConstData::HeroSkill skill);
	void showRecover();
	void resetChain();
	void enableSkillButton(int skillButtonIndex, bool enable = true);
	void disableSkillButton(int skillButtonIndex, bool disable = true);
	void disableAllSkillButtons();

signals:
	void skillClicked(sgs::ConstData::HeroSkill, int);

public slots:
	void setSelected(bool selected = true);
	void setUnselected(bool unselected = true);
	void setSelectable(bool selectable = true);
	void setUnselectable(bool unselectable = true);
	void setHP(int HP);
	void setHPDamage(int damage);

private slots:
	void skillSlot0();
	void skillSlot1();
	void skillSlot2();
	void skillSlot3();

private:
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent*);
	MainWindow * mainWindow() const;

private:
	const int m_playerIndex;
	const sgs::ConstData::PlayerRole		m_role;
	const sgs::ConstData::HeroType			m_heroType;
	const sgs::ConstData::HeroSex			m_sex;
	const sgs::ConstData::HeroSide			m_side;

	int										m_skillCount;
	SkillButton *							m_skillButtons[4];

	const QPixmap							m_playerPixmap;
	const QPixmap							m_countrySmallPixmap;
	const QPixmap							m_rolePixmap;
	const QPixmap							m_deadRolePixmap;
	const QPixmap							m_dyingPixmap;
	const QPixmap							m_chainPixmap;

	int										m_HP;
	const int								m_maxHP;

	bool									m_isDead;
	bool									m_isDying;
	bool									m_isChained;

	bool									m_playerSelected;
	bool									m_playerSelectable;

	SkillLabel *							m_skillLabel;

	QLabel *								m_recoverLabel;
	QTimer *								m_recoverTimer;

	const static QSize						ms_fixedSize;
	const static QPoint						ms_skillPosition;
	const static QPoint						ms_playerPixmapMargin;
	const static QPoint						ms_countrySmallMargin;
	const static QPoint						ms_rolePixmapMargin;
	const static QPoint						ms_chainPixmapMargin;
	const static QPoint						ms_dyingPixmapMargin;
	const static QPoint						ms_deadPixmapMargin;
	const static QPoint						ms_recoverLabelMargin;

	const static int						ms_horizontalHPMargin;
	const static int						ms_verticalHPMargin;
	const static int						ms_maxVerticalHPDifference;
	const static int						ms_maxHPHeight;

	const static QRect						ms_clickableRect;
	const static QRect						ms_coloredRect;
	const static QColor						ms_selectedColor;
	const static QColor						ms_unselectableColor;
};

} /* namespace sgsui */
#endif /* SGSUI_WUJIANGAREA_H_ */
