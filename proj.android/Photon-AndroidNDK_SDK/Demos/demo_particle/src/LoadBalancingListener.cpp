#include <iostream>
#include <stdlib.h>
#include "Common-cpp/inc/UTF8String.h"
#include "Common-cpp/inc/MemoryManagement/Allocate.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/EventCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/ParameterCode.h"
#include "LoadBalancingListener.h"
#include "DemoConstants.h"

#ifdef _EG_ANDROID_PLATFORM
#	include <stdarg.h>
#	include <android/log.h>
#endif

using namespace ExitGames::Common;
using namespace ExitGames::LoadBalancing;

static int randomColor(int from = 0, int to = 256) 
{
	int r = from + rand() % (to - from);
	int g = from + rand() % (to - from);
	int b = from + rand() % (to - from);
	return (r << 16) + (g << 8) + b;
}

const char* PeerStatesStr[] = {
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
	PeerStatesStrChecker(void)
	{
		assert(sizeof(PeerStatesStr)/sizeof(const char*) == PeerStates::Disconnected + 1);
	}
} checker;

LocalPlayer::LocalPlayer(void) : x(0), y(0), color(randomColor(100)), lastUpdateTime(0)
{
}

LoadBalancingListener::LoadBalancingListener(BaseView* pView) : mpLbc(NULL), mpView(pView), mLcalPlayerNr(0), mGridSize(DEFAULT_GRID_SIZE), mMap(DEFAULT_MAP), mAutomove(true), mUseGroups(true), mSendGroup(0)
{
}

LoadBalancingListener::~LoadBalancingListener(void)
{
	delete mpView; 
}

void LoadBalancingListener::setLBC(ExitGames::LoadBalancing::Client* pLbc)
{
	this->mpLbc = pLbc;
}

void LoadBalancingListener::connect(void)
{
	mpLbc->connect(); 
}

void LoadBalancingListener::setUseGroups(bool value)
{
	mUseGroups = value;
}

bool LoadBalancingListener::getUseGroups(void)
{
	return mUseGroups;
}

void LoadBalancingListener::setSendGroup(nByte value)
{
	mSendGroup = value;
}

void LoadBalancingListener::setAutomove(bool a)
{
	mAutomove = a; 
}

bool LoadBalancingListener::getAutomove(void)
{
	return mAutomove;
}

void LoadBalancingListener::debugReturn(const JString& string)
{
	std::wcerr << string << std::endl;
#ifdef _EG_ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_INFO, "DemoParticle", "%s", string.UTF8Representation().cstr());
#endif
}

void LoadBalancingListener::debugReturn(DebugLevel::DebugLevel /*debugLevel*/, const JString& string)
{
	debugReturn(string);
}

void LoadBalancingListener::connectionErrorReturn(int errorCode)
{
	mpView->warn("connection failed with error %d", errorCode);
	updateState();
	// reconnect on error
	//	connect();
}

void LoadBalancingListener::clientErrorReturn(int errorCode)
{
	mpView->warn("received error %d from client", errorCode);
	updateState();
}

void LoadBalancingListener::warningReturn(int warningCode)
{
	mpView->warn("received warning %d from client", warningCode);
}

void LoadBalancingListener::serverErrorReturn(int errorCode)
{
	mpView->warn("received error %d from server", errorCode);
	updateState();
}

void LoadBalancingListener::joinRoomEventAction(int playerNr, const JVector<int>& playernrs, const Player& player)
{
	mpView->info("player %d %s has joined the game", playerNr, player.getName().UTF8Representation().cstr());
	mpView->addPlayer(playerNr, player.getName().UTF8Representation().cstr(), player.getNumber() == mpLbc->getLocalPlayer().getNumber());
	mpView->changePlayerColor(mLcalPlayerNr, mLocalPlayer.color);
	updateGroups();
}

void LoadBalancingListener::leaveRoomEventAction(int playerNr, bool isInactive)
{    
	if(isInactive)
		mpView->info("player %d has suspended the game", playerNr);		
	else
	{
		mpView->info("player %d has abandoned the game", playerNr);
		mpView->removePlayer(playerNr);
	}
}

void LoadBalancingListener::disconnectEventAction(int playerNr)
{
	mpView->info("player %d has disconnected", playerNr);
}

void LoadBalancingListener::customEventAction(int playerNr, nByte eventCode, const Object& eventContentObj)
{
	ExitGames::Common::Hashtable eventContent = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContentObj).getDataCopy();
	if(eventCode == 1)
	{
		Object const* obj = eventContent.getValue("1");
		if(obj == NULL) obj = eventContent.getValue((nByte)1);
		if(obj == NULL) obj = eventContent.getValue(1);
		if(obj == NULL) obj = eventContent.getValue(1.0);
		if(obj) 
		{
			int color = (int)(obj->getType() == 'd' ?  ((ValueObject<double>*)obj)->getDataCopy() : ((ValueObject<int>*)obj)->getDataCopy());
			mpView->changePlayerColor(playerNr, color);
		}
		else 
			mpView->warn("bad color event");
	}
	else if(eventCode == 2)
	{
		Object const* obj = eventContent.getValue("1");
		if(obj == NULL) obj = eventContent.getValue((nByte)1);
		if(obj == NULL) obj = eventContent.getValue(1);
		if(obj == NULL) obj = eventContent.getValue(1.0);
		if(obj && obj->getDimensions() == 1 && obj->getSizes()[0] == 2) 
		{
			int x = 0; int y = 0;
			if(obj->getType() == 'd') 
			{
				double* data = ((ValueObject<double*>*)obj)->getDataCopy();
				x = (int)data[0];
				y = (int)data[1];
			}
			if(obj->getType() == 'i') 
			{
				int* data = ((ValueObject<int*>*)obj)->getDataCopy();
				x = (int)data[0];
				y = (int)data[1];
			}
			else if(obj->getType() == 'b') 
			{
				nByte* data = ((ValueObject<nByte*>*)obj)->getDataCopy();
				x = (int)data[0];
				y = (int)data[1];
			}
			else if(obj->getType() == 'z') 
			{
				Object* data = ((ValueObject<Object*>*)obj)->getDataCopy();
				if(data[0].getType() == 'i') 
				{
					x = ((ValueObject<int>*)(data + 0))->getDataCopy();
					y = ((ValueObject<int>*)(data + 1))->getDataCopy();
				}
				else 
				{
					x = (int)((ValueObject<double>*)(data + 0))->getDataCopy();
					y = (int)((ValueObject<double>*)(data + 1))->getDataCopy();
				}
			}
			mpView->changePlayerPos(playerNr, x, y);
		}
		else 
			mpView->warn("Bad position event");
	}
}

void LoadBalancingListener::connectReturn(int errorCode, const JString& errorString)
{
	updateState();
	if(errorCode == ErrorCode::OK)
	{
		mpView->info("connected");
		mpLbc->opJoinRandomRoom();
	}
	else
		mpView->warn("Warn: connect failed %d %s", errorCode, errorString.UTF8Representation().cstr());
}

void LoadBalancingListener::disconnectReturn(void)
{
	updateState();
	mpView->info("disconnected");
	mpView->initPlayers();
}

void LoadBalancingListener::createRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	updateState();
	if(errorCode == ErrorCode::OK)
	{
		mpView->info("room has been created");
		afterRoomJoined(localPlayerNr);
	}
	else
		mpView->warn("Warn: opCreateRoom() failed: %s", errorString.UTF8Representation().cstr());
}

void LoadBalancingListener::joinRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	updateState();
	if(errorCode == ErrorCode::OK)
	{
		mpView->info("game room has been successfully joined");
		afterRoomJoined(localPlayerNr);
	}
	else
		mpView->info("opJoinRoom() failed: %s", errorString.UTF8Representation().cstr());
}

void LoadBalancingListener::joinRandomRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	updateState();
	if(errorCode == ErrorCode::NO_MATCH_FOUND)
		createRoom();
	else if(errorCode == ErrorCode::OK)
	{
		mpView->info("game room has been successfully joined");
		afterRoomJoined(localPlayerNr);
	}
	else
		mpView->info("opJoinRandomRoom() failed: %s", errorString.UTF8Representation().cstr());
}

void LoadBalancingListener::leaveRoomReturn(int errorCode, const JString& errorString)
{
	updateState();
	if(errorCode == ErrorCode::OK)
		mpView->info("game room has been successfully left");
	else
		mpView->info("opLeaveRoom() failed: %s", errorString.UTF8Representation().cstr());
	mpView->initPlayers();
}

void LoadBalancingListener::gotQueuedReturn(void)
{
	updateState();
}

void LoadBalancingListener::joinLobbyReturn(void)
{
	mpView->info("joined lobby");
	updateState();
}

void LoadBalancingListener::leaveLobbyReturn(void)
{
	mpView->info("left lobby");
	updateState();
}

void LoadBalancingListener::onLobbyStatsUpdate(const JVector<LobbyStatsResponse>& res)
{
	mpView->info("===================== lobby stats update");
	for(unsigned int i=0; i<res.getSize(); ++i) 
		mpView->info(res[i].toString().UTF8Representation().cstr());

	// lobby stats request test
	JVector<LobbyStatsRequest> ls;
	ls.addElement(LobbyStatsRequest());
	ls.addElement(LobbyStatsRequest(L"AAA"));
	ls.addElement(LobbyStatsRequest(L"BBB"));
	ls.addElement(LobbyStatsRequest(L"CCC", LobbyType::DEFAULT));
	ls.addElement(LobbyStatsRequest(L"AAA", LobbyType::SQL_LOBBY));
	mpLbc->opLobbyStats(ls);
	//	mpLbc->opLobbyStats();
}

void LoadBalancingListener::onLobbyStatsResponse(const ExitGames::Common::JVector<LobbyStatsResponse>& res)
{
	mpView->info("===================== lobby stats response");
	for(unsigned int i=0; i<res.getSize(); ++i) 
		mpView->info(res[i].toString().UTF8Representation().cstr());
}

void LoadBalancingListener::onRoomListUpdate()
{	
	const JVector<Room*>& rooms = mpLbc->getRoomList();
	const char** names = MemoryManagement::allocateArray<const char*>(rooms.getSize());
	for(unsigned int i=0; i<rooms.getSize(); ++i) 
	{
		const Room* r = rooms.getElementAt(i);
		UTF8String u = r->getName();
		const char* name = u.cstr();
		char* tmp = MemoryManagement::allocateArray<char>(strlen(name) + 1);
		strcpy(tmp, name);
		names[i] = tmp;
	}
	mpView->updateRoomList(names, rooms.getSize());
	for(unsigned int i = 0; i < rooms.getSize();++i) 
		MemoryManagement::deallocateArray(names[i]);
	MemoryManagement::deallocateArray(names);
}

void LoadBalancingListener::onRoomPropertiesChange(const ExitGames::Common::Hashtable& changes) 
{
	if(updateGridSize(changes)) 
		mpView->setupScene(mGridSize);
}

void LoadBalancingListener::updateState() 
{
	int state = mpLbc->getState();
	mpView->updateState(state, PeerStatesStr[state], state == PeerStates::Joined ? ((ExitGames::Common::UTF8String)mpLbc->getCurrentlyJoinedRoom().getName()).cstr() : NULL);
}

bool LoadBalancingListener::updateGridSize(const ExitGames::Common::Hashtable& props) 
{
	if(props.contains("s")) 
	{
		const Object* v = props.getValue("s");
		switch(v->getType()) 
		{
		case 'i':
			mGridSize = ((ValueObject<int>*)v)->getDataCopy();
			return true;
		case 'd':
			mGridSize = (int)((ValueObject<double>*)v)->getDataCopy();
			return true;
		}
	}
	return false;
}

void LoadBalancingListener::afterRoomJoined(int localPlayerNr)
{
	mpView->info( "afterRoomJoined: localPlayerNr=", localPlayerNr );
	this->mLcalPlayerNr = localPlayerNr;
	MutableRoom& myRoom = mpLbc->getCurrentlyJoinedRoom();
	Hashtable props = myRoom.getCustomProperties();
	updateGridSize(props);
	if(props.contains("m"))
		mMap = ((ValueObject<JString>*)props.getValue("m"))->getDataCopy();

	mpView->initPlayers();
	mpView->setupScene(mGridSize);

	const JVector<Player*>& players = myRoom.getPlayers();
	for(unsigned int i=0; i<players.getSize(); ++i)
	{
		const Player* p = players[i];
		mpView->addPlayer(p->getNumber(), p->getName().UTF8Representation().cstr(), p->getNumber() == mpLbc->getLocalPlayer().getNumber());
	}
	mpView->changePlayerColor(localPlayerNr, mLocalPlayer.color);
	raiseColorEvent();
}

void LoadBalancingListener::createRoom() 
{
	char name[16];
	sprintf(name, "native-%d",rand() % 100);
	Hashtable props;
	props.put("s", mGridSize);
	props.put("m", mMap);
	mpLbc->opCreateRoom(name, true, true, 0, props);
	mpView->info( "Creating room %s", name );
}

void LoadBalancingListener::service(void)
{

	unsigned long t = GETTIMEMS();
	if((t - mLocalPlayer.lastUpdateTime) > PLAYER_UPDATE_INTERVAL_MS) 
	{
		mLocalPlayer.lastUpdateTime = t;
		if(mpLbc->getState() == PeerStates::Joined && mAutomove) 
			moveLocalPlayer();
	}
}

void LoadBalancingListener::moveLocalPlayer(void)
{
	int d = rand() % 8;
	int xOffs[] = {-1, 0, 1, -1, 1, -1, 0, 1};
	int yOffs[] = {1, 1, 1, 0, 0, -1, -1, -1};
	int x = mLocalPlayer.x + xOffs[d];
	int y = mLocalPlayer.y += yOffs[d];
	if(x < 0) x = 1;
	if(x >= mGridSize) x = mGridSize - 2;
	if(y < 0) y = 1;
	if(y >= mGridSize) y = mGridSize - 2;
	setLocalPlayerPos(x, y);
}

bool LoadBalancingListener::setLocalPlayerPos(int x, int y)
{
	if(x >= 0 && x < mGridSize && y >= 0 && y < mGridSize) 
	{
		int prevGroup = getGroupByPos();
		mLocalPlayer.x = x;
		mLocalPlayer.y = y;
		if(prevGroup != getGroupByPos()) 
			updateGroups();
		Hashtable data;
		nByte coords[] = {static_cast<nByte>(mLocalPlayer.x), static_cast<nByte>(mLocalPlayer.y)};
		data.put((nByte)1, coords, 2);

		if(mSendGroup)
			mpLbc->opRaiseEvent(false, data, 2, 0, 0, 0, 0, 0, mSendGroup);
		else
		{
			if(mUseGroups)
				mpLbc->opRaiseEvent(false, data, 2, 0, 0, 0, 0, 0, getGroupByPos());
			else
				mpLbc->opRaiseEvent(false, data, 2);
		}
		mpView->changePlayerPos(mLcalPlayerNr, mLocalPlayer.x, mLocalPlayer.y);
		return true;
	}
	else 
		return false;
}

void LoadBalancingListener::raiseColorEvent(void)
{
	Hashtable data;	
	data.put((nByte)1, mLocalPlayer.color);
	mpLbc->opRaiseEvent(true, data, 1,0,ExitGames::Lite::EventCache::ADD_TO_ROOM_CACHE);
}

int LoadBalancingListener::getGroupByPos(void)
{
	int xp = mLocalPlayer.x * GROUPS_PER_AXIS / mGridSize;
	int yp = mLocalPlayer.y * GROUPS_PER_AXIS / mGridSize;
	return 1 + xp + yp * GROUPS_PER_AXIS;
}

void LoadBalancingListener::changeRandomColor(void)
{
	mLocalPlayer.color = randomColor(100);
	raiseColorEvent();
	mpView->changePlayerColor(mLcalPlayerNr, mLocalPlayer.color);
}

void LoadBalancingListener::nextGridSize(void)
{
	mGridSize = mGridSize << 1;
	if(mGridSize > GRID_SIZE_MAX) 
		mGridSize = GRID_SIZE_MIN;
	mpView->info("nextGridSize: %d", mGridSize);
	mpLbc->getCurrentlyJoinedRoom().addCustomProperty("s", mGridSize);
	mpView->setupScene(mGridSize);
}

void LoadBalancingListener::updateGroups(void)
{
	if(mpLbc->getIsInRoom())
	{	
		ExitGames::Common::JVector<nByte> remove;
		if(mUseGroups)
		{
			ExitGames::Common::JVector<nByte> add;
			add.addElement(getGroupByPos());
			mpLbc->opChangeGroups(&remove, &add);
		}
		else
			mpLbc->opChangeGroups(&remove, NULL);
	}
}