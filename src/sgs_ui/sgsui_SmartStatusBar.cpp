#include "sgsui_SmartStatusBar.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_nameConvert.h"
#include <QTimer>

namespace sgsui {

const int SmartStatusBar::ms_fixedHeight = 20;

SmartStatusBar::SmartStatusBar(QWidget *parent)
	: QLabel(parent)
{
	setFixedHeight(ms_fixedHeight);
	setFont(GUIStaticData::statusbarFont);
	setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	m_timer = new QTimer(this);
	m_timer->setSingleShot(true);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(clear()));
}

void SmartStatusBar::showStatusBarMessage(const QString &tip, bool richText/* = false*/, int duration/* = GUIStaticData::showTipDuration*/)
{
	if (richText)
		setText(tip);
	else
		setText(setColored(tip, false));

	if (duration > 0)
		m_timer->start(duration);
	else
		m_timer->stop();
}

void SmartStatusBar::clearMessage()
{
	clear();
}

} /* namespace sgsui */
