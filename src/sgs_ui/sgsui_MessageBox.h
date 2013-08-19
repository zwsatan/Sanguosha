#ifndef SGSUI_MESSAGEBOX_H_
#define SGSUI_MESSAGEBOX_H_

#include "sgsui_GUIStaticData.h"
#include <QLabel>

class QGraphicsOpacityEffect;

namespace sgsui {

class SGSMessageBox : public QLabel
{
	Q_OBJECT

public:
	explicit SGSMessageBox(QWidget * parent = 0);

public slots:
	void showTipMessage(const QString &tip, bool richText = false, int duration = GUIStaticData::showTipDuration);
	void showPauseMessage(const QString &pauseText, bool richText = false);

private slots:
	void startFading();
	void fading();

private:
	qreal							m_opacity;
	QTimer *						m_showTextTimer;
	QTimer *						m_fadeTimer;
	QGraphicsOpacityEffect *		m_tipEffect;
	int								m_textDuration;

	const static QSize				ms_fixedSize;
};

} /* namespace sgsui */
#endif /* SGSUI_MESSAGEBOX_H_ */
