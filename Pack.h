#pragma once
#include "Card.h"
#include <array>
#include <string>

class Pack
{
public:
	/* Default constructor
	* @post Initializes the Pack from lowest card to highest card. Arrange
	* cards of lowest suit from lowest to highest rank then move onto the
	* next suit. Order of suits and ranks are defined in the project specs.
	*/
	Pack();

	/* Parameterized constructor
	* @pre pack_input contains a valid representation of a Pack as defined
	*	in the project specs.
	* @post Initializes the Pack by reading from pack_input
	* @param pack_input istream to read pack from
	*/
	Pack(std::istream& pack_input);

	/* Removes the next card from the pack and returns it
	* @pre Requires that there are cards remaining in the pack
	*	if there are no more cards remaining in the pack, 
	*	throw range_error("No more cards in the pack to deal.");
	* @post Increment the next index
	* @return next card in the pack
	*/
	Card deal_one();

	/* Reset the new index to first card in the Pack
	* @post reset index to initial value
	*/
	void reset();

	/* In-place Faro shuffle on pack of cards
	*  @post Shuffles the cards using in-place faro shuffle fifteen times
	*	and resets the index 
	*	See: https://en.wikipedia.org/wiki/In_shuffle
	*/
	void shuffle();

	/* Determines if there are no more cards in the pack
	* @return true if the pack is empty
	*/
	bool empty() const;

private:
	static const int PACK_SIZE = 52;
	std::array<Card, PACK_SIZE> cards;
	int next; // Index of next card to be dealt
};

