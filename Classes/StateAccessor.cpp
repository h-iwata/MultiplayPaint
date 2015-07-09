#include "StateAccessor.h"

NetworkState StateAccessor::getState(void) const
{
    return mState;
}

void StateAccessor::setState(NetworkState newState)
{
    mState = newState;
    for (unsigned int i = 0; i < mStateUpdateListeners.getSize(); i++)
        mStateUpdateListeners[i]->stateUpdate(newState);
}

void StateAccessor::registerForStateUpdates(NetworkLogicListener* listener)
{
    mStateUpdateListeners.addElement(listener);
}