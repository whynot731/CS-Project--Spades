# CHANGES
- Not including Game as a part of Project 3 because of testing issues. I am not removing the Game.h file nor the explanation to `Game`. 
- Modified some functions and explanations for Team. Please read through these sections again to find the updated explanations.
    - `calculate_nil_score`
    - `calculate_bid_score`
    - `get_score`
- Fixed the Makefile which had an error

## Note
The test file for `Team` does not test if you properly deleted the Players in the destructor.
Instead, you should use valgrind, which is a program on linux that analyizes the performance of a program,
including whether any dynamically allocated memory was properly freed.
If valgrind is installed on your linux distro, then you can use the make command `make test_team_valgrind`.

# Project 3 Introduction
[We're almost done!](https://www.youtube.com/watch?v=9jK-NcRmVcw) 
In Project 1, you worked on the `Card` and `Pack` classes.
In Project 2, you worked on the `Player` class, specifically the `Human` class that inherits from the `Player`.
Now, in Project 3, you must put it all together through the `Team` and `Game` class.
In reality, just having these two classes is not enough since you would need to have one more file that has the `main` function
that would instantiate the Game object and run it, but I think stopping the project here is good enough. 

# Notes
## Updates
I've updated the `Player.h` file to include a [POD](https://en.wikipedia.org/wiki/Passive_data_structure) called `PlayerComponents`.
This struct contains the two attributes to create a `Player`: `name` and `strategy`.
The reason why I added this is because it makes the contructors for `Team` and `Game` shorter compared to passing the attributes separately.
Ideally, I would have modified the player factory to take in this `PlayerComponents`,
but that would make the code not backward compatible so we all must live with this poor design choice I made a few weeks ago
and keep it as it is.

# Team class
## Overview
This class contains all the players of the team.
Any operations on the players in the `Game` class would happen through the `Team` class instead.
This means we must create a few [wrapper functions](https://en.wikipedia.org/wiki/Wrapper_function)
that would allow us to use the `Player` functions via the `Team`.

The players in the `Team` class are stored in a vector.
This plus the fact that players are referenced via values of $0$ or $1$ means that
it's easy to access a particular player in the `Team` class.

The other attributes within the `Team` are `total_bags` and `game_score`.
These variables represent score values that persist beyond one game play.
The other score values mentioned below are values that only exist with only one game play.

### Score
Also in the `Team.h` file is the `Score` POD. This is used to show the various scores of the team at the end of the hand game play. 

#### `int bid`
The total number of bids the team made.

#### `int tricks`
The total number of tricks the team earned.

#### `int bags`
The number of extra tricks earned.

#### `int bid_score`
The score when considering the tricks, bids and bags.

#### `int nil_score`
The score when considering any type of nil bid.

#### `int bag_penalty`
The penalty when the amount of bags reach or exceed 10.

#### `int hand_score`
The score after consider the bid score, nil score, and bag penalty.

#### `int total_bags`
The total amount of bags earned throughout the game.

#### `int game_score`
The team's overall score throughout the game.

## Public Functions
### `Team(const PlayerComponents& player0, const PlayerComponents& player1);`
Parameterized constructor of `Team`. Initialize the vector with the players using their components in the order mentioned.
_Assumes that the strategy attribute in `PlayerComponents` is valid._ 

### `void get_name(int player);`
Calls the `get_name` function on the specified player. Must check to see if the player is valid before calling the function.
If not valid then `throw std::out_of_range("Not a valid player.");`.

### `void set_bid(int player);`
Calls the `set_bid` function on the specified player. Must check to see if the player is valid before calling the function.
If not valid then `throw std::out_of_range("Not a valid player.");`.

### `bool add_trick(int player);`
Calls the `add_trick` function on the specified player. Must check to see if the player is valid before calling the function.
If not valid then `throw std::out_of_range("Not a valid player.");`.

### `bool add_card(int player, const Card& card);`
Calls the `add_card` function on the specified player using the provided `card` parameter.
Must check to see if the player is valid before calling the function. If not valid then `throw std::out_of_range("Not a valid player.");`.

### `Card lead_card(int player, bool broken_spade);`
Calls the `lead_card` function on the specified player using the provided `broken_spade` parameter.
Must check to see if the player is valid before calling the function. If not valid then `throw std::out_of_range("Not a valid player.");`.

### `Card play_card(int player, Suit led_suit);`
Calls the `play_card` function on the specified player using the provided `led_suit` parameter.
Must check to see if the player is valid before calling the function. If not valid then `throw std::out_of_range("Not a valid player.");`.

### `void reset();`
Call `reset` on all the players in the team.

### `Score get_score();`
Calculates the score of the team and returns the `Score` object. Must calculate the values in this order:
1. Nil Score
2. Bid Score
3. Total Bags
4. Hand Score
5. Game Score

By implementing the private helper functions, this function would just be calling these helper functions
rather than implementing everything in one big function.

One thing to note is that one of these functions need a `possible_bag` variable to be passed in.
This variable should be created inside the `get_score` function and be set to zero.
Another variable that needs to be created is the `non_nil_tricks`; it should be set to zero intitally.

### `~Team();`
Destructor for `Team`. Should deallocate the players in the vector.

## Private Functions
Because these functions are private, I will not be testing them directly.
However, I still have these functions here because they are helpful with either calculating the scores of the team or
figuring out if a player value is valid. 

Some of these functions have pass by reference for their parameters.
This is because these functions need to modify more than one value, which can be done with pass by reference.

### `inline bool valid_player(int player);`
Return true if player value is $0$ or $1$.

### `inline int calculate_nil_score(int& tricks, int& non_nil_tricks, int& bid, int& possible_bags);`
- Changed the function prototype so that takes an extra parameter called `non_nil_tricks`
	- This parameter is the total number of tricks from players who didn't bid nil.
	- This parameter is pass by reference because it is needed to correctly calculate the `bid_score`
- For every player in the team, regardless of what they bid, add the total tricks and bid to the `tricks` and `bid` parameters.
- If the player bid nil, then calculate their nil_score based on the type of nil they bid and add it to the `nil_score` parameter
	- The absolute value of a blind nil score is 200.
	- The absolute value of a regular nil score is 100.
	- The calculation of the player's nil_score is the same regardless of the type of nil. The only difference is the absolute values based on the type of nil bid they have.
	- If the player who bid any type of nil, didn't recieve any tricks then their nil score is positive
	- Otherwise, their nil score is negative and the number of tricks they recieved is added to the `possible_bags` parameter.
- If the player didn't bid nil, then add their number of tricks to the `non_nil_tricks` parameter 

### `inline int calculate_bid_score(int non_nil_tricks, int bid, int& bags, int possible_bags);`
- Changed the function prototype so that `non_nil_tricks` is the parameter rather than `tricks`
- If both players bid any type of nil, then just return 0
- If the number of `non_nil_tricks` is less than the `bid`, then there are no `bags` and `bid_score` is the negation of ten times `bid`
- Else, the number of `bags` is the addition of the `possible_bags`, `non_nil_tricks`, and negation of `bid`; the `bid_score` is `bid` times ten plus `bags`

### `inline int update_total_bags(int bags, int& bag_penalty);`
Upate the `total_bags` attribute of the `Team` class and return it. The `total_bags` is adding the `bags` onto the `total_bags` and then checking to see if the `total_bags` value is greater than or equal to 10. If so then update the `bag_penalty` to be $-100$ and the `total_bags` to be modulo 10. Otherwise, the `bag_penalty` is just zero.

### `inline int calculate_hand_score(int bid_score, int nil_score, int bag_penalty);`
Calculate the `hand_score` and return it. The `hand_score` is just the summation of the `bid_score`, `nil_score`, and `bag_penalty`.

### `inline int update_game_score(int hand_score);`
Update the `game_score` attribute of the `Team` class and return it. The `game_score` value is adding the `hand_score` onto the `game_score`.

## Other Functions
### `std::ostream& operator<<(std::ostream& os, const Score& score);`
Should print out the score in the format below (include a new line at the end of the last line):
```
Bid: <value>
Tricks: <value>
Bags: <value>
Bid Score: <value>
Nil Score: <value>
Bag Penalty: <value>
Hand Score: <value>
Total Bags: <value>
Game Score: <value>
```

### `std::ostream& operator<<(std::ostream& os, const Team& team);`
Should print out the team in the format: `Team <player_one_name>/<player_two_name>` \(there is no newline at the end\).

# Game class
## Overview
This class combines all of the classes we've worked on to create the logic of the game.
There is not much too it since we've already implemented the logic in other classes.
All we need to do now is to use the functions we've created.

## Public Functions
### `Game(PlayerComponents player0, PlayerComponents player1, PlayerComponents player2, PlayerComponents player3, Pack pack);`
Parameterized constructor for `Game`.
Players zero and two are on team zero and players one and three are on team one.
Initalizes the pack attribute with its corresponding parameter and initializes the dealer to $-1$. 

The reason for initializing the dealer to $-1$ is because of `deal` function;
when we first call the `deal` function, we want the dealer to become `0`. Setting the dealer to $-1$ will change allow this.

### `void run();`
Runs the entire game.
While no one has won yet, deal the cards to the players, have the players bid, play all thirteen hands, and then score the two teams.
If the private functions are implemented correctly, all that needs to be done in this function are to call the private helper functions.

## Private Functions
These functions below implement all of the game play.

### `void deal();`
Update the dealer attribute with the player to the left of the previous dealer.
Shuffle the pack.
The first player to be dealt a card is the player to the left of the dealer.
For every card in the pack, deal it to the players in a clockwise rotation. 

### `void bid();`
Everyone makes their bid starting from the left of the dealer going clockwise.

### `void play();`
Starts the game play with the player to the left of the dealer and `broken_spade = false`.
In one play session, there are thirteen tricks that must be played.
At the beginning of every trick, have this line of code `std::cout << "Playing trick number " << <trick_count> << ":\n";`;
this trick count starts at 1 and should increment at every trick.
During one trick, the previous winner \(or the player to the left of the dealer if this is the first trick\), must lead the trick.
If the player leads with a Spades, then `broken_spade = true`.
The other three players must play their cards and follow the lead suit if they can.
If any of these players play a Spades, then `broken_spade = true`.
Whoever wins the trick using the `Card_less` rules gets to have a trick added to their count.
Make sure that you have this line of code after you figure out who is the winner
`std::cout << "Player " << <winner_name> << " wins the trick.\n";`

### `bool score();`
Get the scores of both teams.
Print out `std::cout << <Team> << " Score:\n" << <Team_score>;` for team zero and team one in that order.
If a team won then print out `std::cout << <Team> << " won the game!\n"` for that winning team and return true.
A team wins if that team has reached the winning-points minimum or the other team had reach the losing-points minimum.
If not one has won yet, then reset both teams and return false.

### `inline int next_player(int player) const;`
Returns the value of the next player.
This value represents the player to the left of the current player, aka going in a clockwise rotation.
Valid values of the players are $0, 1, 2, 3$. _HINT: use modulo._

### `inline int Game::get_team(int player) const`
Returns the value of the player's team. _HINT: use modulo._

### `inline int Game::get_pos_in_team(int player) const`
Returns the value of the player's position on their team. This is calculated by dividing the player's value by 2. _HINT: use division._

# Submission
Make sure you include the `.cpp` files from the previous project, along with the `Team.cpp` and `Game.cpp` files.
As always, **DO NOT MODIFY** the header files. Please submit the files to Gradescope via your GitHub repos. 

----

# Project 2 Introduction
In Project 2, you are working on the `Player` class that would represent a player in the game of Spades. Notice in `Player.h` that the `Player` class is an abstract class since it has at least one pure virtual function (in reality, all of the functions in the class are pure virtual). This is because a `Player` should not represent an actual instantiable player but act as an abstract concept of one. It is through inheritance that we will create an instantiable player class.

For Project 2, I would like you to create a `Human` class that inherits from `Player` to allow a person to play the game on the terminal. This Human class should be declared and defined inside the Player.cpp file. 

You might notice that when you are done with creating this Human class in the .cpp file, that when you go to test it, you are not able to work with the Human class directly. This is because the interface file (aka the header file), doesn't include Human but only Player. We can only work with code that was in the interface file. 

So how does one create a `Human` object without directly creating a `Human` object? Well, notice at the bottom of the header file, there is a function called `Player_factory`. This function acts as the interface to any `Player` type defined in the `Player.cpp` file. To use this function, all you need to do is give the name of the `Player`, and mention the type of `Player` they will be. For our case, the only `Player` we will be creating in this project is the `Human` player, but just imagine if we were to work on this project more and had created other types of players like a `SimpleAI` player or a `DifficultAI` player. This factory function would allow us to choose which type of player we want arbitrarily, rather than hard coding it. 

# Human Player
## Overview
This player would represent a person playing the game. The implementation of this class is left up to you as I don't care how this class is implemented but just that it works the way I expect it to work. I do have some tips below in case you need some guidance on where to start. 

Ideally the Human class should have at least four private member variables:
- Container that stores the players card, aka the hand (*suggestion*: `vector`)
- `string` to store the name of the player
- `int` for the number of tricks the player took during the hand
- `int` for the bid that the player placed before playing the hand

## Functions
### `Human(std::string name);` 
The parameterize constructor for Human. Should set the member variable for the name of the player, along with starting out with an empty hand (player has no cards). The number of tricks and the bid should be set to zero.

### `const std::string& get_name() const;`
Returns the name of the player.

### `void set_bid();`
Sets the bid for the player. Please follow these steps below:
- Ask the player `"Does Player " + player.name + " want to bid blind nil? [y/n]\n"`
	- User input is `y` 
		- Set their bid to be zero and mark it as blind nil. *DO NOT PRINT OUT THEIR HAND*
	- User input is `n` 
	  	- Print out user's complete hand in ascending order (details below) 
		- Print out `"How much would you like to bid (0 to 13 inclusive)?\n"`
		- If the user gives a value from 0 to 13 inclusive, then set the bid to that
		- Otherwise, print `"How much would you like to bid (0 to 13 inclusive)?\n"` and repeat until the previous step is fulfilled. 
	- User input is something else
		- Print out ``"Please answer using only `y` for yes or `n` for no.\n"``
		- Repeat until the user gives `y` or `n`

#### Example:
```cpp
Player* player = Player_factory("Bob", "Human");
// cards were added to player's hand
player->set_bid();
```
Terminal below after calling set_bid function on player:
```
>> Does Player Bob want to bid blind nil? [y/n]
<< g
>> Please answer using only `y` for yes or `n` for no.
<< Y
>> Please answer using only `y` for yes or `n` for no.
<< n
>> [1] Two of Clubs
>> [2] Nine of Clubs
>> [3] Queen of Clubs
>> [4] King of Clubs
>> [5] Ace of Clubs
>> [6] Eight of Diamonds
>> [7] Ten of Diamonds
>> [8] Nine of Hearts
>> [9] Ten of Hearts
>> [10] Two of Spades
>> [11] Five of Spades
>> [12] Nine of Spades
>> [13] Ace of Spades
>> How much would you like to bid (0 to 13 inclusive)?
<< -1
>> Please answer with values from 0 to 13 inclusive.
<< 14
>> Please answer with values from 0 to 13 inclusive.
<< 13
```

### `int get_bid() const;`
Returns the bid of the player during the hand. Remember that a nil bid and blind nil bid should both be a bid of zero. 

### `bool add_trick();`
Increments the player's trick count by one. Only does this if the player's current trick count is less than the max hand size. If the addition is successful, return true.

### `int get_trick() const;`
Returns the number of tricks the player won during the hand.

### `bool nil_bid() const;`
Returns true if the player bid nil. Remember that a nil bid is different from a blind nil bid. Therefore, if a player bid blind nil, then this function should return false.

### `bool blind_nil_bid() const;`
Returns true is the player bid blind nil. 

### `bool add_card(const Card& c);`
Adds a card to the player's hand. Only does this if the size of the hand is less than the max hand size. Return true if the addition was successful. 

If you are using a `vector` to store the cards, then you might want to add the cards in such a way so that it the hand remains sorted (think insertion sort). This would make printing out the sorted hand so much easier since it would then just be a simple traversal.

### `Card lead_card(bool broken_spade)`
Play the leading card of the trick. If `broken_spade` is `false` and there exist at least one card in the player's hand that is not a Spade, then the playable cards are any card that is not Spades. Else, all cards in the hand are considered playable.

Follow the steps below:
1. If the hand is empty, throw `std::out_of_range("Player has no more cards to play with.")`
2. Figure out the playable cards based on the rules above
3. Print out the playable cards (details below)
4. Ask the user for the card using this question: `"Please enter a number between 1 and " << playable_hand.size() << " inclusive:\n"`
	a. Repeat until the user gives a correct value
5. Select the card, remove it from the player's hand, and return the card

### `Card play_card(Suit led_suit);`
Play a non-leading card during the trick. This function is very similar to the `lead_card` function but the only difference is what is considered playable. Whereas the `lead_card` function means that the player is leading the trick, the `play_card` function means that someone already led the trick and that the player must follow suit if they have cards of that lead suit. If the player doesn't have any cards of the leading suit, then they can play any card.

Therefore the playable hand for the `play_card` function should only be lead suit cards if the player's hand contains any; otherwise the playable hand is the player's hand itself. 

Follow the steps mentioned in `lead_card`, while keeping in mind of the playable hand for `play_card`.

### `void reset();`
Resets the player to initial values. This means bid and tricks are zero and there are no cards in the player's hand.

## Tips
- Implement some private helper functions that would help you implement the public facing functions: 
	- `print_hand` function that takes in a playable hand as a parameter and prints the cards in the hand as such `[[card_index + 1]] [rank] of [suit]\n` in ascending sorted order. So for example, `"[1] Two of Clubs\n[2] Three of Diamonds\n[3] Ace of Spades\n"`. Notice that there is the `[]` surround the index in the example. It is needed.
	- `remove_card` function that would remove the Card from the player's hand in such a way that the hand would still remain in sorted order.
- While the type of container that would store the player's hand is up to you, I suggest using a vector for it's simplicity. 

# Notes
## Improved Compilation Times
I've improved the compilation times when you compile the test files via the Makefile. If you take a look at the Makefile, you'll notice that I am compiling every file to an object file then combining the object files into the executable. Compiling this way improves the compilation time as we only need to re-execute the compilation of the files that were changed rather than compiling everything at once every time the command is ran.

## Updated `test_card.cpp` 
This repo has the updated version of the test_card.cpp file. 

## Testing Incrementally Using the Test Files
I have not mentioned this before, but if you would like to test a function from your code instead of the entire code itself, you can do that with the test files. In the test files, notice how there are two parameters for every `TEST_CASE`; the first parameter is the name of the test case, and the second parameter is the tag. Using this tag, we can select which test we would like to run during the execution of the test file.

To do this, instead of using the make command `make test_file_name`, use `make test_[file_name].exe`.
This other command only makes the executable rather than both making the executable and executing it.
Once the executable is created, you can pass in the tag of test case you would like to run as such `./test_file.exe [tag]`.
The `[]` surrounding the tag name is necessary when running this command.
For example, I would run `make test_player.exe` on the terminal, then run `./test_player.exe [one]`
to run only the first test case. *KEEP IN MIND* that if you make any new changes to any files,
that you need to run the command that recompiled the code.

----

# Project 1 Introduction
The projects in this class will have you work your way up to create a game of Spades. Spades is a simple trick-taking card game. A trick-taking card game is one where whoever plays the highest card, however it is defined, during the "trick" wins the "trick." With Spades, there are four players who play in teams of two. The objective of the game is to have your team win 300 points or have the opposing team lose with -200 points. Points earned or lost are determined by how many tricks the team wins in comparison to how many tricks they predicted they would take during the game session (aka, their bid). 

If reading the rules don't help you to understand the game, then [play a few games](https://spades-game.online/) to get a feel for it. Our rules are mostly the same as the rules on this website.

# Rules
## Players
- Four players
- Fixed partnerships of two. Partners sit opposite to each other. Deal and play is clockwise.
	- *Example:* A list of players is `[P0, P1, P2, P3]`. Game play is clockwise so it always moves to the right in the array, which in the perspective of a player, it is to their left. `P0` and `P2` are on `Team0` and `P1` and `P3` are on `Team1`.

## Cards
- 52 cards
- Spades is trump, meaning it has a higher value than a card of another suit regardless of rank.
- Rank from highest to lowest: Ace, King, Queen, Jack, 10, 9 ,8 , 7, 6, 5, 4, 3, 2
- Suit from highest to lowest: Spades, Hearts, Diamonds, Clubs

## Deal
- First dealer chosen at random and turn to deal is clockwise.
- Cards are shuffled.
- Cards dealt singly in clockwise order from starting from player to the left of the dealer.
	- *Example:* If I have the Cards in a Pack like this `[A, B, C, D, E, F, G, H]` and a list of players `[P0, P1, P2, P3]` with `P0` as the dealer, then `P1` cards are `A, E`, `P2` cards are `[B, F]`, `P3` cards are `[C, G]` and `P0` cards are `[D, H]`.
- Each player gets 13 cards at the end of the deal.

## Bidding
- All four players bid the number of tricks they think they'll win during the game's session (the hand).
- Bidding starts with player to the left of the dealer and continues clockwise.
- Everyone must bid a number from 0 to 13 after looking at their cards. 
	- A bid of zero is known as a *nil*. The team can get special points if it succeeds and a penalty if it fails.
	- A *blind nil* is when the player declares a *nil* bid before looking at their cards. Again, there are special points if successful and penalty if it fails.
- The bid for the team is derived from the adding together the two members' bids.
	- *Example*: North bids 2 and Souths bids 3, therefore their team has a bid of 5.
- Once the bidding stage is done, no player can change their bid.

## Play
- Player to the left of the dealer starts the first trick.
- The leading player cannot start with a Spade card unless either the player has no more non-Spade cards, or someone broke spades. To break spades means to be the first player to play a Spade card during the hand.
- Game play is clockwise.
- Each player must follow suit if able. If not, then the player can play any card.
- The winner of the trick leads the next trick.

### Winning a Trick
- A trick containing a spade is won by the highest spade played.
- If no spade is played, then the trick is won by the highest card leading suit, meaning the highest card that played the same suit as the leading card.

## Scoring
- When a team takes at least or more tricks than they bid, then they receive a score equal to 10 times the bid. Any additional tricks are called *bags* and are worth an extra one point each.
	- *Example*: Team One bid 5 tricks and won 5 tricks, therefore their score is `current_score + 50`. 
	- *Example*: Team Two bid 6 tricks and won 8 tricks so their score is `current_score + 62`.
- If a team does not fulfill their bid, then they lose 10 points times the number of tricks they bid.
	- *Example*: Team One bid 8 tricks but only won 6 tricks, therefore their score is `current_score - 80`.
- *Sandbagging rule*: Overtricks are known as *bags*. If a team accumulates ten or more bags, then 100 will be deducted from their score. Any bags beyond ten will be carried over to the next cycle of ten overtricks.
	- *Example*: Team One bids 3 tricks and wins 6 tricks. If the number of bags they've accumulated up until that point was 9 bags, then after the trick, they'll have 12 bags, which means they have to add (33 - 100) to their score. Since they have more then ten bags (12 bags) at that point, the extra two bags carry over to the next game, meaning they start off with two bags. 
- If a bid of *nil* is successful by a player, then the team receives 100 points in addition to the points won or lost by the partner of the nil bidder. 
	- *Example*: Player One of Team One successfully bid nil. Player Two of Team One bid three tricks and won four. Therefore, Team One's total score is `current_score + 100 + 31`.
	- *Example*: Player One of Team One successfully bid nil. Player Two of Team One bid five tricks but lost with four tricks. Therefore, Team One's total score is `current_score + 100 - 50`.
- If a bid of *nil* is not successful, then the team loses 100 points and the tricks earned by the nil bidder counts as bags for the team.
	- *Example*: Player One of Team One lost their *nil* bid by two tricks. Player Two bid three tricks and won four. Therefore, Team One's total score is `current_score - 100 + 33`.
	- *Example*: Player One of Team One lost their nil bid by two tricks. Player Two bid five tricks but lost by four. Therefore, Team One's total score is `current_score - 100 - 50`.
- *Blind nil* bids are worth double that of a regular *nil* bid meaning +/- 200 points.
- Once the scoring is finished, if a team ends up with at least 300 points or higher, they win the game. If a team ends up with at least -200 points or lower, they lose the game. If neither team meet these criteria, then we deal cards to the players again and start again, keeping track of the score from previous sessions.

# Card class
A `Card` object is represented by two attributes, a `Rank` and a `Suit`. The provided starter file `Card.h` is pretty self explanatory. Only thing I want to specify is that the comparison operators that we are overloading for the Cards should not be considering the trump suit or lead suit. It is only based on standard card values. Comparison of cards is based on suit first then rank. The `card_less` function on the other hand does consider trump suit and lead suit. Go back to [[#Winning a Trick]] to read on how this should be determined.

# Pack class
Once you finish creating the implementation of the `Card` class, then you can move onto the `Pack` class. A `Pack` object represents a pack of cards. There are different operations that you can act on these pack of card and these are declared in the provided starter file `Pack.h`. 

A input stream used to initialize a Pack would have all 52 Cards in any order using the format `[rank] of [suit]\n`. Below is an example of such an input stream with only the first three lines.
```txt
Two of Hearts
Ace of Spades
Jack of Diamonds
```
