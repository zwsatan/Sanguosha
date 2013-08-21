#include "sgsui_AudioPlayer.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_MainWindow.h"
#include "sgsui_nameConvert.h"
#include "sgsui_output.h"
#include <QDir>

namespace sgsui {

// 为了解决mac下声音无法输出,总之发现使用绝对路径可以完成目标,蛋疼,因此以此种方式暂时处理
static const QString gs_curDir = QDir::currentPath() + "/";
std::list<AudioPlayer *> AudioPlayer::ms_audioPlayerList;

AudioPlayer::AudioPlayer(MainWindow * parent)
	: QObject(parent)
	, m_mainWindow(parent)
	, m_aboutToTerminate(false)
{
	m_backgroundMusic = new QMediaPlayer(this);
	m_backgroundMusicList = new QMediaPlaylist();

	// make enough enqueue to ensure that background music never ends
	QMediaContent media(QUrl::fromLocalFile(gs_curDir + GUIStaticData::backgroundMusicLocation));
	m_backgroundMusicList->addMedia(media);
//	for (int i = 0; i < 100; ++i)
	m_backgroundMusicList->setCurrentIndex(0);
	m_backgroundMusicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
	m_backgroundMusic->setPlaylist(m_backgroundMusicList);

	ms_audioPlayerList.push_back(this);
}

AudioPlayer::~AudioPlayer()
{
	printDebug("AudioPlayer::~AudioPlayer: start");
	delete m_backgroundMusic;
	m_backgroundMusic = NULL;
	printDebug("AudioPlayer::~AudioPlayer: backgroundMusic has been deleted");

	for (auto it = m_soundVec.begin(); it != m_soundVec.end(); ++it)
		delete *it;
	printDebug(	"AudioPlayer::~AudioPlayer: all sound objects have been deleted");

	ms_audioPlayerList.remove(this);
	printDebug("AudioPlayer::~AudioPlayer: over");
}

void AudioPlayer::terminateThisPlayer()
{
	m_aboutToTerminate = true;
	m_backgroundMusic->stop();
	printDebug("AudioPlayer::terminateThisPlayer: backgroundMusic is stopped");

	for (auto it = m_soundVec.begin(); it != m_soundVec.end(); ++it)
		(*it)->stop();
	printDebug("AudioPlayer::terminateThisPlayer: all sound objects are stopped");
}

void AudioPlayer::terminateAllPlayers()
{
	foreach (AudioPlayer *player, ms_audioPlayerList)
		player->terminateThisPlayer();
}

void AudioPlayer::startBGM()
{
	m_backgroundMusic->play();
}

void AudioPlayer::pauseBGM()
{
	m_backgroundMusic->pause();
}

void AudioPlayer::playEndingMusic(bool isWinner)
{
	if (m_aboutToTerminate)
		return;

	m_backgroundMusicList->clear();

	QString filename = isWinner ? QString("music/win.mp3") : QString("music/lose.mp3");
	QUrl url = QUrl::fromLocalFile(gs_curDir + filename);
	m_backgroundMusicList->addMedia(url);
	m_backgroundMusicList->setCurrentIndex(0);
	m_backgroundMusic->play();
}

void AudioPlayer::playSound(SoundType soundType, bool sex)
{
	if (m_aboutToTerminate)
		return;

	if (m_mainWindow->isSoundOn())
	{
		Q_ASSERT(DamageEffectSound <= soundType && soundType <= PlayersCreated);

		QString fileLocation;
		switch (soundType)
		{
		case DamageEffectSound:
			fileLocation = sex ? "sound/card/male/damage.mp3" : "sound/card/female/damage.mp3";
			break;
		case LightningEffectSound:
			fileLocation = "sound/system/lightning.mp3";
			break;
		case YourTurn:
			fileLocation = "sound/system/yourturn.mp3";
			break;
		case UseEquip:
			fileLocation = "sound/system/use-equip.mp3";
			break;
		case PlayersCreated:
			fileLocation = "sound/system/players-created.mp3";
			break;
		default:
			break;
		}

		playSoundStr(fileLocation);
	}
}

void AudioPlayer::playSound(sgs::ConstData::CardType cardType, bool sex)
{
	if (m_aboutToTerminate)
		return;

	if (m_mainWindow->isSoundOn())
		playSoundStr(cardSoundStr(cardType, sex));
}

void AudioPlayer::playSound(sgs::ConstData::HeroSkill skill, int playerIndex)
{
	if (m_aboutToTerminate)
		return;

	if (m_lastSkill == skill && m_lastPlayerIndex == playerIndex && m_lastSkill == sgs::ConstData::LUOSHEN)
		return;

	m_lastSkill = skill;
	m_lastPlayerIndex = playerIndex;
	if (m_mainWindow->isSoundOn())
		playSoundStr(skillSoundStr(skill));
}

void AudioPlayer::playSound(sgs::ConstData::HeroType deadHeroType)
{
	if (m_aboutToTerminate)
		return;

	if (m_mainWindow->isSoundOn())
		playSoundStr(wujiangDeathSoundStr(deadHeroType));
}

void AudioPlayer::playSoundStr(const QString &file)
{
	if (m_aboutToTerminate)
		return;

	QMediaContent media(QUrl::fromLocalFile(gs_curDir + file));
	for (auto it = m_soundVec.begin(); it != m_soundVec.end(); ++it)
	{
		if ((*it)->state() == QMediaPlayer::StoppedState || (*it)->state() == QMediaPlayer::PausedState)
		{
			(*it)->stop();
			(*it)->setMedia(media);
			(*it)->play();
			return;
		}
	}

	QMediaPlayer *soundEffect = new QMediaPlayer();
	soundEffect->setMedia(media);
	soundEffect->play();
	m_soundVec.push_back(soundEffect);

	static const QString s_playSoundStr = "<font color=red><b>Notice: </b></font>AudioPlayer::playSoundStr: creating new MediaObject, current MediaObject count = ";
	printDebug(s_playSoundStr + QString::number(static_cast<unsigned>(m_soundVec.size())));
}

} /* namespace sgsui */
