#ifndef SGSUI_WUJIANGBUTTON_H_
#define SGSUI_WUJIANGBUTTON_H_

#include <QLabel>
#include <QPixmap>
#include "sgsdata_enum.h"

namespace sgsui {

class WujiangButton : public QLabel
{
	Q_OBJECT

public:
	explicit WujiangButton(QWidget * parent = 0);

	void setWujiang(sgs::ConstData::HeroType heroType);

signals:
	void choosen(sgs::ConstData::HeroType heroType);

private:
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);

private:
	bool							m_clicked;
	QPixmap							m_wujiangCardPixmap;
	sgs::ConstData::HeroType		m_heroType;

	const static QPoint				ms_selectedMargin;
	const static QPoint				ms_unselectedMargin;
};

} /* namespace sgsui */
#endif /* SGSUI_WUJIANGBUTTON_H_ */
