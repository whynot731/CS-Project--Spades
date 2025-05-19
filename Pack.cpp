/*
* AUTHOR: Kimberly Cabrera
* CSCI 235 Summer 2023 Spades Project
* Pack.cpp
*/

#include "Pack.h"
#include <stdexcept>
#include <string>

Pack::Pack() : next{ 0 }
{
	static const int max_suit_size = 4;

	static const Suit suits[max_suit_size] =
	{ Suit::CLUBS, Suit::DIAMONDS, Suit::HEARTS, Suit::SPADES };

	static const int max_rank_size = 13;
	static const Rank ranks[max_rank_size] =
	{
		Rank::TWO, Rank::THREE, Rank::FOUR, Rank::FIVE, Rank::SIX,
		Rank::SEVEN, Rank::EIGHT, Rank::NINE, Rank::TEN,
		Rank::JACK, Rank::QUEEN, Rank::KING, Rank::ACE
	};

	int card_index = 0;
	for (int s = 0; s < max_suit_size; ++s)
	{
		for (int r = 0; r < max_rank_size; ++r)
		{
			cards[card_index] = Card(ranks[r], suits[s]);
			++card_index;
		}
	}
}

Pack::Pack(std::istream& pack_input) : next{ 0 }
{
	for (int i = 0; i < PACK_SIZE; ++i)
	{
		pack_input >> cards[i];
	}
}

Card Pack::deal_one()
{
	if (empty())
	{
		throw std::range_error("No more cards in the pack to deal.");
	}
	return cards[next++];
}

void Pack::reset()
{
	next = 0;
}

void Pack::shuffle()
{
	std::array<Card, PACK_SIZE> temp_pack;
	static const int max_shuffle = 15;
	int half_pack_size = PACK_SIZE / 2;
	for (int s = 0; s < max_shuffle; ++s)
	{
		for (int c = 0; c < PACK_SIZE; ++c)
		{
			if (c < half_pack_size)
			{
				temp_pack[2 * c + 1] = cards[c];
			}
			else
			{
				temp_pack[2 * (c - half_pack_size)] = cards[c];
			}
		}
		cards = temp_pack;
	}
	reset();
}

bool Pack::empty() const
{
	return PACK_SIZE == next;
}