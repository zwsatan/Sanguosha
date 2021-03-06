#ifndef SGSUI_BLOCKTIMER_H_
#define SGSUI_BLOCKTIMER_H_

#include <QObject>
#include "sgsui_BlockLoop.h"

class QTimer;

namespace sgsui {

class BlockTimer : public QObject
{
	Q_OBJECT

public:
	explicit BlockTimer(QObject * parent);
	virtual ~BlockTimer();

	void block(int blockTime);

private slots:
	void unblock();

private:
	QTimer *		m_blockTimer;
	BlockLoop		m_blockLoop;
};

} /* namespace sgsui */
#endif /* SGSUI_BLOCKTIMER_H_ */
