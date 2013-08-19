#ifndef SGSUI_ABSTRACTDRAGPROCESSOR_H_
#define SGSUI_ABSTRACTDRAGPROCESSOR_H_

namespace sgsui {

class DragableCardFrame;

class AbstractDragProcessor
{
public:
	virtual ~AbstractDragProcessor() {}

	virtual QPoint processDrag(DragableCardFrame * card) = 0;
};

} /* namespace sgsui */
#endif /* SGSUI_ABSTRACTDRAGPROCESSOR_H_ */
