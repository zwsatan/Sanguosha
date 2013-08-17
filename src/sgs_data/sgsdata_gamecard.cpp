#include "sgsdata_card.h"
#include "sgsdata_gamecard.h"
#include "sgsdata_extern.h"
#include <iostream>
#include <cstdlib>

using namespace sgs;
using namespace ConstData;
using namespace DataType;

GameCard::GameCard()
{
}

GameCard::~GameCard()
{
	m_pool.clear();
	m_dustbin.clear();
}

void GameCard::refresh()
{
	if (!m_pool.empty())
		return;

	int len = m_dustbin.size();
	for (int i = 1; i <= len; ++i)
	{
		int index = rand() % i;
		const Card * temp = m_dustbin.at();
		m_dustbin.pop();
		m_pool.push(temp, index);
	}
}

void GameCard::load()
{
	const Card * temp = 0;
	for (int i = 0; i < 108; ++i)
	{
		temp = &ExternData::card[i];
		m_dustbin.push(temp);
	}

	refresh();
}

const Card * GameCard::get()
{
	if (m_pool.empty())
		refresh();

	const Card * temp = m_pool.at();
	m_pool.pop();
	return temp;
}

void GameCard::pooltop(const Card * card)
{
	m_pool.push(card);
}

void GameCard::poolbottom(const Card * card)
{
	m_pool.push_back(card);
}

void GameCard::abandon(const Card * card)
{
	card->setmask();
	m_dustbin.push(card);
}

void GameCard::clearcard()
{
	m_pool.clear();
	m_dustbin.clear();
}

const Card * GameCard::peekpool(int n) const
{
	if (n < 0 || n >= m_pool.size())
		return 0;
	return m_pool.at(n + 1);
}
const Card * GameCard::peekdustbin(int n) const
{
	if (n < 0 || n >= m_dustbin.size())
		return 0;
	return m_dustbin.at(n + 1);
}

int GameCard::poolsize() const
{
	return m_pool.size();
}

int GameCard::dustbinsize() const
{
	return m_dustbin.size();
}

void GameCard::setpool(int n, const sgs::DataType::Card * card)
{
	if (n < 0 || n >= m_pool.size() || !card)
		return;

	m_pool.at(n + 1) = card;
}

void GameCard::setdustbin(int n, const sgs::DataType::Card * card)
{
	if (n < 0 || n >= m_dustbin.size() || !card)
		return;

	m_dustbin.at(n + 1) = card;
}
