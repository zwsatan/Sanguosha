#ifndef SGSUI_GAMEOVERBOX_H_
#define SGSUI_GAMEOVERBOX_H_

#include "ui_GameOverBox.h"
#include "sgsui_BlockingFrame.h"
#include <vector>

class QLabel;
class QFrame;

namespace sgsui {

class ResultPlayerLabel;

class GameOverBox : public BlockingFrame, protected Ui::GameOverBox
{
public:
	explicit GameOverBox(QWidget * parent = 0);
	void showResult(char result);
	virtual void enterKeyPressedEvent();

private:
	bool getResult(int Index);

private:
	char									m_result;

	std::vector<QLabel *>					m_roleLabelVec;
	std::vector<ResultPlayerLabel *>		m_resultPixmapVec;
	std::vector<QLabel *>					m_resultLabelVec;
	std::vector<QFrame *>					m_resultFrameVec;
	const static QSize						ms_fixedSize;
};

} /* namespace sgsui */
#endif /* SGSUI_GAMEOVERBOX_H_ */
