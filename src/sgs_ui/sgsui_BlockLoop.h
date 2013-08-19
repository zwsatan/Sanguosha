#ifndef SGSUI_BLOCKLOOP_H_
#define SGSUI_BLOCKLOOP_H_

#include <QEventLoop>
#include <list>

namespace sgsui {

class BlockLoop
{
public:
	void exec();
	bool isRunning() const;
	void quit();
	static void terminateAllLoops();

private:
	QEventLoop							m_blocker;

	static std::list<QEventLoop *>		ms_blockLoopList;
	static bool							ms_isTerminated;
};

} /* namespace sgsui */
#endif /* SGSUI_BLOCKLOOP_H_ */
