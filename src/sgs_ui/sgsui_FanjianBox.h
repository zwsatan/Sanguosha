#ifndef SGSUI_FANJIANBOX_H_
#define SGSUI_FANJIANBOX_H_

#include "ui_FanjianBox.h"
#include "sgsui_BlockingFrame.h"
#include "sgsdata_enum.h"

namespace sgsui {

class FanjianBox
		: public BlockingFrame
		, protected Ui::FanjianBox
{
Q_OBJECT
public:
	explicit FanjianBox(QWidget * parent = 0);
	sgs::ConstData::CardColor getFanjianColor();

private slots:
	void chooseHeart();
	void chooseSpade();
	void chooseDiamond();
	void chooseClub();

private:
	sgs::ConstData::CardColor		m_returnColor;

	const static QSize				ms_fixedSize;
};

} /* namespace sgsui */
#endif /* SGSUI_FANJIANBOX_H_ */
