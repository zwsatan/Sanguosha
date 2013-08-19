#include "sgsui_BlockLoop.h"
#include "sgsui_output.h"
#include <iostream>

namespace sgsui {

std::list<QEventLoop *> BlockLoop::ms_blockLoopList;
bool BlockLoop::ms_isTerminated = false;

void BlockLoop::exec()
{
	ms_blockLoopList.push_back(&m_blocker);
	m_blocker.exec();
	if (ms_isTerminated)
	{
		// this code is to make the thread sleep with little CPU cost
		while (true)
			std::cin.get();
	}
	else
	{
		ms_blockLoopList.remove(&m_blocker);
	}
}

bool BlockLoop::isRunning() const
{
	return m_blocker.isRunning();
}

void BlockLoop::quit()
{
	m_blocker.quit();
}

void BlockLoop::terminateAllLoops()
{
	ms_isTerminated = true;
	for (auto iter = ms_blockLoopList.begin(); iter != ms_blockLoopList.end(); ++iter)
		(*iter)->quit();
	printDebug("BlockLoop::terminateAllLoops: all loops terminated");
}

} /* namespace sgsui */
