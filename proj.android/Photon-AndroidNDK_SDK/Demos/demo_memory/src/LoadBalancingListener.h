#pragma once
#include "LoadBalancing-cpp/inc/Client.h"
#include "BaseView.h"

class Game;

// Also used as main controlling object
class LoadBalancingListener : public ExitGames::LoadBalancing::Listener {
private:
	ExitGames::LoadBalancing::Client* lbc;
	BaseView* view;
	Game* game;
	ExitGames::Common::Dictionary<ExitGames::Common::JString, ExitGames::Common::Dictionary<ExitGames::Common::Object, ExitGames::Common::Object> > savedRoomList;

public: 
	LoadBalancingListener();
	~LoadBalancingListener();
	
	void init(ExitGames::LoadBalancing::Client* lbc, BaseView* view);
	void connect(const char* userName);
	bool leave();
	void service();
	void createRoom(const char* userName);
	void click(int tileIdx);
	void querySavedRoomList();
	void joinRoom(const char* gameId);
	void joinSavedRoom(const char* gameId);
	void joinRandomRoom();
	void suspendRoom();
	void abandonRoom();
	int getNextPlayer(int fromPlayer);
	ExitGames::LoadBalancing::Client* getClient();
private:

	//From LoadBalancing::LoadBalancingListener
	// receive and print out Photon LoadBalancing debug out here
	virtual void debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string);

	// implement your error-handling here
	virtual void connectionErrorReturn(int errorCode);
	virtual void clientErrorReturn(int errorCode);
	virtual void warningReturn(int warningCode);
	virtual void serverErrorReturn(int errorCode);

	// events, triggered by certain operations of all players in the same room
	virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player);
	virtual void leaveRoomEventAction(int playerNr, bool isInactive);
	virtual void disconnectEventAction(int playerNr);
	virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent);

	// callbacks for operations on PhotonLoadBalancing server
	virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString);
	virtual void disconnectReturn(void);
	virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString);
	virtual void gotQueuedReturn(void);
	virtual void joinLobbyReturn(void);
	virtual void leaveLobbyReturn(void);
	virtual void onRoomListUpdate(void);
	virtual void onRoomPropertiesChange(const ExitGames::Common::Hashtable& changes);
	virtual void webRpcReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& uriPath, int resultCode, const ExitGames::Common::Dictionary<ExitGames::Common::Object, ExitGames::Common::Object>& data);

	void updateState(void);
	void afterRoomJoined(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties);
};

