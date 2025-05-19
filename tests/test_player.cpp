#define CATCH_CONFIG_MAIN 
#include "catch_amalgamated.hpp"
#include "../Player.h"
#include "../Card.h"
#include <sstream>
#include <random>
//#include <algorithm>
#include <string>

const size_t MAX_HAND_SIZE = 13;

std::string random_name_generator()
{
	std::random_device rd;
	std::minstd_rand gen(rd());
	std::uniform_int_distribution<> distrib(0, 25);

	std::string name(distrib(gen), 'a');
	
	for (size_t i = 0; i < name.size(); ++i)
	{
		name[i] = 'a' + distrib(gen);
	}

	return name;
}

Card random_card_generator()
{
	static const int pack_size = 52;
	static int index = 0;
	static bool randomized = false;
	static bool start = false;
	static Card cards[pack_size];

	if (!start)
	{
		start = true;
		static const Suit suits[4] = { Suit::CLUBS, Suit::DIAMONDS,
	Suit::HEARTS, Suit::SPADES };

		static const Rank ranks[13] = { Rank::TWO, Rank::THREE, Rank::FOUR,
	Rank::FIVE, Rank::SIX, Rank::SEVEN, Rank::EIGHT,
	Rank::NINE, Rank::TEN, Rank::JACK, Rank::QUEEN,
	Rank::KING, Rank::ACE };

		int i = 0;
		for (int s = 0; s < 4; ++s)
		{
			for (int r = 0; r < 13; ++r)
			{
				cards[i++] = Card(ranks[r], suits[s]);
			}
		}
	}

	if (!randomized)
	{
		randomized = true;
		// Fisher-Yates Shuffle
		std::random_device rd;
		std::minstd_rand gen(rd());
		for (int i = 0; i < pack_size - 1; ++i)
		{
			std::uniform_int_distribution<> distrib(i, pack_size - 1);
			int j = distrib(gen);
			std::swap(cards[i], cards[j]);
		}
	}

	if (index == pack_size)
	{
		index = 0;
	}

	return cards[index++];
}

std::string print_hand(Card hand[], size_t size)
{
	std::string str;
	for (size_t i = 0; i < size; ++i)
	{
		std::stringstream ss;
		ss << hand[i];
		str += '[' + std::to_string(i + 1) + ']' + ' ' + ss.str() + '\n';
	}
	return str;
}

TEST_CASE("Testing factory function and get_name function", "[one]")
{
	std::string name(random_name_generator());
	Player* rando = Player_factory(name, "Human");
	REQUIRE(rando->get_name() == name);
	delete rando;
	rando = nullptr;
}

TEST_CASE("Testing add_card function", "[two]")
{
	Player* player = Player_factory("Kim", "Human");

	for (size_t i = 0; i < MAX_HAND_SIZE; ++i)
	{
		REQUIRE(player->add_card(random_card_generator()) == true);
	}

	REQUIRE(player->add_card(random_card_generator()) == false);

	delete player;
	player = nullptr;
}

TEST_CASE("Testing set_bid and get_bid functions", "[three]")
{
	std::string name = random_name_generator();
	Player* player = Player_factory(name, "Human");
	REQUIRE(player->get_bid() == 0);

	Card hand[13];
	for (size_t i = 0; i < MAX_HAND_SIZE; ++i)
	{
		hand[i] = random_card_generator();
		player->add_card(hand[i]);
	}

	std::sort(std::begin(hand), std::end(hand));

	SECTION("Testing for blind nil")
	{
		SECTION("Testing for valid input")
		{
			std::istringstream iss("y");
			std::streambuf* cinbuf = std::cin.rdbuf();
			std::cin.rdbuf(iss.rdbuf());

			std::ostringstream oss;
			std::streambuf* coutbuf = std::cout.rdbuf();
			std::cout.rdbuf(oss.rdbuf());

			player->set_bid();

			std::string expected_output{ "Does Player " + name + " want to bid blind nil? [y/n]\n" };
			REQUIRE(oss.str() == expected_output);
			REQUIRE(player->get_bid() == 0);

			std::cin.rdbuf(cinbuf);
			std::cout.rdbuf(coutbuf);
		}
		
		SECTION("Testing for invalid input")
		{
			// Use "Yes\nyes\nY\ny" for advanced stream input manipulation testing
			std::istringstream iss("0\nY\nN\ny");
			std::streambuf* cinbuf = std::cin.rdbuf();
			std::cin.rdbuf(iss.rdbuf());

			std::ostringstream oss;
			std::streambuf* coutbuf = std::cout.rdbuf();
			std::cout.rdbuf(oss.rdbuf());

			player->set_bid();

			std::string expected_output{ "Does Player " + name + " want to bid blind nil? [y/n]\n\
Please answer using only `y` for yes or `n` for no.\n\
Please answer using only `y` for yes or `n` for no.\n\
Please answer using only `y` for yes or `n` for no.\n" };
			REQUIRE(oss.str() == expected_output);
			REQUIRE(player->get_bid() == 0);

			std::cin.rdbuf(cinbuf);
			std::cout.rdbuf(coutbuf);
		}
	}

	SECTION("Testing for valid input")
	{
		std::stringstream iss;
		std::streambuf* cinbuf = std::cin.rdbuf();
		std::cin.rdbuf(iss.rdbuf());

		for (int i = 0; i < 14; ++i)
		{
			std::ostringstream oss;
			std::streambuf* coutbuf = std::cout.rdbuf();
			std::cout.rdbuf(oss.rdbuf());

			iss << "n\n" << i << '\n';
			player->set_bid();
			
			std::string expected_output{ "Does Player " + name + " want to bid blind nil? [y/n]\n" };
			expected_output += print_hand(hand, MAX_HAND_SIZE) 
				+ "How much would you like to bid (0 to 13 inclusive)?\n";

			REQUIRE(oss.str() == expected_output);
			REQUIRE(player->get_bid() == i);

			std::cout.rdbuf(coutbuf);
		}
		std::cin.rdbuf(cinbuf);
	}

	SECTION("Testing for invalid input")
	{
		std::istringstream iss("n\n-1\n14\n6\n");
		std::streambuf* cinbuf = std::cin.rdbuf();
		std::cin.rdbuf(iss.rdbuf());

		std::ostringstream oss;
		std::streambuf* coutbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		player->set_bid();

		std::string expected_output{ "Does Player " + name + " want to bid blind nil? [y/n]\n" };
		expected_output += print_hand(hand, MAX_HAND_SIZE)
			+ "How much would you like to bid (0 to 13 inclusive)?\n"
			+ "Please answer with values from 0 to 13 inclusive.\n"
			+ "Please answer with values from 0 to 13 inclusive.\n";

		REQUIRE(oss.str() == expected_output);
		REQUIRE(player->get_bid() == 6);

		std::cout.rdbuf(coutbuf);
		std::cin.rdbuf(cinbuf);
	}

	delete player;
	player = nullptr;
}

TEST_CASE("Testing add_trick and get_trick functions", "[four]")
{
	std::string name(random_name_generator());
	Player* rando = Player_factory(name, "Human");

	REQUIRE(rando->get_trick() == 0);
	for (int i = 1; i < static_cast<int>(MAX_HAND_SIZE) + 1; ++i)
	{
		REQUIRE(rando->add_trick());
		REQUIRE(rando->get_trick() == i);
	}
	REQUIRE_FALSE(rando->add_trick());
	REQUIRE(rando->get_trick() == 13);

	delete rando;
	rando = nullptr;
}

TEST_CASE("Testing nil_bid and blind_nil_bid functions", "[five]")
{
	std::string name = random_name_generator();
	Player* player = Player_factory(name, "Human");

	Card hand[13];
	for (size_t i = 0; i < MAX_HAND_SIZE; ++i)
	{
		hand[i] = random_card_generator();
		player->add_card(hand[i]);
	}

	std::sort(std::begin(hand), std::end(hand));

	//SECTION("Testing before set_bid")
	//{
	//	REQUIRE_FALSE(player->blind_nil_bid());
	//	REQUIRE_FALSE(player->nil_bid());
	//}
	
	SECTION("Testing with setting bid to nil")
	{
		std::istringstream iss("n\n0\n");
		std::streambuf* cinbuf = std::cin.rdbuf();
		std::cin.rdbuf(iss.rdbuf());

		std::ostringstream oss;
		std::streambuf* coutbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		player->set_bid();
		REQUIRE_FALSE(player->blind_nil_bid());
		REQUIRE(player->nil_bid());

		std::cin.rdbuf(cinbuf);
		std::cout.rdbuf(coutbuf);
	}

	SECTION("Testing with setting bid to blind nil")
	{
		std::istringstream iss("y\n");
		std::streambuf* cinbuf = std::cin.rdbuf();
		std::cin.rdbuf(iss.rdbuf());

		std::ostringstream oss;
		std::streambuf* coutbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		player->set_bid();
		REQUIRE(player->blind_nil_bid());
		REQUIRE_FALSE(player->nil_bid());

		std::cin.rdbuf(cinbuf);
		std::cout.rdbuf(coutbuf);
	}
	
	SECTION("Testing with setting bid to non nil")
	{
		std::istringstream iss("n\n6\n");
		std::streambuf* cinbuf = std::cin.rdbuf();
		std::cin.rdbuf(iss.rdbuf());

		std::ostringstream oss;
		std::streambuf* coutbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		player->set_bid();
		REQUIRE_FALSE(player->blind_nil_bid());
		REQUIRE_FALSE(player->nil_bid());

		std::cin.rdbuf(cinbuf);
		std::cout.rdbuf(coutbuf);
	}

	delete player;
	player = nullptr;
}

TEST_CASE("Testing lead_card function", "[six]")
{
	std::string name = random_name_generator();

	Card card_2s(Rank::TWO, Suit::SPADES);
	Card card_7d(Rank::SEVEN, Suit::DIAMONDS);
	Card card_8d(Rank::EIGHT, Suit::DIAMONDS);
	Card card_ac(Rank::ACE, Suit::CLUBS);
	Card card_js(Rank::JACK, Suit::SPADES);

	SECTION("Testing when broken spades is true")
	{
		// Must print out all cards including Spades 
		Player* player = Player_factory(name, "Human");

		player->add_card(card_2s);
		player->add_card(card_7d);
		player->add_card(card_8d);
		player->add_card(card_ac);
		player->add_card(card_js);

		std::istringstream iss("0\n6\n5\n5\n1\n2\n2\n1\n"); // 0n,6n,5y,5n,1y,2y,1y
		std::streambuf* cinbuf = std::cin.rdbuf();
		std::cin.rdbuf(iss.rdbuf());

		std::ostringstream oss;
		std::streambuf* coutbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		std::string expected_output 
		{ "\
[1] Ace of Clubs\n\
[2] Seven of Diamonds\n\
[3] Eight of Diamonds\n\
[4] Two of Spades\n\
[5] Jack of Spades\n\
Please enter a number between 1 and 5 inclusive:\n\
Please enter a number between 1 and 5 inclusive:\n\
Please enter a number between 1 and 5 inclusive:\n\
[1] Ace of Clubs\n\
[2] Seven of Diamonds\n\
[3] Eight of Diamonds\n\
[4] Two of Spades\n\
Please enter a number between 1 and 4 inclusive:\n\
Please enter a number between 1 and 4 inclusive:\n\
[1] Seven of Diamonds\n\
[2] Eight of Diamonds\n\
[3] Two of Spades\n\
Please enter a number between 1 and 3 inclusive:\n\
[1] Seven of Diamonds\n\
[2] Two of Spades\n\
Please enter a number between 1 and 2 inclusive:\n\
[1] Seven of Diamonds\n\
Please enter a number between 1 and 1 inclusive:\n\
" };
		
		REQUIRE(player->lead_card(true) == card_js);
		REQUIRE(player->lead_card(true) == card_ac);
		REQUIRE(player->lead_card(true) == card_8d);
		REQUIRE(player->lead_card(true) == card_2s);
		REQUIRE(player->lead_card(true) == card_7d);
		REQUIRE_THROWS(player->lead_card(true));
		REQUIRE(oss.str() == expected_output);

		std::cin.rdbuf(cinbuf);
		std::cout.rdbuf(coutbuf);

		delete player;
		player = nullptr;
	}
	
	SECTION("Testing when broken spades is false")
	{
		// Cannot print out Spade cards until all other non-spades are gone
		Player* player = Player_factory(name, "Human");

		player->add_card(card_2s);
		player->add_card(card_7d);
		player->add_card(card_8d);
		player->add_card(card_ac);
		player->add_card(card_js);

		std::istringstream iss("0\n5\n4\n3\n1\n1\n2\n1\n"); // 0n,5n,4n,3y,1y,1y,2y,1y
		std::streambuf* cinbuf = std::cin.rdbuf();
		std::cin.rdbuf(iss.rdbuf());

		std::ostringstream oss;
		std::streambuf* coutbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		std::string expected_output
		{ "\
[1] Ace of Clubs\n\
[2] Seven of Diamonds\n\
[3] Eight of Diamonds\n\
Please enter a number between 1 and 3 inclusive:\n\
Please enter a number between 1 and 3 inclusive:\n\
Please enter a number between 1 and 3 inclusive:\n\
Please enter a number between 1 and 3 inclusive:\n\
[1] Ace of Clubs\n\
[2] Seven of Diamonds\n\
Please enter a number between 1 and 2 inclusive:\n\
[1] Seven of Diamonds\n\
Please enter a number between 1 and 1 inclusive:\n\
[1] Two of Spades\n\
[2] Jack of Spades\n\
Please enter a number between 1 and 2 inclusive:\n\
[1] Two of Spades\n\
Please enter a number between 1 and 1 inclusive:\n\
" };

		REQUIRE(player->lead_card(false) == card_8d);
		REQUIRE(player->lead_card(false) == card_ac);
		REQUIRE(player->lead_card(false) == card_7d);
		REQUIRE(player->lead_card(false) == card_js);
		REQUIRE(player->lead_card(false) == card_2s);
		REQUIRE_THROWS(player->lead_card(true));
		REQUIRE(oss.str() == expected_output);

		std::cin.rdbuf(cinbuf);
		std::cout.rdbuf(coutbuf);

		delete player;
		player = nullptr;
	}
}

TEST_CASE("Testing play_card function", "[seven]")
{
	std::string name = random_name_generator();

	Card card_2s(Rank::TWO, Suit::SPADES);
	Card card_7d(Rank::SEVEN, Suit::DIAMONDS);
	Card card_8d(Rank::EIGHT, Suit::DIAMONDS);
	Card card_ac(Rank::ACE, Suit::CLUBS);
	Card card_4h(Rank::FOUR, Suit::HEARTS);
	Card card_js(Rank::JACK, Suit::SPADES);

	SECTION("Cards that follow lead suit exist")
	{
		Player* player = Player_factory(name, "Human");
		player->add_card(card_2s);
		player->add_card(card_7d);
		player->add_card(card_8d);
		player->add_card(card_ac);
		player->add_card(card_4h);
		player->add_card(card_js);

		std::istringstream iss("0\n6\n1\n3\n2\n1\n1\n1\n1\n"); // 0n,6n,1y,3n,2y,1y,1y,1y,1y
		std::streambuf* cinbuf = std::cin.rdbuf();
		std::cin.rdbuf(iss.rdbuf());

		std::ostringstream oss;
		std::streambuf* coutbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		std::string expected_output
		{"\
[1] Four of Hearts\n\
Please enter a number between 1 and 1 inclusive:\n\
Please enter a number between 1 and 1 inclusive:\n\
Please enter a number between 1 and 1 inclusive:\n\
[1] Seven of Diamonds\n\
[2] Eight of Diamonds\n\
Please enter a number between 1 and 2 inclusive:\n\
Please enter a number between 1 and 2 inclusive:\n\
[1] Seven of Diamonds\n\
Please enter a number between 1 and 1 inclusive:\n\
[1] Two of Spades\n\
[2] Jack of Spades\n\
Please enter a number between 1 and 2 inclusive:\n\
[1] Jack of Spades\n\
Please enter a number between 1 and 1 inclusive:\n\
[1] Ace of Clubs\n\
Please enter a number between 1 and 1 inclusive:\n\
"};

		REQUIRE(player->play_card(Suit::HEARTS) == card_4h);
		REQUIRE(player->play_card(Suit::DIAMONDS) == card_8d);
		REQUIRE(player->play_card(Suit::DIAMONDS) == card_7d);
		REQUIRE(player->play_card(Suit::SPADES) == card_2s);
		REQUIRE(player->play_card(Suit::SPADES) == card_js);
		REQUIRE(player->play_card(Suit::CLUBS) == card_ac);
		REQUIRE_THROWS(player->lead_card(true));
		REQUIRE(oss.str() == expected_output);

		std::cin.rdbuf(cinbuf);
		std::cout.rdbuf(coutbuf);

		delete player;
		player = nullptr;
	}

	SECTION("Cards that follow lead suit do not exist")
	{
		Player* player = Player_factory(name, "Human");
		player->add_card(card_2s);
		player->add_card(card_7d);
		player->add_card(card_8d);
		player->add_card(card_ac);
		player->add_card(card_4h);
		player->add_card(card_js);

		std::istringstream iss("1\n1\n2\n3\n1\n1\n2\n1\n");
		std::streambuf* cinbuf = std::cin.rdbuf();
		std::cin.rdbuf(iss.rdbuf());

		std::ostringstream oss;
		std::streambuf* coutbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		std::string expected_output
		{"\
[1] Four of Hearts\n\
Please enter a number between 1 and 1 inclusive:\n\
[1] Ace of Clubs\n\
[2] Seven of Diamonds\n\
[3] Eight of Diamonds\n\
[4] Two of Spades\n\
[5] Jack of Spades\n\
Please enter a number between 1 and 5 inclusive:\n\
[1] Seven of Diamonds\n\
[2] Eight of Diamonds\n\
[3] Two of Spades\n\
[4] Jack of Spades\n\
Please enter a number between 1 and 4 inclusive:\n\
[1] Seven of Diamonds\n\
[2] Two of Spades\n\
[3] Jack of Spades\n\
Please enter a number between 1 and 3 inclusive:\n\
[1] Seven of Diamonds\n\
[2] Two of Spades\n\
Please enter a number between 1 and 2 inclusive:\n\
[1] Two of Spades\n\
Please enter a number between 1 and 1 inclusive:\n\
[1] Ace of Clubs\n\
[2] Four of Hearts\n\
Please enter a number between 1 and 2 inclusive:\n\
[1] Ace of Clubs\n\
Please enter a number between 1 and 1 inclusive:\n\
"};

		REQUIRE(player->play_card(Suit::HEARTS) == card_4h); // 1
		REQUIRE(player->play_card(Suit::HEARTS) == card_ac); // 1
		REQUIRE(player->play_card(Suit::CLUBS) == card_8d); // 2
		REQUIRE(player->play_card(Suit::HEARTS) == card_js); // 3
		REQUIRE(player->play_card(Suit::CLUBS) == card_7d); // 1
		REQUIRE(player->play_card(Suit::DIAMONDS) == card_2s); // 1
		player->add_card(card_ac);
		player->add_card(card_4h);
		REQUIRE(player->play_card(Suit::SPADES) == card_4h); // 2
		REQUIRE(player->play_card(Suit::SPADES) == card_ac); // 1
		REQUIRE_THROWS(player->play_card(Suit::CLUBS));
		REQUIRE_THROWS(player->play_card(Suit::DIAMONDS));
		REQUIRE_THROWS(player->play_card(Suit::HEARTS));
		REQUIRE_THROWS(player->play_card(Suit::SPADES));
		REQUIRE(oss.str() == expected_output);

		std::cin.rdbuf(cinbuf);
		std::cout.rdbuf(coutbuf);

		delete player;
		player = nullptr;
	}
}

TEST_CASE("Testing reset function", "[eight]")
{
	std::string name = random_name_generator();
	Player* player = Player_factory(name, "Human");
	
	std::istringstream iss("n\n5\n");
	std::streambuf* cinbuf = std::cin.rdbuf();
	std::cin.rdbuf(iss.rdbuf());

	std::ostringstream oss;
	std::streambuf* coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(oss.rdbuf());

	player->add_card(Card(Rank::TWO, Suit::SPADES));
	player->add_card(Card(Rank::SEVEN, Suit::DIAMONDS));
	player->add_card(Card(Rank::EIGHT, Suit::DIAMONDS));
	player->add_card(Card(Rank::ACE, Suit::CLUBS));
	player->add_card(Card(Rank::FOUR, Suit::HEARTS));
	player->add_card(Card(Rank::JACK, Suit::SPADES));
	
	player->add_trick();
	player->set_bid();

	std::cin.rdbuf(cinbuf);
	std::cout.rdbuf(coutbuf);

	player->reset();

	REQUIRE(player->get_bid() == 0);
	REQUIRE(player->get_trick() == 0);
	REQUIRE_THROWS(player->lead_card(false));

	delete player;
	player = nullptr;
}