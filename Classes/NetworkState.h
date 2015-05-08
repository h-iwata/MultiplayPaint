#ifndef __PhotonTest__NetworkState__
#define __PhotonTest__NetworkState__

enum NetworkState
{
    STATE_INITIALIZED = 0,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_JOINING,
    STATE_JOINED,
    STATE_LEAVING,
    STATE_LEFT,
    STATE_DISCONNECTING,
    STATE_DISCONNECTED
};

enum NetworkEvent
{
    TOUCH_MOVING = 1,
    TOUCH_BEGAN,
    TAP_RESET,
};


#endif /* defined(__PhotonTest__NetworkState__) */
