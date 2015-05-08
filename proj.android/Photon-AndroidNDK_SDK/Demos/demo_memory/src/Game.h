
#pragma once

#include "LoadBalancingListener.h"
#include "BaseView.h"
#include "DemoConstants.h"

class Game 
{
public:
	Game(LoadBalancingListener* lbl, BaseView* view);
	~Game();

	void service();
	void click(int tileIdx);
	void updateStateFromProps(const ExitGames::Common::Hashtable& changedCustomProps);
	void refreshInfo();
	bool checkCompleted(ExitGames::Common::JString& msg);
	void onActorJoin();
	void onActorAbandon(int playerNr);
	void afterRoomJoined();
	void generateNewBoard();
	void updateBoard();
	void saveStateToProps(bool);
	ExitGames::Common::Hashtable stateToProps(bool turnPass);

private:
	LoadBalancingListener* lbl;
	BaseView* view;

	nByte tiles[TILE_COUNT];
	nByte flippedTiles[2];
	
	int nextTurnPlayer;
	int turnNumber;
	nByte myPoints;
	int othersPoints;
	bool abandoned;

	unsigned long endTurnDelayedRunAt;

	bool logic();
	bool endTurn();
	void endTurnDelayed();
	int myActorNr();
	void resetFlippedTiles();
	ExitGames::Common::JString renderMessage();
};