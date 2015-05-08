/* Exit Games Photon Chat - C++ Client Lib
 * Copyright (C) 2004-2015 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Common-cpp/inc/Common.h"

namespace ExitGames
{
	namespace Chat
	{
		class Channel : public Common::Base
		{
		public:
			using Common::ToString::toString;

			const Common::JString& getName(void) const;
			unsigned int getMessageCount(void) const;
			const Common::JVector<Common::JString>& getSenders() const;
			const Common::JVector<Common::Object>& getMessages() const;

			// TODO: move to internal
			Channel(const Common::JString& name, bool isPrivate);
			void add(const Common::JString& sender, const Common::Object& message);
			void add(const Common::JVector<Common::JString>& senders, const Common::JVector<Common::Object>& messages);
			void clearMessages();
			bool isPrivate();

			virtual Common::JString& toString(Common::JString& retStr, bool withTypes=false) const;
		private:
			Common::JString mName;
			Common::JVector<Common::JString> mSenders;
			Common::JVector<Common::Object> mMessages;
			bool mIsPrivate;
		};
	}
}