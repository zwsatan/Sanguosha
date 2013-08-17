#include "sgsai_CardSort.h"

namespace sgsai {

CardSort::CardSort(CardAlgorithm algorithm)
	: m_algorithm(algorithm)
{
}

bool CardSort::operator()(sgs::DataType::Card * /*card1*/, sgs::DataType::Card * /*card2*/)
{
	// TODO complete it
	return false;
}

} /* namespace sgsai */
