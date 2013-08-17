#include "sgsui_PixmapAnimationWidget.h"
#include "sgsui_output.h"
#include <QTimer>

namespace sgsui {

PixmapAnimationWidget::PixmapAnimationWidget(QWidget * parent/* = 0*/)
	: QLabel(parent)
{
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/0.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/1.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/2.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/3.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/4.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/5.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/6.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/7.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/8.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/9.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/10.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/11.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/12.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/13.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/14.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/15.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/16.png"));
	m_useBlackSlashPixmapList.push_back(new QPixmap("images/animation/slash_black/17.png"));

	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/0.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/1.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/2.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/3.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/4.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/5.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/6.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/7.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/8.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/9.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/10.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/11.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/12.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/13.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/14.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/15.png"));
	m_useRedSlashPixmapList.push_back(new QPixmap("images/animation/slash_red/16.png"));

	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/0.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/1.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/2.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/3.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/4.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/5.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/6.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/7.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/8.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/9.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/10.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/11.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/12.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/13.png"));
	m_useFireSlashPixmapList.push_back(new QPixmap("images/animation/fire_slash/14.png"));

	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/0.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/1.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/2.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/3.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/4.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/5.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/6.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/7.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/8.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/9.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/10.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/11.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/12.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/13.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/14.png"));
	m_useThunderSlashPixmapList.push_back(new QPixmap("images/animation/thunder_slash/15.png"));

	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/0.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/1.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/2.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/3.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/4.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/5.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/6.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/7.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/8.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/9.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/10.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/11.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/12.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/12.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/12.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/12.png"));
	m_useJinkPixmapList.push_back(new QPixmap("images/animation/jink/12.png"));

	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/0.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/1.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/2.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/3.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/4.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/5.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/6.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/7.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/8.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/9.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/10.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/11.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/12.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/13.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/14.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/15.png"));
	m_usePeachPixmapList.push_back(new QPixmap("images/animation/peach/16.png"));

	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/0.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/1.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/2.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/3.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/4.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/5.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/6.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/7.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/8.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/9.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/10.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/11.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/12.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/13.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/14.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/15.png"));
	m_useAnalepticPixmapList.push_back(new QPixmap("images/animation/analeptic/16.png"));

	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/0.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/1.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/2.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/3.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));
	m_damagePixmapList.push_back(new QPixmap("images/animation/damage/4.png"));

	m_lightningPixmapList.push_back(new QPixmap("images/animation/lightning/0.png"));

	m_chainPixmapList.push_back(new QPixmap("images/animation/chain/0.png"));
	m_chainPixmapList.push_back(new QPixmap("images/animation/chain/1.png"));
	m_chainPixmapList.push_back(new QPixmap("images/animation/chain/2.png"));
	m_chainPixmapList.push_back(new QPixmap("images/animation/chain/3.png"));
	m_chainPixmapList.push_back(new QPixmap("images/animation/chain/4.png"));
	m_chainPixmapList.push_back(new QPixmap("images/animation/chain/5.png"));
	m_chainPixmapList.push_back(new QPixmap("images/animation/chain/6.png"));
	m_chainPixmapList.push_back(new QPixmap("images/animation/chain/7.png"));
	m_chainPixmapList.push_back(new QPixmap("images/animation/chain/8.png"));
	m_chainPixmapList.push_back(new QPixmap("images/animation/chain/9.png"));
	m_chainPixmapList.push_back(new QPixmap("images/animation/chain/10.png"));

	m_duelPixmapList.push_back(new QPixmap("images/animation/duel/0.png"));
	m_duelPixmapList.push_back(new QPixmap("images/animation/duel/1.png"));

	m_wuxiePixmapList.push_back(new QPixmap("images/animation/wuxie/0.png"));

	m_blackSlashSize	= QSize(300, 320);
	m_redSlashSize		= QSize(310, 320);
	m_fireSlashSize		= QSize(200, 258);
	m_thunderSlashSize	= QSize(640, 406);
	m_jinkSize			= QSize(128, 128);
	m_peachSize			= QSize(320, 314);
	m_analepticSize		= QSize(268, 263);
	m_damageSize		= QSize(172, 170);
	m_lightningSize		= QSize(112, 189);
	m_chainSize			= QSize(202, 180);
	m_duelSize			= QSize(180, 185);
	m_wuxieSize			= QSize(106, 130);

	m_basicTimer		= new QTimer(this);
	connect(m_basicTimer, SIGNAL(timeout()), this, SLOT(updatePixmap()));
}

PixmapAnimationWidget::~PixmapAnimationWidget()
{
	std::list<QPixmap*>::iterator iter;
	for (iter = m_useBlackSlashPixmapList.begin(); iter != m_useBlackSlashPixmapList.end(); ++iter)
		delete *iter;

	for (iter = m_useRedSlashPixmapList.begin(); iter != m_useRedSlashPixmapList.end(); ++iter)
		delete *iter;

	for (iter = m_useFireSlashPixmapList.begin(); iter != m_useFireSlashPixmapList.end(); ++iter)
		delete *iter;

	for (iter = m_useThunderSlashPixmapList.begin(); iter != m_useThunderSlashPixmapList.end(); ++iter)
		delete *iter;

	for (iter = m_useJinkPixmapList.begin(); iter != m_useJinkPixmapList.end(); ++iter)
		delete *iter;

	for (iter = m_usePeachPixmapList.begin(); iter != m_usePeachPixmapList.end(); ++iter)
		delete *iter;

	for (iter = m_useAnalepticPixmapList.begin(); iter != m_useAnalepticPixmapList.end(); ++iter)
		delete *iter;

	for (iter = m_damagePixmapList.begin(); iter != m_damagePixmapList.end(); ++iter)
		delete *iter;

	for (iter = m_lightningPixmapList.begin(); iter != m_lightningPixmapList.end(); ++iter)
		delete *iter;

	for (iter = m_chainPixmapList.begin(); iter != m_chainPixmapList.end(); ++iter)
		delete *iter;

	for (iter = m_duelPixmapList.begin(); iter != m_duelPixmapList.end(); ++iter)
		delete *iter;

	for (iter = m_wuxiePixmapList.begin(); iter != m_wuxiePixmapList.end(); ++iter)
		delete *iter;
}

void PixmapAnimationWidget::pixmapAnimation(const QPoint &position, PixmapAnimationType animationType)
{
	QSize pixmapSize;
	std::list<QPixmap*> * pixmapList = 0;
	switch (animationType)
	{
	case BlackSlashAnimation:
		pixmapList = &m_useBlackSlashPixmapList;
		pixmapSize = m_blackSlashSize;
		break;
	case RedSlashAnimation:
		pixmapList = &m_useRedSlashPixmapList;
		pixmapSize = m_redSlashSize;
		break;
	case PeachAnimation:
		pixmapList = &m_usePeachPixmapList;
		pixmapSize = m_peachSize;
		break;
	case FireSlashAnimation:
		pixmapList = &m_useFireSlashPixmapList;
		pixmapSize = m_fireSlashSize;
		break;
	case ThunderSlashAnimation:
		pixmapList = &m_useThunderSlashPixmapList;
		pixmapSize = m_thunderSlashSize;
		break;
	case JinkAnimation:
		pixmapList = &m_useJinkPixmapList;
		pixmapSize = m_jinkSize;
		break;
	case AnalepticAnimation:
		pixmapList = &m_useAnalepticPixmapList;
		pixmapSize = m_analepticSize;
		break;
	case DamageAnimation:
		pixmapList = &m_damagePixmapList;
		pixmapSize = m_damageSize;
		break;
	case LightningAnimation:
		pixmapList = &m_lightningPixmapList;
		pixmapSize = m_lightningSize;
		break;
	case ChainAnimation:
		pixmapList = &m_chainPixmapList;
		pixmapSize = m_chainSize;
		break;
	case DuelAnimation:
		pixmapList = &m_duelPixmapList;
		pixmapSize = m_duelSize;
		break;
	case WuxieAnimation:
		pixmapList = &m_wuxiePixmapList;
		pixmapSize = m_wuxieSize;
		break;
	default:
		printDebug("<font color=red><b>Warning: </b></font>"
				   "PixmapAnimationWidget::pixmapAnimation: "
				   "running PixmapAnimation of an invalid type");
		break;
	}

	int x = -pixmapSize.width() / 2;
	int y = -pixmapSize.height() / 2;
	setGeometry(QRect(position + QPoint(x, y), pixmapSize));
	setPixmap(*(pixmapList->front()));
	show();

	m_basicAnimationIter = ++(pixmapList->begin());
	m_endIter = pixmapList->end();
	m_basicTimer->start(GUIStaticData::basicAnimationDuration / int(pixmapList->size()));
}

void PixmapAnimationWidget::updatePixmap()
{
	if (m_basicAnimationIter != m_endIter)
	{
		QPixmap * pix = *m_basicAnimationIter;
		++m_basicAnimationIter;
		setPixmap(*pix);
	}
	else
	{
		m_basicTimer->stop();
		hide();
	}
}

} /* namespace sgsui */
