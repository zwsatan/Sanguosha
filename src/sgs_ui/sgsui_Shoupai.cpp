#include "sgsui_Shoupai.h"
#include "sgsui_MainWindow.h"
#include "sgsui_output.h"
#include "sgsdata_card.h"

namespace sgsui {

Shoupai::Shoupai(const sgs::DataType::Card * card, AbstractCardContainer * container, QWidget * parent/* = 0*/)
	: CardButton(card, container, true, parent)
{
}

void Shoupai::mousePressEvent(QMouseEvent * event)
{
	if (static_cast<MainWindow*>(window())->isResponding())
		CardButton::mousePressEvent(event);
}

void Shoupai::mouseDoubleClickEvent(QMouseEvent * event)
{
	if (static_cast<MainWindow*>(window())->isResponding())
		CardButton::mouseDoubleClickEvent(event);
}

} /* namespace sgsui */
