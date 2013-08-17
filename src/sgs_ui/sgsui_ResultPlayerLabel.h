#ifndef SGSUI_RESULTPLAYERLABEL_H_
#define SGSUI_RESULTPLAYERLABEL_H_

#include <QLabel>
#include "sgsdata_enum.h"

namespace sgsui {

class ResultPlayerLabel : public QLabel
{
public:
	explicit ResultPlayerLabel(QWidget * parent = 0);

	void paintEvent(QPaintEvent * event);
	void showResult(sgs::ConstData::HeroType heroType, bool isDead = false);

private:
	sgs::ConstData::HeroType		m_heroType;
	bool							m_isDead;

	const static QSize				ms_fixedSize;
	const static QColor				ms_deadColor;
};

} /* namespace sgsui */
#endif /* SGSUI_RESULTPLAYERLABEL_H_ */
