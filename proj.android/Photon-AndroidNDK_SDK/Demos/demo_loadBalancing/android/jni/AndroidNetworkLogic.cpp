#include <string.h>
#include <android/log.h>

#include "AndroidNetworkLogic.h"

#define APP_TAG "demo-loadBalancing"

AndroidNetworkLogic::AndroidNetworkLogic(JNIEnv* env, jobject obj, jmethodID writeMethod, jmethodID stateUpdateMethod)
	: NetworkLogic(this)
	, mEnv(env)
	, mObj(obj)
	, mWriteMethod(writeMethod)
	, mStateUpdateMethod(stateUpdateMethod)
{
	registerForStateUpdates(this);
}

void AndroidNetworkLogic::write(const ExitGames::Common::JString& str)
{
	write(str.UTF8Representation().cstr());
}

void AndroidNetworkLogic::writeLine(const ExitGames::Common::JString& str)
{
	ExitGames::Common::JString string(str + '\n');
	write(string.UTF8Representation().cstr());
}

void AndroidNetworkLogic::write(const char* str)
{
	__android_log_print(ANDROID_LOG_INFO, APP_TAG, "%s", str);
	jstring jstr = mEnv->NewStringUTF(str);
	mEnv->CallVoidMethod(mObj, mWriteMethod, jstr);
	mEnv->DeleteLocalRef(jstr);
}

void AndroidNetworkLogic::debugReturn(const ExitGames::Common::JString& string)
{
	__android_log_print(ANDROID_LOG_INFO, APP_TAG, "%s", string.UTF8Representation().cstr());
}

void AndroidNetworkLogic::stateUpdate(State newState)
{
	mEnv->CallVoidMethod(mObj, mStateUpdateMethod, newState);
}