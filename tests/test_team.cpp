#include "catch_amalgamated.hpp"
#include "../Team.h"
#include <string>
#include <random>
#include <algorithm>
#include <vector>

class IORedirect
{
private:
	std::streambuf* cinbuf;
	std::streambuf* coutbuf;
public:
	std::istringstream iss;
	std::ostringstream oss;

	IORedirect(const std::string& str = "") : cinbuf(std::cin.rdbuf()), coutbuf(std::cout.rdbuf()), iss(str)
	{
		open();
	}

	void open()
	{
		std::cin.rdbuf(iss.rdbuf());
		std::cout.rdbuf(oss.rdbuf());
	}

	void close()
	{
		std::cin.rdbuf(cinbuf);
		std::cout.rdbuf(coutbuf);
	}

	~IORedirect()
	{
		close();
	}
};

bool operator==(const Score& lhs, const Score& rhs)
{
	return	lhs.bid == rhs.bid &&
		lhs.tricks == rhs.tricks &&
		lhs.bags == rhs.bags &&
		lhs.bid_score == rhs.bid_score &&
		lhs.nil_score == rhs.nil_score &&
		lhs.bag_penalty == rhs.bag_penalty &&
		lhs.hand_score == rhs.hand_score &&
		lhs.total_bags == rhs.total_bags &&
		lhs.game_score == rhs.game_score;
}

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

TEST_CASE("Testing constructor and get_name function", "[one]")
{
	PlayerComponents player0 = { random_name_generator(), "Human" };
	PlayerComponents player1 = { random_name_generator(), "Human" };

	Team team(player0, player1);

	SECTION("Testing invalid input to get_name")
	{
		REQUIRE_THROWS(team.get_name(-1));
		REQUIRE_THROWS(team.get_name(2));
	}

	SECTION("Testing valid input")
	{
		REQUIRE(team.get_name(0) == player0.name);
		REQUIRE(team.get_name(1) == player1.name);
	}
}

TEST_CASE("Testing add_card and set_bid", "[two]")
{
	PlayerComponents player0 = { random_name_generator(), "Human" };
	PlayerComponents player1 = { random_name_generator(), "Human" };

	Team team(player0, player1);

	SECTION("Testing invalid input to add_card")
	{
		REQUIRE_THROWS(team.add_card(-1, random_card_generator()));
		REQUIRE_THROWS(team.add_card(2, random_card_generator()));
	}

	SECTION("Testing invalid input to set_bid")
	{
		REQUIRE_THROWS(team.set_bid(-1));
		REQUIRE_THROWS(team.set_bid(2));
	}

	SECTION("Testing valid input")
	{
		const int max_hand_size = 13;

		Card player0_cards[max_hand_size];
		Card player1_cards[max_hand_size];

		for (int i = 0; i < max_hand_size; ++i)
		{
			player0_cards[i] = random_card_generator();
			player1_cards[i] = random_card_generator();

			REQUIRE(team.add_card(0, player0_cards[i]));
			REQUIRE(team.add_card(1, player1_cards[i]));
		}

		REQUIRE_FALSE(team.add_card(0, random_card_generator()));
		REQUIRE_FALSE(team.add_card(1, random_card_generator()));

		std::sort(std::begin(player0_cards), std::end(player0_cards));
		std::sort(std::begin(player1_cards), std::end(player1_cards));

		std::string part1{"Does Player "},
			part2{ " want to bid blind nil? [y/n]\n" },
			part3{ "How much would you like to bid (0 to 13 inclusive)?\n" };

		std::string expected_output_player0 = part1 + player0.name + part2
			+ print_hand(player0_cards, max_hand_size) + part3;

		std::string expected_output_player1 = part1 + player1.name + part2
			+ print_hand(player1_cards, max_hand_size) + part3;

		IORedirect iod("n\n3\nn\n5");

		team.set_bid(1);
		team.set_bid(0);

		iod.close();
		REQUIRE(iod.oss.str() == expected_output_player1 + expected_output_player0);
	}
}

TEST_CASE("Testing output operator for Team", "[three]")
{
	PlayerComponents player0 = { random_name_generator(), "Human" };
	PlayerComponents player1 = { random_name_generator(), "Human" };

	Team team(player0, player1);

	std::stringstream ss;
	ss << team;
	REQUIRE(ss.str() == "Team " + player0.name + '/' + player1.name);
}

TEST_CASE("Testing lead_card", "[four]")
{
	PlayerComponents player0 = { random_name_generator(), "Human" };
	PlayerComponents player1 = { random_name_generator(), "Human" };

	Team team(player0, player1);

	SECTION("Testing invalid input to lead_card")
	{
		REQUIRE_THROWS(team.lead_card(-1, false));
		REQUIRE_THROWS(team.lead_card(-1, true));

		REQUIRE_THROWS(team.lead_card(2, false));
		REQUIRE_THROWS(team.lead_card(2, true));
	}

	SECTION("Testing broken_spade == true")
	{
		const int max_hand_size = 13;

		Card player0_cards[max_hand_size];
		Card player1_cards[max_hand_size];

		for (int i = 0; i < max_hand_size; ++i)
		{
			player0_cards[i] = random_card_generator();
			player1_cards[i] = random_card_generator();

			team.add_card(0, player0_cards[i]);
			team.add_card(1, player1_cards[i]);
		}

		std::sort(std::begin(player0_cards), std::end(player0_cards));
		std::sort(std::begin(player1_cards), std::end(player1_cards));

		std::random_device rd;
		std::minstd_rand gen(rd());
		std::uniform_int_distribution<> distrib(0, max_hand_size - 1);

		int card_to_pick0 = distrib(gen);
		int card_to_pick1 = distrib(gen);
		std::string input = std::to_string(card_to_pick0 + 1) + '\n' + std::to_string(card_to_pick1 + 1);

		IORedirect iod(input);

		REQUIRE(team.lead_card(0, true) == player0_cards[card_to_pick0]);
		REQUIRE(team.lead_card(1, true) == player1_cards[card_to_pick1]);
	}

	SECTION("Testing broken_spade == false")
	{
		const int max_hand_size = 13;

		Card player0_cards[max_hand_size] =
		{
			Card(Rank::TWO, Suit::DIAMONDS),
			Card(Rank::NINE, Suit::DIAMONDS),
			Card(Rank::FIVE, Suit::HEARTS),
			Card(Rank::TWO, Suit::CLUBS),
			Card(Rank::QUEEN, Suit::CLUBS),
			Card(Rank::SEVEN, Suit::HEARTS),
			Card(Rank::EIGHT, Suit::HEARTS),
			Card(Rank::SIX, Suit::HEARTS),
			Card(Rank::ACE, Suit::CLUBS),
			Card(Rank::JACK, Suit::CLUBS),
			Card(Rank::TEN, Suit::HEARTS),
			Card(Rank::FOUR, Suit::CLUBS),
			Card(Rank::QUEEN, Suit::DIAMONDS),
		};
		Card player1_cards[max_hand_size] =
		{
			Card(Rank::SIX, Suit::SPADES),
			Card(Rank::TEN, Suit::SPADES),
			Card(Rank::NINE, Suit::SPADES),
			Card(Rank::SIX, Suit::CLUBS),
			Card(Rank::KING, Suit::SPADES),
			Card(Rank::SEVEN, Suit::SPADES),
			Card(Rank::FIVE, Suit::CLUBS),
			Card(Rank::KING, Suit::CLUBS),
			Card(Rank::TWO, Suit::SPADES),
			Card(Rank::ACE, Suit::SPADES),
			Card(Rank::TEN, Suit::DIAMONDS),
			Card(Rank::NINE, Suit::HEARTS),
			Card(Rank::KING, Suit::DIAMONDS),
		};
		const int player1_cards_spades_removed_size = 6;
		Card player1_cards_spades_removed[player1_cards_spades_removed_size] =
		{
			Card(Rank::SIX, Suit::CLUBS),
			Card(Rank::FIVE, Suit::CLUBS),
			Card(Rank::KING, Suit::CLUBS),
			Card(Rank::TEN, Suit::DIAMONDS),
			Card(Rank::NINE, Suit::HEARTS),
			Card(Rank::KING, Suit::DIAMONDS),
		};
		const int player1_cards_spades_size = 7;
		Card player1_cards_spades[player1_cards_spades_size] =
		{
			Card(Rank::SIX, Suit::SPADES),
			Card(Rank::TEN, Suit::SPADES),
			Card(Rank::NINE, Suit::SPADES),
			Card(Rank::KING, Suit::SPADES),
			Card(Rank::SEVEN, Suit::SPADES),
			Card(Rank::TWO, Suit::SPADES),
			Card(Rank::ACE, Suit::SPADES),
		};

		for (int i = 0; i < max_hand_size; ++i)
		{
			team.add_card(0, player0_cards[i]);
			team.add_card(1, player1_cards[i]);
		}

		std::sort(std::begin(player0_cards), std::end(player0_cards));
		std::sort(std::begin(player1_cards), std::end(player1_cards));
		std::sort(std::begin(player1_cards_spades_removed), std::end(player1_cards_spades_removed));
		std::sort(std::begin(player1_cards_spades), std::end(player1_cards_spades));

		std::random_device rd;
		std::minstd_rand gen(rd());
		int card_to_pick0 = std::uniform_int_distribution<>(0, max_hand_size - 1)(gen);
		int card_to_pick1_wo_spades = std::uniform_int_distribution<>(0, player1_cards_spades_removed_size - 1)(gen);
		int card_to_pick1_spades = std::uniform_int_distribution<>(0, player1_cards_spades_size - 1)(gen);

		std::string input = std::to_string(card_to_pick0 + 1) + '\n' + std::to_string(card_to_pick1_wo_spades + 1)
			+ "\n1\n1\n1\n1\n1\n" + std::to_string(card_to_pick1_spades + 1);

		IORedirect iod(input);

		REQUIRE(team.lead_card(0, false) == player0_cards[card_to_pick0]);
		REQUIRE(team.lead_card(1, false) == player1_cards_spades_removed[card_to_pick1_wo_spades]);

		for (int i = 0; i < player1_cards_spades_removed_size - 1; ++i)
		{
			team.lead_card(1, false);
		}

		REQUIRE(team.lead_card(1, false) == player1_cards_spades[card_to_pick1_spades]);
	}
}

TEST_CASE("Testing play_card", "[five]")
{
	PlayerComponents player0 = { random_name_generator(), "Human" };
	PlayerComponents player1 = { random_name_generator(), "Human" };

	Team team(player0, player1);

	SECTION("Testing invalid input to play_card")
	{
		REQUIRE_THROWS(team.play_card(-1, Suit::CLUBS));
		REQUIRE_THROWS(team.play_card(-1, Suit::DIAMONDS));
		REQUIRE_THROWS(team.play_card(-1, Suit::HEARTS));
		REQUIRE_THROWS(team.play_card(-1, Suit::SPADES));

		REQUIRE_THROWS(team.play_card(2, Suit::CLUBS));
		REQUIRE_THROWS(team.play_card(2, Suit::DIAMONDS));
		REQUIRE_THROWS(team.play_card(2, Suit::HEARTS));
		REQUIRE_THROWS(team.play_card(2, Suit::SPADES));
	}

	SECTION("Testing valid input")
	{

		const int max_hand_size = 13;

		Card player0_cards[max_hand_size] =
		{
			Card(Rank::QUEEN, Suit::HEARTS),
			Card(Rank::ACE, Suit::SPADES),
			Card(Rank::TWO, Suit::CLUBS),
			Card(Rank::JACK, Suit::SPADES),
			Card(Rank::NINE, Suit::HEARTS),
			Card(Rank::KING, Suit::DIAMONDS),
			Card(Rank::KING, Suit::SPADES),
			Card(Rank::EIGHT, Suit::CLUBS),
			Card(Rank::SIX, Suit::SPADES),
			Card(Rank::KING, Suit::HEARTS),
			Card(Rank::SIX, Suit::CLUBS),
			Card(Rank::ACE, Suit::HEARTS),
			Card(Rank::FIVE, Suit::DIAMONDS)
		};

		const int player0_cards_w_hearts_size = 4;
		Card player0_cards_w_hearts[player0_cards_w_hearts_size] =
		{
			Card(Rank::QUEEN, Suit::HEARTS),
			Card(Rank::NINE, Suit::HEARTS),
			Card(Rank::KING, Suit::HEARTS),
			Card(Rank::ACE, Suit::HEARTS)
		};
		const int player0_cards_wo_hearts_size = max_hand_size - player0_cards_w_hearts_size;
		Card player0_cards_wo_hearts[player0_cards_wo_hearts_size] =
		{
			Card(Rank::ACE, Suit::SPADES),
			Card(Rank::TWO, Suit::CLUBS),
			Card(Rank::JACK, Suit::SPADES),
			Card(Rank::KING, Suit::DIAMONDS),
			Card(Rank::KING, Suit::SPADES),
			Card(Rank::EIGHT, Suit::CLUBS),
			Card(Rank::SIX, Suit::SPADES),
			Card(Rank::SIX, Suit::CLUBS),
			Card(Rank::FIVE, Suit::DIAMONDS)
		};


		Card player1_cards[max_hand_size] =
		{
			Card(Rank::JACK, Suit::DIAMONDS),
			Card(Rank::SIX, Suit::DIAMONDS),
			Card(Rank::FIVE, Suit::HEARTS),
			Card(Rank::THREE, Suit::DIAMONDS),
			Card(Rank::SEVEN, Suit::HEARTS),
			Card(Rank::QUEEN, Suit::DIAMONDS),
			Card(Rank::TEN, Suit::DIAMONDS),
			Card(Rank::SEVEN, Suit::SPADES),
			Card(Rank::FOUR, Suit::CLUBS),
			Card(Rank::TEN, Suit::SPADES),
			Card(Rank::TWO, Suit::HEARTS),
			Card(Rank::TEN, Suit::HEARTS),
			Card(Rank::FOUR, Suit::DIAMONDS)
		};

		const int player1_cards_w_spades_size = 2;
		Card player1_cards_w_spades[player1_cards_w_spades_size] =
		{
			Card(Rank::SEVEN, Suit::SPADES),
			Card(Rank::TEN, Suit::SPADES)
		};
		const int player1_cards_wo_spades_size = max_hand_size - player1_cards_w_spades_size;
		Card player1_cards_wo_spades[player1_cards_wo_spades_size] =
		{
			Card(Rank::JACK, Suit::DIAMONDS),
			Card(Rank::SIX, Suit::DIAMONDS),
			Card(Rank::FIVE, Suit::HEARTS),
			Card(Rank::THREE, Suit::DIAMONDS),
			Card(Rank::SEVEN, Suit::HEARTS),
			Card(Rank::QUEEN, Suit::DIAMONDS),
			Card(Rank::TEN, Suit::DIAMONDS),
			Card(Rank::FOUR, Suit::CLUBS),
			Card(Rank::TWO, Suit::HEARTS),
			Card(Rank::TEN, Suit::HEARTS),
			Card(Rank::FOUR, Suit::DIAMONDS)
		};


		for (int i = 0; i < max_hand_size; ++i)
		{
			team.add_card(0, player0_cards[i]);
			team.add_card(1, player1_cards[i]);
		}

		std::sort(std::begin(player0_cards), std::end(player0_cards));
		std::sort(std::begin(player0_cards_w_hearts), std::end(player0_cards_w_hearts));
		std::sort(std::begin(player0_cards_wo_hearts), std::end(player0_cards_wo_hearts));
		std::sort(std::begin(player1_cards), std::end(player1_cards));
		std::sort(std::begin(player1_cards_w_spades), std::end(player1_cards_w_spades));
		std::sort(std::begin(player1_cards_wo_spades), std::end(player1_cards_wo_spades));

		std::random_device rd;
		std::minstd_rand gen(rd());
		int card_to_pick0_w_hearts = std::uniform_int_distribution<>(0, player0_cards_w_hearts_size - 1)(gen);
		int card_to_pick0_wo_hearts = std::uniform_int_distribution<>(0, player0_cards_wo_hearts_size - 1)(gen);
		int card_to_pick1_w_spades = std::uniform_int_distribution<>(0, player1_cards_w_spades_size - 1)(gen);
		int card_to_pick1_wo_spades = std::uniform_int_distribution<>(0, player1_cards_wo_spades_size - 1)(gen);

		std::string input = std::to_string(card_to_pick0_w_hearts + 1) + '\n' + std::to_string(card_to_pick1_w_spades + 1) + '\n';
		for (int i = 0; i < player0_cards_w_hearts_size - 1; ++i)
		{
			input += "1\n";
		}
		for (int i = 0; i < player1_cards_w_spades_size - 1; ++i)
		{
			input += "1\n";
		}
		input += std::to_string(card_to_pick0_wo_hearts + 1) + '\n' + std::to_string(card_to_pick1_wo_spades + 1) + '\n';

		IORedirect iod(input);

		REQUIRE(team.play_card(0, Suit::HEARTS) == player0_cards_w_hearts[card_to_pick0_w_hearts]);
		REQUIRE(team.play_card(1, Suit::SPADES) == player1_cards_w_spades[card_to_pick1_w_spades]);

		for (int i = 0; i < player0_cards_w_hearts_size - 1; ++i)
		{
			team.play_card(0, Suit::HEARTS);
		}
		for (int i = 0; i < player1_cards_w_spades_size - 1; ++i)
		{
			team.play_card(1, Suit::SPADES);
		}

		REQUIRE(team.play_card(0, Suit::HEARTS) == player0_cards_wo_hearts[card_to_pick0_wo_hearts]);
		REQUIRE(team.play_card(1, Suit::SPADES) == player1_cards_wo_spades[card_to_pick1_wo_spades]);
	}
}

TEST_CASE("Testing add_trick", "[six]")
{
	PlayerComponents player0 = { random_name_generator(), "Human" };
	PlayerComponents player1 = { random_name_generator(), "Human" };

	Team team(player0, player1);

	SECTION("Testing invalid input")
	{
		REQUIRE_THROWS(team.add_trick(-1));
		REQUIRE_THROWS(team.add_trick(2));
	}

	SECTION("Testing valid input")
	{
		const int max_hand_size = 13;
		for (int i = 0; i < max_hand_size; ++i)
		{
			REQUIRE(team.add_trick(0));
			REQUIRE(team.add_trick(1));
		}
		REQUIRE_FALSE(team.add_trick(0));
		REQUIRE_FALSE(team.add_trick(1));
	}
}

// TODO
// also testing add_trick
TEST_CASE("Testing get_score and reset", "[seven]")
{
	PlayerComponents player0 = { random_name_generator(), "Human" };
	PlayerComponents player1 = { random_name_generator(), "Human" };

	Team team(player0, player1);

	SECTION("Testing empty Score for team")
	{
		REQUIRE(team.get_score() == Score());
	}

	SECTION("Both players bid non-nil")
	{
		SECTION("Win")
		{
			SECTION("Both players reached their own bids")
			{
				SECTION("With no bags")
				{
					IORedirect iod("n\n4\nn\n5");
					team.set_bid(0);
					team.set_bid(1);

					for (int i = 0; i < 4; ++i) { team.add_trick(0); }
					for (int i = 0; i < 5; ++i) { team.add_trick(1); }

					Score actual_output = team.get_score();

					Score expected_output;
					expected_output.bid = 9;
					expected_output.tricks = 9;
					expected_output.bid_score = 90;
					expected_output.hand_score = 90;
					expected_output.game_score = 90;

					iod.close();

					REQUIRE(expected_output == actual_output);
				}
				SECTION("With bags")
				{
					IORedirect iod("n\n2\nn\n3");
					team.set_bid(0);
					team.set_bid(1);

					for (int i = 0; i < 4; ++i) { team.add_trick(0); }
					for (int i = 0; i < 5; ++i) { team.add_trick(1); }

					Score actual_output = team.get_score();

					Score expected_output;
					expected_output.bid = 5;
					expected_output.tricks = 9;
					expected_output.bags = 4;
					expected_output.bid_score = 54;
					expected_output.hand_score = 54;
					expected_output.total_bags = 4;
					expected_output.game_score = 54;

					iod.close();

					REQUIRE(expected_output == actual_output);
				}
			}
			SECTION("Only one of the players reached their own bid")
			{
				SECTION("With no bags")
				{
					IORedirect iod("n\n3\nn\n4");
					team.set_bid(0);
					team.set_bid(1);

					for (int i = 0; i < 2; ++i) { team.add_trick(0); }
					for (int i = 0; i < 5; ++i) { team.add_trick(1); }

					Score actual_output = team.get_score();

					Score expected_output;
					expected_output.bid = 7;
					expected_output.tricks = 7;
					expected_output.bid_score = 70;
					expected_output.hand_score = 70;
					expected_output.game_score = 70;

					iod.close();

					REQUIRE(expected_output == actual_output);
				}
				SECTION("With bags")
				{
					IORedirect iod("n\n4\nn\n2");
					team.set_bid(0);
					team.set_bid(1);

					for (int i = 0; i < 2; ++i) { team.add_trick(0); }
					for (int i = 0; i < 6; ++i) { team.add_trick(1); }

					Score actual_output = team.get_score();

					Score expected_output;
					expected_output.bid = 6;
					expected_output.tricks = 8;
					expected_output.bags = 2;
					expected_output.bid_score = 62;
					expected_output.hand_score = 62;
					expected_output.total_bags = 2;
					expected_output.game_score = 62;

					iod.close();

					REQUIRE(expected_output == actual_output);
				}
			}
		}
		SECTION("Lost")
		{
			SECTION("Both players didn't reach their own bid")
			{
				IORedirect iod("n\n4\nn\n8");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 2; ++i) { team.add_trick(0); }
				for (int i = 0; i < 6; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.bid = 12;
				expected_output.tricks = 8;
				expected_output.bid_score = -120;
				expected_output.hand_score = -120;
				expected_output.game_score = -120;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
			SECTION("Only one of the players reached their own bid")
			{
				SECTION("Reached exactly their bid")
				{
					IORedirect iod("n\n8\nn\n4");
					team.set_bid(0);
					team.set_bid(1);

					for (int i = 0; i < 8; ++i) { team.add_trick(0); }
					for (int i = 0; i < 2; ++i) { team.add_trick(1); }

					Score actual_output = team.get_score();

					Score expected_output;
					expected_output.bid = 12;
					expected_output.tricks = 10;
					expected_output.bid_score = -120;
					expected_output.hand_score = -120;
					expected_output.game_score = -120;

					iod.close();

					REQUIRE(expected_output == actual_output);
				}
				SECTION("Reached more than their bid")
				{
					IORedirect iod("n\n4\nn\n8");
					team.set_bid(0);
					team.set_bid(1);

					for (int i = 0; i < 2; ++i) { team.add_trick(0); }
					for (int i = 0; i < 9; ++i) { team.add_trick(1); }

					Score actual_output = team.get_score();

					Score expected_output;
					expected_output.bid = 12;
					expected_output.tricks = 11;
					expected_output.bid_score = -120;
					expected_output.hand_score = -120;
					expected_output.game_score = -120;

					iod.close();

					REQUIRE(expected_output == actual_output);
				}
			}
		}
	}

	SECTION("Both players bid type of nil")
	{
		SECTION("Double Blind nil")
		{
			SECTION("Both Win")
			{
				IORedirect iod("y\ny\n");
				team.set_bid(0);
				team.set_bid(1);

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.nil_score = 400;
				expected_output.hand_score = 400;
				expected_output.game_score = 400;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
			SECTION("One Win")
			{
				IORedirect iod("y\ny\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 4; ++i) { team.add_trick(0); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.tricks = 4;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
			SECTION("Both Lose")
			{
				IORedirect iod("y\ny\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 1; ++i) { team.add_trick(0); }
				for (int i = 0; i < 4; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.tricks = 5;
				expected_output.nil_score = -400;
				expected_output.hand_score = -400;
				expected_output.game_score = -400;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
		}
		SECTION("Double Regular nil")
		{
			SECTION("Both Win")
			{
				IORedirect iod("n\n0\nn\n0\n");
				team.set_bid(0);
				team.set_bid(1);

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.nil_score = 200;
				expected_output.hand_score = 200;
				expected_output.game_score = 200;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
			SECTION("One Win")
			{
				IORedirect iod("n\n0\nn\n0\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 4; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.tricks = 4;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
			SECTION("Both Lose")
			{
				IORedirect iod("n\n0\nn\n0\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 1; ++i) { team.add_trick(0); }
				for (int i = 0; i < 4; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.tricks = 5;
				expected_output.nil_score = -200;
				expected_output.hand_score = -200;
				expected_output.game_score = -200;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
		}
		SECTION("One blind nil and the other regular nil")
		{
			SECTION("Both Win")
			{
				IORedirect iod("y\nn\n0\n");
				team.set_bid(0);
				team.set_bid(1);

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.nil_score = 300;
				expected_output.hand_score = 300;
				expected_output.game_score = 300;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
			SECTION("Both Lose")
			{
				IORedirect iod("y\nn\n0\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 1; ++i) { team.add_trick(0); }
				for (int i = 0; i < 3; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.tricks = 4;
				expected_output.nil_score = -300;
				expected_output.hand_score = -300;
				expected_output.game_score = -300;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
			SECTION("Only blind nil wins")
			{
				IORedirect iod("y\nn\n0\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 3; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.tricks = 3;
				expected_output.nil_score = 100;
				expected_output.hand_score = 100;
				expected_output.game_score = 100;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
			SECTION("Only regular nil wins")
			{
				IORedirect iod("y\nn\n0\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 7; ++i) { team.add_trick(0); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.tricks = 7;
				expected_output.nil_score = -100;
				expected_output.hand_score = -100;
				expected_output.game_score = -100;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
		}
	}

	SECTION("One player bids blind nil")
	{
		SECTION("Both Win")
		{
			SECTION("With no bags")
			{
				IORedirect iod("y\nn\n6\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 6; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.bid = 6;
				expected_output.tricks = 6;
				expected_output.bid_score = 60;
				expected_output.nil_score = 200;
				expected_output.hand_score = 260;
				expected_output.game_score = 260;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
			SECTION("With bags")
			{
				IORedirect iod("y\nn\n6\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 7; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.bid = 6;
				expected_output.tricks = 7;
				expected_output.bags = 1;
				expected_output.bid_score = 61;
				expected_output.nil_score = 200;
				expected_output.hand_score = 261;
				expected_output.total_bags = 1;
				expected_output.game_score = 261;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
		}
		SECTION("Both Lost")
		{
			IORedirect iod("y\nn\n6\n");
			team.set_bid(0);
			team.set_bid(1);

			for (int i = 0; i < 3; ++i) { team.add_trick(0); }

			Score actual_output = team.get_score();

			Score expected_output;
			expected_output.bid = 6;
			expected_output.tricks = 3;
			expected_output.bid_score = -60;
			expected_output.nil_score = -200;
			expected_output.hand_score = -200 - 60;
			expected_output.game_score = -200 - 60;

			iod.close();

			REQUIRE(actual_output == expected_output);
		}
		SECTION("Only blind nil won")
		{
			IORedirect iod("y\nn\n6\n");
			team.set_bid(0);
			team.set_bid(1);

			for (int i = 0; i < 5; ++i) { team.add_trick(1); }

			Score actual_output = team.get_score();

			Score expected_output;
			expected_output.bid = 6;
			expected_output.tricks = 5;
			expected_output.bid_score = -60;
			expected_output.nil_score = 200;
			expected_output.hand_score = 200 - 60;
			expected_output.game_score = 200 - 60;

			iod.close();

			REQUIRE(expected_output == actual_output);
		}
		SECTION("Only non-nil won")
		{
			SECTION("With no bags from non-nil player")
			{
				IORedirect iod("y\nn\n6\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 3; ++i) { team.add_trick(0); }
				for (int i = 0; i < 6; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.bid = 6;
				expected_output.tricks = 9;
				expected_output.bags = 3;
				expected_output.bid_score = 63;
				expected_output.nil_score = -200;
				expected_output.hand_score = -200 + 63;
				expected_output.total_bags = 3;
				expected_output.game_score = -200 + 63;

				iod.close();

				REQUIRE(actual_output == expected_output);
			}
			SECTION("With bags from non-nil player")
			{
				IORedirect iod("y\nn\n6\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 3; ++i) { team.add_trick(0); }
				for (int i = 0; i < 8; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.bid = 6;
				expected_output.tricks = 11;
				expected_output.bags = 5;
				expected_output.bid_score = 65;
				expected_output.nil_score = -200;
				expected_output.hand_score = -200 + 65;
				expected_output.total_bags = 5;
				expected_output.game_score = -200 + 65;

				iod.close();

				REQUIRE(actual_output == expected_output);
			}
		}
	}

	SECTION("One player bids regular nil")
	{
		SECTION("Both Win")
		{
			SECTION("With no bags")
			{
				IORedirect iod("n\n0\nn\n6\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 6; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.bid = 6;
				expected_output.tricks = 6;
				expected_output.bid_score = 60;
				expected_output.nil_score = 100;
				expected_output.hand_score = 160;
				expected_output.game_score = 160;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
			SECTION("With bags")
			{
				IORedirect iod("n\n0\nn\n6\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 7; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.bid = 6;
				expected_output.tricks = 7;
				expected_output.bags = 1;
				expected_output.bid_score = 61;
				expected_output.nil_score = 100;
				expected_output.hand_score = 161;
				expected_output.total_bags = 1;
				expected_output.game_score = 161;

				iod.close();

				REQUIRE(expected_output == actual_output);
			}
		}
		SECTION("Both Lost")
		{
			IORedirect iod("n\n0\nn\n6\n");
			team.set_bid(0);
			team.set_bid(1);

			for (int i = 0; i < 3; ++i) { team.add_trick(0); }

			Score actual_output = team.get_score();

			Score expected_output;
			expected_output.bid = 6;
			expected_output.tricks = 3;
			expected_output.bid_score = -60;
			expected_output.nil_score = -100;
			expected_output.hand_score = -100 - 60;
			expected_output.game_score = -100 - 60;

			iod.close();

			REQUIRE(actual_output == expected_output);
		}
		SECTION("Only blind nil won")
		{
			IORedirect iod("n\n0\nn\n6\n");
			team.set_bid(0);
			team.set_bid(1);

			for (int i = 0; i < 5; ++i) { team.add_trick(1); }

			Score actual_output = team.get_score();

			Score expected_output;
			expected_output.bid = 6;
			expected_output.tricks = 5;
			expected_output.bid_score = -60;
			expected_output.nil_score = 100;
			expected_output.hand_score = 100 - 60;
			expected_output.game_score = 100 - 60;

			iod.close();

			REQUIRE(expected_output == actual_output);
		}
		SECTION("Only non-nil won")
		{
			SECTION("With no bags from non-nil player")
			{
				IORedirect iod("n\n0\nn\n6\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 3; ++i) { team.add_trick(0); }
				for (int i = 0; i < 6; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.bid = 6;
				expected_output.tricks = 9;
				expected_output.bags = 3;
				expected_output.bid_score = 63;
				expected_output.nil_score = -100;
				expected_output.hand_score = -100 + 63;
				expected_output.total_bags = 3;
				expected_output.game_score = -100 + 63;

				iod.close();

				REQUIRE(actual_output == expected_output);
			}
			SECTION("With bags from non-nil player")
			{
				IORedirect iod("n\n0\nn\n6\n");
				team.set_bid(0);
				team.set_bid(1);

				for (int i = 0; i < 3; ++i) { team.add_trick(0); }
				for (int i = 0; i < 8; ++i) { team.add_trick(1); }

				Score actual_output = team.get_score();

				Score expected_output;
				expected_output.bid = 6;
				expected_output.tricks = 11;
				expected_output.bags = 5;
				expected_output.bid_score = 65;
				expected_output.nil_score = -100;
				expected_output.hand_score = -100 + 65;
				expected_output.total_bags = 5;
				expected_output.game_score = -100 + 65;

				iod.close();

				REQUIRE(actual_output == expected_output);
			}
		}
	}

	SECTION("Testing get_score and reset together")
	{
		IORedirect iod("n\n0\nn\n6\nn\n1\nn\n1\nn\n1\nn\n1\n");
		team.set_bid(0);
		team.set_bid(1);

		for (int i = 0; i < 7; ++i) { team.add_trick(1); }

		Score actual_output = team.get_score();

		Score expected_output;
		expected_output.bid = 6;
		expected_output.tricks = 7;
		expected_output.bags = 1;
		expected_output.bid_score = 61;
		expected_output.nil_score = 100;
		expected_output.hand_score = 161;
		expected_output.total_bags = 1;
		expected_output.game_score = 161;

		iod.close();

		REQUIRE(expected_output == actual_output);

		team.reset();

		expected_output = Score();
		expected_output.total_bags = 1;
		expected_output.game_score = 161;
		actual_output = team.get_score();

		REQUIRE(expected_output == actual_output);

		iod.open();
		team.set_bid(0);
		team.set_bid(1);
		iod.close();

		for (int i = 0; i < 6; ++i) { team.add_trick(0); }
		for (int i = 0; i < 4; ++i) { team.add_trick(1); }

		actual_output = team.get_score();

		expected_output.bid = 2;
		expected_output.tricks = 10;
		expected_output.bags = 8;
		expected_output.bid_score = 28;
		expected_output.hand_score = 28;
		expected_output.total_bags = 9;
		expected_output.game_score = 189;

		REQUIRE(expected_output == actual_output);

		team.reset();
		expected_output = Score();
		expected_output.total_bags = 9;
		expected_output.game_score = 189;
		actual_output = team.get_score();

		REQUIRE(expected_output == actual_output);

		iod.open();
		team.set_bid(0);
		team.set_bid(1);
		iod.close();

		for (int i = 0; i < 3; ++i) { team.add_trick(0); }
		for (int i = 0; i < 4; ++i) { team.add_trick(1); }

		actual_output = team.get_score();

		expected_output.bid = 2;
		expected_output.tricks = 7;
		expected_output.bags = 5;
		expected_output.bid_score = 25;
		expected_output.hand_score = -75;
		expected_output.bag_penalty = -100;
		expected_output.total_bags = 4;
		expected_output.game_score = 189 + -75;

		REQUIRE(expected_output == actual_output);
	}
}

// Destructor is tested via valgrind