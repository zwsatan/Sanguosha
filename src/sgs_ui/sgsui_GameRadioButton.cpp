#include "sgsui_GameRadioButton.h"
#include <QPainter>

namespace sgsui {

GameRadioButton::GameRadioButton(QWidget *parent/* = 0*/,
								 const QPixmap& radioOffPixmap/* = QPixmap("images/button/radio-off.png")*/,
								 const QPixmap& radioOnPixmap/* = QPixmap("images/button/radio-on.png")*/)
	: QRadioButton(parent)
	, m_radioOffPixmap(radioOffPixmap)
	, m_radioOnPixmap(radioOnPixmap)
{
	setFixedSize(m_radioOffPixmap.size());
}

void GameRadioButton::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, (isEnabled() && isChecked() ? m_radioOnPixmap : m_radioOffPixmap));
}

} /* namespace sgsui */
