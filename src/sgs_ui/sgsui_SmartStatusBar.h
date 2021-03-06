#ifndef SGSUI_SMARTSTATUSBAR_H_
#define SGSUI_SMARTSTATUSBAR_H_

#include "sgsui_GUIStaticData.h"
#include <QLabel>

class QTimer;

namespace sgsui {

class SmartStatusBar : public QLabel
{
public:
	explicit SmartStatusBar(QWidget * parent = 0);
	void showStatusBarMessage(const QString &tip, bool richText = false, int duration = GUIStaticData::showTipDuration);
	void clearMessage();

private:
	QTimer *				m_timer;
	const static int		ms_fixedHeight;
};

} /* namespace sgsui */
#endif /* SGSUI_SMARTSTATUSBAR_H_ */
