#ifndef SGSUI_HELPBOX_H_
#define SGSUI_HELPBOX_H_

#include "ui_HelpBox.h"
#include "sgsui_BlockingFrame.h"

namespace sgsui {

class HelpBox : public BlockingFrame, protected Ui::HelpBox
{
	Q_OBJECT

public:
	explicit HelpBox(QWidget * parent = 0);
	virtual void enterKeyPressedEvent();
	virtual void escapeKeyPressedEvent();

public slots:
	void displayHelp();

private:
	const static QSize ms_fixedSize;
};

} /* namespace sgsui */
#endif /* SGSUI_HELPBOX_H_ */
