#ifndef SGSUI_PLAYERAREA_H_
#define SGSUI_PLAYERAREA_H_

#include "ui_PlayerArea.h"
#include "sgsui_AbstractPlayerArea.h"
#include "sgsdata_enum.h"

namespace sgsui {

class MainWindow;
class CardFrame;
class SkillLabel;
class ZhuangbeiLabel;

class PlayerArea
		: public QFrame
		, protected Ui::PlayerArea
		, public AbstractPlayerArea
{
	Q_OBJECT

public:
	PlayerArea(int thisPlayerIndex, QWidget * parent);

	QPoint topLeftPoint() const;
	QRect realGeometry() const;
	int playerIndex() const;
	sgs::DataType::Player * getPlayer();
	void click();
	bool isSelected();
	bool isSelectable();
	sgs::ConstData::HeroSex getSex() const;
	sgs::ConstData::HeroType getHeroType() const;
	sgs::ConstData::PlayerRole getRole() const;
	int getHP() const;
	int getPhase() const;
	int getShoupaiNumber() const;
	const CardFrame * getWuqi() const;
	const CardFrame * getFangju() const;
	const CardFrame * getJianma() const;
	const CardFrame * getJiama() const;
	void addJudge(sgs::ConstData::CardType judgeCard);
	void removeJudge(sgs::ConstData::CardType judgeCard);
	bool chained() const;
	bool isDead() const;
	bool isDying() const;
	void setDead();
	void setDying(bool dying);
	void setResponding(bool responding);

	//Return value indicates whether any old card is deserted (processed by GUI algorithm)
	void addWuqi(CardFrame * newWuqiCard);
	void addFangju(CardFrame * newFangjuCard);
	void addJianma(CardFrame * newJianmaCard);
	void addJiama(CardFrame * newJiamaCard);
	void removeWuqi();
	void removeFangju();
	void removeJianma();
	void removeJiama();

	void showSkill(sgs::ConstData::HeroSkill skill);
	void showRecover();
	void showZhuangbei(sgs::ConstData::CardType equipCard);
	void resetChain();

public slots:
	void setSelected(bool selected = true);
	void setUnselected(bool unselected = true);
	void setSelectable(bool selectable = true);
	void setUnselectable(bool unselectable = true);
	void setHP(int HP);
	void setHPDamage(int damage);
	void setPhase(sgs::ConstData::PlayerPhase phase);
	void setShoupaiNumber(int shoupaiNumber);

private:
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent*);
	MainWindow* mainWindow() const;

private:
	const int									m_playerIndex;

	const sgs::ConstData::PlayerRole			m_role;
	const sgs::ConstData::HeroType				m_heroType;
	const sgs::ConstData::HeroSex				m_sex;
	const sgs::ConstData::HeroSide				m_side;

	const QPixmap								m_framePixmap;
	const QPixmap								m_playerPixmap;
	const QPixmap								m_countryFramePixmap;
	const QPixmap								m_countrySmallPixmap;
	QPixmap										m_rolePixmap;
	const QPixmap								m_deadRolePixmap;
	const QPixmap								m_dyingPixmap;
	const QPixmap								m_shoupaiPixmap;
	const QPixmap								m_chainPixmap;
	const QPixmap								m_lebuPixmap;
	const QPixmap								m_shandianPixmap;
	const QPixmap								m_bingliangPixmap;

	int											m_HP;
	const int									m_maxHP;

	int											m_shoupaiNumber;

	bool										m_isDead;
	bool										m_isDying;
	bool										m_isResponding;

	bool										m_isChained;

	sgs::ConstData::PlayerPhase					m_phase;

	bool										m_playerSelected;
	bool										m_playerSelectable;

	CardFrame *									m_wuqiCard;
	CardFrame *									m_fangjuCard;
	CardFrame *									m_jianmaCard;
	CardFrame *									m_jiamaCard;

	SkillLabel *								m_skillLabel;
	ZhuangbeiLabel *							m_zhuangbeiLabel;

	QLabel *									m_recoverLabel;
	QTimer *									m_recoverTimer;

	const static QSize							ms_fixedSize;
	const static QPoint							ms_skillPosition;
	const static QPoint							ms_zhuangbeiPosition;
	const static QPoint							ms_playerFrameMargin;
	const static QPoint							ms_playerPixmapMargin;
	const static QPoint							ms_countryFrameMargin;
	const static QPoint							ms_countrySmallMargin;
	const static QPoint							ms_rolePixmapMargin;
	const static QPoint							ms_shoupaiPixmapMargin;

	const static QPoint							ms_wuqiColorMargin;
	const static QPoint							ms_wuqiPixmapMargin;
	const static QPoint							ms_wuqiNumberMargin;
	const static QPoint							ms_fangjuColorMargin;
	const static QPoint							ms_fangjuPixmapMargin;
	const static QPoint							ms_fangjuNumberMargin;
	const static QPoint							ms_jianmaColorMargin;
	const static QPoint							ms_jianmaPixmapMargin;
	const static QPoint							ms_jianmaNumberMargin;
	const static QPoint							ms_jiamaColorMargin;
	const static QPoint							ms_jiamaPixmapMargin;
	const static QPoint							ms_jiamaNumberMargin;
	const static QPoint							ms_chainPixmapMargin;
	const static QPoint							ms_phasePixmapMargin;
	const static QPoint							ms_dyingPixmapMargin;
	const static QPoint							ms_deadPixmapMargin;
	const static QPoint							ms_statusPixmapMargin;
	const static QPoint							ms_recoverLabelMargin;
	const static int							ms_horizontalHPMargin;
	const static int							ms_verticalHPMargin;
	const static int							ms_maxHorizontalHPDifference;
	const static int							ms_maxHPWidth;

	const static QRect							ms_playerFrameRect;
	const static QColor							ms_selectedColor;
	const static QColor							ms_unselectableColor;

	std::list<sgs::ConstData::CardType>			ms_judgeList;
};

} /* namespace sgsui */
#endif /* SGSUI_PLAYERAREA_H_ */
