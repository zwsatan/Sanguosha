#include "sgsui_HelpBox.h"
#include "sgsui_GUIStaticData.h"

namespace sgsui {

const QSize HelpBox::ms_fixedSize(1024, 600);

HelpBox::HelpBox(QWidget * parent)
	: BlockingFrame(parent)
{
	setupUi(this);
	setFixedSize(ms_fixedSize);
	setStyleSheet("QFrame#HelpBox{background-image:url(images/frame/helpBox.png)}");

	okButton->setEnabledPixmap(QPixmap("images/button/ok.png"));
	helpText->setSource(GUIStaticData::helpTextLocation);
	connect(okButton, SIGNAL(clicked()), this, SLOT(unblock()));
}

void HelpBox::enterKeyPressedEvent()
{
	unblock();
}

void HelpBox::escapeKeyPressedEvent()
{
	unblock();
}

void HelpBox::displayHelp()
{
	int x = window()->width() / 2 - width() / 2;
	int y = window()->height() / 2 - height() / 2 - 40;
	setGeometry(QRect(QPoint(x, y), size()));
	exec();
}

} /* namespace sgsui */
