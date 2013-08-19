#include "sgsui_MessageBox.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_nameConvert.h"
#include <QGraphicsOpacityEffect>
#include <QTimer>

namespace sgsui {

const QSize SGSMessageBox::ms_fixedSize(480, 200);

SGSMessageBox::SGSMessageBox(QWidget * parent)
	: QLabel(parent)
{
	setObjectName("MessageBox");
	setStyleSheet("QLabel#MessageBox{background-image:url(images/frame/tip.png)}");
	setFixedSize(ms_fixedSize);
	setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	setFont(GUIStaticData::headingFont);
	setWordWrap(true);
	setMargin(20);

	m_tipEffect = new QGraphicsOpacityEffect(this);
	m_showTextTimer = new QTimer(this);
	m_showTextTimer->setSingleShot(true);
	m_fadeTimer = new QTimer(this);

	connect(m_showTextTimer, SIGNAL(timeout()), this, SLOT(startFading()));
	connect(m_fadeTimer, SIGNAL(timeout()), this, SLOT(fading()));
}

void SGSMessageBox::showTipMessage(const QString &tip, bool richText/* = false*/, int duration/* = GUIStaticData::showTipDuration*/)
{
	m_textDuration = (duration > GUIStaticData::dialogFadeDuration) ? duration : GUIStaticData::showTipDuration;
	m_opacity = 1;
	m_tipEffect->setOpacity(m_opacity);
	setGraphicsEffect(m_tipEffect);

	if (richText)
		setText(tip);
	else
		setText(setColored(tip));

	show();
	raise();
	m_fadeTimer->stop();
	m_showTextTimer->start(m_textDuration);
}

void SGSMessageBox::showPauseMessage(const QString &pauseText, bool richText)
{
	if (richText)
		setText(pauseText);
	else
		setText(setColored(pauseText));

	show();
	raise();
}

void SGSMessageBox::startFading()
{
	m_opacity = 1;
	m_fadeTimer->start(GUIStaticData::dialogFadeDuration / 10);
}

void SGSMessageBox::fading()
{
	if (m_opacity > 0)
	{
		m_tipEffect->setOpacity(m_opacity);
		setGraphicsEffect(m_tipEffect);
		m_opacity -= 0.1;
	}
	else
	{
		m_fadeTimer->stop();
		m_opacity = 0;
		hide();
	}
}

} /* namespace sgsui */
