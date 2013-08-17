#ifndef SGSUI_CARDBUTTON_H_
#define SGSUI_CARDBUTTON_H_

#include "ui_CardButton.h"
#include "sgsui_CardFrame.h"
#include "sgsdata_card.h"

namespace sgsui {

class AbstractCardContainer;

class CardButton
		: public QFrame
		, protected Ui::CardButton
{
Q_OBJECT
public:
	CardButton(const sgs::DataType::Card * platformCard, AbstractCardContainer * container, bool visible = true, QWidget *parent = 0);

	bool isSelected() const;
	bool isSelectable() const;
	const CardFrame * card() const;
	const sgs::DataType::Card * platformCard();

public slots:
	void click();
	void setSelected(bool selected = true);
	void setUnselected(bool unselected = true);
	void setSelectable(bool selectable = true);
	void setUnselectable(bool unselectable = true);

protected:
	void mousePressEvent(QMouseEvent *);
	AbstractCardContainer * container() const;

private:
	AbstractCardContainer * const		m_cardContainer;
	CardFrame *							m_cardFrame;
	bool								m_cardSelectable;
	const sgs::DataType::Card * const	m_plaformCard;

	const static QSize					ms_fixedSize;
};

} /* namespace sgsui */
#endif /* SGSUI_CARDBUTTON_H_ */
