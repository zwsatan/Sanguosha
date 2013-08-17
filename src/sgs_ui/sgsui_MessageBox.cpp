#include "sgsui_MessageBox.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_nameConvert.h"
#include <QGraphicsOpacityEffect>
#include <QTimer>

namespace sgsui {

const QSize SGSMessageBox::fixedSize(480, 200);

SGSMessageBox::SGSMessageBox(QWidget * parent)
	: QLabel(parent)
{
	setObjectName("MessageBox");
	setStyleSheet("QLabel#MessageBox{background-image:url(images/frame/tip.png)}");
	setFixedSize(fixedSize);
	setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	setFont(GUIStaticData::headingFont);
	setWordWrap(true);
	setMargin(20);

	tipEffect = new QGraphicsOpacityEffect(this);
	showTextTimer = new QTimer(this);
	showTextTimer->setSingleShot(true);
	fadeTimer = new QTimer(this);

	connect(showTextTimer, SIGNAL(timeout()), this, SLOT(startFading()));
	connect(fadeTimer, SIGNAL(timeout()), this, SLOT(fading()));
}

void SGSMessageBox::showTipMessage(const QString &tip, bool richText/* = false*/, int duration/* = GUIStaticData::showTipDuration*/)
{
	textDuration = (duration > GUIStaticData::dialogFadeDuration) ? duration : GUIStaticData::showTipDuration;
	opacity = 1;
	tipEffect->setOpacity(opacity);
	setGraphicsEffect(tipEffect);

	if (richText)
		setText(tip);
	else
		setText(setColored(tip));

	show();
	raise();
	fadeTimer->stop();
	showTextTimer->start(textDuration);
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
	opacity = 1;
	fadeTimer->start(GUIStaticData::dialogFadeDuration / 10);
}

void SGSMessageBox::fading()
{
	if (opacity > 0)
	{
		tipEffect->setOpacity(opacity);
		setGraphicsEffect(tipEffect);
		opacity -= 0.1;
	}
	else
	{
		fadeTimer->stop();
		opacity = 0;
		hide();
	}
}

} /* namespace sgsui */
