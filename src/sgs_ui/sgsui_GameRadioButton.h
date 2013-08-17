#ifndef SGSUI_GAMERADIOBUTTON_H_
#define SGSUI_GAMERADIOBUTTON_H_

#include <QRadioButton>

namespace sgsui {

class GameRadioButton: public QRadioButton
{
public:
	GameRadioButton(QWidget *parent = 0,
					const QPixmap& radioOffPixmap = QPixmap("images/button/radio-off.png"),
					const QPixmap& radioOnPixmap = QPixmap("images/button/radio-on.png"));

private:
	void paintEvent(QPaintEvent *);

	QPixmap			m_radioOffPixmap;
	QPixmap			m_radioOnPixmap;
};

} /* namespace sgsui */
#endif /* SGSUI_GAMERADIOBUTTON_H_ */
