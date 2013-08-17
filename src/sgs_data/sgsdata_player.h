#ifndef _SGSDATA_PLAYER_H
#define _SGSDATA_PLAYER_H
#include <vector>
#include <list>
#include "sgsdata_enum.h"
#include "sgsdata_namespace.h"

// 玩家类。
// 玩家类是数据量最大的一个类，记录着每个玩家的全部消息。也提供平台和玩家交互的接口。
class sgs::DataType::Player
{
public:
	Player(int seat, sgs::ConstData::PlayerRole role);
	~Player();

	// 访问用函数头
	// 英雄信息
	// 最大血量
	int maxHP() const;
	// 英雄种类
	sgs::ConstData::HeroType type() const;
	// 英雄性别
	sgs::ConstData::HeroSex sex() const;
	// 英雄势力
	sgs::ConstData::HeroSide side() const;
	// 英雄技能，从0开始计数
	sgs::DataType::Skill skill(int pos);
	// 英雄技能个数
	int skillnum() const;

	// 玩家数据信息
	// 返回输入来源
	sgs::Interface * input() const;
	// 返回后面玩家的指针(无论死活)
	Player * next() const;
	// 返回前面玩家的指针(无论死活)
	Player * prev() const;
	// 返回后面活着玩家的指针
	Player * after() const;
	// 返回前面活着玩家的指针
	Player * before() const;
	// 寻找座位号为n的玩家
	Player * find(int n) const;

	// 计算由自己到对方的距离，-1作为报错值
	int distance(const Player * player) const;
	// 计算攻击距离
	int range() const;
	// 座位号
	int seat() const;
	// 身份
	sgs::ConstData::PlayerRole role() const;
	// 阶段
	sgs::ConstData::PlayerPhase phase() const;
	// 血量
	int hp() const;
	// 状态
	char status() const;
	// 手牌张数
	int handnum();
	// 手牌，从0开始计数
	const sgs::DataType::Card * hand(int pos);
	// 判定牌张数
	int judgenum();
	// 判定区，从0开始计数
	const sgs::DataType::Card * judge(int pos);
	const sgs::DataType::Card * atkhorse() const;
	const sgs::DataType::Card * dfdhorse() const;
	const sgs::DataType::Card * weapon() const;
	const sgs::DataType::Card * armor() const;
	// 当回合使用杀的次数
	int shaCount() const;
	// 根据位置信息访问卡牌
	const sgs::DataType::Card * visitCard(std::pair<sgs::ConstData::PlayerRegionType, int>);

	//平台用函数头，包括修改用函数头
	// 修改状态
	char& setstatus();
	// 修改当前血量
	int& sethp();
	// 修改判定区
	chain<const sgs::DataType::Card *>& setjudge();
	// 增加一次使用杀的记录
	void addShaCount();
	// 将出杀的个数归0
	void clearShaCount();
	// 修改阶段
	void setphase(sgs::ConstData::PlayerPhase phase);
	void setatk(const sgs::DataType::Card * card);
	void setdfd(const sgs::DataType::Card * card);
	void setweapon(const sgs::DataType::Card * card);
	void setarmor(const sgs::DataType::Card * card);
	void choosehero(std::vector<sgs::ConstData::HeroType>& herovec, sgs::ConstData::HeroType lord = sgs::ConstData::heroTypeNone);
	// 设置输入端口
	void setinput(sgs::Interface * input);
	// 根据位置信息弹出卡牌
	void popCard(std::pair<sgs::ConstData::PlayerRegionType, int>);
	void pushHand(const sgs::DataType::Card *card);

	// 逻辑用函数头
	// 执行过程
	sgs::DataType::Message * round(sgs::DataType::Platform&);
	// 请求出闪
	sgs::DataType::Message * ShanOrNot(sgs::DataType::Message *);
	// 请求出桃
	sgs::DataType::Message * TaoOrNot(sgs::DataType::Message *msg);
	// 请求出无懈，传入最初引发无懈的消息和已经被无懈的次数。
	sgs::DataType::Message * WuXieOrNot(sgs::DataType::Message *, int times);
	// 请求出杀
	sgs::DataType::Message * ShaOrNot(sgs::DataType::Message *msg);

	// 玩家管理员为友元
	friend class sgs::DataType::Manager;

private:
	void herotrans(sgs::ConstData::HeroType heroType); //依据武将类型改变武将指针

private:
	// 初始化信息
	sgs::Interface *					m_input;		// 指令来源，UI/AI
	const int							m_seat;			// 从0开始编号，0表示用户
	sgs::DataType::Player *				m_next;			// 下一位置的玩家
	sgs::DataType::Player *				m_prev;			// 上一位置的玩家
	const sgs::ConstData::PlayerRole	m_role;			// 玩家身份
	sgs::DataType::Hero *				m_hero;			// 玩家所选武将

	// 游戏中使用的信息
	char								m_status;		// 玩家的状态
	int									m_shaCount;	// 当回合使用过的杀的个数
	sgs::ConstData::PlayerPhase			m_phase;		// 玩家所处的阶段
	int									m_hp;			// 玩家当前的血量
	chain<const sgs::DataType::Card *>	m_hand;			// 玩家的手牌
	chain<const sgs::DataType::Card *>	m_judge;		// 玩家的判定区

	// 玩家的装备区
	struct
	{
		const sgs::DataType::Card * atkhorse;	//减一马
		const sgs::DataType::Card * dfdhorse;	//加一马
		const sgs::DataType::Card * weapon;		//武器
		const sgs::DataType::Card * armor;		//防具
	} m_equip;


};

#endif /*_SGSDATA_PLAYER_H*/
