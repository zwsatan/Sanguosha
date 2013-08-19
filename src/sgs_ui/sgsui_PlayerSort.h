#ifndef SGSUI_PLAYERSORT_H_
#define SGSUI_PLAYERSORT_H_

#include "sgsdata_player.h"

namespace sgs {
namespace DataType {
class Player;
}
}

namespace sgsui {

enum ArrangeType
{
	BySeat,		// 按座位排序
	ByThreat	// 按威胁性排序
};

class PlayerSort
{
public:
	PlayerSort(int mySeat, ArrangeType arrangeType);

	bool operator()(sgs::DataType::Player * player1, sgs::DataType::Player * player2) const;

private:
	const int									m_mySeat;
	const ArrangeType							m_arrangeType;
	const sgs::DataType::Player * const			m_myself;
};

// 危险性比较,估计AI也是依靠排序,来选择需要攻击的目标
inline int threatingIndex(const sgs::DataType::Player * myself, const sgs::DataType::Player * player)
{
	// to accelerate sort, don't check whether the point is 0
	using namespace sgs::ConstData;
	PlayerRole myRole = myself->role();
	PlayerRole playerRole = player->role();
	int threat = 0;

	switch (myRole)
	{
	case ZHU:
	case ZHONG:
		switch (playerRole)
		{
		case ZHU:
			threat = -200;
			break;
		case ZHONG:
			threat = -100;
			break;
		case FAN:
			threat = 100;
			break;
		case NEI:
			threat = 95;
			break;
		default:
			break;
		}
		break;

	case FAN:
		switch (playerRole)
		{
		case ZHU:
			threat = 200;
			break;
		case ZHONG:
			threat = 100;
			break;
		case FAN:
			threat = -100;
			break;
		case NEI:
			threat = 95;
			break;
		default:
			break;
		}
		break;

	case NEI:
		switch (playerRole)
		{
		case ZHU:
			threat = 95;
			break;
		case ZHONG:
			threat = 100;
			break;
		case FAN:
			threat = 100;
			break;
		case NEI:
			threat = -100;
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}

	threat -= myself->distance(player);
	threat -= player->hp();
	return threat;
}

} /* namespace sgsui */
#endif /* SGSUI_PLAYERSORT_H_ */
