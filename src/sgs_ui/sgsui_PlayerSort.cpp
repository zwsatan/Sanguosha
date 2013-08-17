#include "sgsui_PlayerSort.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_output.h"
#include "sgsdata_extern.h"
#include "sgsdata_gamedata.h"

namespace sgsui {

PlayerSort::PlayerSort(int mySeat, ArrangeType arrangeType)
	: m_mySeat(mySeat)
	, m_arrangeType(arrangeType)
	, m_myself(sgs::ExternData::gamedata.players(mySeat))
{
}

bool PlayerSort::operator()(sgs::DataType::Player * player1, sgs::DataType::Player * player2) const
{
	if (!player1 || !player2)
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "PlayerSort::operator(): null pointer");
		return false;
	}

	if (m_arrangeType == BySeat)
	{
		int playerSeat1 = player1->seat();
		int playerSeat2 = player2->seat();
		if (playerSeat1 < m_mySeat)
			playerSeat1 += GUIStaticData::getPlayerCount();
		if (playerSeat2 < m_mySeat)
			playerSeat2 += GUIStaticData::getPlayerCount();
		return (playerSeat1 < playerSeat2);
	}
	else if (m_arrangeType == ByThreat)
	{
		//return true means that player1 is less threating than player2
		int threat1 = threatingIndex(m_myself, player1);
		int threat2 = threatingIndex(m_myself, player2);
		return threat1 < threat2;
	}
	else
	{
		printDebug("<font color=red><b>Warning: </b></font>"
				   "PlayerSort::operator(): invalid type");
		return false;
	}
}

} /* namespace sgsui */
