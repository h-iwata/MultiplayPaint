/* Exit Games Photon - C++ Client Lib
 * Copyright (C) 2004-2015 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Common-cpp/inc/BaseListener.h"

namespace ExitGames
{
	namespace Common
	{
		class Logger
		{
		public:
			Logger(DebugLevel::DebugLevel debugLevel=DebugLevel::WARNINGS);
			void log(DebugLevel::DebugLevel debugLevel, const EG_CHAR* const file, const EG_CHAR* const function, bool printBrackets, unsigned int line, const EG_CHAR* const dbgMsg, ...) const;
			void vlog(DebugLevel::DebugLevel debugLevel, const EG_CHAR* const file, const EG_CHAR* const function, bool printBrackets, unsigned int line, const EG_CHAR* const dbgMsg, va_list args) const;
			DebugLevel::DebugLevel getDebugOutputLevel(void) const;
			bool setDebugOutputLevel(DebugLevel::DebugLevel debugLevel);
			void setListener(const BaseListener& listener);
		private:
			DebugLevel::DebugLevel mDebugLevel;
#ifdef EG_LOGGING
			BaseListener* mpListener;
#endif
		};
	}
}

// public logging API
#ifdef EG_LOGGING
#	define EGLOG(debugLevel, ...) mLogger.log(debugLevel, __WFILE__, __WFUNCTION__, true, __LINE__, __VA_ARGS__)
#else
#	define EGLOG(debugLevel, ...) ((void)0)
#endif