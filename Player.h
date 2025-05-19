#pragma once
#include "Card.h"
#include <string>
#include <vector>

struct PlayerComponents
{
	std::string name;
	std::string strategy;
};

class Player
{
public:
	/* Getter for player's name
	* @return player's name as string
	*/
	virtual const std::string& get_name() const = 0;

	/* Bid setter
	* @pre Valid bid (-1 <= bid < MAX_HAND_SIZE). -1 represents a blind nil bid
	* @post Sets the bid of the player
	*/
	virtual void set_bid() = 0;

	/* Bid getter
	* @return bid of the player
	*/
	virtual int get_bid() const = 0;
	
	/* Add a trick to the Player
	* @pre tricks < MAX_HAND_SIZE
	* @post increment the trick count by one
	* @return true if the adding the trick was successful
	*/
	virtual bool add_trick() = 0;

	/* Trick getter
	* @return the number of tricks the Player won
	*/
	virtual int get_trick() const = 0;

	/* Determines if the player bid nil
	* @return true if player's bid was zero and they didn't bid blind
	*/
	virtual bool nil_bid() const = 0;

	/* Determines if the player bid blind nil
	* @return true if the palyer's bid was zero and they bid blind
	*/
	virtual bool blind_nil_bid() const = 0;

	/* Adds Card to player's hand
	* @pre current_hand_size < MAX_HAND_SIZE
	* @post Adds the Card c to player's hand. Makes sure that the cards are
	*	sorted in ascending order after the addition
	* @param c Card to add to hand
	* @return true if the card was successfully added
	*/
	virtual bool add_card(const Card& c) = 0;

	/* Plays the first card of the trick
	* @pre Player has at least one card
	* @post Leads the trick with a card. Card cannot be Spades until there are
	*	no other cards that are not Spades. Played card is removed from hand.
	* @return Card that player played
	*/
	virtual Card lead_card(bool broken_spade) = 0;
	
	/* Plays a non-leading card during a trick
	* @pre Player has at least one card
	* @post Plays a card during the trick. Player can only play cards of the
	*	leading suit if they still have cards of that suit. Otherwise, player
	*	can play any other card. Played card is remove from hand.
	* @param led_card the card that led the trick
	* @return Card that player played
	*/
	virtual Card play_card(Suit led_suit) = 0;

	/* Reset the values of the Player to initial state
	*/
	virtual void reset() = 0;

	virtual ~Player() {};

	static const int MAX_HAND_SIZE = 13;

};

/* Create player with given name and strategy
* @post Need to dynamically create an player with the given name. The only
*	strategy we are implementing in this project is a Human Player, but
*	having a factory function allows us to create an new type of player in
*	the future like an Simple or Difficult Player that would have its
*	own logic on how to play. 
* @param name of the player
* @param strategy of the player, Only "Human" for this project
* @return pointer to the created player; nullptr if incorrect strategy
*/
Player* Player_factory(const std::string& name, const std::string& strategy);

/* Prints name of player to stream
*/
std::ostream& operator <<(std::ostream& os, const Player& p);
