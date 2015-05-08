#include <stdlib.h>
#include "LoadBalancingListener.h"
#include "DemoConstants.h"
#include "Game.h"
#include "CCApplication.h"
#include "CCEGLView.h"
#include "CCDirector.h"

#ifdef _EG_ANDROID_PLATFORM
#include <string.h>
#include <stdio.h>
#include <jni.h>
void parsePushSubscribeToUserChannel(const char* userId);
#endif

using namespace ExitGames::Common;
using namespace ExitGames::LoadBalancing;

const char* PeerStatesStr[] =
{
	"Uninitialized",
	"PeerCreated",
	"ConnectingToNameserver",
	"ConnectedToNameserver",
	"DisconnectingFromNameserver",
	"Connecting",
	"Connected",
	"WaitingForCustomAuthenticationNextStepCall",
	"Authenticated",
	"JoinedLobby",
	"DisconnectingFromMasterserver",
	"ConnectingToGameserver",
	"ConnectedToGameserver",
	"AuthenticatedOnGameServer",
	"Joining",
	"Joined",
	"Leaving",
	"Left",
	"DisconnectingFromGameserver",
	"ConnectingToMasterserver",
	"ConnectedComingFromGameserver",
	"AuthenticatedComingFromGameserver",
	"Disconnecting",
	"Disconnected"
};

// TODO: update PeerStatesStr when PeerStates changes
// Checker below checks count match only
class PeerStatesStrChecker
{
public:
	PeerStatesStrChecker()
	{
		assert(sizeof(PeerStatesStr)/sizeof(const char*) == PeerStates::Disconnected + 1);
	}
} checker;

LoadBalancingListener::LoadBalancingListener() 
	: lbc(NULL)
	, view(NULL)
	, game(NULL)
{
}

LoadBalancingListener::~LoadBalancingListener()  
{ 
	delete game; 
}

void LoadBalancingListener::init(ExitGames::LoadBalancing::Client* lbc, BaseView* view)
{
	this->lbc = lbc;
	this->view = view;
	game = new Game(this, view);
	updateState();
}

void LoadBalancingListener::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
}

ExitGames::LoadBalancing::Client* LoadBalancingListener::getClient()
{
	return lbc;
}

void LoadBalancingListener::debugReturn(DebugLevel::DebugLevel debugLevel, const JString& string)
{
	if(debugLevel <= DebugLevel::WARNINGS)
	{
		view->info(string.UTF8Representation().cstr());
		CCLOG("%ls", string.cstr());
	}
}

void LoadBalancingListener::connectionErrorReturn(int errorCode)
{
    view->warn("connection failed with error %d", errorCode);
	updateState();
}

void LoadBalancingListener::clientErrorReturn(int errorCode)
{
    view->warn("received error %d from client", errorCode);
	updateState();
}

void LoadBalancingListener::warningReturn(int warningCode)
{
    view->warn("received warning %d from client", warningCode);
}

void LoadBalancingListener::serverErrorReturn(int errorCode)
{
    view->warn("received error %d from server", errorCode);
	updateState();
}

void LoadBalancingListener::joinRoomEventAction(int playerNr, const JVector<int>& playernrs, const Player& player)
{
    view->info("player %d %s joined game", playerNr, player.getName().UTF8Representation().cstr());
	game->onActorJoin();
	game->refreshInfo();
//    view->updatePlayerList(self:myRoomActors());

	// saving names
	JString names;
	for(int i = 0; i < getClient()->getCurrentlyJoinedRoom().getPlayerCount();++i) 
	{
		if(names.length())
			names += ";";
		names += getClient()->getCurrentlyJoinedRoom().getPlayers()[i]->getName();
	}
	getClient()->getCurrentlyJoinedRoom().addCustomProperty(PROP_NAMES, names);
		
}

void LoadBalancingListener::leaveRoomEventAction(int playerNr, bool isInactive)
{
    view->info( "player %d %s", playerNr, isInactive ? "suspended game" : "abandoned game");
	if(!isInactive)
		game->onActorAbandon(playerNr);
	game->refreshInfo();
//    view->updatePlayerList(self:myRoomActors())	;
}

void LoadBalancingListener::disconnectEventAction(int playerNr)
{
	view->info( "player %d disconnected game", playerNr);
}

void LoadBalancingListener::connectReturn(int errorCode, const JString& errorString)
{
	updateState();
    if(errorCode == ErrorCode::OK)
	{
		view->info("connected");
//		lbc->opJoinRandomRoom();
//		lbc->opJoinRandomRoom(Hashtable(), 0, MatchmakingMode::SERIAL_MATCHING, "DDD", LobbyType::SQL_LOBBY, "");
//		lbc->opJoinLobby("AAA", LobbyType::SQL_LOBBY);
	}
	else
		view->warn("Warn: connect failed %d %s", errorCode, errorString.UTF8Representation().cstr());
}

void LoadBalancingListener::disconnectReturn(void)
{
	updateState();
    view->info("disconnected");
}

void LoadBalancingListener::createRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	updateState();
	if(errorCode == ErrorCode::OK)
	{
		view->info("room created");
        afterRoomJoined(localPlayerNr, gameProperties);
	}
	else
		view->warn("Warn: opCreateRoom() failed: %s", errorString.UTF8Representation().cstr());
}

void LoadBalancingListener::joinRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	updateState();
	if(errorCode == ErrorCode::OK)
	{
		view->info("game room joined");
        afterRoomJoined(localPlayerNr, gameProperties);
	}
	else
		view->info("opJoinRoom() failed: %s", errorString.UTF8Representation().cstr());
}

void LoadBalancingListener::joinRandomRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	updateState();
	if(errorCode == ErrorCode::NO_MATCH_FOUND)
	{
        view->info("no match found for random room");
	}
    else if(errorCode == ErrorCode::OK)
	{
		view->info("game room joined");
        afterRoomJoined(localPlayerNr, gameProperties);
	}
	else
		view->info("opJoinRandomRoom() failed: %s", errorString.UTF8Representation().cstr());
}

void LoadBalancingListener::leaveRoomReturn(int errorCode, const JString& errorString)
{
	updateState();
        
	if(errorCode == ErrorCode::OK)
        view->info("game room left");
	else
		view->info("opLeaveRoom() failed: %s", errorString.UTF8Representation().cstr());
}

void LoadBalancingListener::gotQueuedReturn(void)
{
	updateState();
}

void LoadBalancingListener::joinLobbyReturn(void)
{
    view->info("joined lobby");
	updateState();
//	lbc->opJoinRandomRoom();
}

void LoadBalancingListener::leaveLobbyReturn(void)
{
    view->info("left lobby");
	updateState();
}

void LoadBalancingListener::onRoomListUpdate()
{	
	const JVector<Room*>& rooms = lbc->getRoomList();
	const char** names = MemoryManagement::allocateArray<const char*>(rooms.getSize());
	for(unsigned int i = 0; i < rooms.getSize();++i) 
	{
		const Room* r = rooms.getElementAt(i);
		UTF8String u = r->getName();
		const char* name = u.cstr();
		char* tmp = MemoryManagement::allocateArray<char>(strlen(name) + 1);
		strcpy(tmp, name);
		names[i] = tmp;
	}
	view->updateRoomList(names, rooms.getSize());
	for(unsigned int i = 0; i < rooms.getSize();++i) 
	{
		MemoryManagement::deallocateArray(names[i]);
	}
	MemoryManagement::deallocateArray(names);
}

void LoadBalancingListener::onRoomPropertiesChange(const Hashtable& changes) 
{
	game->updateStateFromProps(changes);
}

void LoadBalancingListener::webRpcReturn(int errorCode, const JString& errorString, const JString& uriPath, int resultCode, const Dictionary<Object, Object>& data)
{
//	view->info("webRpcReturn:", errorCode, errorString, uriPath, resultCode, data);
	if(errorCode == 0 && resultCode == 0)
	{
		const char** names = MemoryManagement::allocateArray<const char*>(data.getSize());
		const JVector<Object>& keys = data.getHashtable().getKeys(); // keys are saved rooms names
		savedRoomList.removeAllElements();
		for(unsigned int i=0; i<keys.getSize(); i++)
		{
			JString nameStr = ((ValueObject<JString>)keys[i]).getDataCopy();
			Dictionary<Object, Object> info = ((ValueObject<Dictionary<Object, Object> >)data.getHashtable().getValue(keys[i])).getDataCopy();
			UTF8String u = nameStr.UTF8Representation();
			const char* name = u.cstr();
			char* tmp = MemoryManagement::allocateArray<char>(strlen(name) + 1);
			strcpy(tmp, name);
			names[i] = tmp;

			savedRoomList.put(name, info);
		}
		view->updateSavedRoomList(names, keys.getSize());
		for(unsigned int i=0; i<keys.getSize(); ++i) 
			MemoryManagement::deallocateArray(names[i]);
		MemoryManagement::deallocateArray(names);

	}
}

void LoadBalancingListener::connect(const char* userName) 
{ 
	view->info("Connecting...");
#ifdef _EG_ANDROID_PLATFORM
	parsePushSubscribeToUserChannel(userName);
#endif
	lbc->getLocalPlayer().setName(userName);
//	view.client:connectToNameServer()
	lbc->connect();
}

void LoadBalancingListener::updateState() 
{
	int state = lbc->getState();
	if(lbc->getIsInLobby())
		querySavedRoomList();

	view->onStateChange(state, PeerStatesStr[state], lbc->getIsInLobby(), lbc->getIsInGameRoom());
	game->refreshInfo();
}

void LoadBalancingListener::afterRoomJoined(int localPlayerNr, const Hashtable& gameProperties)
{
	view->info( "afterRoomJoined: localPlayerNr=", localPlayerNr );
	game->updateStateFromProps(gameProperties);
	game->onActorJoin();
	game->refreshInfo();
//    view->updatePlayerList(self:myRoomActors());

	view->setupBoard(BOARD_SIZE, BOARD_SIZE);
	game->updateBoard();

	if(lbc->getCurrentlyJoinedRoom().getPlayerCount() == 2)
	{
		lbc->getCurrentlyJoinedRoom().setIsVisible(false); // hide room when it's full
		lbc->getCurrentlyJoinedRoom().setIsOpen(false); // hide room when it's full
	}
}

void LoadBalancingListener::createRoom(const char* userName) 
{
	lbc->getLocalPlayer().setName(userName);
	const int SIZE = 1024;
	char name[SIZE];
	sprintf(name, "%s-%d", userName,rand() % 1000 );

	// reset all data
	delete game;
	game = new Game(this, view);
	game->generateNewBoard();
	Hashtable props = game->stateToProps(false);
	JVector<JString> propsListedInLobby;
	propsListedInLobby.addElement(PROP_TURN);
	propsListedInLobby.addElement(PROP_NAMES);

	lbc->opCreateRoom(name, true, true, MAX_PLAYERS, props
		, propsListedInLobby, JString()
		, LobbyType::DEFAULT
		, INT_MAX           // player ttl
		, 5000              // empty room ttl
		);
    view->info( "Creating room %s", name );
}

void LoadBalancingListener::service()
{
	game->service();	
}
		   
void LoadBalancingListener::click(int tileIdx)
{
	game->click(tileIdx);
}
	
void LoadBalancingListener::querySavedRoomList() 
{
	lbc->opWebRpc("GetGameList", Object());
}

void LoadBalancingListener::joinRoom(const char* gameId)
{
	view->info("Joining Room: ", gameId);
	lbc->opJoinRoom(gameId);
}

void LoadBalancingListener::joinRandomRoom()
{
	view->info("Joining Random Room");
	lbc->opJoinRandomRoom();
}

void LoadBalancingListener::suspendRoom()
{
	view->info("Suspending");
	lbc->opLeaveRoom(true);
}

void LoadBalancingListener::abandonRoom()
{
	view->info("Abandoning");
	lbc->opLeaveRoom();
}

void LoadBalancingListener::joinSavedRoom(const char* gameId)
{
	const Dictionary<Object, Object>* info = savedRoomList.getValue(JString(gameId));
	if(info)
	{
		const Object* actorNrObj = info->getHashtable().getValue(JString("ActorNr"));
		if(actorNrObj)
		{
			int actorNr = ((ValueObject<int>)actorNrObj).getDataCopy();
			view->info("Joining Saved Room: ", gameId, ", avtorNr:", actorNr);
			lbc->opJoinRoom(gameId, false, actorNr);
			return;
		}
	}

	view->error("No saved room for id", gameId);

}

int LoadBalancingListener::getNextPlayer(int fromPlayer)
{
	int lowest = fromPlayer;
	int next = INT_MAX;
	JVector<Player*> const& players = lbc->getCurrentlyJoinedRoom().getPlayers();
	for(unsigned i=0; i<players.getSize(); i++)
	{
		int nr = players[i]->getNumber();
		if(nr > fromPlayer && nr < next)
			next = nr;
		if(nr < lowest)
			lowest = nr;
	}
	if(next == INT_MAX)
		return lowest;
	else
		return next;
}

bool LoadBalancingListener::leave()
{
	if(lbc->getIsInLobby())
	{
		lbc->disconnect();
		return true;
	}
	else
		return false;
}