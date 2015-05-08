/* Exit Games Photon Chat - C++ Client Lib
 * Copyright (C) 2004-2015 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Photon-cpp/inc/PhotonPeer.h"
#include "Chat-cpp/inc/AuthenticationValues.h"

namespace ExitGames
{
	namespace Chat
	{
		class Peer : public Photon::PhotonPeer
		{
		public:
			Peer(Photon::PhotonListener& listener, nByte connectionProtocol=Photon::ConnectionProtocol::UDP);
			virtual ~Peer(void);

			virtual bool opAuthenticateOnNameServer(const Common::JString& appID, const Common::JString& appVersion, const Common::JString& region, const Common::JString& userID=Common::JString(), const AuthenticationValues& authenticationValues=AuthenticationValues());
			virtual bool opAuthenticateOnFrontEnd(const Common::JString& secret);

			virtual bool opSubscribe(const Common::JVector<Common::JString>& channels, int messagesFromHistory);
			virtual bool opUnsubscribe(const Common::JVector<Common::JString>& channels);
			virtual bool opPublishMessage(const Common::JString& channelName, const Common::Object& message);
			virtual bool opSendPrivateMessage(const Common::JString& target, const Common::Object& message, bool encrypt=false);
			virtual bool opSetOnlineStatus(int status, const Common::Object& message, bool skipMessage);
			virtual bool opAddFriends(const Common::JVector<Common::JString>& userIds);
			virtual bool opRemoveFriends(const Common::JVector<Common::JString>& userIds);
		private:
			bool sendChannelOperation(const Common::JVector<Common::JString>& channels, nByte operation, int messagesFromHistory=0);
			typedef PhotonPeer super;
		};
	}
}