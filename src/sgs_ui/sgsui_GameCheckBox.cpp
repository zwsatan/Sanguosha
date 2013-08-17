#include "sgsui_GameCheckBox.h"
#include <QPainter>

namespace sgsui {

GameCheckBox::GameCheckBox(QWidget * parent/* = 0*/,
						   const QPixmap& noncheckedPixmap/* = QPixmap("images/button/nonchecked.png")*/,
						   const QPixmap& checkedPixmap/* = QPixmap("images/button/checked.png")*/)
	: QCheckBox(parent)
	, m_noncheckedPixmap(noncheckedPixmap)
	, m_checkedPixmap(checkedPixmap)
{
	setFixedSize(m_noncheckedPixmap.size());
}

void GameCheckBox::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, (isEnabled() && isChecked() ? m_checkedPixmap : m_noncheckedPixmap));
}

} /* namespace sgsui */
