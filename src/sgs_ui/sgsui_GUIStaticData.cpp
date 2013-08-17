#include "sgsui_GUIStaticData.h"
#include "sgsui_BlockLoop.h"
#include "sgsui_AudioPlayer.h"
#include "sgsui_output.h"
#include "sgsdata_extern.h"
#include "sgsdata_gamedata.h"
#include "sgsdata_player.h"
#include "sgsdata_platform.h"
#include "sgsdata_history.h"
#include "sgsdata_skill.h"
#include <QApplication>
#include <QFontDatabase>
#include <cstdlib>

namespace sgsui {

bool GUIStaticData::m_playersCreated			= false;
bool GUIStaticData::m_debugLogOn				= false;
bool GUIStaticData::m_gameOver					= false;
int GUIStaticData::m_options					= 0;
int GUIStaticData::m_package					= 0;
int GUIStaticData::m_fullPlayerCount			= 2;
int GUIStaticData::m_humanPlayerIndex			= 1;

const int GUIStaticData::sujiangDefaultHP		= 4;
const sgs::ConstData::PlayerRole GUIStaticData::sujiangDefaultRole		= sgs::ConstData::ZHONG;
const sgs::ConstData::HeroType GUIStaticData::sujiangDefaultHeroType	= sgs::ConstData::heroTypeNone;
const sgs::ConstData::HeroSex GUIStaticData::sujiangDefaultSex			= sgs::ConstData::MALE;
const sgs::ConstData::HeroSide GUIStaticData::sujiangDefaultSide		= sgs::ConstData::QUN;

const int GUIStaticData::initWaitTime					= 1000;
const int GUIStaticData::useCardAnimationDuration		= 1000;
const int GUIStaticData::basicAnimationDuration			= 1000;
const int GUIStaticData::damageAnimationDuration		= 500;
const int GUIStaticData::lineAnimationDuration			= 500;
const int GUIStaticData::blockingFrameMovingDuration	= 100;
const int GUIStaticData::showTipDuration				= 2000;
const int GUIStaticData::dialogFadeDuration				= 250;
const int GUIStaticData::switchPhaseDuration			= 300;
const int GUIStaticData::skillBlockDuration				= 1000;
const int GUIStaticData::showSkillDuration				= 2000;
const int GUIStaticData::showEquipDuration				= 1000;
const int GUIStaticData::createPlayerDuration			= 500;
const int GUIStaticData::dyingDuration					= 800;
const int GUIStaticData::killDuration					= 1000;
const int GUIStaticData::showRecoverDuration			= 800;

QFont GUIStaticData::appFont("Microsoft YaHei", 11);
QFont GUIStaticData::buttonFont("FZLiBian-S02", 12);
QFont GUIStaticData::headingFont("FZLiBian-S02", 28);
QFont GUIStaticData::contentFont("FZLiBian-S02", 16);
QFont GUIStaticData::statusbarFont("Microsoft YaHei", 10);
QFont GUIStaticData::skillFont("FZLiBian-S02", 30);
QFont GUIStaticData::equipFont("FZLiBian-S02", 30);

const QString GUIStaticData::startingGame(QObject::trUtf8("正在启动三国杀EX..."));
const QString GUIStaticData::initWindowWidgets(QObject::trUtf8("正在初始化用户界面……"));
const QString GUIStaticData::initStyleSheet(QObject::trUtf8("正在初始化样式表……"));
const QString GUIStaticData::initAnimation(QObject::trUtf8("正在初始化动画……"));
const QString GUIStaticData::initMusic(	QObject::trUtf8("正在初始化声音……"));
const QString GUIStaticData::initDebug(	QObject::trUtf8("正在初始化调试器……"));
const QString GUIStaticData::initOver(QObject::trUtf8("初始化完毕，正在打开游戏界面……"));
const QString GUIStaticData::trustTurnedOn(QObject::trUtf8("已开启托管模式，下次响应时生效"));
const QString GUIStaticData::trustTurnedOff(QObject::trUtf8("已关闭托管模式"));
const QString GUIStaticData::debugTurnedOn(QObject::trUtf8("已开启调试模式"));
const QString GUIStaticData::debugTurnedOff(QObject::trUtf8("已关闭调试模式"));
const QString GUIStaticData::yourRoleIsZhu(QObject::trUtf8("您的身份是主公，请选择武将"));
const QString GUIStaticData::yourRoleIsZhong(QObject::trUtf8("您的身份是忠臣，请选择武将"));
const QString GUIStaticData::yourRoleIsFan(QObject::trUtf8("您的身份是反贼，请选择武将"));
const QString GUIStaticData::yourRoleIsNei(QObject::trUtf8("您的身份是内奸，请选择武将"));
const QString GUIStaticData::gamePaused(QObject::trUtf8("游戏暂停，按F7键继续"));
const QString GUIStaticData::singleStep(QObject::trUtf8("单步调试，按F7继续"));
const QString GUIStaticData::quitOrNot(QObject::trUtf8("是否要退出三国杀EX ？"));
const QString GUIStaticData::pleaseChooseCard(QObject::trUtf8("选择一张手牌"));
const QString GUIStaticData::judgeCardText(QObject::trUtf8("判定牌"));
const QString GUIStaticData::judgePositive(QObject::trUtf8("，判定结果：生效！"));
const QString GUIStaticData::judgeNegative(QObject::trUtf8("，判定结果：不生效！"));
const QString GUIStaticData::threeTargets(QObject::trUtf8("你可以指定3个目标"));
const QString GUIStaticData::twoTargets(QObject::trUtf8("你可以指定2个目标"));
const QString GUIStaticData::chooseWeaponOwner(QObject::trUtf8("选择一个有武器的角色"));
const QString GUIStaticData::chooseJiedaoVictim(QObject::trUtf8("选择一个攻击目标（包括你自己）"));
const QString GUIStaticData::chooseZhangbaTarget(QObject::trUtf8("请选择一个攻击目标"));
const QString GUIStaticData::gameIsQuiting(QObject::trUtf8("正在退出三国杀EX..."));

const QString GUIStaticData::backgroundMusicLocation("music/background.mp3");
const QUrl GUIStaticData::helpTextLocation("file:help/overview.html");
const QUrl GUIStaticData::aboutTextLocation("file:help/about.html");

const int GUIStaticData::maxUsedCardContain			= 4;
const int GUIStaticData::defaultMaxCardSelect		= 1;

bool GUIStaticData::isPlayersCreated()
{
	return m_playersCreated;
}

void GUIStaticData::setPlayersCreated()
{
	m_playersCreated = true;
}

bool GUIStaticData::isGameOver()
{
	return m_gameOver;
}

void GUIStaticData::setGameOver()
{
	m_gameOver = true;
}

bool GUIStaticData::isDebugLogOn()
{
	return m_debugLogOn;
}

int GUIStaticData::getOptions()
{
	return m_options;
}

int GUIStaticData::getPackage()
{
	return m_package;
}

int GUIStaticData::getPlayerCount()
{
	return m_fullPlayerCount;
}

int GUIStaticData::getHumanIndex()
{
	return m_humanPlayerIndex;
}

void GUIStaticData::loadFonts()
{
	QFontDatabase::addApplicationFont("font/FZLiBian-S02.TTF");
	appFont.setStyleStrategy(QFont::PreferAntialias);
	buttonFont.setStyleStrategy(QFont::PreferAntialias);
	headingFont.setStyleStrategy(QFont::PreferAntialias);
	contentFont.setStyleStrategy(QFont::PreferAntialias);
	statusbarFont.setStyleStrategy(QFont::PreferAntialias);
	skillFont.setStyleStrategy(QFont::PreferAntialias);
	equipFont.setStyleStrategy(QFont::PreferAntialias);
	QApplication::setFont(GUIStaticData::appFont);
}

int GUIStaticData::zhuangbeiIndex(sgs::ConstData::CardType cardType)
{
	using namespace sgs::ConstData;
	switch (cardType)
	{
	case CHITU:
	case DAWAN:
	case ZICI:
		// jianma
		return 2;

	case DILU:
	case JUEYING:
	case ZHUAHUANG:
	case HUALIU:
		// jiama
		return 3;

	case BAIYIN:
	case TENGJIA:
	case BAGUA:
	case RENWANG:
		// fangju
		return 1;

	case ZHUGE:
	case QINGGANG:
	case HANBING:
	case CIXIONG:
	case GUANSHI:
	case YINYUE:
	case QINGLONG:
	case GUDING:
	case ZHANGBA:
	case FANGTIAN:
	case QILIN:
	case ZHUQUE:
		// wuqi
		return 0;

	default:
		// -1 indicates that the card is not zhuangbei card
		return -1;
	}
}

sgs::DataType::Player * GUIStaticData::getPlayer(int playerIndex)
{
	using sgs::ExternData::gamedata;
	--playerIndex;
	return gamedata.players(playerIndex);
}

sgs::ConstData::HeroType GUIStaticData::getPlayerType(int playerIndex)
{
	using sgs::ExternData::gamedata;
	--playerIndex;
	return gamedata.players(playerIndex)->type();
}

sgs::ConstData::HeroSex GUIStaticData::getPlayerSex(int playerIndex) {
	using sgs::ExternData::gamedata;
	--playerIndex;
	return gamedata.players(playerIndex)->sex();
}

sgs::ConstData::HeroSide GUIStaticData::getPlayerSide(int playerIndex)
{
	using sgs::ExternData::gamedata;
	--playerIndex;
	return gamedata.players(playerIndex)->side();
}

sgs::ConstData::PlayerRole GUIStaticData::getPlayerRole(int playerIndex, bool visible/* = true*/)
{
	using sgs::ExternData::gamedata;
	--playerIndex;
	sgs::ConstData::PlayerRole role = gamedata.players(playerIndex)->role();
	if (!visible && role != sgs::ConstData::ZHU)
		role = sgs::ConstData::playerRoleNone;

	return role;
}

sgs::ConstData::HeroSkill GUIStaticData::getPlayerSkill(int playerIndex, int skillIndex)
{
	using sgs::ExternData::gamedata;
	--playerIndex;
	return gamedata.players(playerIndex)->skill(skillIndex).skill();
}

int GUIStaticData::getSkillCount(int playerIndex)
{
	using sgs::ExternData::gamedata;
	--playerIndex;
	return gamedata.players(playerIndex)->skillnum();
}

int GUIStaticData::getPlayerHP(int playerIndex)
{
	using sgs::ExternData::gamedata;
	--playerIndex;
	return gamedata.players(playerIndex)->hp();
}

const sgs::DataType::Card * GUIStaticData::getCardDeckAt(int index)
{
	using sgs::ExternData::platform;
	return platform.peekpool(index);
}

const sgs::DataType::Card * GUIStaticData::getDustbinAt(int index)
{
	using sgs::ExternData::platform;
	return platform.peekdustbin(index);
}

void GUIStaticData::setCardDeckAt(int index, const sgs::DataType::Card * card)
{
	using sgs::ExternData::platform;
	platform.setpool(index, card);
}

void GUIStaticData::setDustbinAt(int index, const sgs::DataType::Card * card)
{
	using sgs::ExternData::platform;
	platform.setdustbin(index, card);
}

int GUIStaticData::getCardDeckSize()
{
	using sgs::ExternData::platform;
	return platform.poolsize();
}

int GUIStaticData::getDustbinSize()
{
	using sgs::ExternData::platform;
	return platform.dustbinsize();
}

const std::vector<sgs::DataType::Message *>& GUIStaticData::getHistoryVec()
{
	using sgs::ExternData::history;
	return history.viewVec();
}

void GUIStaticData::quitGame()
{
	printDebug("GUIStaticData::quitGame: start");
	BlockLoop::terminateAllLoops();
	AudioPlayer::terminateAllPlayers();
	printEndingMessage();
	QApplication::quit();
	exit(0);
}

void GUIStaticData::resetData()
{
	m_playersCreated = false;
	m_gameOver = false;
}

} /* namespace sgsui */
