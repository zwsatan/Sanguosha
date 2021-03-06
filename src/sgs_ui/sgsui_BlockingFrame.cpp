#include "sgsui_BlockingFrame.h"
#include "sgsui_MainWindow.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_output.h"

namespace sgsui {

BlockingFrame::BlockingFrame(QWidget * parent/* = 0*/)
	: QFrame(parent)
{
	setFont(GUIStaticData::headingFont);
}

void BlockingFrame::enterKeyPressedEvent()
{
	// this function should be overrided by any child class that need to process Enter key
}

void BlockingFrame::escapeKeyPressedEvent()
{
	// this function should be overrided by any child class that need to process ESC key
}

void BlockingFrame::exec(bool hide/* = true*/)
{
	if (!blockLoop.isRunning())
	{
		static_cast<MainWindow*>(window())->setResponding(false, this);
		show();
		raise();
		printDebug("<font color=red><b>Notice: </b></font>"
				   "BlockingFrame::exec: Blocking frame is blocked");
		blockLoop.exec();
		printDebug("<font color=red><b>Notice: </b></font>"
				   "BlockingFrame::exec: Blocking frame is unblocked");
		static_cast<MainWindow*>(window())->setResponding(true, this);
		setVisible(!hide);
	}
	else
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "BlockingFrame::exec: "
				   "Message Ignored because another DialogBox is still running");
	}
}

void BlockingFrame::unblock()
{
	blockLoop.quit();
}

} /* namespace sgsui */
