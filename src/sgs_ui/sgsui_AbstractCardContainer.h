#ifndef SGSUI_ABSTRACTCARDCONTAINER_H_
#define SGSUI_ABSTRACTCARDCONTAINER_H_

#include <vector>
#include <utility>
#include "sgsdata_enum.h"

namespace sgsui {

typedef std::pair<sgs::ConstData::PlayerRegionType, int> CardPosPair;
typedef std::vector<CardPosPair> CardPosPairVec;

class CardButton;

class AbstractCardContainer
{
public:
	virtual ~AbstractCardContainer() {}

	virtual void cardClicked(CardButton* clickedCard) = 0;
};

} /* namespace sgsui */
#endif /* SGSUI_ABSTRACTCARDCONTAINER_H_ */
