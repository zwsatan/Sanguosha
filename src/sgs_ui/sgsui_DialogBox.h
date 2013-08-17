#ifndef SGSUI_DIALOGBOX_H_
#define SGSUI_DIALOGBOX_H_

#include "ui_DialogBox.h"
#include "sgsui_BlockingFrame.h"
#include "sgsui_MainWindow.h"

namespace sgsui {

// Qt5.1 Windows上DialogBox是WinApi的一个宏名
class SGS_DialogBox
		: public BlockingFrame
		, protected Ui::SGS_DialogBox
{
Q_OBJECT

public:
	explicit SGS_DialogBox(QWidget *parent = 0);

	void enterKeyPressedEvent();
	void escapeKeyPressedEvent();
	int exec(const QString &message);

private slots:
	void accept();
	void reject();

private:
	int m_returnValue;

	const static QSize fixedSize;
};

} /* namespace sgsui */
#endif /* SGSUI_DIALOGBOX_H_ */
