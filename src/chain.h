#ifndef _CHAIN_
#define _CHAIN_

template<class T>
class chain
{
public:
	chain();

	int size() const;
	bool empty();
	void clear();
	// insert a new data after index
	bool push(T t, int index = 0);
	bool push_back(T t);
	// delete the data at index
	bool pop(int index = 1);
	bool pop_back();
	// visit the data at index
	const T& at(int index = 1) const;
	// visit the data at index
	T& at(int index = 1);
	const T& at_back() const;
	T& at_back();

private:
	chain * find(int index);
	// used to support const use.
	const chain * nfind(int index) const;
	// insert a new node after pos.
	chain * ninsert(chain * pos, chain * data);
	// insert a new node after pos.
	chain * ndelete(chain * pos);

private:
	T				m_data;
	chain *			m_next;
};

template<class T>
chain<T> * chain<T>::find(int index)
{
	if (index <= 0)
		return 0;

	chain<T> * temp = this;
	for (int i = 0; i < index - 1; ++i)
	{
		temp = temp->m_next;
		if (temp == this)
			return 0;
	}
	return temp;
}

template<class T>
const chain<T> * chain<T>::nfind(int index) const
{
	if (index <= 0)
		return 0;

	const chain<T> * temp = this;
	for (int i = 0; i < index - 1; ++i)
	{
		temp = temp->m_next;
		if (temp == this)
			return 0;
	}
	return temp;
}

template<class T>
chain<T> * chain<T>::ninsert(chain<T> * pos, chain<T> * data)
{
	data->m_next = pos->m_next;
	pos->m_next = data;
	return data;
}

template<class T>
chain<T> * chain<T>::ndelete(chain<T> * pos)
{
	chain<T> * temp = pos->m_next;
	pos->m_next = temp->m_next;
	delete temp;
	return pos;
}

template<class T>
chain<T>::chain()
{
	m_next = this;
}

template<class T>
int chain<T>::size() const
{
	int len = 0;
	for (const chain<T> * temp = this; temp->m_next != this; temp = temp->m_next)
		++len;
	return len;
}

template<class T>
bool chain<T>::empty()
{
	if (m_next == this)
		return true;
	return false;
}

template<class T>
void chain<T>::clear()
{
	while (m_next != this)
		ndelete(this);
}

template<class T>
bool chain<T>::push(T t, int index/* = 0*/)
{
	chain<T> * pos = find(index + 1);
	if (pos == 0)
		return false;

	chain<T> * data = new chain<T>;
	if (data == 0)
		return false;

	data->m_data = t;
	ninsert(pos, data);
	return true;
}
template<class T>
bool chain<T>::pop(int index/* = 1*/)
{
	chain<T> * pos = find(index);
	if (pos == 0)
		return false;

	ndelete(pos);
	return true;
}

template<class T>
bool chain<T>::push_back(T t)
{
	return push(t, size());
}

template<class T>
bool chain<T>::pop_back()
{
	return pop(size());
}

template<class T>
const T& chain<T>::at(int index/* = 1*/) const
{
	return nfind(index)->m_next->m_data;
}

template<class T>
T& chain<T>::at(int index/* = 1*/)
{
	return nfind(index)->m_next->m_data;
}

template<class T>
const T& chain<T>::at_back() const
{
	return at(size());
}

template<class T>
T& chain<T>::at_back()
{
	return at(size());
}

#endif
