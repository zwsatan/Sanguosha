#ifndef SGSUI_INITIATEBOX_H_
#define SGSUI_INITIATEBOX_H_

#include "ui_InitiateBox.h"

class QCloseEvent;

namespace sgsui {

class RoleViewer;

class InitiateBox : public QDialog, protected Ui::InitiateBox
{
	Q_OBJECT

public:
	InitiateBox();
	~InitiateBox();

public slots:
	void start();
	void showRoleViewer();

private:
	void closeEvent(QCloseEvent *event);
	void keyPressEvent(QKeyEvent *event);

private:
	const static QSize			ms_fixedSize;
	const static QString		ms_playAgain;

	RoleViewer *				m_roleViewer;
};

} /* namespace sgsui */
#endif /* SGSUI_INITIATEBOX_H_ */
