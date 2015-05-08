#ifndef __DEMO_CONSTANTS_H
#define __DEMO_CONSTANTS_H

static const char* APP_ID = "<no-app-id>";
static const char* APP_VERSION = "1.0";

static const char* SUBSCRIBE_CHANNELS[] = {"a", "b", "c"};
const int USER_COUNT = 1000;
static const ExitGames::Common::JString USER_PREFIX = L"user";
static const ExitGames::Common::JString DEFAULT_USER_ID = USER_PREFIX+abs(GETTIMEMS())%USER_COUNT;

#endif