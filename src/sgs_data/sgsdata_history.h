#ifndef _SGSDATA_HISTORY_H
#define _SGSDATA_HISTORY_H
#include "sgsdata_enum.h"
#include "sgsdata_namespace.h"
#include <vector>
 
// 历史记录类的定义
class sgs::DataType::History
{
public:
	~History();

	// 插入一个事件记录
	void push(sgs::DataType::Message *);
	// 按照顺序访问历史，越靠后的事件数字越小，从0开始计数，通常0就是当前的事件
	sgs::DataType::Message * view(int pos) const;
	// 访问历史长度
	int size() const;
	// 找到最近的一次连锁的开始位置
	int begin() const;
	// 希望能够用访问标准库vector的方式访问viewvec，故返回了它的常引用
	const std::vector<sgs::DataType::Message *>& viewVec() const;

private:
	std::vector<sgs::DataType::Message *> m_history;
};
 
#endif /*_SGSDATA_HISTORY_H*/
