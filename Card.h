#pragma once

#include <iostream>
#include <string>

// ---------------------------------Rank---------------------------------------
// Represents the rank of a playing card. Since this is an enum, the ranks are 
// represented with integers starting from zero to twelve. However, not only is
// it an enum but rather an enum class, so referring to the values of the rank
// requires you to use the scope resolution operator. Using enum classes makes 
// references to the values more strict.
//		Example: Rank r1 = Rank::FIVE;
//				 Rank r2 = Rank::JACK;
// Enums and enum classes allow you to compare the types values since they are
// just integers under the hood.
//		Example: r1 <= r2 // True
// Since we've overloaded the input and output operators below, we can use Rank
// objects with streams.
//		Example: cout << r1; // prints "Five"
//				 cin >> r2;	 // reads a string from cin and updates r2
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
enum class Rank
{
	TWO, THREE, FOUR, FIVE, SIX, SEVEN, 
	EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE
};

/* Converts a string representing a Rank to a Rank object
* @pre The str represents a valid Rank ("Two", "Three", ... , "Ace")
*	If the string is not valid,
	throw std::invalid_argument("Invalid value for string_to_rank function.");
* @param str The string representing the rank
* @return Rank object
*/
Rank string_to_rank(const std::string& str);

/* Prints the rank to the stream object
* @param os The ostream object to print to
* @param rank The rank to print 
* @return reference to ostream object passed into the function
*/
std::ostream& operator<<(std::ostream& os, Rank rank);

/* Updates the rank object to the rank from the stream
* @pre The read input must be a valid rank, (HINT: use string_to_rank)
* @param is The istream object where the data is coming from
* @param rank The rank to update
* @return reference to istream object passed into the function
*/
std::istream& operator>>(std::istream& is, Rank& rank);





// ---------------------------------Suit---------------------------------------
// Suit is analogous to Rank in that it represents the suit of the card.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
enum class Suit
{
	CLUBS, DIAMONDS, HEARTS, SPADES
};


/* Converts a string representing a Suit to a Suit object
* @pre The str represents Suit ("Spades", "Hearts", "Clubs", or "Diamonds")
*	If the string is not valid,
*	throw std::invalid_argument("Invalid value for string_to_suit function.");
* @param str The string representing the suit
* @return Suit object
*/
Suit string_to_suit(const std::string& str);


/* Prints the suit to the stream object
* @param os The ostream object to print to
* @param suit The suit to print
* @return reference to ostream object passed into the function
*/
std::ostream& operator<<(std::ostream& os, Suit suit);


/* Updates the suit object to the suit from the stream
* @pre The read input must be a valid suit, (HINT: use string_to_suit)
* @param is The istream object where the data is coming from
* @param suit The suit to update
* @return reference to istream object passed into the function
*/
std::istream& operator>>(std::istream& is, Suit& suit);

class Card
{
public:
	/* Default constructor
	* @post Initializes Card to Two of Clubs
	*/
	Card();

	/* Parameterized constructor
	* @post Initializes Card to the specified rank and suit
	* @param rank The rank for the card
	* @param suit The suit for the card
	*/
	Card(Rank rank, Suit suit);
		
	/* Getter for rank
	* @return Rank of the card
	*/
	Rank get_rank() const;

	/* Getter for suit
	* @return Suit of the card
	*/
	Suit get_suit() const;

	/* Determines if the Card is of the trump suit
	* @return True if card has a suit of Spades
	*/
	bool is_trump() const;

private:
	Rank rank;
	Suit suit;

	/* Friend input operator. Allows the implementation of the operator>>
	*  to access the private member variables of the Card class.
	*  Reminder: This is a friend function, so it is not a part of the class.
	*/
	friend std::istream& operator>>(std::istream& is, Card& card);
};

/* Prints the Card to the stream
* @post Prints to stream the card in this format: `[rank] of [suit]`. 
*	 Examples: "Two of Spades", "Queen of Hearts", "Ace of Diamonds"
* @param os The ostream object to print to
* @param card The card to print
* @return Reference to ostream object passed into the function
*/
std::ostream& operator<<(std::ostream& os, const Card& card);

/* Reads a Card from a stream into a Card object
* Since this function was declared a friend of the Card class, it has access
* to the Card's private members (card.rank and card.suit)
* @pre stream contains valid representation of Card
* @post Reads in a Card from the stream in the format of `[rank] of [suit]`
*	 and use those values to update the card object
* @param is The istream object where the data is coming from
* @param card The card to update
* @return reference to istream object passed into the function
*/
std::istream& operator>>(std::istream& is, Card& card);

/* Less than comparision of two cards
* @param lhs Card on left handside of operator
* @param rhs Card on right handside of operator
* @return true if lhs is lower value than rhs. Doesn't consider lead or trump
*/
bool operator<(const Card& lhs, const Card& rhs);

/* Less than or equal to comparision of two cards
* @param lhs Card on left handside of operator
* @param rhs Card on right handside of operator
* @return true if lhs is lower value than rhs or the same card as rhs.
*		   Does not consider lead or trump
*/
bool operator<=(const Card& lhs, const Card& rhs);


/* Greater than comparision of two cards
* @param lhs Card on left handside of operator
* @param rhs Card on right handside of operator
* @return true if lhs is higher value than rhs. Doesn't consider lead or trump
*/
bool operator>(const Card& lhs, const Card& rhs);


/* Greater than or equal to comparision of two cards
* @param lhs Card on left handside of operator
* @param rhs Card on right handside of operator
* @return true if lhs is higher value than rhs or the same card as rhs.
*   Doesn't consider lead or trump
*/
bool operator>=(const Card& lhs, const Card& rhs);

/* Equality comparision of two cards
* @param lhs Card on left handside of operator
* @param rhs Card on right handside of operator
* @return true if lhs is same card as rhs. Does not consider lead or trump
*/
bool operator==(const Card& lhs, const Card& rhs);

/* Inequality comparision of two cards
* @param lhs Card on left handside of operator
* @param rhs Card on right handside of operator
* @return true if lhs is not the same card as rhs. 
*   Does not consider lead or trump
*/
bool operator!=(const Card& lhs, const Card& rhs);

/* Determine if a is a lower value than b based on Spade rules
* @param a First card
* @param b Second card
* @param lead the lead suit during a trick
* @return true if a is a lower value than b based on Spade rules.
*   Must take into account the if the cards are Spades, a
*   leading suit, or neither
*/
bool Card_less(const Card& a, const Card& b, Suit lead);