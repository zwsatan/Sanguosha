#include "sgsui_GameButton.h"
#include "sgsui_GUIStaticData.h"
#include <QMouseEvent>
#include <QPainter>

namespace sgsui {

GameButton::GameButton(QWidget * parent,
					   bool enable/* = true*/,
					   const QPixmap &enabledPixmap/* = QPixmap("images/button/enabled.png")*/,
					   const QPixmap &disabledPixmap/* = QPixmap("images/button/disabled.png")*/,
					   const QPixmap &pressedPixmap/* = QPixmap("images/button/pressed.png")*/)
	: QLabel(parent)
	, m_buttonEnabledPixmap(enabledPixmap)
	, m_buttonDisabledPixmap(disabledPixmap)
	, m_buttonPressedPixmap(pressedPixmap)
	, m_buttonEnabled(enable)
	, m_isPressed(false)
{
	setFixedSize(enabledPixmap.size());
	setFont(GUIStaticData::buttonFont);
	setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

void GameButton::click()
{
	QMouseEvent temp(QEvent::MouseButtonRelease, QPoint(width() / 2, height() / 2), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	mousePressEvent(&temp);
	mouseReleaseEvent(&temp);
}

bool GameButton::isEnabled()
{
	return m_buttonEnabled;
}

void GameButton::setEnabled(bool enabled/* = true*/)
{
	if (m_buttonEnabled != enabled)
	{
		m_buttonEnabled = enabled;
		update();
	}
}

void GameButton::setDisabled(bool disabled/* = true*/)
{
	setEnabled(!disabled);
}

void GameButton::setEnabledPixmap(const QPixmap& enabledPixmap)
{
	m_buttonEnabledPixmap = enabledPixmap;
	if (m_buttonEnabled)
	{
		setFixedSize(m_buttonEnabledPixmap.size());
		update();
	}
}

void GameButton::setPressedPixmap(const QPixmap& pressedPixmap)
{
	m_buttonPressedPixmap = pressedPixmap;
}

void GameButton::setDisabledPixmap(const QPixmap& disabledPixmap)
{
	m_buttonDisabledPixmap = disabledPixmap;
	if (!m_buttonEnabled)
	{
		setFixedSize(m_buttonDisabledPixmap.size());
		update();
	}
}

void GameButton::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	QPixmap * pixPressed = NULL;
	if (m_buttonEnabled)
		pixPressed = m_isPressed ? &m_buttonPressedPixmap : &m_buttonEnabledPixmap;
	else
		pixPressed = &m_buttonDisabledPixmap;

	painter.drawPixmap(0, 0, *pixPressed);
	QLabel::paintEvent(event);
}

void GameButton::mousePressEvent(QMouseEvent * event)
{
	if (m_buttonEnabled && event->button() == Qt::LeftButton)
	{
		m_isPressed = true;
		move(x(), y() + 2);
		update();
	}
}

void GameButton::mouseReleaseEvent(QMouseEvent * event)
{
	if (m_buttonEnabled && event->button() == Qt::LeftButton)
	{
		m_isPressed = false;
		move(x(), y() - 2);
		update();

		int eventX = event->pos().x();
		int eventY = event->pos().y();
		if (-1 <= eventX && eventX <= width() + 1 && -1 <= eventY && eventY <= height() + 1)
			emit clicked();
	}
}

} /* namespace sgsui */
