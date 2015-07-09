#ifndef __NETWORK_LOGIC_H
#define __NETWORK_LOGIC_H

#include "cocos2d.h"
#include "LoadBalancing-cpp/inc/Client.h"
#include "StateAccessor.h"
#include <array>
#include <queue>

using namespace ExitGames;

class NetworkListener : private LoadBalancing::Listener
{
public:
    enum Input {
        INPUT_NON = 0,
        INPUT_CREATE_GAME,
        INPUT_JOIN_GAME,
        INPUT_EXIT
    };
	NetworkListener(const LoadBalancing::AuthenticationValues& authenticationValues=LoadBalancing::AuthenticationValues());
    
    void run(void);
    bool isRoomExists(void);
    void sendEvent(nByte code, Common::Hashtable* eventContent);
    
	void registerForStateUpdates(NetworkLogicListener* listener);
	
	void connect(void);
	void opCreateRoom(void);
	void opJoinRandomRoom(void);
	void disconnect(void);
	void sendEvent(void);

	NetworkState getState(void) const;
    
    int playerNr = 0;
    std::queue<std::array<float, 3>> eventQueue;
    std::queue<std::array<float, 3>> beginEventQueue;
    std::queue<bool> tapResetEnventQueue;
    
    CC_SYNTHESIZE(int, mLastInput, LastInput);
    
private:
	virtual void connectionErrorReturn(int errorCode);
    virtual void customEventAction(int playerNr, nByte eventCode, const Common::Object& eventContent);
    virtual void onAvailableRegions(const Common::JVector<Common::JString>& availableRegions, const Common::JVector<Common::JString>& availableRegionServers);
    
    virtual void connectReturn(int errorCode, const Common::JString& errorString);
    virtual void disconnectReturn(void);
    virtual void createRoomReturn(int localPlayerNr, const Common::Hashtable& gameProperties, const Common::Hashtable& playerProperties, int errorCode, const Common::JString& errorString);
    virtual void joinRoomReturn(int localPlayerNr, const Common::Hashtable& gameProperties, const Common::Hashtable& playerProperties, int errorCode, const Common::JString& errorString);
    virtual void joinRandomRoomReturn(int localPlayerNr, const Common::Hashtable& gameProperties, const Common::Hashtable& playerProperties, int errorCode, const Common::JString& errorString);
    virtual void leaveRoomReturn(int errorCode, const Common::JString& errorString);
    
    
    //以下は必要に応じて実装
    virtual void debugReturn(Common::DebugLevel::DebugLevel debugLevel, const Common::JString& string) {};
    
    virtual void clientErrorReturn(int errorCode) {};
    virtual void warningReturn(int warningCode) {};
    virtual void serverErrorReturn(int errorCode) {};

    virtual void joinRoomEventAction(int playerNr, const Common::JVector<int>& playernrs, const LoadBalancing::Player& player) {};
    virtual void leaveRoomEventAction(int playerNr, bool isInactive) {};
    virtual void disconnectEventAction(int playerNr) {};
	
    virtual void onLobbyStatsResponse(const Common::JVector<LoadBalancing::LobbyStatsResponse>& lobbyStats) {};
    virtual void onLobbyStatsUpdate(const Common::JVector<LoadBalancing::LobbyStatsResponse>& lobbyStats) {};
    
    virtual void joinLobbyReturn(void) {};
    virtual void leaveLobbyReturn(void) {};

	LoadBalancing::Client mLoadBalancingClient;
	Common::JString mLastJoinedRoom;
	int mLastActorNr;
	StateAccessor mStateAccessor;
    
};

#endif
