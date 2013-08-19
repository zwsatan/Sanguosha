#ifndef SGSUI_AUDIOPLAYER_H_
#define SGSUI_AUDIOPLAYER_H_

#include "sgsdata_enum.h"
#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <vector>
#include <list>

namespace sgsui {

class MainWindow;

enum SoundType
{
	DamageEffectSound,
	LightningEffectSound,
	YourTurn,
	UseEquip,
	PlayersCreated
};

class AudioPlayer : public QObject
{
	Q_OBJECT

public:
	explicit AudioPlayer(MainWindow *parent = 0);
	~AudioPlayer();

	void terminateThisPlayer();
	static void terminateAllPlayers();

public slots:
	void startBGM();
	void pauseBGM();
	void playEndingMusic(bool isWinner);
	void playSound(SoundType soundType, bool sex = true);
	void playSound(sgs::ConstData::CardType cardType, bool sex = true);
	void playSound(sgs::ConstData::HeroSkill Skill, int playerIndex);
	void playSound(sgs::ConstData::HeroType deadHeroType);

private:
	void playSoundStr(const QString &file);

private:
	QMediaPlayer *					m_backgroundMusic;		// 只用来播放背景音乐
	QMediaPlaylist *				m_backgroundMusicList;	// 通过播放链表反复播放背景音乐
	const MainWindow *				m_mainWindow;
	std::vector<QMediaPlayer *>		m_soundVec;

	sgs::ConstData::HeroSkill		m_lastSkill;
	int								m_lastPlayerIndex;
	bool							m_aboutToTerminate;

	static std::list<AudioPlayer *>	ms_audioPlayerList;
};

} /* namespace sgsui */
#endif /* SGSUI_AUDIOPLAYER_H_ */
