#include "NetworkListener.h"
#include "NetworkEvent.h"
#include "DataType.h"

static const Common::JString APP_ID = L"5b09cec4-926b-4668-aef7-74292e07c22c"; // set your app id here
static const Common::JString APP_VERSION = L"1.0";
static const Common::JString PLAYER_NAME = "user";
static const bool autoLobbbyStats = true;
static const bool useDefaultRegion = false;

NetworkListener::NetworkListener(const LoadBalancing::AuthenticationValues& authenticationValues):
    mLoadBalancingClient(*this,
                         APP_ID,
                         APP_VERSION,
                         PLAYER_NAME + GETTIMEMS(),
                         Photon::ConnectionProtocol::UDP,
                         authenticationValues,
                         autoLobbbyStats,
                         useDefaultRegion)
    , mLastActorNr(0)
    , mLastInput(INPUT_NON)
{
    mStateAccessor.setState(STATE_INITIALIZED);
    mLoadBalancingClient.setDebugOutputLevel(DEBUG_RELEASE(Common::DebugLevel::INFO, Common::DebugLevel::WARNINGS));
    Common::Base::setListener(this);
    Common::Base::setDebugOutputLevel(DEBUG_RELEASE(Common::DebugLevel::INFO, Common::DebugLevel::WARNINGS));
}

void NetworkListener::run(void)
{
    NetworkState state = mStateAccessor.getState();
    if (mLastInput == INPUT_EXIT &&
        state != STATE_DISCONNECTING &&
        state != STATE_DISCONNECTED) {
        disconnect();
        mStateAccessor.setState(STATE_DISCONNECTING);
    } else {
        switch (state) {
        case STATE_INITIALIZED:
            connect();
            mStateAccessor.setState(STATE_CONNECTING);
            break;
        case STATE_CONNECTED:
            switch (mLastInput) {
            case INPUT_CREATE_GAME:
                opCreateRoom();
                break;
            case INPUT_JOIN_GAME:
                // remove false to enable rejoin
                if (false && mLastJoinedRoom.length()) {
                    mLoadBalancingClient.opJoinRoom(mLastJoinedRoom, true, mLastActorNr);
                } else {
                    opJoinRandomRoom();
                }
                mStateAccessor.setState(STATE_JOINING);
                break;
            default:
                break;
            }
            break;
        case STATE_JOINED:
            sendEvent();
            switch (mLastInput) {
            case INPUT_CREATE_GAME:
                mLoadBalancingClient.opLeaveRoom();
                mStateAccessor.setState(STATE_LEAVING);
                break;
            case INPUT_JOIN_GAME:
                mLoadBalancingClient.opLeaveRoom(true);
                mStateAccessor.setState(STATE_LEAVING);
                break;
            default:
                break;
            }
            break;
        case STATE_LEFT:
            mStateAccessor.setState(STATE_CONNECTED);
            break;
        case STATE_DISCONNECTED:
            connect();
        case STATE_CONNECTING:
        case STATE_JOINING:
        case STATE_LEAVING:
        case STATE_DISCONNECTING:
            break;
        default:
            break;
        }
    }
    mLastInput = INPUT_NON;
    mLoadBalancingClient.service();
}

void NetworkListener::connect(void)
{
    mLoadBalancingClient.connect();
    mStateAccessor.setState(STATE_CONNECTING);
}

bool NetworkListener::isRoomExists(void)
{
    if (mLoadBalancingClient.getRoomList().getIsEmpty()) {
        return false;
    }
    return true;
}

void NetworkListener::disconnect(void)
{
    mLoadBalancingClient.disconnect();
}

NetworkState NetworkListener::getState(void) const
{
    return mStateAccessor.getState();
}

void NetworkListener::sendEvent(nByte code, Common::Hashtable* eventContent)
{
    mLoadBalancingClient.opRaiseEvent(true, eventContent, 1, code);
}

void NetworkListener::registerForStateUpdates(NetworkLogicListener* listener)
{
    mStateAccessor.registerForStateUpdates(listener);
}

void NetworkListener::opCreateRoom(void)
{
    Common::JString tmp;
    mLoadBalancingClient.opCreateRoom(tmp = GETTIMEMS(), true, true, 4, Common::Hashtable(), Common::JVector<Common::JString>(), Common::JString(), 1, INT_MAX / 2, 10000);
    mStateAccessor.setState(STATE_JOINING);
}

void NetworkListener::opJoinRandomRoom(void)
{
    mLoadBalancingClient.opJoinRandomRoom();
}

void NetworkListener::customEventAction(int playerNr, nByte eventCode, const Common::Object& eventContent)
{
    Common::Hashtable* event;

    switch (eventCode) {
    case TOUCH_BEGAN: {
        event = Common::ValueObject<Common::Hashtable*>(eventContent).getDataCopy();
        float x = Common::ValueObject<float>(event->getValue(static_cast<int>(DataType::LOCATION_X))).getDataCopy();
        float y = Common::ValueObject<float>(event->getValue(static_cast<int>(DataType::LOCATION_Y))).getDataCopy();
        beginEventQueue.push({ static_cast<float>(playerNr), x, y });
        break;
    }

    case TOUCH_MOVING: {
        event = Common::ValueObject<Common::Hashtable*>(eventContent).getDataCopy();
        float x = Common::ValueObject<float>(event->getValue(static_cast<int>(DataType::LOCATION_X))).getDataCopy();
        float y = Common::ValueObject<float>(event->getValue(static_cast<int>(DataType::LOCATION_Y))).getDataCopy();
        movingEventQueue.push({ static_cast<float>(playerNr), x, y });
        break;
    }

    case TAP_RESET:
        tapResetEnventQueue.push(true);
        break;
    }
}

void NetworkListener::sendEvent(void)
{
    static int64 count = 0;
    mLoadBalancingClient.opRaiseEvent(false, ++count, 0);
}

void NetworkListener::connectionErrorReturn(int errorCode)
{
    mStateAccessor.setState(STATE_DISCONNECTED);
}

void NetworkListener::connectReturn(int errorCode, const Common::JString& errorString)
{
    if (errorCode) {
        mStateAccessor.setState(STATE_DISCONNECTING);
        return;
    }
    mStateAccessor.setState(STATE_CONNECTED);
}

void NetworkListener::disconnectReturn(void)
{
    mStateAccessor.setState(STATE_DISCONNECTED);
}

void NetworkListener::createRoomReturn(int localPlayerNr, const Common::Hashtable& /*gameProperties*/, const Common::Hashtable& /*playerProperties*/, int errorCode, const Common::JString& errorString)
{
    if (errorCode) {
        mStateAccessor.setState(STATE_CONNECTED);
        return;
    }
    mLastJoinedRoom = mLoadBalancingClient.getCurrentlyJoinedRoom().getName();
    mLastActorNr = localPlayerNr;

    playerNr = localPlayerNr;

    mStateAccessor.setState(STATE_JOINED);
}

void NetworkListener::joinRoomReturn(int localPlayerNr, const Common::Hashtable& /*gameProperties*/, const Common::Hashtable& /*playerProperties*/, int errorCode, const Common::JString& errorString)
{
    if (errorCode) {
        mLastJoinedRoom = "";
        mLastActorNr = 0;
        mStateAccessor.setState(STATE_CONNECTED);
        return;
    }
    mStateAccessor.setState(STATE_JOINED);
}

void NetworkListener::joinRandomRoomReturn(int localPlayerNr, const Common::Hashtable& /*gameProperties*/, const Common::Hashtable& /*playerProperties*/, int errorCode, const Common::JString& errorString)
{
    if (errorCode) {
        mStateAccessor.setState(STATE_CONNECTED);
        return;
    }

    mLastJoinedRoom = mLoadBalancingClient.getCurrentlyJoinedRoom().getName();
    mLastActorNr = localPlayerNr;

    mStateAccessor.setState(STATE_JOINED);
}

void NetworkListener::leaveRoomReturn(int errorCode, const Common::JString& errorString)
{
    if (errorCode) {
        mStateAccessor.setState(STATE_DISCONNECTING);
        return;
    }
    mStateAccessor.setState(STATE_LEFT);
}

void NetworkListener::onAvailableRegions(const Common::JVector<Common::JString>& availableRegions, const Common::JVector<Common::JString>& availableRegionServers)
{
    mLoadBalancingClient.selectRegion(availableRegions[0]);
}
