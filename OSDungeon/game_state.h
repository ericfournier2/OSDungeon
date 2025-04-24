#pragma once
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

	void setGlobalState(GameGlobalState state) { global_state = state; };
	void clearDialog() { message = ""; global_state = WAITING_FOR_INPUT; }
	void showDialog(const std::string& message_) { message = message_; global_state = MESSAGE_BOX; }
	void gameOver(const std::string& message_) { message = message_; global_state = GAME_OVER; }
private:
	GameGlobalState global_state = WAITING_FOR_INPUT;
	std::string message;
};

