#include "Game.h"
#include "DemoConstants.h"
#include <cstdio>
#include <stdlib.h>

using namespace ExitGames::Common;

Game::Game(LoadBalancingListener* lbl, BaseView* view)
	: lbl(lbl)
	, view(view)
	, nextTurnPlayer(0)
	, turnNumber(1)
	, myPoints(0)
	, othersPoints(0)
	, abandoned(false)
	, endTurnDelayedRunAt(0)
	
{
	resetFlippedTiles();
	for(int i = 0;i < TILE_COUNT;++i)
		tiles[i] = 0;
}

Game::~Game()
{
}

void Game::resetFlippedTiles()
{
	flippedTiles[0] = NOT_FLIPPED_TILE;
	flippedTiles[1] = NOT_FLIPPED_TILE;
}

void Game::service()
{
	unsigned long t = GETTIMEMS();
	if(endTurnDelayedRunAt && t >= endTurnDelayedRunAt) 
	{
		endTurnDelayedRunAt = 0;
		endTurnDelayed();
	}
}

bool Game::logic()
{
	bool turnPass = false;
	if(flippedTiles[0] != NOT_FLIPPED_TILE && flippedTiles[1] != NOT_FLIPPED_TILE)
		turnPass = endTurn();
	updateBoard();
	refreshInfo();
	return turnPass;
}

void Game::updateStateFromProps(const ExitGames::Common::Hashtable& changedCustomProps)
{
	const int BUF_SIZE = 20; // 1 int with some prefix
	char buf[BUF_SIZE];
	for(int i = 0;i < TILE_COUNT;++i)
	{		
		sprintf(buf, "%d", i);
		Object const* v = changedCustomProps.getValue(buf);
		if(v)
			tiles[i] = ((ValueObject<nByte>*)v)->getDataCopy();
	}
	Object const* v = changedCustomProps.getValue("flips");
	if(v)
	{
		if(v->getDimensions() == 1 && v->getSizes()[0] == 2) 
		{
			nByte* data = ((ValueObject<nByte*>*)v)->getDataCopy();
			flippedTiles[0] = data[0];
			flippedTiles[1] = data[1];
		}
		else
			view->warn("Bad flips incoming data");
	}
	
	v = changedCustomProps.getValue("pt");
	if(v)
		nextTurnPlayer = ((ValueObject<int>*)v)->getDataCopy();
	v = changedCustomProps.getValue("t#");
	if(v)
		turnNumber = ((ValueObject<int>*)v)->getDataCopy();

	sprintf(buf, "pt%d", myActorNr());
	v = changedCustomProps.getValue(buf);
	if(v)
		myPoints = ((ValueObject<nByte>*)v)->getDataCopy();
	v = changedCustomProps.getValue("abandoned");
	if(v)
		abandoned = ((ValueObject<bool>*)v)->getDataCopy();

	int otherId = lbl->getNextPlayer(myActorNr());
	if(otherId != myActorNr())
	{
		sprintf(buf, "pt%d", otherId);
		v = changedCustomProps.getValue(buf);
		if(v)
			othersPoints = ((ValueObject<nByte>*)v)->getDataCopy();
	}

	logic();
}

void Game::saveStateToProps(bool turnPass)
{
	const Hashtable& props = stateToProps(turnPass);
	lbl->getClient()->getCurrentlyJoinedRoom().mergeCustomProperties(props, turnPass);
}
	
Hashtable Game::stateToProps(bool turnPass)
{
	const int BUF_SIZE = 20; // 1 int with some prefix
	char buf[BUF_SIZE]; 
	Hashtable props;
	
    for(int i = 0;i < TILE_COUNT;++i)
	{		
		if(tiles[i])
		{
			sprintf(buf, "%d", i);
			props.put(buf, tiles[i]);
		}
	}
	props.put("flips", flippedTiles, 2);
    props.put("pt", nextTurnPlayer);
    props.put("t#", turnNumber);
	sprintf(buf, "pt%d", myActorNr());
	props.put(buf, myPoints); // we always only save "our" points. this will not affect the opponent's score.
	if(abandoned)
		props.put("abandoned", abandoned);
	if(turnPass)
		props.put(PROP_TURN, lbl->getNextPlayer(nextTurnPlayer));
				
	return props;
}

bool Game::endTurn()
{
	view->info("EndTurn");

	bool turnPass = false;
	assert(flippedTiles[0] != NOT_FLIPPED_TILE);
	assert(flippedTiles[1] != NOT_FLIPPED_TILE);
		
	if(tiles[flippedTiles[0]] == tiles[flippedTiles[1]])
		view->info("Match!");
	else
	{
		view->info("Miss. Next player:", nextTurnPlayer);
		turnPass = true;
	}

	refreshInfo();
	
	endTurnDelayedRunAt = GETTIMEMS() + END_OF_TURN_DELAY;

	return turnPass;
}
	
void Game::endTurnDelayed()
{
	if(tiles[flippedTiles[0]] == tiles[flippedTiles[1]])
	{
		tiles[flippedTiles[0]] = 0;
		tiles[flippedTiles[1]] = 0;
		if(nextTurnPlayer == myActorNr())
			++myPoints;
		else
			++othersPoints;
	}
	else
		nextTurnPlayer = lbl->getNextPlayer(nextTurnPlayer);

	resetFlippedTiles();
	updateBoard();
	refreshInfo();
}

void Game::generateNewBoard()
{
	for(int i=0; i<TILE_COUNT; ++i)
        tiles[i] = i % TILES_TYPE_COUNT + 1;
	for(int i=0; i<TILE_COUNT * 10; ++i)
	{
		int i1 = rand() % TILE_COUNT;
		int i2 = rand() % TILE_COUNT;
		nByte tmp = tiles[i1];
		tiles[i1] = tiles[i2];
		tiles[i2] = tmp;
	}
}
	
void Game::click(int tileIdx)
{
	view->info("click");
	int otherId = lbl->getNextPlayer(myActorNr());
	if(otherId == myActorNr())
	{
		view->info("Wait for other's join");
		return;
	}
	if(nextTurnPlayer != myActorNr())
	{
		view->info("Not your turn");
		return;
	}
		
	if(flippedTiles[0] != NOT_FLIPPED_TILE && flippedTiles[1] != NOT_FLIPPED_TILE)
	{
		view->info("Please wait");
		return;
	}
	if(tiles[tileIdx] != 0)
	{
		if(flippedTiles[0] == NOT_FLIPPED_TILE)
		{
			flippedTiles[0] = tileIdx;
			view->info("flip 0");
		}
		else if(flippedTiles[1] == NOT_FLIPPED_TILE)
		{
			if(flippedTiles[0] != tileIdx) // check double click
			{
				flippedTiles[1] = tileIdx;
				view->info("flip 1");
				if(flippedTiles[0] == flippedTiles[1])
					view->info("Match");
			}
		}
		bool turnPass = logic();
		saveStateToProps(turnPass);
	}
}


bool Game::checkCompleted(JString& msg)
{
	msg = L"";
	for(int i=0; i<TILE_COUNT; ++i)
		if(tiles[i] != 0)
			return false;

	if(myPoints > othersPoints)
		msg = L"You won!";
    else if(myPoints < othersPoints)
        msg = L"You lost!";
	else
		msg = L"Draw!";
	return true;
}
	
void Game::updateBoard()
{
	view->updateBoard(tiles, flippedTiles);
}
	
JString Game::renderMessage()
{
	int otherId = lbl->getNextPlayer(myActorNr());
	JString complMsg;
	bool completed = checkCompleted(complMsg);
	if(abandoned)
		return L"Other left for good";
	else if(completed)
		return complMsg;
	else if(nextTurnPlayer == otherId)
		return L"Other's turn";
	else if(nextTurnPlayer == myActorNr())
		return L"Your turn";
	else if(nextTurnPlayer == 0 || otherId == myActorNr())
		return L"Waiting for other's join";
	else
		return L"Wrong game state";
}

void Game::onActorJoin()
{
	if(nextTurnPlayer == 0 && lbl->getClient()->getCurrentlyJoinedRoom().getPlayerCount() == 2)
		nextTurnPlayer = lbl->getClient()->getCurrentlyJoinedRoom().getMasterClientID();
}
	
void Game::onActorAbandon(int playerNr)
{
	abandoned = true;
	saveStateToProps(false);
}
	

// updates state bar and game message
void Game::refreshInfo()
{
    if(lbl->getClient()->getIsInGameRoom()) // put some additional info in state bar
	{
		const int INFO_SIZE = 1024;
		char info[INFO_SIZE];
		int otherNr = lbl->getNextPlayer(myActorNr());
		if(otherNr == myActorNr())
			otherNr = 0;
		bool otherInactive = false;
		if(otherNr)
		{
			const ExitGames::LoadBalancing::Player* o = lbl->getClient()->getCurrentlyJoinedRoom().getPlayerForNumber(otherNr);
			otherInactive = o && o->getIsInactive();
		}
		const int OTHER_STR_SIZE = 20;
		char otherStr[OTHER_STR_SIZE];
		if(otherNr)
			sprintf(otherStr, "%d", otherNr);
		else
			sprintf(otherStr, "%s", "?");
		sprintf(info, "%s, %d vs. %s%s%s, sc: %d/%d, t: %d",
			lbl->getClient()->getCurrentlyJoinedRoom().getName().UTF8Representation().cstr(),
			myActorNr(),
			otherInactive ? "(" : "",
			otherStr,
			otherInactive ? ")" : "",
			myPoints, othersPoints, nextTurnPlayer
			);
		view->refreshStateInfo(info);
	}
	else
		view->refreshStateInfo(NULL);
        		
	UTF8String msg = renderMessage();
	view->refreshMessage(msg.cstr());
}

int Game::myActorNr()
{
	return lbl->getClient()->getLocalPlayer().getNumber();
}