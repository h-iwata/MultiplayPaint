#include "NetworkLogic.h"

static const ExitGames::Common::JString APP_ID = L"5b09cec4-926b-4668-aef7-74292e07c22c"; // set your app id here
static const ExitGames::Common::JString APP_VERSION = L"1.0";
static const ExitGames::Common::JString PLAYER_NAME = "user";
static const bool autoLobbbyStats = true;
static const bool useDefaultRegion = false;

NetworkLogic::NetworkLogic(const ExitGames::LoadBalancing::AuthenticationValues& authenticationValues):
    mLoadBalancingClient(*this, APP_ID, APP_VERSION, PLAYER_NAME+GETTIMEMS(), ExitGames::Photon::ConnectionProtocol::UDP, authenticationValues, autoLobbbyStats, useDefaultRegion)
	, mLastActorNr(0)
	, mLastInput(INPUT_NON)
{
	mStateAccessor.setState(STATE_INITIALIZED);
	mLoadBalancingClient.setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS));
	ExitGames::Common::Base::setListener(this);
	ExitGames::Common::Base::setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS));
}

bool NetworkLogic::isRoomExists(void)
{
    if (mLoadBalancingClient.getRoomList().getIsEmpty()) {
        return false;
    }
    return true;
}

void NetworkLogic::connect(void)
{
    mLoadBalancingClient.connect();
    mStateAccessor.setState(STATE_CONNECTING);
}

void NetworkLogic::disconnect(void)
{
    mLoadBalancingClient.disconnect();
}

NetworkState NetworkLogic::getState(void) const
{
    return mStateAccessor.getState();
}

void NetworkLogic::sendEvent(nByte code, ExitGames::Common::Hashtable* eventContent)
{
    mLoadBalancingClient.opRaiseEvent(true, eventContent, 1, code);
}

void NetworkLogic::registerForStateUpdates(NetworkLogicListener* listener)
{
	mStateAccessor.registerForStateUpdates(listener);
}

void NetworkLogic::opCreateRoom(void)
{
	ExitGames::Common::JString tmp;
	mLoadBalancingClient.opCreateRoom(tmp=GETTIMEMS(), true, true, 4, ExitGames::Common::Hashtable(), ExitGames::Common::JVector<ExitGames::Common::JString>(), ExitGames::Common::JString(), 1, INT_MAX/2, 10000);
    mStateAccessor.setState(STATE_JOINING);
}

void NetworkLogic::opJoinRandomRoom(void)
{
	mLoadBalancingClient.opJoinRandomRoom();
}

void NetworkLogic::run(void)
{
    NetworkState state = mStateAccessor.getState();
    if(mLastInput == INPUT_EXIT && state != STATE_DISCONNECTING && state != STATE_DISCONNECTED) {
		disconnect();
		mStateAccessor.setState(STATE_DISCONNECTING);
	} else {
		switch(state) {
			case STATE_INITIALIZED:
				connect();
				mStateAccessor.setState(STATE_CONNECTING);
				break;
			case STATE_CONNECTING:
				break; // wait for callback
			case STATE_CONNECTED:
                switch(mLastInput) {
                    case INPUT_1: // create Game
                        opCreateRoom();
                        break;
                    case INPUT_2: // join Game
                        // remove false to enable rejoin
                        if(false && mLastJoinedRoom.length()) {
                            mLoadBalancingClient.opJoinRoom(mLastJoinedRoom, true, mLastActorNr);
                        } else {
                            opJoinRandomRoom();
                        }
                        mStateAccessor.setState(STATE_JOINING);
                        break;
                    default: // no or illegal input -> stay waiting for legal input
                        break;
                }
                break;
            case STATE_JOINING:
				break; // wait for callback
			case STATE_JOINED:
                sendEvent();
                switch(mLastInput) {
                    case INPUT_1: // leave Game
                        mLoadBalancingClient.opLeaveRoom();
                        mStateAccessor.setState(STATE_LEAVING);
                        break;
                    case INPUT_2: // leave Game
                        mLoadBalancingClient.opLeaveRoom(true);
                        mStateAccessor.setState(STATE_LEAVING);
                        break;
                    default: // no or illegal input -> stay waiting for legal input
                        break;
                }
                break;
            case STATE_LEAVING:
				break;
			case STATE_LEFT:
				mStateAccessor.setState(STATE_CONNECTED);
				break;
			case STATE_DISCONNECTING:
				break;
			default:
				break;        
		}
	}
	mLastInput = INPUT_NON;
	mLoadBalancingClient.service();
}

void NetworkLogic::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent){
    ExitGames::Common::Hashtable* event;
    
    switch (eventCode) {
        case TOUCH_BEGAN: {
            event = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable*>(eventContent).getDataCopy();
            float x = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            float y = ExitGames::Common::ValueObject<float>(event->getValue(2)).getDataCopy();
            beginEventQueue.push({static_cast<float>(playerNr), x, y});
            break;
        }
        case TOUCH_MOVING: {
            event = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable*>(eventContent).getDataCopy();
            float x = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            float y = ExitGames::Common::ValueObject<float>(event->getValue(2)).getDataCopy();
            eventQueue.push({static_cast<float>(playerNr), x, y});
            break;
        }
        case TAP_RESET:
            tapResetEnventQueue.push(true);
            break;
    }
}

void NetworkLogic::sendEvent(void)
{
	static int64 count = 0;
	mLoadBalancingClient.opRaiseEvent(false, ++count, 0);
}

void NetworkLogic::connectionErrorReturn(int errorCode)
{
    mStateAccessor.setState(STATE_DISCONNECTED);
}



void NetworkLogic::connectReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	if(errorCode) {
        mStateAccessor.setState(STATE_DISCONNECTING);
		return;
	}
	mStateAccessor.setState(STATE_CONNECTED);
}

void NetworkLogic::disconnectReturn(void)
{
    mStateAccessor.setState(STATE_DISCONNECTED);
}

void NetworkLogic::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	if(errorCode)
	{
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	mLastJoinedRoom = mLoadBalancingClient.getCurrentlyJoinedRoom().getName();
	mLastActorNr = localPlayerNr;

    playerNr = localPlayerNr;
    
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	if(errorCode) {
		mLastJoinedRoom = "";
		mLastActorNr = 0;
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	if(errorCode) {
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}

	mLastJoinedRoom = mLoadBalancingClient.getCurrentlyJoinedRoom().getName();
	mLastActorNr = localPlayerNr;
	
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	if(errorCode) {
        mStateAccessor.setState(STATE_DISCONNECTING);
		return;
	}
	mStateAccessor.setState(STATE_LEFT);
}

void NetworkLogic::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers)
{
    mLoadBalancingClient.selectRegion(availableRegions[0]);
}








