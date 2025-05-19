#define CATCH_CONFIG_MAIN 
#include "catch_amalgamated.hpp"
#include "../Card.h"
#include <sstream>
#include <random>

const int S_ARR_SIZE = 4;
const Suit S_ARR[S_ARR_SIZE] = { Suit::CLUBS, Suit::DIAMONDS,
	Suit::HEARTS, Suit::SPADES };
const std::string S_STRING_ARR[S_ARR_SIZE] = 
	{"Clubs", "Diamonds", "Hearts", "Spades"};
std::random_device s_rd;
std::minstd_rand s_gen(s_rd());
std::uniform_int_distribution<> s_distrib(0, S_ARR_SIZE - 1);

const int R_ARR_SIZE = 13;
const Rank R_ARR[R_ARR_SIZE] = { Rank::TWO, Rank::THREE, Rank::FOUR,
	Rank::FIVE, Rank::SIX, Rank::SEVEN, Rank::EIGHT,
	Rank::NINE, Rank::TEN, Rank::JACK, Rank::QUEEN,
	Rank::KING, Rank::ACE };
const std::string R_STRING_ARR[R_ARR_SIZE] = { "Two", "Three", "Four", "Five",
	"Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace" };
std::random_device r_rd;
std::minstd_rand r_gen(r_rd());
std::uniform_int_distribution<> r_distrib(0, R_ARR_SIZE - 1);

TEST_CASE("Testing string_to_rank function", "[one]")
{
	SECTION("Successful mapping of string to rank value")
	{
		for (int i = 0; i < R_ARR_SIZE; ++i)
		{
			REQUIRE(R_ARR[i] == string_to_rank(R_STRING_ARR[i]));
		}
	}

	SECTION("Invalid input should throw error")
	{
		std::string s = GENERATE("two", "three", "four", "five", "six", "seven",
			"eight", "nine", "ten", "jack", "queen", "king", "ace", "TWO", "THREE",
			"FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN",
			"KING", "ACE");

		REQUIRE_THROWS(string_to_rank(s));
	}
}

TEST_CASE("Testing overloaded output operator (<<) with rank", "[two]")
{
	for (int i = 0; i < R_ARR_SIZE; ++i)
	{
		std::ostringstream os;
		os << R_ARR[i];
		REQUIRE(os.str() == R_STRING_ARR[i]);
	}
}

TEST_CASE("Testing overloaded input operator (>>) with rank", "[three]")
{
	Rank r;
	std::istringstream iss("Two Three Four Five Six Seven Eight Nine Ten Jack Queen King Ace");

	for (int i = 0; i < R_ARR_SIZE; ++i)
	{
		iss >> r;
		REQUIRE(r == R_ARR[i]);
	}
}

TEST_CASE("Testing string_to_suit function", "[four]")
{
	SECTION("Successful mapping of string to suit value")
	{
		for (int i = 0; i < S_ARR_SIZE; ++i)
		{
			REQUIRE(S_ARR[i] == string_to_suit(S_STRING_ARR[i]));
		}
	}

	SECTION("Invalid input should throw error")
	{
		std::string s = GENERATE("clubs", "club", "CLUBS" , "CLUB", "diamonds",
			"diamond", "DIAMONDS", "DIAMOND", "hearts", "heart", "HEARTS", 
			"Heart", "spades", "spade", "SPADES", "SPADE");

		REQUIRE_THROWS(string_to_suit(s));
	}
}

TEST_CASE("Testing overloaded output operator (<<) with suit", "[five]")
{
	for (int i = 0; i < S_ARR_SIZE; ++i)
	{
		std::ostringstream os;
		os << S_ARR[i];
		REQUIRE(os.str() == S_STRING_ARR[i]);
	}
}

TEST_CASE("Testing overloaded input operator (>>) with suit", "[six]")
{
	Suit s;
	std::istringstream iss("Clubs Diamonds Hearts Spades");

	for (int i = 0; i < S_ARR_SIZE; ++i)
	{
		iss >> s;
		REQUIRE(s == S_ARR[i]);
	}
}

TEST_CASE("Testing Card constructors, rank getters and suit getters", "[seven]")
{
	SECTION("Default constructor")
	{
		Card c;
		REQUIRE(c.get_suit() == Suit::CLUBS);
		REQUIRE(c.get_rank() == Rank::TWO);
	}

	SECTION("Parameterized constructor")
	{
		for (int i = 0; i < 20; ++i)
		{
			int s_i = s_distrib(s_gen);
			int r_i = r_distrib(r_gen);

			Card c(R_ARR[r_i], S_ARR[s_i]);
			REQUIRE(c.get_rank() == R_ARR[r_i]);
			REQUIRE(c.get_suit() == S_ARR[s_i]);
		}
	}
}

TEST_CASE("Testing is_trump Card function", "[eight]")
{
	for (int i = 0; i < S_ARR_SIZE; ++i)
	{
		Suit s = S_ARR[i];
		Card c(Rank::FIVE, s);
		if (s == Suit::SPADES)
		{
			REQUIRE(c.is_trump() == true);
		}
		else
		{
			REQUIRE(c.is_trump() == false);
		}
	}
}

TEST_CASE("Testing overloaded output operator (<<) with Card", "[nine]")
{
	for (int i = 0; i < 20; ++i)
	{
		int r = r_distrib(r_gen);
		int s = s_distrib(s_gen);

		std::stringstream ss;
		ss << Card(R_ARR[r], S_ARR[s]);
		std::string str = R_STRING_ARR[r] + " of " + S_STRING_ARR[s];
		REQUIRE(ss.str() == str);
	}
}

TEST_CASE("Testing overloaded input operator (>>) with Card", "[ten]")
{
	const int test_max = 20;
	Suit s_arr[test_max];
	Rank r_arr[test_max];
	std::stringstream ss;

	for (int i = 0; i < test_max; ++i)
	{
		int s = s_distrib(s_gen);
		int r = r_distrib(r_gen);
		r_arr[i] = R_ARR[r];
		s_arr[i] = S_ARR[s];
		ss << r_arr[i] << " of " << s_arr[i] << "\t";
	}

	for (int i = 0; i < test_max; ++i)
	{
		Card c;
		ss >> c;
		REQUIRE(Card(r_arr[i], s_arr[i]) == c);
	}
}

TEST_CASE("Testing comparison operators with Card", "[eleven]")
{
	SECTION("Testing less than")
	{
		SECTION("Same Suit")
		{
			Suit random_suit = S_ARR[s_distrib(s_gen)];

			for (int i = 0; i < R_ARR_SIZE - 1; ++i)
			{
				REQUIRE(Card(R_ARR[i], random_suit) < Card(R_ARR[i + 1], random_suit));
			}
			
			REQUIRE_FALSE(Card(Rank::TWO, random_suit) < Card(Rank::TWO, random_suit));
		}

		SECTION("Different Suit")
		{
			Rank two = Rank::TWO;
			Rank ace = Rank::ACE;
			REQUIRE(Card(ace, Suit::CLUBS)		< Card(two, Suit::DIAMONDS));
			REQUIRE(Card(ace, Suit::DIAMONDS)	< Card(two, Suit::HEARTS));
			REQUIRE(Card(ace, Suit::HEARTS)		< Card(two, Suit::SPADES));
			
			REQUIRE(Card(two, Suit::CLUBS)		< Card(ace, Suit::DIAMONDS));
			REQUIRE(Card(two, Suit::DIAMONDS)	< Card(ace, Suit::HEARTS));
			REQUIRE(Card(two, Suit::HEARTS)		< Card(ace, Suit::SPADES));
			
			REQUIRE(Card(two, Suit::CLUBS)		< Card(two, Suit::DIAMONDS));
			REQUIRE(Card(two, Suit::DIAMONDS)	< Card(two, Suit::HEARTS));
			REQUIRE(Card(two, Suit::HEARTS)		< Card(two, Suit::SPADES));
		}
	}

	SECTION("Testing less than or equal to")
	{
		SECTION("Same Suit")
		{
			Suit random_suit = S_ARR[s_distrib(s_gen)];

			for (int i = 0; i < R_ARR_SIZE - 1; ++i)
			{
				REQUIRE(Card(R_ARR[i], random_suit) <= Card(R_ARR[i + 1], random_suit));
			}

			REQUIRE(Card(Rank::TWO, random_suit) <= Card(Rank::TWO, random_suit));
		}

		SECTION("Different Suit")
		{
			Rank two = Rank::TWO;
			Rank ace = Rank::ACE;
			REQUIRE(Card(ace, Suit::CLUBS)		<= Card(two, Suit::DIAMONDS));
			REQUIRE(Card(ace, Suit::DIAMONDS)	<= Card(two, Suit::HEARTS));
			REQUIRE(Card(ace, Suit::HEARTS)		<= Card(two, Suit::SPADES));

			REQUIRE(Card(two, Suit::CLUBS)		<= Card(ace, Suit::DIAMONDS));
			REQUIRE(Card(two, Suit::DIAMONDS)	<= Card(ace, Suit::HEARTS));
			REQUIRE(Card(two, Suit::HEARTS)		<= Card(ace, Suit::SPADES));

			REQUIRE(Card(two, Suit::CLUBS)		<= Card(two, Suit::DIAMONDS));
			REQUIRE(Card(two, Suit::DIAMONDS)	<= Card(two, Suit::HEARTS));
			REQUIRE(Card(two, Suit::HEARTS)		<= Card(two, Suit::SPADES));
		}
	}

	SECTION("Testing greater than")
	{
		SECTION("Same Suit")
		{
			Suit random_suit = S_ARR[s_distrib(s_gen)];

			for (int i = 0; i < R_ARR_SIZE - 1; ++i)
			{
				REQUIRE(Card(R_ARR[i + 1], random_suit) > Card(R_ARR[i], random_suit));
			}

			REQUIRE_FALSE(Card(Rank::TWO, random_suit) > Card(Rank::TWO, random_suit));
		}

		SECTION("Different Suit")
		{
			Rank two = Rank::TWO;
			Rank ace = Rank::ACE;
			REQUIRE(Card(two, Suit::DIAMONDS)	> Card(ace, Suit::CLUBS));
			REQUIRE(Card(two, Suit::HEARTS)		> Card(ace, Suit::DIAMONDS));
			REQUIRE(Card(two, Suit::SPADES)		> Card(ace, Suit::HEARTS));

			REQUIRE(Card(ace, Suit::DIAMONDS)	> Card(two, Suit::CLUBS));
			REQUIRE(Card(ace, Suit::HEARTS)		> Card(two, Suit::DIAMONDS));
			REQUIRE(Card(ace, Suit::SPADES)		> Card(two, Suit::HEARTS));

			REQUIRE(Card(two, Suit::DIAMONDS)	> Card(two, Suit::CLUBS));
			REQUIRE(Card(two, Suit::HEARTS)		> Card(two, Suit::DIAMONDS));
			REQUIRE(Card(two, Suit::SPADES)		> Card(two, Suit::HEARTS));
		}
	}

	SECTION("Testing greater than or equal to")
	{
		SECTION("Same Suit")
		{
			Suit random_suit = S_ARR[s_distrib(s_gen)];

			for (int i = 0; i < R_ARR_SIZE - 1; ++i)
			{
				REQUIRE(Card(R_ARR[i + 1], random_suit) >= Card(R_ARR[i], random_suit));
			}

			REQUIRE(Card(Rank::TWO, random_suit) >= Card(Rank::TWO, random_suit));
		}

		SECTION("Different Suit")
		{
			Rank two = Rank::TWO;
			Rank ace = Rank::ACE;
			REQUIRE(Card(two, Suit::DIAMONDS)	>= Card(ace, Suit::CLUBS));
			REQUIRE(Card(two, Suit::HEARTS)		>= Card(ace, Suit::DIAMONDS));
			REQUIRE(Card(two, Suit::SPADES)		>= Card(ace, Suit::HEARTS));

			REQUIRE(Card(ace, Suit::DIAMONDS)	>= Card(two, Suit::CLUBS));
			REQUIRE(Card(ace, Suit::HEARTS)		>= Card(two, Suit::DIAMONDS));
			REQUIRE(Card(ace, Suit::SPADES)		>= Card(two, Suit::HEARTS));

			REQUIRE(Card(two, Suit::DIAMONDS)	>= Card(two, Suit::CLUBS));
			REQUIRE(Card(two, Suit::HEARTS)		>= Card(two, Suit::DIAMONDS));
			REQUIRE(Card(two, Suit::SPADES)		>= Card(two, Suit::HEARTS));
		}
	}

	SECTION("Testing equality")
	{
		int s = s_distrib(s_gen);
		int r = r_distrib(r_gen);
		
		REQUIRE(Card(R_ARR[r], S_ARR[s]) == Card(R_ARR[r], S_ARR[s]));
		REQUIRE_FALSE(Card(R_ARR[r], S_ARR[s]) == Card(R_ARR[(r + 1) % R_ARR_SIZE], S_ARR[s]));
		REQUIRE_FALSE(Card(R_ARR[r], S_ARR[s]) == Card(R_ARR[r], S_ARR[(s + 1) % S_ARR_SIZE]));
	}

	SECTION("Testing inequality")
	{
		int s = s_distrib(s_gen);
		int r = r_distrib(r_gen);

		REQUIRE_FALSE(Card(R_ARR[r], S_ARR[s]) != Card(R_ARR[r], S_ARR[s]));
		REQUIRE(Card(R_ARR[r], S_ARR[s]) != Card(R_ARR[(r + 1) % R_ARR_SIZE], S_ARR[s]));
		REQUIRE(Card(R_ARR[r], S_ARR[s]) != Card(R_ARR[r], S_ARR[(s + 1) % S_ARR_SIZE]));
	}
}

TEST_CASE("Testing Card_less function", "[twelve]")
{
	SECTION("Both cards have the same suit")
	{
		for (int i = 0; i < S_ARR_SIZE; ++i)
		{
			Suit card_suit = S_ARR[i];
			for (int j = 0; j < S_ARR_SIZE; ++j)
			{
				Suit lead_suit = S_ARR[j];
				int r1 = r_distrib(r_gen);
				int r2 = (r1 == R_ARR_SIZE - 1) ? (--r1, r1 + 1) : r1 + 1;

				REQUIRE(Card_less(Card(R_ARR[r1], card_suit), Card(R_ARR[r2], card_suit), lead_suit));
				REQUIRE_FALSE(Card_less(Card(R_ARR[r1], card_suit), Card(R_ARR[r1], card_suit), lead_suit));
			}
		}
	}

	SECTION("Spade against non-Spade card (lead or not)")
	{
		for (int i = 0; i < S_ARR_SIZE - 1; ++i)
		{
			Suit card_suit = S_ARR[i];
			for (int j = 0; j < S_ARR_SIZE; ++j)
			{
				Suit lead_suit = S_ARR[j];
				REQUIRE(Card_less(Card(Rank::ACE, card_suit), Card(Rank::TWO, Suit::SPADES), lead_suit));
			}
		}
	}

	SECTION("Non-spade, non-lead card against lead card of different suits")
	{
		for (int i = 0; i < S_ARR_SIZE - 1; ++i)
		{
			Suit card_suit = S_ARR[i];
			for (int j = 0; j < S_ARR_SIZE; ++j)
			{
				Suit lead_suit = S_ARR[j];
				if (card_suit == lead_suit)
				{
					continue;
				}
				REQUIRE(Card_less(Card(Rank::ACE, card_suit), Card(Rank::TWO, lead_suit), lead_suit));
			}
		}
	}

	SECTION("Both cards are non-spade, non-leading and of different suits")
	{
		REQUIRE(Card_less(Card(Rank::ACE, Suit::CLUBS), Card(Rank::TWO, Suit::DIAMONDS), Suit::SPADES));
		REQUIRE(Card_less(Card(Rank::ACE, Suit::DIAMONDS), Card(Rank::TWO, Suit::HEARTS), Suit::SPADES));
	}
}