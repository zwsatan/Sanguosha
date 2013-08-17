#include "sgsdata_history.h"
#include "sgsdata_msg.h"
using namespace sgs;
using namespace DataType;

History::~History()
{
	//delete all the stored message
	for (auto iter = m_history.begin(); iter != m_history.end(); ++iter)
		delete *iter;
}

void History::push(Message * m)
{
	if (m == 0 || m->dissolved())
		return;
	m_history.push_back(m);
}

Message * History::view(int pos) const
{
	return *(m_history.rbegin() + pos);
}

int History::size() const
{
	return m_history.size();
}

const std::vector<sgs::DataType::Message *>& History::viewVec() const
{
	return m_history;
}
