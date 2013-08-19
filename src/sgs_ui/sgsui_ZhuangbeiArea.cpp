#include "sgsui_ZhuangbeiArea.h"
#include "sgsui_ZhuangbeiPic.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_output.h"

namespace sgsui {

const QSize ZhuangbeiArea::ms_fixedSize(164, 170);

ZhuangbeiArea::ZhuangbeiArea(MainWindow * mainWindowPtr, QWidget * parent)
	: QWidget(parent)
	, m_mainWindow(mainWindowPtr)
	, m_wuqiCard(0)
	, m_fangjuCard(0)
	, m_jianmaCard(0)
	, m_jiamaCard(0)
	, m_wuqiPic(0)
	, m_fangjuPic(0)
	, m_jianmaPic(0)
	, m_jiamaPic(0)
	, m_lebuPixmap("images/status/indulgence.png")				// 乐不思蜀图片
	, m_shandianPixmap("images/status/lightning.png")			// 闪电图片
	, m_bingliangPixmap("images/status/supply_shortage.png")	// 兵粮寸断图片缺失
{
	setupUi(this);
	setFixedSize(ms_fixedSize);

	m_judgeLabel[0] = judge0;
	m_judgeLabel[1] = judge1;
	m_judgeLabel[2] = judge2;

	judge0->hide();
	judge1->hide();
	judge2->hide();
}

const CardFrame * ZhuangbeiArea::getWuqi() const
{
	return m_wuqiCard;
}

const CardFrame * ZhuangbeiArea::getFangju() const
{
	return m_fangjuCard;
}

const CardFrame * ZhuangbeiArea::getJianma() const
{
	return m_jianmaCard;
}

const CardFrame * ZhuangbeiArea::getJiama() const
{
	return m_jiamaCard;
}

ZhuangbeiPic * ZhuangbeiArea::getWuqiPic() const
{
	return m_wuqiPic;
}

ZhuangbeiPic * ZhuangbeiArea::getFangjuPic() const
{
	return m_fangjuPic;
}

ZhuangbeiPic * ZhuangbeiArea::getJianmaPic() const
{
	return m_jianmaPic;
}

ZhuangbeiPic * ZhuangbeiArea::getJiamaPic() const
{
	return m_jiamaPic;
}

bool ZhuangbeiArea::isSelected(int zhuangbeiIndex)
{
	switch (zhuangbeiIndex)
	{
	case 0:
		if (m_wuqiPic)
			m_wuqiPic->isSelected();
		else
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::isSelected: trying to get the status of weapon, while there is no weapon!");
		break;
	case 1:
		if (m_fangjuPic)
			m_fangjuPic->isSelected();
		else
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::isSelected: trying to get the status of armor, while there is no armor!");
		break;
	case 2:
		if (m_jianmaPic)
			m_jianmaPic->isSelected();
		else
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::isSelected: trying to get the status of dfdhorse, while there is no dfdhorse!");
		break;
	case 3:
		if (m_jiamaPic)
			m_jiamaPic->isSelected();
		else
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::isSelected: trying to get the status of atkhorse, while there is no atkhorse!");
		break;
	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "ZhuangbeiArea::isSelected: invalid index");
		break;
	}
	return true;
}

bool ZhuangbeiArea::isSelectable(int zhuangbeiIndex)
{
	switch (zhuangbeiIndex)
	{
	case 0:
		if (m_wuqiPic)
			return m_wuqiPic->isSelectable();
		printDebug("<font color=red><b>Warning: </b></font>"
				   "ZhuangbeiArea::isSelectable: trying to get the status of weapon, while there is no weapon!");
		break;
	case 1:
		if (m_fangjuPic)
			m_fangjuPic->isSelectable();
		else
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::isSelectable: trying to get the status of armor, while there is no armor!");
		break;
	case 2:
		if (m_jianmaPic)
			m_jianmaPic->isSelectable();
		else
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::isSelectable: trying to get the status of dfdhorse, while there is no dfdhorse!");
		break;
	case 3:
		if (m_jiamaPic)
			m_jiamaPic->isSelectable();
		else
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::isSelectable: trying to get the status of atkhorse, while there is no atkhorse!");
		break;
	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "ZhuangbeiArea::isSelectable: invalid index");
		break;
	}
	return true;
}

void ZhuangbeiArea::setSelected(int zhuangbeiIndex, bool selected, bool force/* = false*/)
{
	switch (zhuangbeiIndex)
	{
	case 0:
		if (m_wuqiPic)
			m_wuqiPic->setSelected(selected);
		else if (!force)
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::setSelected: trying to change the status of weapon, while there is no weapon!");
		break;
	case 1:
		if (m_fangjuPic)
			m_fangjuPic->setSelected(selected);
		else if (!force)
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::setSelected: trying to change the status of armor, while there is no armor!");
		break;
	case 2:
		if (m_jianmaPic)
			m_jianmaPic->setSelected(selected);
		else if (!force)
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::setSelected: trying to change the status of dfdhorse, while there is no dfdhorse!");
		break;
	case 3:
		if (m_jiamaPic)
			m_jiamaPic->setSelected(selected);
		else if (!force)
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::setSelected: trying to change the status of atkhorse, while there is no atkhorse!");
		break;
	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "ZhuangbeiArea::setSelected: invalid index");
		break;
	}
}

void ZhuangbeiArea::setSelectable(int zhuangbeiIndex, bool selectable, bool force/* = false*/)
{
	switch (zhuangbeiIndex)
	{
	case 0:
		if (m_wuqiPic)
			m_wuqiPic->setSelectable(selectable);
		else if (!force)
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::setSelectable: trying to change the status of weapon, while there is no weapon!");
		break;

	case 1:
		if (m_fangjuPic)
			m_fangjuPic->setSelectable(selectable);
		else if (!force)
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::setSelectable: trying to change the status of armor, while there is no armor!");
		break;

	case 2:
		if (m_jianmaPic)
			m_jianmaPic->setSelectable(selectable);
		else if (!force)
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::setSelectable: trying to change the status of dfdhorse, while there is no dfdhorse!");
		break;

	case 3:
		if (m_jiamaPic)
			m_jiamaPic->setSelectable(selectable);
		else if (!force)
			printDebug("<font color=red><b>Warning: </b></font>"
					   "ZhuangbeiArea::setSelectable: trying to change the status of atkhorse, while there is no atkhorse!");
		break;

	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "ZhuangbeiArea::setSelectable: invalid index");
		break;
	}
}

void ZhuangbeiArea::addWuqi(CardFrame * newWuqiCard)
{
	if (m_wuqiCard)
	{
		delete m_wuqiCard;
		delete m_wuqiPic;
	}

	m_wuqiCard = newWuqiCard;
	m_wuqiPic = new ZhuangbeiPic(m_wuqiCard, m_mainWindow, wuqiWidget);
	m_wuqiPic->show();
}

void ZhuangbeiArea::addFangju(CardFrame * newFangjuCard)
{
	if (m_fangjuCard)
	{
		delete m_fangjuCard;
		delete m_fangjuPic;
	}

	m_fangjuCard = newFangjuCard;
	m_fangjuPic = new ZhuangbeiPic(m_fangjuCard, m_mainWindow, fangjuWidget);
	m_fangjuPic->show();
}

void ZhuangbeiArea::addJianma(CardFrame * newJianmaCard)
{
	if (m_jianmaCard)
	{
		delete m_jianmaCard;
		delete m_jianmaPic;
	}

	m_jianmaCard = newJianmaCard;
	m_jianmaPic = new ZhuangbeiPic(m_jianmaCard, m_mainWindow, jianmaWidget);
	m_jianmaPic->show();
}

void ZhuangbeiArea::addJiama(CardFrame *newJiamaCard)
{
	if (m_jiamaCard)
	{
		delete m_jiamaCard;
		delete m_jiamaPic;
	}

	m_jiamaCard = newJiamaCard;
	m_jiamaPic = new ZhuangbeiPic(m_jiamaCard, m_mainWindow, jiamaWidget);
	m_jiamaPic->show();
}

void ZhuangbeiArea::removeWuqi()
{
	if (m_wuqiCard)
	{
		delete m_wuqiCard;
		delete m_wuqiPic;
	}

	m_wuqiPic = 0;
	m_wuqiCard = 0;
}

void ZhuangbeiArea::removeFangju()
{
	if (m_fangjuCard)
	{
		delete m_fangjuCard;
		delete m_fangjuPic;
	}

	m_fangjuPic = 0;
	m_fangjuCard = 0;
}

void ZhuangbeiArea::removeJianma()
{
	if (m_jianmaCard)
	{
		delete m_jianmaCard;
		delete m_jianmaPic;
	}

	m_jianmaPic = 0;
	m_jianmaCard = 0;
}

void ZhuangbeiArea::removeJiama()
{
	if (m_jiamaCard)
	{
		delete m_jiamaCard;
		delete m_jiamaPic;
	}

	m_jiamaPic = 0;
	m_jiamaCard = 0;
}

void ZhuangbeiArea::addJudge(sgs::ConstData::CardType judgeCard)
{
	m_judgeList.push_back(judgeCard);
	refreshJudge();
}

void ZhuangbeiArea::removeJudge(sgs::ConstData::CardType judgeCard)
{
	m_judgeList.remove(judgeCard);
	refreshJudge();
}

void ZhuangbeiArea::refreshJudge()
{
	for (int i = 0; i < 3; ++i)
		m_judgeLabel[i]->hide();

	int index = 0;
	for (auto iter = m_judgeList.begin(); iter != m_judgeList.end(); ++iter)
	{
		const QPixmap * pix =	NULL;
		switch (*iter)
		{
		case sgs::ConstData::SHANDIAN:
			pix = &m_shandianPixmap;
			break;
		case sgs::ConstData::SISHU:
			pix = &m_lebuPixmap;
			break;
		case sgs::ConstData::BINGLIANG:
			pix = &m_bingliangPixmap;
			break;
		default:
			break;
		}

		if (pix)
		{
			m_judgeLabel[index]->setPixmap(*pix);
			m_judgeLabel[index]->show();
			++index;
		}
	}
}

} /* namespace sgsui */
