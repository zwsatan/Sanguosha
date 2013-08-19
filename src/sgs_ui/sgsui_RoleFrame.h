#ifndef SGSUI_ROLEFRAME_H_
#define SGSUI_ROLEFRAME_H_

#include "ui_RoleFrame.h"
#include "sgsdata_enum.h"

namespace sgsui {

class RoleFrame : public QFrame, protected Ui::RoleFrame
{
public:
	explicit RoleFrame(QWidget * parent = 0);
	void setupRole(int zhugongNum, int zhongchenNum, int fanzeiNum, int neijianNum);
	void setDead(sgs::ConstData::PlayerRole killedPlayerRole);

private:
	const static QSize		ms_fixedSize;
	QLabel *				m_roleLabel[8];
	int						m_zhugongNum;
	int						m_zhongchenNum;
	int						m_fanzeiNum;
	int						m_neijianNum;
	int						m_aliveZhugongNum;
	int						m_aliveZhongchenNum;
	int						m_aliveFanzeiNum;
	int						m_aliveNeijianNum;
};

} /* namespace sgsui */
#endif /* SGSUI_ROLEFRAME_H_ */
