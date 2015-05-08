#include <jni.h>

#include "AndroidNetworkLogic.h"

extern "C"
{
	JNIEXPORT void JNICALL Java_demo_android_loadBalancing_DemoActivity_startDemo(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_demo_android_loadBalancing_DemoActivity_createGame(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_demo_android_loadBalancing_DemoActivity_joinRandomGame(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_demo_android_loadBalancing_DemoActivity_leaveGame(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_demo_android_loadBalancing_DemoActivity_gameRun(JNIEnv* env, jobject obj);
};

AndroidNetworkLogic* gGameLogic = NULL;

JNIEXPORT void JNICALL Java_demo_android_loadBalancing_DemoActivity_startDemo(JNIEnv* env, jobject obj)
{
	jclass clazz = env->FindClass("demo/android/loadBalancing/DemoActivity");
	jmethodID addTextMethod = env->GetMethodID(clazz, "addText", "(Ljava/lang/String;)V");
	jmethodID stateUpdateMethod = env->GetMethodID(clazz, "stateUpdate", "(I)V");

	gGameLogic = new AndroidNetworkLogic(env, obj, addTextMethod, stateUpdateMethod);
}

JNIEXPORT void JNICALL Java_demo_android_loadBalancing_DemoActivity_createGame(JNIEnv* env, jobject obj)
{
	gGameLogic->setLastInput(INPUT_1);
}

JNIEXPORT void JNICALL Java_demo_android_loadBalancing_DemoActivity_joinRandomGame(JNIEnv* env, jobject obj)
{
	gGameLogic->setLastInput(INPUT_2);
}

JNIEXPORT void JNICALL Java_demo_android_loadBalancing_DemoActivity_leaveGame(JNIEnv* env, jobject obj)
{
	gGameLogic->setLastInput(INPUT_1);
}

JNIEXPORT void JNICALL Java_demo_android_loadBalancing_DemoActivity_gameRun(JNIEnv* env, jobject obj)
{
	gGameLogic->run();
}