#include "Team.h"
#include <iostream>

//Should print out the score in the format below(include a new line at the end of the last line) :
//	```
//	Bid : <value>
//	Tricks : <value>
//	Bags : <value>
//	Bid Score : <value>
//	Nil Score : <value>
//	Bag Penalty : <value>
//	Hand Score : <value>
//	Total Bags : <value>
//	Game Score : <value>
//	```
std::ostream& operator<<(std::ostream& os, const Score& score)
{

	return os << "Bid: " << score.bid << '\n' << "Tricks: " << score.tricks << '\n' << "Bags: " << score.bags << '\n'
		<< "Bid Score: " << score.bid_score << '\n' << "Nil Score: " << score.nil_score << '\n' << "Bag Penalty: " << score.bag_penalty << '\n'
		<< "Hand Score: " << score.hand_score << '\n' << "Total Bags: " << score.total_bags << '\n' << "Game Score: " << score.game_score;
}

//Parameterized constructor of `Team`. Initialize the vector with the players using their components in the order mentioned.
//_Assumes that the strategy attribute in `PlayerComponents` is valid._
Team::Team(const PlayerComponents& player1, const PlayerComponents& player2)
{
	team.push_back(Player_factory(player1.name, player1.strategy));
	team.push_back(Player_factory(player2.name, player2.strategy));
	//now do i set values of each player in team to 0?
	//order of values: 1) bid, 2) tricks, 3) bag, 4) bid_score, 5) nil score, 6) bag penalty, 7) hand score, 8) total bags, 9) game score
	game_score = 0;
	total_bags = 0;
}

std::string Team::get_name(int player) const
{
	//if player doesn't exist, throw
	if (valid_player(player) == false) {
		throw std::out_of_range("Not a valid player.");
	}
	return team[player]->get_name();
}

void Team::set_bid(int player)
{
	if (valid_player(player) == false) {
		throw std::out_of_range("Not a valid player.");
	}
	team[player]->set_bid();
}

bool Team::add_trick(int player)
{
	if (valid_player(player) == false) {//test 6
		throw std::out_of_range("Not a valid player.");
	}
	return team[player]->add_trick();
}

bool Team::add_card(int player, const Card& card)
{
	if (valid_player(player) == false) {
		throw std::out_of_range("Not a valid player.");
	}
	return team[player]->add_card(card);
}

Card Team::lead_card(int player, bool broken_spade) //test 4
{//Calls the `lead_card` function on the specified player using the provided `broken_spade` parameter.
	//Must check to see if the player is valid before calling the function.If not valid then 
	//`throw std::out_of_range("Not a valid player.");`.
	if (valid_player(player) == false) {
		throw std::out_of_range("Not a valid player.");
	}
	return team[player]->lead_card(broken_spade);
	//return Card();
}

Card Team::play_card(int player, Suit led_suit) //test 5
//Calls the `play_card` function on the specified player using the provided `led_suit` parameter.
//Must check to see if the player is valid before calling the function.If not valid then `throw std::out_of_range("Not a valid player."); `.
{
	if (valid_player(player) == false) {
		throw std::out_of_range("Not a valid player.");
	}
	return team[player]->play_card(led_suit);
}

void Team::reset()
{
	//prob not right, have a lot more values to reset than Player
	team[0]->reset();
	team[1]->reset();
}

////Calculates the score of the team and returns the `Score` object. Must calculate the values in this order:
//1. Nil Score
//2. Bid Score
//3. Total Bags
//4. Hand Score
//5. Game Score
//
//By implementing the private helper functions, this function would just be calling these helper functions rather than
//implementing everything in one big function.
//
//One thing to note is that one of these functions need a `possible_bag` variable to be passed in.
//This variable should be created inside the `get_score` function and be set to zero.Another variable that needs to be created is the `non_nil_tricks`;
//it should be set to zero intitally.
Score Team::get_score()
{
	int non_nil_tricks = 0;
	int possible_bag = 0;
	struct Score values;
	values.nil_score = calculate_nil_score(values.tricks, non_nil_tricks, values.bid, possible_bag);
	values.bid_score = calculate_bid_score(non_nil_tricks, values.bid, values.bags, possible_bag);
	values.total_bags = update_total_bags(values.bags, values.bag_penalty);
	values.hand_score = calculate_hand_score(values.bid_score, values.nil_score, values.bag_penalty);
	values.game_score = update_game_score(values.hand_score);
	return values;
}



Team::~Team()
{
	delete team[0];
	team[0] = nullptr;
	delete team[1];
	team[1] = nullptr;
}

std::ostream& operator<<(std::ostream& os, const Team& team) //test 3
{//Should print out the team in the format: `Team <player_one_name>/<player_two_name>` \(there is no newline at the end\).
	return os << "Team " << team.get_name(0) << '/' << team.get_name(1);
}

//private functions starts Line 124 README

inline bool Team::valid_player(int player) const
{
	//Return true if player value is $0$ or $1$.
	if (player == 0 || player == 1) {
		return true;
	}
	return false;
}

//- Changed the function prototype so that takes an extra parameter called `non_nil_tricks`
//- This parameter is the total number of tricks from players who didn't bid nil. 
//- This parameter is pass by reference because it is needed to correctly calculate the `bid_score` 
//			- non_nil_tricks += tricks
//- For every player in the team, regardless of what they bid, add the total tricks and bid to the `tricks` and `bid` parameters.
//		trick = player1 & player2 get_trick			bid = player1 & player2 get_bid
//- If the player bid nil, then calculate their nil_score based on the type of nil they bid and add it to the `nil_score` parameter
//- The absolute value of a blind nil score is 200.
//				games played to 500, win 200 if 0 tricks won, lose 200 if 1 or greater tricks won, (by player)
//- The absolute value of a regular nil score is 100.
//				same as above but for regular nil
//- The calculation of the player's nil_score is the same regardless of the type of nil. 
// The only difference is the absolute values based on the type of nil bid they have.
//- If the player who bid any type of nil, didn't recieve any tricks then their nil score is positive
//			if nil == true && tricks == 0  +
//- Otherwise, their nil score is negative and the number of tricks they recieved is added to the `possible_bags` parameter.
//			else -
//- If the player didn't bid nil, then add their number of tricks to the `non_nil_tricks` parameter 
//			if nil == false,		non_nil_tricks += tricks
inline int Team::calculate_nil_score(int& tricks, int& non_nil_tricks, int& bid, int& possible_bags)
{
	int nil_bid_score = 0;
	bid += team[0]->get_bid() + team[1]->get_bid();	//add both player bid to 'bid' parameters
	tricks += team[0]->get_trick() + team[1]->get_trick(); //add both player tricks to 'tricks' parameter
	for (int pNum = 0; pNum < 2; pNum++) {
		if (team[pNum]->nil_bid() == true && team[pNum]->get_trick() == 0) {//player nil bid, 0 tricks
			nil_bid_score += 100;
		}
		else if (team[pNum]->nil_bid() == true && team[pNum]->get_trick() >= 1) {//player nil bid, 1+ tricks
			nil_bid_score -= 100;
			possible_bags += team[pNum]->get_trick();
		}
		if (team[pNum]->blind_nil_bid() == true && team[pNum]->get_trick() == 0) {//player blind nil bid, 0 tricks
			nil_bid_score += 200;
		}
		else if (team[pNum]->blind_nil_bid() == true && team[pNum]->get_trick() >= 1) {//player blind nil bid, 1+ tricks
			nil_bid_score -= 200;
			possible_bags += team[pNum]->get_trick();
		}
		if (team[pNum]->nil_bid() == 0 && team[pNum]->blind_nil_bid() == 0) { //player no nil bids
			non_nil_tricks += team[pNum]->get_trick();
		}
	}
	return nil_bid_score;
}

//- Changed the function prototype so that `non_nil_tricks` is the parameter rather than `tricks`
//- If both players bid any type of nil, then just return 0
//- If the number of `non_nil_tricks` is less than the `bid`, then there are no `bags` and `bid_score` is the negation of ten times `bid`
//- Else, the number of `bags` is the addition of the `possible_bags`, `non_nil_tricks`, and negation of `bid`; the `bid_score` is `bid` times ten plus `bags`
inline int Team::calculate_bid_score(int non_nil_tricks, int bid, int& bags, int possible_bags)
{
	int bid_score = 0;
	if ((team[0]->nil_bid() == true || team[0]->blind_nil_bid() == true) &&
		(team[1]->nil_bid() == true || team[1]->blind_nil_bid() == true)) //if both players did any nil bid -- or asking for if any player bid nil?
	{
		return 0;
	}
	if (non_nil_tricks < bid) {
		bid_score = -bid * 10;
	}
	else {
		bags = possible_bags + non_nil_tricks - bid;
		bid_score = 10 * bid + bags;
	}
	return bid_score;

}

inline int Team::update_total_bags(int bags, int& bag_penalty)
{
	//Upate the `total_bags` attribute of the `Team` class and return it.
	//The `total_bags` is adding the `bags` onto the `total_bags` and then checking to see if
	//the `total_bags` value is greater than or equal to 10. If so then update the `bag_penalty` to be $-100$
	//and the `total_bags` to be modulo 10. Otherwise, the `bag_penalty` is just zero.
	total_bags += bags;
	if (total_bags >= 10) {//if 10+ bags
		bag_penalty = -100;
		total_bags %= 10; //modulo = 10
	}
	else {
		bag_penalty = 0;
	}
	return total_bags;
}

inline int Team::calculate_hand_score(int bid_score, int nil_score, int bag_penalty)
{
	//Calculate the `hand_score` and return it. The `hand_score` is just the summation of the `bid_score`, `nil_score`, and `bag_penalty`.
	return bid_score + nil_score + bag_penalty;
}

inline int Team::update_game_score(int hand_score)
{
	return game_score += hand_score;
}