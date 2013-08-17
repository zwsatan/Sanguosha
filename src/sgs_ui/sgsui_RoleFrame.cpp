#include "sgsui_RoleFrame.h"
#include "sgsui_nameConvert.h"

namespace sgsui {

const QSize RoleFrame::ms_fixedSize(190, 70);

RoleFrame::RoleFrame(QWidget * parent/* = 0*/)
	: QFrame(parent)
	, m_zhugongNum(0)
	, m_zhongchenNum(0)
	, m_fanzeiNum(0)
	, m_neijianNum(0)
	, m_aliveZhugongNum(0)
	, m_aliveZhongchenNum(0)
	, m_aliveFanzeiNum(0)
	, m_aliveNeijianNum(0)
{
	setupUi(this);
	setFixedSize(ms_fixedSize);
	setObjectName("roleFrame");
	setStyleSheet("QFrame#roleFrame{background-image:url(images/frame/role.png)}");

	m_roleLabel[0] = roleLabel0;
	m_roleLabel[1] = roleLabel1;
	m_roleLabel[2] = roleLabel2;
	m_roleLabel[3] = roleLabel3;
	m_roleLabel[4] = roleLabel4;
	m_roleLabel[5] = roleLabel5;
	m_roleLabel[6] = roleLabel6;
	m_roleLabel[7] = roleLabel7;

	for (int i = 0; i < 8; ++i)
		m_roleLabel[i]->hide();
}

void RoleFrame::setupRole(int zhugongNum, int zhongchenNum, int fanzeiNum, int neijianNum)
{
	m_zhugongNum = zhugongNum;
	m_zhongchenNum = zhongchenNum;
	m_fanzeiNum = fanzeiNum;
	m_neijianNum = neijianNum;

	m_aliveZhugongNum = m_zhugongNum;
	m_aliveZhongchenNum = m_zhongchenNum;
	m_aliveFanzeiNum = m_fanzeiNum;
	m_aliveNeijianNum = m_neijianNum;

	using namespace sgs::ConstData;
	int count = 0;
	for (int i = 0; i < m_zhugongNum; ++i, ++count)
	{
		m_roleLabel[count]->setPixmap(roleSmallPixmapStr(ZHU));
		m_roleLabel[count]->show();
	}
	for (int i = 0; i < m_zhongchenNum; ++i, ++count)
	{
		m_roleLabel[count]->setPixmap(roleSmallPixmapStr(ZHONG));
		m_roleLabel[count]->show();
	}
	for (int i = 0; i < m_fanzeiNum; ++i, ++count)
	{
		m_roleLabel[count]->setPixmap(roleSmallPixmapStr(FAN));
		m_roleLabel[count]->show();
	}
	for (int i = 0; i < m_neijianNum; ++i, ++count)
	{
		m_roleLabel[count]->setPixmap(roleSmallPixmapStr(NEI));
		m_roleLabel[count]->show();
	}
}

void RoleFrame::setDead(sgs::ConstData::PlayerRole killedPlayerRole)
{
	using namespace sgs::ConstData;
	int index = 0;
	switch (killedPlayerRole)
	{
	case ZHU:
		index = --m_aliveZhugongNum;
		m_roleLabel[index]->setPixmap(roleSmallPixmapStr(ZHU, true));
		break;
	case ZHONG:
		index = m_zhugongNum + (--m_aliveZhongchenNum);
		m_roleLabel[index]->setPixmap(roleSmallPixmapStr(ZHONG, true));
		break;
	case FAN:
		index = m_zhugongNum + m_zhongchenNum + (--m_aliveFanzeiNum);
		m_roleLabel[index]->setPixmap(roleSmallPixmapStr(FAN, true));
		break;
	case NEI:
		index = m_zhugongNum + m_zhongchenNum + m_fanzeiNum + (--m_aliveNeijianNum);
		m_roleLabel[index]->setPixmap(roleSmallPixmapStr(NEI, true));
		break;
	default:
		break;
	}
}

} /* namespace sgsui */
