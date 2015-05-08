#include <stdarg.h>
#include <stdio.h>

#include "BaseView.h"

#ifdef _EG_ANDROID_PLATFORM
#	include <android/log.h>
#endif

void BaseView::info(const char* format, ...) 
{
	fprintf(stdout, "INFO: ");
	va_list argptr;
    va_start(argptr, format);
    vfprintf(stdout, format, argptr);
    va_end(argptr);
	fprintf(stdout, "\n");
#ifdef _EG_ANDROID_PLATFORM
	__android_log_vprint(ANDROID_LOG_INFO, "DemoMemory", format, argptr);
#endif
}

void BaseView::warn(const char* format, ...) 
{
	fprintf(stderr, "WARN: ");
	va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
	fprintf(stderr, "\n");
#ifdef _EG_ANDROID_PLATFORM
	__android_log_vprint(ANDROID_LOG_INFO, "DemoMemory", format, argptr);
#endif
}

void BaseView::error(const char* format, ...) 
{
	fprintf(stderr, "ERROR: ");
	va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
	fprintf(stderr, "\n");
#ifdef _EG_ANDROID_PLATFORM
	__android_log_vprint(ANDROID_LOG_INFO, "DemoMemory", format, argptr);
#endif
}

void BaseView::onStateChange(int state, const char* stateStr, bool inLobby, bool inRoom) 
{
	info("state: %d/%s", state, stateStr);
}

void BaseView::refreshStateInfo(const char* stateInfo)
{
	info("state info: %s", stateInfo);
}

void BaseView::refreshMessage(const char* msg)
{
	info("game message: %s", msg);
}

void BaseView::setup()
{
	info("setup view");
}

void BaseView::setupBoard(int tileCols, int tileRows)
{
	info("setup board %dx%d", tileCols, tileRows);
}

void BaseView::updateBoard(unsigned char* tiles, unsigned char flippedTiles[2])
{
	info("update board, flips: [%d, %d]", flippedTiles[0], flippedTiles[1]);
}

void BaseView::updateRoomList(const char** roomNames, unsigned int size) 
{
	info("room list:");
	for(unsigned int i=0; i<size; ++i) 
		info("  %s", roomNames[i]);
}

void BaseView::updateSavedRoomList(const char** savedRoomNames, unsigned int size) 
{
	info("saved room list:");
	for(unsigned int i=0; i<size; ++i) 
	{
		info("  %s", savedRoomNames[i]);
	}
}