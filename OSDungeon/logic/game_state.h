#pragma once
#include "common.h"
#include <string>

enum GameGlobalState {
	WAITING_FOR_INPUT,
	MESSAGE_BOX,
	GAME_OVER
};

class GameState {
public:
	GameGlobalState getGlobalState() const { return global_state; }
	std::string getMessage() const { return message; }
	CardinalCoord getPlayerPos() const { return player_pos; }

	void setGlobalState(GameGlobalState state) { global_state = state; };
	void clearDialog() { message = ""; global_state = WAITING_FOR_INPUT; }
	void showDialog(const std::string& message_) { message = message_; global_state = MESSAGE_BOX; }
	void gameOver(const std::string& message_) { message = message_; global_state = GAME_OVER; }
	void setPlayerPos(CardinalCoord pos) { player_pos = pos; }
private:
	GameGlobalState global_state = WAITING_FOR_INPUT;
	std::string message;
	CardinalCoord player_pos = CardinalCoord(0,0,CardinalDirection::NORTH);
};

