#include "sgsui_AboutFrame.h"
#include "sgsui_GUIStaticData.h"

namespace sgsui {

QSize AboutFrame::ms_fixedSize(560, 346);

AboutFrame::AboutFrame(QWidget *parent)
	: BlockingFrame(parent)
{
	setupUi(this);

	setFixedSize(ms_fixedSize);

	okButton->setEnabledPixmap(QPixmap("images/button/ok.png"));
	setStyleSheet("QFrame#AboutFrame{background-image:url(images/frame/aboutFrame.png)}");

	aboutText->setSource(GUIStaticData::aboutTextLocation);
	connect(okButton, SIGNAL(clicked()), this, SLOT(unblock()));
}

void AboutFrame::enterKeyPressedEvent()
{
	okButton->click();
}

void AboutFrame::escapeKeyPressedEvent()
{
	okButton->click();
}

void AboutFrame::showAboutInfo()
{
	int x = window()->width() / 2 - width() / 2;
	int y = window()->height() / 2 - height() / 2 - 40;
	setGeometry(QRect(QPoint(x, y), size()));
	exec();
}

} /* namespace sgsui */
