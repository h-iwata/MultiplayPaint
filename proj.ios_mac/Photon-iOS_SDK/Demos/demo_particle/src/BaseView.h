#pragma once

class BaseView
{
public:
	virtual ~BaseView(void) {}
	virtual void info(const char* format, ...) = 0;
	virtual void warn(const char* format, ...) = 0;
	virtual void error(const char* format, ...) = 0;
	virtual void updateState(int state, const char* stateStr, const char* joinedRoomName) = 0;
	virtual void initPlayers(void) {}
	virtual void addPlayer(int playerNr, const char* playerName, bool local) = 0;
	virtual void removePlayer(int playerNr) = 0;
	virtual void changePlayerColor(int playerNr, int color) = 0;
	virtual void changePlayerPos(int playerNr, int x, int y) = 0;
    virtual void setupScene(int gridSize) = 0;
	virtual void updateRoomList(const char* roomNames[], unsigned int size) = 0;
};