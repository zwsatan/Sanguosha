#include "sgsui_ZhuangbeiLabel.h"
#include "sgsui_nameConvert.h"
#include <QTimer>

namespace sgsui {

const QSize ZhuangbeiLabel::fixedSize(150, 50);

ZhuangbeiLabel::ZhuangbeiLabel(QWidget * parent/* = 0*/)
	: QLabel(parent)
{
	zhuangbeiTimer = new QTimer(this);
	zhuangbeiTimer->setSingleShot(true);
	connect(zhuangbeiTimer, SIGNAL(timeout()), this, SLOT(hide()));
	setFont(GUIStaticData::equipFont);
	setFixedSize(fixedSize);
}

void ZhuangbeiLabel::showZhuangbei(sgs::ConstData::CardType equipCard, int equipDuration/* = GUIStaticData::showEquipDuration*/) {
	if (equipDuration < 0)
		equipDuration = GUIStaticData::showEquipDuration;

	setText(setColored(cardDisplayName(equipCard), false));
	zhuangbeiTimer->start(equipDuration);
	show();
}

} /* namespace sgsui */
