#ifndef __NETWORK_LOGIC_H
#define __NETWORK_LOGIC_H

#include "cocos2d.h"
#include "LoadBalancing-cpp/inc/Client.h"
#include "StateAccessor.h"
#include <array>
#include <queue>



class NetworkListener : private ExitGames::LoadBalancing::Listener
{
public:
    enum Input
    {
        INPUT_NON = 0,
        INPUT_CREATE_GAME,
        INPUT_JOIN_GAME,
        INPUT_EXIT
    };
	NetworkListener(const ExitGames::LoadBalancing::AuthenticationValues& authenticationValues=ExitGames::LoadBalancing::AuthenticationValues());
    
    bool isRoomExists(void);
    void sendEvent(nByte code, ExitGames::Common::Hashtable* eventContent);
    
	void registerForStateUpdates(NetworkLogicListener* listener);
	void run(void);
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
    virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent);
    virtual void onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers);
    
    virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString);
    virtual void disconnectReturn(void);
    virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
    virtual void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
    virtual void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
    virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString);
    
    
    //以下は必要に応じて実装
    virtual void debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string) {};
    
    virtual void clientErrorReturn(int errorCode) {};
    virtual void warningReturn(int warningCode) {};
    virtual void serverErrorReturn(int errorCode) {};

    virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) {};
    virtual void leaveRoomEventAction(int playerNr, bool isInactive) {};
    virtual void disconnectEventAction(int playerNr) {};
	
    virtual void onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats) {};
    virtual void onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats) {};
    
    virtual void joinLobbyReturn(void) {};
    virtual void leaveLobbyReturn(void) {};

	ExitGames::LoadBalancing::Client mLoadBalancingClient;
	ExitGames::Common::JString mLastJoinedRoom;
	int mLastActorNr;
	StateAccessor mStateAccessor;
    
};

#endif
