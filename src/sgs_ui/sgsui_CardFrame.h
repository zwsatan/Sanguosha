#ifndef SGSUI_CARDFRAME_H_
#define SGSUI_CARDFRAME_H_

#include <QWidget>
#include "sgsdata_enum.h"

namespace sgsui {

class CardFrame : public QWidget
{
Q_OBJECT
public:
	CardFrame(sgs::ConstData::CardType cardType, sgs::ConstData::CardColor cardColor, int number, QWidget *parent = 0);
	CardFrame(sgs::ConstData::HeroType heroType, sgs::ConstData::CardType cardType, sgs::ConstData::CardColor cardColor, int number, bool result, QWidget *parent = 0);
	explicit CardFrame(QWidget * parent = 0);

	QPoint topLeftPoint() const;
	bool isDimed() const;

	sgs::ConstData::CardType getCardType() const;
	sgs::ConstData::CardColor getCardColor() const;
	int getNumber() const;

public slots:
	void setDimed(bool dim);
	void setNotDimed(bool notDim);

protected:
	void paintEvent(QPaintEvent*);

protected:
	const bool							m_judge;
	const QPixmap						m_wujiangMiniPixmap;
	const QString						m_judgeString;

	const sgs::ConstData::CardType		m_cardType;
	const sgs::ConstData::CardColor		m_cardColor;
	const int							m_number;

	const QPixmap						m_cardPixmap;
	const QPixmap						m_colorPixmap;
	const QPixmap						m_numberPixmap;

	bool								m_dimed;

	const bool							m_judgeResult;
	const QPixmap						m_judgePixmap;
	const QColor						m_judgeTextColor;

	const static QSize					ms_fixedSize;
	const static QPoint					ms_numberMargin;
	const static QPoint					ms_colorMargin;
	const static QPoint					ms_wujiangMiniMargin;
	const static QPoint					ms_judgePixmapMargin;
	const static QColor					ms_unselectableColor;
	const static QRect					ms_judgeTextRect;
};

} /* namespace sgsui */
#endif /* SGSUI_CARDFRAME_H_ */
