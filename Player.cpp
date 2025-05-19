/*
* AUTHOR: Kimberly Cabrera
* CSCI 235 Summer 2023 Spades Project
* Player.cpp
*/

#include "Player.h"
#include <iostream>
#include <algorithm>
#include <limits>

class Human : public Player
{
public:
	Human(std::string name);

	const std::string& get_name() const override;

	//const std::string& get_strategy() const override;

	void set_bid() override;

	int get_bid() const override;

	bool add_trick() override;

	int get_trick() const override;

	bool nil_bid() const override;

	bool blind_nil_bid() const override;

	bool add_card(const Card& c) override;

	Card lead_card(bool broken_spade) override;

	Card play_card(Suit led_suit) override;

	void reset();

private:
	std::vector<Card> hand;
	std::string name;
	int trick;
	int bid;

	/* Gets all the playable cards when leading a trick
	* @post The playable cards when leading a trick are cards that are not
	*	Spades. If there are no cards that are not Spades, then the entire
	*	hand is playable.
	* @return vector of playable Cards when leading a trick
	*/
	std::vector<Card> get_playable_hand() const;

	/* Gets all the playable cards during a trick
	* @post The playable cards are those of the lead Suit. If there are no
	*	Cards of the lead suit the then entire hand is playable
	* @param lead Suit of the trick
	* @return vector of playable Cards during the trick
	*/
	std::vector<Card> get_playable_hand(Suit lead) const;

	/* Prints out the Player's playable_hand
	* @post The playable hand is defined as the parameter rather than the
	*	hand data member since the concept of a playable hand changes
	*	depending on the state of the game. The format for print a card in the
	*	hand should be `[[card_index + 1]] [rank] of [suit]\n`. So for example,
	*	"[1] Two of Clubs\n[2] Three of Diamonds\n[3] Ace of Spades\n". Notice
	*	that there is the `[]` surround the index in the example. It is needed.
	* @param playable_hand
	*/
	void print_hand(const std::vector<Card>& playable_hand) const;

	/* Asks the user to select the card they want to play
	* @param playable_hand The hand the user is able to use
	* @return the index of the card the user selected from the the playable_hand
	*/
	int ask_for_card_index(const std::vector<Card>& playable_hand) const;

	/* Remove the card from the hand
	* @pre
	* @post removes the card from the hand and makes sure that the vector is still sorted
	* @param card to remove from the hand
	*/
	void remove_card(Card card);
};

Human::Human(std::string name) : name(name), trick(0), bid(0)
{
	hand.reserve(MAX_HAND_SIZE);
}

const std::string& Human::get_name() const
{
	return name;
}

//const std::string& Human::get_strategy() const
//{
//	return "Human";
//}

void Human::set_bid()
{
	char is_blind_nil;
	std::cout << "Does Player " << name << " want to bid blind nil? [y/n]\n";
	while (!(std::cin >> is_blind_nil) ||
		(is_blind_nil != 'y' && is_blind_nil != 'n'))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please answer using only `y` for yes or `n` for no.\n";
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	if (is_blind_nil == 'y')
	{
		bid = -2;
		return;
	}

	print_hand(hand);
	std::cout << "How much would you like to bid (0 to 13 inclusive)?\n";
	int input_bid;
	while (!(std::cin >> input_bid) ||
		0 > input_bid || input_bid > MAX_HAND_SIZE)
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please answer with values from 0 to 13 inclusive.\n";
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	bid = input_bid == 0 ? -1 : input_bid;
}

int Human::get_bid() const
{
	return bid <= 0 ? 0 : bid;
}

bool Human::add_trick()
{
	bool is_add_successful = trick < MAX_HAND_SIZE;
	if (is_add_successful)
	{
		++trick;
	}
	return is_add_successful;
}

int Human::get_trick() const
{
	return trick;
}

bool Human::nil_bid() const
{
	return bid == -1;
	return bid == -1;
}

bool Human::blind_nil_bid() const
{
	return bid == -2;
}

bool Human::add_card(const Card& c)
{
	bool is_add_successful = hand.size() < MAX_HAND_SIZE;
	if (is_add_successful)
	{
		hand.push_back(c);

		for (int i = static_cast<int>(hand.size()) - 1; i > 0 && hand[i] < hand[i - 1]; --i)
		{
			std::swap(hand[i], hand[i - 1]);
		}
	}
	return is_add_successful;
}

Card Human::lead_card(bool broken_spade)
{
	if (hand.empty())
	{
		throw std::out_of_range("Player has no more cards to play with.");
	}

	std::vector<Card> playable_hand = broken_spade ? hand : get_playable_hand();

	print_hand(playable_hand);

	int card_index = ask_for_card_index(playable_hand);

	Card card = playable_hand[card_index];

	remove_card(card);

	return card;

}

Card Human::play_card(Suit led_suit)
{
	if (hand.empty())
	{
		throw std::out_of_range("Player has no more cards to play with");
	}

	std::vector<Card> playable_hand = get_playable_hand(led_suit);

	print_hand(playable_hand);

	int card_index = ask_for_card_index(playable_hand);

	Card card = playable_hand[card_index];

	remove_card(card);

	return card;
}

void Human::reset()
{
	hand.clear();
	bid = 0;
	trick = 0;
}

std::vector<Card> Human::get_playable_hand() const
{
	std::vector<Card> playable_hand;
	playable_hand.reserve(MAX_HAND_SIZE);

	for (size_t i = 0; i < hand.size() && hand[i].get_suit() != Suit::SPADES; ++i)
	{
		playable_hand.push_back(hand[i]);
	}

	if (playable_hand.empty())
	{
		return hand;
	}
	return playable_hand;
}

std::vector<Card> Human::get_playable_hand(Suit lead) const
{
	std::vector<Card> playable_hand;
	playable_hand.reserve(MAX_HAND_SIZE);

	for (size_t i = 0; i < hand.size(); ++i)
	{
		if (hand[i].get_suit() == lead)
		{
			playable_hand.push_back(hand[i]);
		}
	}

	if (playable_hand.empty())
	{
		return hand;
	}
	return playable_hand;
}

void Human::print_hand(const std::vector<Card>& playable_hand) const
{
	for (size_t i = 0; i < playable_hand.size(); ++i)
	{
		std::cout << '[' << i + 1 << ']' << ' ' << playable_hand[i] << '\n';
	}
}

int Human::ask_for_card_index(const std::vector<Card>& playable_hand) const
{
	size_t card_index = 0;

	std::cout << "Please enter a number between 1 and "
		<< playable_hand.size() << " inclusive:\n";

	while (!(std::cin >> card_index) ||
		card_index <= 0 || card_index > playable_hand.size())
	{
		std::cin.clear();
		std::cin.ignore(10000, '\n');

		std::cout << "Please enter a number between 1 and "
			<< playable_hand.size() << " inclusive:\n";
	}

	return card_index - 1;
}

void Human::remove_card(Card card)
{
	int card_index = -1;
	for (size_t i = 0; i < hand.size(); ++i)
	{
		if (hand[i] == card)
		{
			card_index = i;
			break;
		}
	}

	if (card_index == -1)
	{
		return;
	}

	for (size_t i = card_index; i < hand.size() - 1; ++i)
	{
		std::swap(hand[i], hand[i + 1]);
	}

	hand.pop_back();
}

Player* Player_factory(const std::string& name, const std::string& strategy)
{
	if (strategy == "Human")
	{
		return new Human(name);
	}
	else
	{
		return nullptr;
	}
}

std::ostream& operator<<(std::ostream& os, const Player& p)
{
	os << p.get_name();
	return os;
}