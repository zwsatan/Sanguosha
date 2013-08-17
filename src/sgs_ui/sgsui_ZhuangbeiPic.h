#ifndef SGSUI_ZHUANGBEIPIC_H_
#define SGSUI_ZHUANGBEIPIC_H_

#include <QWidget>
#include <QLabel>
#include "sgsdata_enum.h"

namespace sgsui {

class CardFrame;
class AbstractEquipProcessor;

class ZhuangbeiPic : public QWidget
{
Q_OBJECT
public:
	ZhuangbeiPic(const CardFrame *newZhuangbeiCard, AbstractEquipProcessor * processor, QWidget * parent = 0);
	void click();
	bool isSelected() const;
	bool isSelectable() const;
	sgs::ConstData::CardType getCardType() const;
	sgs::ConstData::CardColor getCardColor() const;
	int getCardNumber() const;

public slots:
	void setSelected(bool selected = true);
	void setUnselected(bool unselected = true);
	void setSelectable(bool selectable = true);
	void setUnselectable(bool unselectable = true);

private:
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent*);

private:
	const sgs::ConstData::CardType		m_cardType;
	const sgs::ConstData::CardColor		m_cardColor;
	const int							m_number;
	AbstractEquipProcessor * const		m_processor;

	const QPixmap						m_zhuangbeiPixmap;
	const QPixmap						m_colorPixmap;
	const QPixmap						m_numberPixmap;

	bool								m_zhuangbeiSelected;
	bool								m_zhuangbeiSelectable;

	const static QSize					ms_fixedSize;
	const static QPoint					ms_pixmapMargin;
	const static QPoint					ms_numberMargin;
	const static QPoint					ms_colorMargin;
	const static QPoint					ms_pixmapMargin_selected;
	const static QPoint					ms_numberMargin_selected;
	const static QPoint					ms_colorMargin_selected;
	const static QColor					ms_unselectableColor;
};

} /* namespace sgsui */
#endif /* SGSUI_ZHUANGBEIPIC_H_ */
