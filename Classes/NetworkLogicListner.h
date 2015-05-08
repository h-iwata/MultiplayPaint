#ifndef __PhotonTest__NetworkLogicListner__
#define __PhotonTest__NetworkLogicListner__

#include "StateAccessor.h"
#include "NetworkState.h"

class NetworkLogicListener : public ExitGames::Common::ToString
{
public:
    using ExitGames::Common::ToString::toString;
    virtual void stateUpdate(NetworkState newState) = 0;
    virtual ExitGames::Common::JString& toString(ExitGames::Common::JString& retStr, bool withTypes=false) const;
};

#endif /* defined(__PhotonTest__NetworkLogicListner__) */
