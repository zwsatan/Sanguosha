#include "sgsui_DialogBox.h"
#include "sgsui_output.h"
#include "sgsui_nameConvert.h"

namespace sgsui {

const QSize SGS_DialogBox::ms_fixedSize(480, 200);

SGS_DialogBox::SGS_DialogBox(QWidget *parent)
	: BlockingFrame(parent)
	, m_returnValue(-1)
{
	setupUi(this);
	setFixedSize(ms_fixedSize);

	okButton->setEnabledPixmap(QPixmap("images/button/ok.png"));
	okButton->setDisabledPixmap(QPixmap("images/button/ok-disabled.png"));
	cancelButton->setEnabledPixmap(QPixmap("images/button/cancel.png"));
	cancelButton->setDisabledPixmap(QPixmap("images/button/cancel-disabled.png"));

	setStyleSheet("QFrame#SGS_DialogBox{background-image:url(images/frame/dialog.png)}");

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void SGS_DialogBox::enterKeyPressedEvent()
{
	okButton->click();
}

void SGS_DialogBox::escapeKeyPressedEvent()
{
	cancelButton->click();
}

int SGS_DialogBox::exec(const QString &message)
{
	messageLabel->setText(setColored(message));

	BlockingFrame::exec();
	return m_returnValue;
}

void SGS_DialogBox::accept()
{
	m_returnValue = 1;
	unblock();
}

void SGS_DialogBox::reject()
{
	m_returnValue = 0;
	unblock();
}

} /* namespace sgsui */
