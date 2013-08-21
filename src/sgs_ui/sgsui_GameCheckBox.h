#ifndef SGSUI_GAMECHECKBOX_H_
#define SGSUI_GAMECHECKBOX_H_

#include <QCheckBox>

namespace sgsui {

class GameCheckBox : public QCheckBox
{
public:
	GameCheckBox(QWidget * parent = 0,
				 const QPixmap &noncheckedPixmap = QPixmap("images/button/nonchecked.png"),
				 const QPixmap &checkedPixmap = QPixmap("images/button/checked.png"));

private:
	void paintEvent(QPaintEvent *);

private:
	QPixmap			m_noncheckedPixmap;
	QPixmap			m_checkedPixmap;
};

} /* namespace sgsui */
#endif /* SGSUI_GAMECHECKBOX_H_ */
