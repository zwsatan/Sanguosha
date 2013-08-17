#ifndef SGSUI_GAMEBUTTON_H_
#define SGSUI_GAMEBUTTON_H_

#include <QLabel>

namespace sgsui {

class GameButton: public QLabel
{
	Q_OBJECT

public:
	GameButton(QWidget * parent,
			   bool enable = true,
			   const QPixmap& enabledPixmap = QPixmap("images/button/enabled.png"),
			   const QPixmap& disabledPixmap = QPixmap("images/button/disabled.png"),
			   const QPixmap& pressedPixmap = QPixmap("images/button/pressed.png"));

	bool isEnabled();
	void click();

	signals:
	void clicked();

public slots:
	void setEnabled(bool enabled = true);
	void setDisabled(bool disabled = true);
	void setEnabledPixmap(const QPixmap& enabledPixmap);
	void setPressedPixmap(const QPixmap& pressedPixmap);
	void setDisabledPixmap(const QPixmap& disabledPixmap);

protected:
	void paintEvent(QPaintEvent	* event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);

protected:
	QPixmap			m_buttonEnabledPixmap;
	QPixmap			m_buttonDisabledPixmap;
	QPixmap			m_buttonPressedPixmap;
	bool			m_buttonEnabled;
	bool			m_isPressed;
};

} /* namespace sgsui */
#endif /* SGSUI_GAMEBUTTON_H_ */
