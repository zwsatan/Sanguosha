#include "sgsui_BlockTimer.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_output.h"
#include <QTimer>

namespace sgsui {

BlockTimer::BlockTimer(QObject *parent)
	: QObject(parent)
{
	m_blockTimer = new QTimer(this);
	m_blockTimer->setSingleShot(true);
	connect(m_blockTimer, SIGNAL(timeout()), this, SLOT(unblock()));
}

BlockTimer::~BlockTimer()
{
}

void BlockTimer::block(int blockTime)
{
	if (m_blockLoop.isRunning())
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "BlockTimer::block: it is already blocked");
		return;
	}

	if (blockTime <= 0)
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "BlockTimer::block: invalid blockTime");
		blockTime = GUIStaticData::basicAnimationDuration;
	}

	m_blockTimer->start(blockTime);
	printDebug("<font color=red><b>Notice: </b></font>"
			   "BlockTimer::block: block start");
	m_blockLoop.exec();
	printDebug("<font color=red><b>Notice: </b></font>"
			   "BlockTimer::block: block over");
}

void BlockTimer::unblock()
{
	m_blockLoop.quit();
}

} /* namespace sgsui */
