/*
* AUTHOR: Kimberly Cabrera
* CSCI 235 Summer 2023 Spades Project
* Card.cpp
*/

#include "Card.h"
#include <cassert>
#include <stdexcept>

Rank string_to_rank(const std::string& str)
{
	Rank r;
	if (str == "Two") { r = Rank::TWO; }
	else if (str == "Three") { r = Rank::THREE; }
	else if (str == "Four") { r = Rank::FOUR; }
	else if (str == "Five") { r = Rank::FIVE; }
	else if (str == "Six") { r = Rank::SIX; }
	else if (str == "Seven") { r = Rank::SEVEN; }
	else if (str == "Eight") { r = Rank::EIGHT; }
	else if (str == "Nine") { r = Rank::NINE; }
	else if (str == "Ten") { r = Rank::TEN; }
	else if (str == "Jack") { r = Rank::JACK; }
	else if (str == "Queen") { r = Rank::QUEEN; }
	else if (str == "King") { r = Rank::KING; }
	else if (str == "Ace") { r = Rank::ACE; }
	else
	{
		throw std::invalid_argument("Invalid value for string_to_rank function.");
	}
	return r;
}

std::ostream& operator<<(std::ostream& os, Rank rank)
{
	static const std::string ranks[13] =
	{
		"Two", "Three", "Four", "Five", "Six", "Seven", "Eight",
		"Nine", "Ten", "Jack", "Queen", "King", "Ace"
	};

	os << ranks[(int)rank];
	return os;
}

std::istream& operator>>(std::istream& is, Rank& rank)
{
	std::string rank_str;
	is >> rank_str;
	rank = string_to_rank(rank_str);
	return is;
}

Suit string_to_suit(const std::string& str)
{
	Suit s;

	if (str == "Clubs") { s = Suit::CLUBS; }
	else if (str == "Diamonds") { s = Suit::DIAMONDS; }
	else if (str == "Hearts") { s = Suit::HEARTS; }
	else if (str == "Spades") { s = Suit::SPADES; }
	else
	{
		throw std::invalid_argument("Invalid value for string_to_suit function.");
	}

	return s;
}

std::ostream& operator<<(std::ostream& os, Suit suit)
{
	static const std::string suits[4] =
	{ "Clubs", "Diamonds", "Hearts", "Spades" };

	os << suits[(int)suit];
	return os;
}

std::istream& operator>>(std::istream& is, Suit& suit)
{
	std::string suit_str;
	is >> suit_str;
	suit = string_to_suit(suit_str);
	return is;
}

Card::Card() : rank(Rank::TWO), suit(Suit::CLUBS) {}

Card::Card(Rank rank, Suit suit) : rank(rank), suit(suit) {}

Rank Card::get_rank() const
{
	return rank;
}

Suit Card::get_suit() const
{
	return suit;
}

bool Card::is_trump() const
{
	return suit == Suit::SPADES;
}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
	os << card.get_rank() << " of " << card.get_suit();
	return os;
}

std::istream& operator>>(std::istream& is, Card& card)
{
	std::string input;
	is >> input; // [rank]
	card.rank = string_to_rank(input);
	is >> input >> input; // of [suit]
	card.suit = string_to_suit(input);
	return is;
}

bool operator<(const Card& lhs, const Card& rhs)
{
	if (lhs.get_suit() < rhs.get_suit()) { return true; }
	else if (lhs.get_suit() > rhs.get_suit()) { return false; }
	return lhs.get_rank() < rhs.get_rank();
}

bool operator<=(const Card& lhs, const Card& rhs)
{
	return !(operator>(lhs, rhs));
}

bool operator>(const Card& lhs, const Card& rhs)
{
	return operator<(rhs, lhs);
}

bool operator>=(const Card& lhs, const Card& rhs)
{
	return !(operator<(lhs, rhs));
}

bool operator==(const Card& lhs, const Card& rhs)
{
	return lhs.get_suit() == rhs.get_suit() &&
		lhs.get_rank() == rhs.get_rank();
}

bool operator!=(const Card& lhs, const Card& rhs)
{
	return !(lhs == rhs);
}

bool Card_less(const Card& a, const Card& b, Suit lead)
{
	if (a.get_suit() == b.get_suit())
	{
		return a.get_rank() < b.get_rank();
	}
	else if (a.get_suit() == Suit::SPADES)
	{
		return false;
	}
	else if (b.get_suit() == Suit::SPADES)
	{
		return true;
	}
	else if (a.get_suit() == lead)
	{
		return false;
	}
	else if (b.get_suit() == lead)
	{
		return true;
	}
	return a < b;
}