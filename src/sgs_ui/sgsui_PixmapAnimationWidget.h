#ifndef SGSUI_PIXMAPANIMATIONWIDGET_H_
#define SGSUI_PIXMAPANIMATIONWIDGET_H_

#include <QLabel>
#include <list>
#include <QSize>
#include "sgsui_GUIStaticData.h"

class QPixmap;
class QTimer;

namespace sgsui {

class PixmapAnimationWidget : public QLabel
{
Q_OBJECT
public:
	explicit PixmapAnimationWidget(QWidget * parent = 0);
	~PixmapAnimationWidget();

public slots:
	void pixmapAnimation(const QPoint &position, PixmapAnimationType animationType);

private slots:
	void updatePixmap();

private:
	std::list<QPixmap *>				m_useBlackSlashPixmapList;			// 黑杀图片
	std::list<QPixmap *>				m_useRedSlashPixmapList;			// 红杀图片
	std::list<QPixmap *>				m_useFireSlashPixmapList;			// 火杀图片缺失
	std::list<QPixmap *>				m_useThunderSlashPixmapList;		// 雷杀图片缺失
	std::list<QPixmap *>				m_usePeachPixmapList;				// 使用桃图片
	std::list<QPixmap *>				m_useJinkPixmapList;				// 使用闪图片
	std::list<QPixmap *>				m_useAnalepticPixmapList;			//
	std::list<QPixmap *>				m_damagePixmapList;					// 造成伤害时的图片
	std::list<QPixmap *>				m_lightningPixmapList;				// 闪电造成伤害时的图片
	std::list<QPixmap *>				m_chainPixmapList;					// 铁索图片缺失
	std::list<QPixmap *>				m_duelPixmapList;					// 决斗时的图片
	std::list<QPixmap *>				m_wuxiePixmapList;					// 无懈可击时的图片
	std::list<QPixmap *>::iterator		m_basicAnimationIter;
	std::list<QPixmap *>::iterator		m_endIter;

	QSize								m_blackSlashSize;
	QSize								m_redSlashSize;
	QSize								m_fireSlashSize;
	QSize								m_thunderSlashSize;
	QSize								m_jinkSize;
	QSize								m_peachSize;
	QSize								m_analepticSize;
	QSize								m_damageSize;
	QSize								m_lightningSize;
	QSize								m_chainSize;
	QSize								m_duelSize;
	QSize								m_wuxieSize;
	QTimer *							m_basicTimer;
};

} /* namespace sgsui */
#endif /* SGSUI_PIXMAPANIMATIONWIDGET_H_ */
