#ifndef __ANDROID_NETWORK_LOGIC_H
#define __ANDROID_NETWORK_LOGIC_H

#include <jni.h>

#include "NetworkLogic.h"

class AndroidNetworkLogic: public NetworkLogic, public OutputListener, public NetworkLogicListener
{
public:
	AndroidNetworkLogic(JNIEnv* env, jobject obj, jmethodID writeMethod, jmethodID stateUpdateMethod);

	virtual void write(const ExitGames::Common::JString& str);
	virtual void writeLine(const ExitGames::Common::JString& str);
	virtual void debugReturn(const ExitGames::Common::JString& string);
	virtual void stateUpdate(State newState);
private:
	void write(const char* str);

	JNIEnv* mEnv;
	jobject mObj;
	jmethodID mWriteMethod;
	jmethodID mStateUpdateMethod;
};

#endif