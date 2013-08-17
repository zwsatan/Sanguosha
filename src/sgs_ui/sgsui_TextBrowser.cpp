#include "sgsui_TextBrowser.h"
#include <QtGui>

namespace sgsui {

const QString TextBrowser::ms_stylesheet(
		"QTextBrowser{background-image:url(images/frame/textbackground.png);}"
		"QScrollBar:vertical {margin: 13px 0px 13px 0px; border: 0px; width: 12px;}"
		".QScrollBar::add-page:vertical {background-color: rgb(76, 77, 67)}"
		".QScrollBar::sub-page:vertical {background-color: rgb(76, 77, 67)}"
		".QScrollBar::handle:vertical {border-image:url(images/frame/scroll-area.png);}"
		".QScrollBar::sub-line:vertical {subcontrol-origin: margin; border: 0px; height:12px}"
		".QScrollBar::up-arrow:vertical {border-image:url(images/frame/scroll-up-arrow.png); height:12px}"
		".QScrollBar::down-arrow:vertical {border-image:url(images/frame/scroll-down-arrow.png); height:12px}"
		".QScrollBar::add-line:vertical {subcontrol-origin: margin; border: 0px; height:12px}");

TextBrowser::TextBrowser(QWidget * parent/* = 0*/)
	: QTextBrowser(parent)
{
	setStyleSheet(ms_stylesheet);
}

} /* namespace sgsui */
