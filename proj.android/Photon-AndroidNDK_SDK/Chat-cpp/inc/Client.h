/* Exit Games Photon Chat - C++ Client Lib
 * Copyright (C) 2004-2015 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Chat-cpp/inc/Channel.h"
#include "Chat-cpp/inc/Listener.h"
#include "Chat-cpp/inc/Peer.h"
#include "Chat-cpp/inc/Enums/ClientState.h"
#include "Chat-cpp/inc/Enums/DisconnectCause.h"
#include "Chat-cpp/inc/Enums/ErrorCode.h"
#include "Chat-cpp/inc/Enums/UserStatus.h"

namespace ExitGames
{
	namespace Chat
	{
		class Client : protected Photon::PhotonListener
		{
		public:
			Client(Listener& listener, const Common::JString& applicationID, const Common::JString& appVersion, const Common::JString& userId, nByte connectionProtocol=Photon::ConnectionProtocol::UDP, AuthenticationValues authenticationValues=AuthenticationValues());
			virtual ~Client(void);

			virtual bool connect(const Common::JString& nameServerAddress=NAMESERVER);
			virtual void disconnect(void);

			virtual void service(bool dispatchIncomingCommands=true);
			virtual void serviceBasic(void);
			virtual bool sendOutgoingCommands(void);
			virtual bool sendAcksOnly(void);
			virtual bool dispatchIncomingCommands(void);
			virtual void fetchServerTimestamp(void);
			virtual void resetTrafficStats(void);
			virtual void resetTrafficStatsMaximumCounters(void);
			virtual Common::JString vitalStatsToString(bool all) const;

			bool opSubscribe(const Common::JVector<Common::JString>& channels, int messagesFromHistory=0);
			bool opUnsubscribe(const Common::JVector<Common::JString>& channels);
			bool opPublishMessage(const Common::JString& channelName, const Common::Object& message);
			bool opSendPrivateMessage(const Common::JString& userName, const Common::Object& message, bool encrypt = false);
			bool opSetOnlineStatus(int status, const Common::Object& message = ExitGames::Common::Object(), bool skipMessage = false);
			bool opAddFriends(const Common::JVector<Common::JString>& userIds);
			bool opRemoveFriends(const Common::JVector<Common::JString>& userIds);

			int getServerTimeOffset(void) const;
			int getServerTime(void) const;
			int getBytesOut(void) const;
			int getBytesIn(void) const;
			int getByteCountCurrentDispatch(void) const;
			int getByteCountLastOperation(void) const;
			int getSentCountAllowance(void) const;
			void setSentCountAllowance(int setSentCountAllowance);
			int getTimePingInterval(void) const;
			void setTimePingInterval(int setTimePingInterval);
			int getRoundTripTime(void) const;
			int getRoundTripTimeVariance(void) const;
			int getTimestampOfLastSocketReceive(void) const;
			Common::DebugLevel::DebugLevel getDebugOutputLevel(void) const;
			bool setDebugOutputLevel(Common::DebugLevel::DebugLevel debugLevel);
			int getIncomingReliableCommandsCount(void) const;
			short getPeerId(void) const;
			int getDisconnectTimeout(void) const;
			void setDisconnectTimeout(int disconnectTimeout);
			int getQueuedIncomingCommands(void) const;
			int getQueuedOutgoingCommands(void) const;
			bool getIsEncryptionAvailable(void) const;
			int getResentReliableCommands(void) const;
			int getLimitOfUnreliableCommands(void) const;
			void setLimitOfUnreliableCommands(int value);
			bool getCrcEnabled(void) const;
			void setCrcEnabled(bool crcEnabled);
			int getPacketLossByCrc(void) const;
			bool getTrafficStatsEnabled(void) const;
			void setTrafficStatsEnabled(bool trafficStatsEnabled);
			int getTrafficStatsElapsedMs(void) const;
			const Photon::TrafficStats& getTrafficStatsIncoming(void) const;
			const Photon::TrafficStats& getTrafficStatsOutgoing(void) const;
			const Photon::TrafficStatsGameLevel& getTrafficStatsGameLevel(void) const;

			static short getPeerCount(void);

			const Common::JString& getUserId();
			void setUserId(const Common::JString& userId);
			ClientState::ClientState getState(void) const;
			int getDisconnectedCause(void) const;
			const Common::JString& getRegion(void) const;
			void setRegion(const Common::JString& region);
			const Common::JVector<Channel>& getPublicChannels(void) const;
			const Common::JVector<Channel>& getPrivateChannels(void) const;
			const Channel* getPublicChannel(const Common::JString& channelName) const;
			const Channel* getPrivateChannel(const Common::JString& userName) const;
		protected:
			// From Photon::PhotonListener
			virtual void onOperationResponse(const Photon::OperationResponse& operationResponse);
			virtual void onStatusChanged(int statusCode);
			virtual void onEvent(const Photon::EventData& eventData);
			virtual void debugReturn(Common::DebugLevel::DebugLevel debugLevel, const Common::JString& string);
		private:
			void handleConnectionFlowError(ClientState::ClientState oldState, int errorCode, const Common::JString& errorString);
			void handleAuthResponse(const Photon::OperationResponse& operationResponse);
			void handleSubscribeResponse(const Photon::EventData& operationResponse);
			void handleUnsubscribeResponse(const Photon::EventData& operationResponse);

			Peer mPeer;
			Listener& mListener;
			Common::Logger mLogger;
			Common::JString mAppVersion;
			Common::JString mAppID;
			Common::JString mUserId;
			Common::JString mFrontEndAddress;
			ClientState::ClientState mState;
			AuthenticationValues mAuthenticationValues;
			int mDisconnectedCause;
			bool mDidAuthenticate;
			Common::JString mRegion;

			Common::JVector<Channel> mPublicChannels;
			Common::JVector<Channel> mPrivateChannels;

			const nByte M_CONNECTION_PROTOCOL;

			static const Common::JString NAMESERVER;

			Channel* getChannel(Common::JVector<Channel>& channels, const Common::JString& name);
			Channel* getOrAddChannel(Common::JVector<Channel>& channels, const Common::JString& name, bool isPrivate);
			void removeChannel(Common::JVector<Channel>& channels, const Common::JString& name);
		};
	}
}