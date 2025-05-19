#define CATCH_CONFIG_MAIN 
#include "catch_amalgamated.hpp"
#include "../Pack.h"
#include <sstream>
#include <random>
#include <algorithm>

const int PACK_SIZE = 52;
std::string ORDERED_PACK[PACK_SIZE] =
{
	"Two of Clubs",
	"Three of Clubs",
	"Four of Clubs",
	"Five of Clubs",
	"Six of Clubs",
	"Seven of Clubs",
	"Eight of Clubs",
	"Nine of Clubs",
	"Ten of Clubs",
	"Jack of Clubs",
	"Queen of Clubs",
	"King of Clubs",
	"Ace of Clubs",
	"Two of Diamonds",
	"Three of Diamonds",
	"Four of Diamonds",
	"Five of Diamonds",
	"Six of Diamonds",
	"Seven of Diamonds",
	"Eight of Diamonds",
	"Nine of Diamonds",
	"Ten of Diamonds",
	"Jack of Diamonds",
	"Queen of Diamonds",
	"King of Diamonds",
	"Ace of Diamonds",
	"Two of Hearts",
	"Three of Hearts",
	"Four of Hearts",
	"Five of Hearts",
	"Six of Hearts",
	"Seven of Hearts",
	"Eight of Hearts",
	"Nine of Hearts",
	"Ten of Hearts",
	"Jack of Hearts",
	"Queen of Hearts",
	"King of Hearts",
	"Ace of Hearts",
	"Two of Spades",
	"Three of Spades",
	"Four of Spades",
	"Five of Spades",
	"Six of Spades",
	"Seven of Spades",
	"Eight of Spades",
	"Nine of Spades",
	"Ten of Spades",
	"Jack of Spades",
	"Queen of Spades",
	"King of Spades",
	"Ace of Spades"
};

TEST_CASE("Testing default constructor and deal_one function", "[one]")
{
	Pack pack;
	for (int i = 0; i < PACK_SIZE; ++i)
	{
		std::stringstream ss;
		ss << pack.deal_one();
		REQUIRE(ss.str() == ORDERED_PACK[i]);
	}
	REQUIRE_THROWS(pack.deal_one());
}

TEST_CASE("Testing parameterized constructor function", "[two]")
{
	std::string unordered_pack[PACK_SIZE];
	for (int i = 0; i < PACK_SIZE; ++i)
	{
		unordered_pack[i] = ORDERED_PACK[i];
	}
	
	// Fisher-Yates Shuffle
	std::random_device rd;
	std::minstd_rand gen(rd());
	for (int i = 0; i < PACK_SIZE - 1; ++i)
	{
		std::uniform_int_distribution<> distrib(i, PACK_SIZE - 1);
		int j = distrib(gen);
		std::swap(unordered_pack[i], unordered_pack[j]);
	}

	std::stringstream ss;
	for (int i = 0; i < PACK_SIZE; ++i)
	{
		ss << unordered_pack[i] << '\n';
	}

	Pack pack(ss);

	for (int i = 0; i < PACK_SIZE; ++i)
	{
		std::ostringstream os;
		os << pack.deal_one();
		REQUIRE(os.str() == unordered_pack[i]);
	}
}

TEST_CASE("Testing reset function", "[three]")
{
	Pack pack;
	pack.deal_one();
	pack.reset();
	REQUIRE(pack.deal_one() == Card());
}

TEST_CASE("Testing shuffle function", "[four]")
{
	SECTION("Testing with an ordered pack")
	{
		std::string ordered_pack_shuffle[2][PACK_SIZE] =
		{
			{
			"Seven of Diamonds",
			"King of Hearts",
			"Five of Clubs",
			"Jack of Diamonds",
			"Four of Spades",
			"Nine of Clubs",
			"Two of Hearts",
			"Eight of Spades",
			"King of Clubs",
			"Six of Hearts",
			"Queen of Spades",
			"Four of Diamonds",
			"Ten of Hearts",
			"Two of Clubs",
			"Eight of Diamonds",
			"Ace of Hearts",
			"Six of Clubs",
			"Queen of Diamonds",
			"Five of Spades",
			"Ten of Clubs",
			"Three of Hearts",
			"Nine of Spades",
			"Ace of Clubs",
			"Seven of Hearts",
			"King of Spades",
			"Five of Diamonds",
			"Jack of Hearts",
			"Three of Clubs",
			"Nine of Diamonds",
			"Two of Spades",
			"Seven of Clubs",
			"King of Diamonds",
			"Six of Spades",
			"Jack of Clubs",
			"Four of Hearts",
			"Ten of Spades",
			"Two of Diamonds",
			"Eight of Hearts",
			"Ace of Spades",
			"Six of Diamonds",
			"Queen of Hearts",
			"Four of Clubs",
			"Ten of Diamonds",
			"Three of Spades",
			"Eight of Clubs",
			"Ace of Diamonds",
			"Seven of Spades",
			"Queen of Clubs",
			"Five of Hearts",
			"Jack of Spades",
			"Three of Diamonds",
			"Nine of Hearts"
		}, {
			"Five of Spades",
			"Eight of Hearts",
			"Jack of Diamonds",
			"Ace of Clubs",
			"Four of Clubs",
			"Eight of Spades",
			"Jack of Hearts",
			"Ace of Diamonds",
			"Four of Diamonds",
			"Seven of Clubs",
			"Jack of Spades",
			"Ace of Hearts",
			"Four of Hearts",
			"Seven of Diamonds",
			"Ten of Clubs",
			"Ace of Spades",
			"Four of Spades",
			"Seven of Hearts",
			"Ten of Diamonds",
			"King of Clubs",
			"Three of Clubs",
			"Seven of Spades",
			"Ten of Hearts",
			"King of Diamonds",
			"Three of Diamonds",
			"Six of Clubs",
			"Ten of Spades",
			"King of Hearts",
			"Three of Hearts",
			"Six of Diamonds",
			"Nine of Clubs",
			"King of Spades",
			"Three of Spades",
			"Six of Hearts",
			"Nine of Diamonds",
			"Queen of Clubs",
			"Two of Clubs",
			"Six of Spades",
			"Nine of Hearts",
			"Queen of Diamonds",
			"Two of Diamonds",
			"Five of Clubs",
			"Nine of Spades",
			"Queen of Hearts",
			"Two of Hearts",
			"Five of Diamonds",
			"Eight of Clubs",
			"Queen of Spades",
			"Two of Spades",
			"Five of Hearts",
			"Eight of Diamonds",
			"Jack of Clubs"
		}
		};

		Pack pack;

		for (int shuffles = 0; shuffles < 2; ++shuffles)
		{
			pack.shuffle();
			for (int i = 0; i < PACK_SIZE; ++i)
			{
				std::stringstream oss;
				oss << pack.deal_one();
				REQUIRE(oss.str() == ordered_pack_shuffle[shuffles][i]);
			}
		}
	}

	SECTION("Testing with a random pack")
	{
		std::string pack_shuffle[3][PACK_SIZE] = 
		{
			{
				"Queen of Diamonds",
				"Five of Diamonds",
				"Eight of Hearts",
				"Ace of Spades",
				"Six of Hearts",
				"Four of Spades",
				"Four of Hearts",
				"Ten of Clubs",
				"Six of Spades",
				"Nine of Hearts",
				"Two of Spades",
				"Eight of Diamonds",
				"Eight of Spades",
				"Jack of Hearts",
				"Two of Hearts",
				"King of Hearts",
				"Four of Clubs",
				"Two of Diamonds",
				"Nine of Spades",
				"Six of Diamonds",
				"Queen of Hearts",
				"Two of Clubs",
				"Eight of Clubs",
				"Five of Clubs",
				"Queen of Spades",
				"Six of Clubs",
				"Seven of Spades",
				"Ace of Hearts",
				"Five of Spades",
				"Three of Diamonds",
				"Jack of Spades",
				"Nine of Diamonds",
				"Three of Clubs",
				"Ace of Diamonds",
				"Seven of Diamonds",
				"Jack of Diamonds",
				"Ten of Hearts",
				"Three of Spades",
				"Queen of Clubs",
				"King of Diamonds",
				"Four of Diamonds",
				"King of Clubs",
				"Seven of Hearts",
				"Nine of Clubs",
				"Seven of Clubs",
				"Three of Hearts",
				"Ten of Spades",
				"King of Spades",
				"Ten of Diamonds",
				"Jack of Clubs",
				"Five of Hearts",
				"Ace of Clubs"
			}, 
			{
				"Nine of Spades",
				"Three of Spades",
				"Ace of Spades",
				"Eight of Clubs",
				"King of Clubs",
				"Ten of Clubs",
				"Seven of Spades",
				"Three of Hearts",
				"Eight of Diamonds",
				"Jack of Spades",
				"Jack of Clubs",
				"King of Hearts",
				"Seven of Diamonds",
				"Queen of Diamonds",
				"Six of Diamonds",
				"Queen of Clubs",
				"Six of Hearts",
				"Five of Clubs",
				"Seven of Hearts",
				"Six of Spades",
				"Ace of Hearts",
				"Ten of Spades",
				"Eight of Spades",
				"Nine of Diamonds",
				"Five of Hearts",
				"Four of Clubs",
				"Jack of Diamonds",
				"Five of Diamonds",
				"Queen of Hearts",
				"King of Diamonds",
				"Four of Spades",
				"Queen of Spades",
				"Nine of Clubs",
				"Nine of Hearts",
				"Five of Spades",
				"King of Spades",
				"Jack of Hearts",
				"Three of Clubs",
				"Ace of Clubs",
				"Two of Diamonds",
				"Ten of Hearts",
				"Eight of Hearts",
				"Two of Clubs",
				"Four of Diamonds",
				"Four of Hearts",
				"Six of Clubs",
				"Seven of Clubs",
				"Two of Spades",
				"Three of Diamonds",
				"Ten of Diamonds",
				"Two of Hearts",
				"Ace of Diamonds"
			}, 
			{
				"Seven of Hearts",
				"Three of Clubs",
				"Eight of Clubs",
				"Eight of Spades",
				"Eight of Hearts",
				"Three of Hearts",
				"Jack of Diamonds",
				"Six of Clubs",
				"King of Hearts",
				"Four of Spades",
				"Ten of Diamonds",
				"Queen of Clubs",
				"Five of Spades",
				"Nine of Spades",
				"Six of Spades",
				"Ace of Clubs",
				"King of Clubs",
				"Nine of Diamonds",
				"Two of Clubs",
				"Eight of Diamonds",
				"Five of Diamonds",
				"Seven of Clubs",
				"Seven of Diamonds",
				"Queen of Spades",
				"Two of Hearts",
				"Six of Hearts",
				"King of Spades",
				"Three of Spades",
				"Ace of Hearts",
				"Two of Diamonds",
				"Ten of Clubs",
				"Five of Hearts",
				"Four of Diamonds",
				"Jack of Spades",
				"Queen of Hearts",
				"Two of Spades",
				"Queen of Diamonds",
				"Nine of Clubs",
				"Ace of Diamonds",
				"Five of Clubs",
				"Jack of Hearts",
				"Ace of Spades",
				"Ten of Spades",
				"Ten of Hearts",
				"Seven of Spades",
				"Four of Clubs",
				"Four of Hearts",
				"Jack of Clubs",
				"King of Diamonds",
				"Three of Diamonds",
				"Six of Diamonds",
				"Nine of Hearts"
			}
		};

		std::stringstream ss;
		for (int i = 0; i < PACK_SIZE; ++i)
		{
			ss << pack_shuffle[0][i] << '\n';
		}

		Pack pack(ss);

		for (int shuffles = 1; shuffles < 3; ++shuffles)
		{
			pack.shuffle();
			for (int i = 0; i < PACK_SIZE; ++i)
			{
				std::stringstream oss;
				oss << pack.deal_one();
				REQUIRE(oss.str() == pack_shuffle[shuffles][i]);
			}
		}
	}
}

TEST_CASE("Testing empty function", "[five]")
{
	Pack pack;
	REQUIRE_FALSE(pack.empty());

	for (int i = 0; i < PACK_SIZE; ++i)
	{
		pack.deal_one();
	}
	REQUIRE(pack.empty());
}