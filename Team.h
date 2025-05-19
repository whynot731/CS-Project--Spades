#pragma once
#include "Player.h"
#include <string>
#include <iostream>

struct Score
{
	int bid = 0;
	int tricks = 0;
	int bags = 0;
	int bid_score = 0;
	int nil_score = 0;
	int bag_penalty = 0;
	int hand_score = 0;
	int total_bags = 0;
	int game_score = 0;
};

std::ostream& operator<<(std::ostream& os, const Score& score);

class Team
{
public:
	Team(const PlayerComponents& player1, const PlayerComponents& player2);

	//Team(const Team& rhs);
	//Team(Team&& rhs) noexcept;

	// Wrapper functions on Player class
	// ---------------------------------------------------------------------------
	/* Call get_name on a player in the team;
	* @pre valid player value is either 0 or 1
	* @param player which player to call the function on
	*/
	std::string get_name(int player) const;

	/* Call set_bid on a player in the team;
	* @pre valid player value is either 0 or 1
	* @param player which player to call the function on
	*/
	void set_bid(int player);

	/* Call add_trick on a player in the team
	* @pre valid player value is either 0 or 1
	* @param player which player to call the function on
	*/
	bool add_trick(int player);

	/* Call add_card on a player in the team
	* @pre valid player value is either 0 or 1
	* @param player which player to call the function on
	* @param card Card to add to the hand of player
	*/
	bool add_card(int player, const Card& card);

	Card lead_card(int player, bool broken_spade);

	Card play_card(int player, Suit led_suit);

	/* Call reset on all the players of the team
	*/
	void reset();

	// Team functions
	// ---------------------------------------------------------------------------
	// Returns the score of the team
	Score get_score();

	~Team();
	friend std::ostream& operator<<(std::ostream& os, const Team& team);
private:
	std::vector<Player*> team;
	int total_bags;
	int game_score;

	// Returns true if the player value is 0 or 1
	inline bool valid_player(int player) const;

	// Calculate the nil score of the team
	// Modifies the tricks, bid, and possible bags 
	inline int calculate_nil_score(int& tricks, int& non_nil_tricks, int& bid, int& possible_bags);

	// Calculates and return the bid score of the team
	// Modifies the bag to correct value
	inline int calculate_bid_score(int non_nil_tricks, int bid, int& bags, int possible_bags);

	// Updates the total bags of the team
	// Modifies the bag_penalty to correct value
	// Returns the total bags
	inline int update_total_bags(int bags, int& bag_penalty);

	// Calculates and returns the hand score of the team
	inline int calculate_hand_score(int bid_score, int nil_score, int bag_penalty);

	// Updates the game score of the team
	// Returns the updated game score
	inline int update_game_score(int hand_score);
};

std::ostream& operator<<(std::ostream& os, const Team& team);