#include "sgsui_ZhuangbeiLabel.h"
#include "sgsui_nameConvert.h"
#include <QTimer>

namespace sgsui {

const QSize ZhuangbeiLabel::ms_fixedSize(150, 50);

ZhuangbeiLabel::ZhuangbeiLabel(QWidget * parent/* = 0*/)
	: QLabel(parent)
{
	m_zhuangbeiTimer = new QTimer(this);
	m_zhuangbeiTimer->setSingleShot(true);
	connect(m_zhuangbeiTimer, SIGNAL(timeout()), this, SLOT(hide()));
	setFont(GUIStaticData::equipFont);
	setFixedSize(ms_fixedSize);
}

void ZhuangbeiLabel::showZhuangbei(sgs::ConstData::CardType equipCard, int equipDuration/* = GUIStaticData::showEquipDuration*/)
{
	if (equipDuration < 0)
		equipDuration = GUIStaticData::showEquipDuration;

	setText(setColored(cardDisplayName(equipCard), false));
	m_zhuangbeiTimer->start(equipDuration);
	show();
}

} /* namespace sgsui */
