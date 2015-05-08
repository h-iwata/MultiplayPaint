#ifndef __PhotonTest__StateAccessor__
#define __PhotonTest__StateAccessor__

#include "LoadBalancing-cpp/inc/Client.h"
#include "NetworkState.h"
#include "NetworkLogicListner.h"

class NetworkLogicListener;

class StateAccessor
{
public:
    NetworkState getState(void) const;
    void setState(NetworkState newState);
    void registerForStateUpdates(NetworkLogicListener* listener);
    
private:
    NetworkState mState;
    ExitGames::Common::JVector<NetworkLogicListener*> mStateUpdateListeners;
};

#endif /* defined(__PhotonTest__StateAccessor__) */
