COMPILER=g++
OPTIONS=-std=c++17 -pedantic -Wall -Werror -g
COMPILE=$(COMPILER) $(OPTIONS)

card=../Card.cpp
pack=../Pack.cpp
player=../Player.cpp
team=../Team.cpp
game=../Game.cpp
test_framework=catch_amalgamated.cpp

test: test_card.exe test_pack.exe test_player.exe test_team.exe #test_game.exe
	./test_card.exe
	./test_pack.exe
	./test_player.exe
	./test_team.exe
	#./test_game.exe

test_valgrind: test_card.exe test_pack.exe test_player.exe test_team.exe #test_game.exe
	valgrind ./test_card.exe
	valgrind ./test_pack.exe
	valgrind ./test_player.exe
	valgrind ./test_team.exe
	#valgrind ./test_game.exe

# Test individual files without valgrind
test_card: test_card.exe
	./test_card.exe

test_pack: test_pack.exe
	./test_pack.exe

test_player: test_player.exe
	./test_player.exe

test_team: test_team.exe
	./test_team.exe

test_game: test_game.exe
	./test_game.exe

run_game_example: run_game.exe
	./run_game.exe Kim Human Hope Human Danielle Human Flor Human

# Test individual files with valgrind
test_card_valgrind: test_card.exe
	valgrind ./test_card.exe

test_pack_valgrind: test_pack.exe
	valgrind ./test_pack.exe

test_player_valgrind: test_player.exe
	valgrind ./test_player.exe

test_team_valgrind: test_team.exe
	valgrind ./test_team.exe

test_game_valgrind: test_game.exe
	valgrind ./test_game.exe

# Object files
Card.o: $(card)
	$(COMPILE) -c $^

Pack.o: $(pack)
	$(COMPILE) -c $^

Player.o: $(player)
	$(COMPILE) -c $^

Team.o: $(team)
	$(COMPILE) -c $^

Game.o: $(game)
	$(COMPILE) -c $^

catch_amalgamated.o: catch_amalgamated.cpp
	$(COMPILE) -c $^

test_card.o: test_card.cpp
	$(COMPILE) -c $^

test_pack.o: test_pack.cpp
	$(COMPILE) -c $^

test_player.o: test_player.cpp
	$(COMPILE) -c $^

test_team.o: test_team.cpp
	$(COMPILE) -c $^

test_game.o: test_game.cpp
	$(COMPILE) -c $^

main.o: ../main.cpp
	$(COMPILE) -c $^

# Executables
test_card.exe: Card.o catch_amalgamated.o test_card.o
	$(COMPILE) -o $@ $^

test_pack.exe: Card.o Pack.o catch_amalgamated.o test_pack.o 
	$(COMPILE) -o $@ $^

test_player.exe: Card.o Player.o catch_amalgamated.o test_player.o
	$(COMPILE) -o $@ $^

test_team.exe: Card.o Player.o Team.o catch_amalgamated.o test_team.o 
	$(COMPILE) -o $@ $^

test_game.exe: Card.o Pack.o Player.o Team.o Game.o catch_amalgamated.o test_game.o
	$(COMPILE) -o $@ $^

run_game.exe: Card.o Pack.o Player.o Team.o Game.o main.o
	$(COMPILE) -o $@ $^

.PHONY: clean

clean:
	rm -rvf *.out *.exe *.dSYM *.stackdump *.o