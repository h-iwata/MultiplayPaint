/* Exit Games Photon Chat - C++ Client Lib
 * Copyright (C) 2004-2015 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "Chat-cpp/inc/Client.h"
#include "Chat-cpp/inc/Enums/DisconnectCause.h"
#include "Chat-cpp/inc/Enums/ErrorCode.h"
#include "Chat-cpp/inc/Internal/AuthenticationValuesSecretSetter.h"
#include "Chat-cpp/inc/Internal/Enums/EventCode.h"
#include "Chat-cpp/inc/Internal/Enums/ParameterCode.h"
#include "Chat-cpp/inc/Internal/Enums/OperationCode.h"
#include "Photon-cpp/inc/Enums/PeerState.h"

/** @file Chat-cpp/inc/Client.h */

namespace ExitGames
{
	namespace Chat
	{
		using namespace Common;
		using namespace Common::MemoryManagement;
		using namespace Photon;
		using namespace Internal;

		const JString Client::NAMESERVER = L"ns.exitgamescloud.com"; // default name server address
		const EG_CHAR* const REGION = L"EU"; // default region

		/** @copydoc PhotonPeer::getServerTimeOffset */ 
		int Client::getServerTimeOffset(void) const
		{
			return mPeer.getServerTimeOffset();
		}

		/** @copydoc PhotonPeer::getServerTime */ 
		int Client::getServerTime(void) const
		{
			return mPeer.getServerTime();
		}

		/** @copydoc PhotonPeer::getBytesOut */ 
		int Client::getBytesOut(void) const
		{
			return mPeer.getBytesOut();
		}

		/** @copydoc PhotonPeer::getBytesIn */ 
		int Client::getBytesIn(void) const
		{
			return mPeer.getBytesIn();
		}

		/** @copydoc PhotonPeer::getByteCountCurrentDispatch */ 
		int Client::getByteCountCurrentDispatch(void) const
		{
			return mPeer.getByteCountCurrentDispatch();
		}

		/** @copydoc PhotonPeer::getByteCountLastOperation */ 
		int Client::getByteCountLastOperation(void) const
		{
			return mPeer.getByteCountLastOperation();
		}

		/** @copydoc PhotonPeer::getSentCountAllowance */ 
		int Client::getSentCountAllowance(void) const
		{
			return mPeer.getSentCountAllowance();
		}

		/** @copydoc PhotonPeer::setSentCountAllowance */ 
		void Client::setSentCountAllowance(int setSentCountAllowance)
		{
			mPeer.setSentCountAllowance(setSentCountAllowance);
		}

		/** @copydoc PhotonPeer::getTimePingInterval */ 
		int Client::getTimePingInterval(void) const
		{
			return mPeer.getTimePingInterval();
		}

		/** @copydoc PhotonPeer::setTimePingInterval */ 
		void Client::setTimePingInterval(int setTimePingInterval)
		{
			mPeer.setTimePingInterval(setTimePingInterval);
		}

		/** @copydoc PhotonPeer::getRoundTripTime */ 
		int Client::getRoundTripTime(void) const
		{
			return mPeer.getRoundTripTime();
		}

		/** @copydoc PhotonPeer::getRoundTripTimeVariance */ 
		int Client::getRoundTripTimeVariance(void) const
		{
			return mPeer.getRoundTripTimeVariance();
		}

		/** @copydoc PhotonPeer::getTimestampOfLastSocketReceive */ 
		int Client::getTimestampOfLastSocketReceive(void) const
		{
			return mPeer.getTimestampOfLastSocketReceive();
		}

		/** @copydoc PhotonPeer::getDebugOutputLevel */ 
		Common::DebugLevel::DebugLevel Client::getDebugOutputLevel(void) const
		{
			return mPeer.getDebugOutputLevel();
		}

		/** @copydoc PhotonPeer::setDebugOutputLevel */ 
		bool Client::setDebugOutputLevel(Common::DebugLevel::DebugLevel debugLevel)
		{
			return mLogger.setDebugOutputLevel(debugLevel) && mPeer.setDebugOutputLevel(debugLevel);
		}

		/** @copydoc PhotonPeer::getIncomingReliableCommandsCount */ 
		int Client::getIncomingReliableCommandsCount(void) const
		{
			return mPeer.getIncomingReliableCommandsCount();
		}

		/** @copydoc PhotonPeer::getPeerId */ 
		short Client::getPeerId(void) const
		{
			return mPeer.getPeerId();
		}

		/** @copydoc PhotonPeer::getDisconnectTimeout */ 
		int Client::getDisconnectTimeout(void) const
		{
			return mPeer.getDisconnectTimeout();
		}

		/** @copydoc PhotonPeer::setDisconnectTimeout */ 
		void Client::setDisconnectTimeout(int disconnectTimeout)
		{
			mPeer.setDisconnectTimeout(disconnectTimeout);
		}

		/** @copydoc PhotonPeer::getQueuedIncomingCommands */ 
		int Client::getQueuedIncomingCommands(void) const
		{
			return mPeer.getQueuedIncomingCommands();
		}

		/** @copydoc PhotonPeer::getQueuedOutgoingCommands */ 
		int Client::getQueuedOutgoingCommands(void) const
		{
			return mPeer.getQueuedOutgoingCommands();
		}

		/** @copydoc PhotonPeer::getIsEncryptionAvailable */ 
		bool Client::getIsEncryptionAvailable(void) const
		{
			return mPeer.getIsEncryptionAvailable();
		}

		/** @copydoc PhotonPeer::getResentReliableCommands */ 
		int Client::getResentReliableCommands(void) const
		{
			return mPeer.getResentReliableCommands();
		}

		/** @copydoc PhotonPeer::getLimitOfUnreliableCommands */ 
		int Client::getLimitOfUnreliableCommands(void) const
		{
			return mPeer.getLimitOfUnreliableCommands();
		}

		/** @copydoc PhotonPeer::setLimitOfUnreliableCommands */ 
		void Client::setLimitOfUnreliableCommands(int value)
		{
			mPeer.setLimitOfUnreliableCommands(value);
		}

		/** @copydoc PhotonPeer::getCrcEnabled */ 
		bool Client::getCrcEnabled(void) const
		{
			return mPeer.getCrcEnabled();
		}

		/** @copydoc PhotonPeer::setCrcEnabled */ 
		void Client::setCrcEnabled(bool crcEnabled)
		{
			mPeer.setCrcEnabled(crcEnabled);
		}

		/** @copydoc PhotonPeer::getPacketLossByCrc */ 
		int Client::getPacketLossByCrc(void) const
		{
			return mPeer.getPacketLossByCrc();
		}

		/** @copydoc PhotonPeer::getTrafficStatsEnabled */ 
		bool Client::getTrafficStatsEnabled(void) const
		{
			return mPeer.getTrafficStatsEnabled();
		}

		/** @copydoc PhotonPeer::setTrafficStatsEnabled */ 
		void Client::setTrafficStatsEnabled(bool trafficStatsEnabled)
		{
			mPeer.setTrafficStatsEnabled(trafficStatsEnabled);
		}

		/** @copydoc PhotonPeer::getTrafficStatsElapsedMs */ 
		int Client::getTrafficStatsElapsedMs(void) const
		{
			return mPeer.getTrafficStatsElapsedMs();
		}

		/** @copydoc PhotonPeer::getTrafficStatsIncoming */ 
		const Photon::TrafficStats& Client::getTrafficStatsIncoming(void) const
		{
			return mPeer.getTrafficStatsIncoming();
		}

		/** @copydoc PhotonPeer::getTrafficStatsOutgoing */ 
		const Photon::TrafficStats& Client::getTrafficStatsOutgoing(void) const
		{
			return mPeer.getTrafficStatsOutgoing();
		}

		/** @copydoc PhotonPeer::getTrafficStatsGameLEvel */ 
		const Photon::TrafficStatsGameLevel& Client::getTrafficStatsGameLevel(void) const
		{
			return mPeer.getTrafficStatsGameLevel();
		}

		/** @copydoc PhotonPeer::getTrafficStatsPeerCount */ 
		short Client::getPeerCount(void)
		{
			return Peer::getPeerCount();
		}

		/**
		   @class Client
		   Central class of the %Photon %Chat API to connect, handle channels and messages.
		   @details
		   This class must be instantiated with a Chat::Listener instance to get the callbacks and
		   with application id that is setup as %Photon %Chat application.
		   Integrate it into your game loop by calling service() regularly.
		   Call connect() with an Name Server address. Note: Connect covers multiple
		   messages between this client and the servers. A short workflow will connect you to a %Chat server.
		   Each Chat::Client resembles a user in chat. Before you send messages in any public channel, that channel must be subscribed.
		   Private channels represent private chats and created automatically on private message sent or received.
		   getPublicChannels() returns list of subscribed channels, containing messages and senders.
		   getPrivateChannels() contains all incoming and sent private messages.*/
		
		/**
		   Constructor.
		   @param listener pointer to the application's implementation of the Chat:Listener callback interface.
		   @param applicationID %Photon %Chat application id
		   @param appVersion %Photon %Chat application version
		   @param userId name of user in chat
		   @param connectionProtocol connection protocol
		   @param authenticationValues a user's authentication values used during connect for Custom Authentication with %Photon.*/
		Client::Client(Listener& listener, const JString& applicationID, const JString& appVersion, const JString& username, nByte connectionProtocol, AuthenticationValues authenticationValues)
#if defined _EG_MS_COMPILER
#	pragma warning(push)
#	pragma warning(disable:4355)
#endif
			: mPeer(*this, connectionProtocol)
			, mListener(listener)
			, mAppVersion(appVersion)
			, mAppID(applicationID)
			, mUserId(username)
			, mState(ClientState::Uninitialized)
			, mAuthenticationValues(authenticationValues)
			, mDisconnectedCause(DisconnectCause::NONE)
			, mDidAuthenticate(false)
			, mRegion(REGION)
			, M_CONNECTION_PROTOCOL(connectionProtocol)
#ifdef _EG_MS_COMPILER
#	pragma warning(pop)
#endif
		{
			mLogger.setListener(listener);
		}
		
		/**
		   Destructor.*/
		Client::~Client(void)
		{
		}
		
		/***/
	
		/**
		   Initiates a connection to the %Photon name server. After a successful connection the client automatically connects to a chat front end server
		   and goes to ConnectedToFrontEnd state. After that the client can subscribe to channels and send and receive messages.
		   @param nameServerAdress used to specify a name server address different from the public %Photon Cloud name server*/
		bool Client::connect(const JString& nameServerAddress)
		{
			mPublicChannels.removeAllElements();
			mPrivateChannels.removeAllElements();
			mDidAuthenticate = false;

			mState = ClientState::ConnectingToNameServer;
			static const int NAMESERVER_PORT_GAP = 3;
			if(mPeer.connect(nameServerAddress + (nameServerAddress.indexOf(L':') == -1?JString(L":")+((M_CONNECTION_PROTOCOL==ConnectionProtocol::UDP?NetworkPort::UDP:NetworkPort::TCP)+NAMESERVER_PORT_GAP):JString())))
			{
				mState = ClientState::ConnectingToNameServer;
				mListener.onStateChange(mState);
				return true;
			}
			else
				return false;
		}
		
		/**
		   Disconnects from servers.*/
		void Client::disconnect(void)
		{
			mListener.onStateChange(mState=ClientState::Disconnecting);
			mPeer.disconnect();
		}

		/** @copydoc PhotonPeer::service */ 
		void Client::service(bool dispatchIncomingCommands)
		{
			mPeer.service(dispatchIncomingCommands);
		}

		/** @copydoc PhotonPeer::serviceBasic */ 
		void Client::serviceBasic(void)
		{
			mPeer.serviceBasic();
		}

		/** @copydoc PhotonPeer::sendOutgoingCommands */ 
		bool Client::sendOutgoingCommands(void)
		{
			return mPeer.sendOutgoingCommands();
		}

		/** @copydoc PhotonPeer::sendAcksOnly */ 
		bool Client::sendAcksOnly(void)
		{
			return mPeer.sendAcksOnly();
		}

		/** @copydoc PhotonPeer::dispatchIncomingCommands */ 
		bool Client::dispatchIncomingCommands(void)
		{
			return mPeer.dispatchIncomingCommands();
		}

		/** @copydoc PhotonPeer::fetchServerTimestamp */ 
		void Client::fetchServerTimestamp(void)
		{
			mPeer.fetchServerTimestamp();
		}

		/** @copydoc PhotonPeer::resetTrafficStats */ 
		void Client::resetTrafficStats(void)
		{
			mPeer.resetTrafficStats();
		}

		/** @copydoc PhotonPeer::resetTrafficStatsMaximumCounters */
		void Client::resetTrafficStatsMaximumCounters(void)
		{
			mPeer.resetTrafficStatsMaximumCounters();
		}

		/** @copydoc PhotonPeer::vitalStatsToString */
		Common::JString Client::vitalStatsToString(bool all) const
		{
			return mPeer.vitalStatsToString(all);
		}
		
			/**
				Sends request to subscribe client to channels, optionally fetching messages newer than a specific ID.
				@param channels list of channels to subscribe to.
				@param messagesFromHistory 0: no history. 1 and higher: number of messages in history. -1: all history.
				@returns true if request sent*/
		bool Client::opSubscribe(const JVector<JString>& channels, int messagesFromHistory)
		{
			if(mState != ClientState::ConnectedToFrontEnd)
			{
				EGLOG(DebugLevel::ERRORS, L"opSubscribe: client in wrong state: %d", mState);
				return false;
			}
			else
				return mPeer.opSubscribe(channels, messagesFromHistory);
		}
		
			/**
				Unsubscribes from a list of channels, which stops getting the messages from those.
				@details
				The client will remove these channels from the PublicChannels dictionary immediately, if it
				could send the operation.
				@param channels list of channels to unsubscribe from.
				@returns true if request sent and channels removed*/
		bool Client::opUnsubscribe(const JVector<JString>& channels)
		{
			if(mState != ClientState::ConnectedToFrontEnd)
			{
				EGLOG(DebugLevel::ERRORS, L"opUnsubscribe: client in wrong state: %d", mState);
				return false;
			}
			else
				return mPeer.opUnsubscribe(channels);
		}
		
			/**
				Sends message to public channels.
				@param channelName channel Name
				@param message message to send
				@returns true if message sent*/
		bool Client::opPublishMessage(const JString& channelName, const Object& message)
		{
			if(mState != ClientState::ConnectedToFrontEnd)
			{
				EGLOG(DebugLevel::ERRORS, L"opPublishMessage: client in wrong state: %d", mState);
				return false;
			}
			else
				return mPeer.opPublishMessage(channelName, message);
		}
		
			/**
				Sends private message and creates private channel if it not exists.
				@param userName user name
				@param message message to send
				@param encrypt encrypt sending message
				@returns true if message sent*/
		bool Client::opSendPrivateMessage(const JString& userName, const Object& message, bool encrypt)
		{
			if(mState != ClientState::ConnectedToFrontEnd)
			{
				EGLOG(DebugLevel::ERRORS, L"opSendPrivateMessage: client in wrong state: %d", mState);
				return false;
			}
			return mPeer.opSendPrivateMessage(userName, message, encrypt);
		}
		
			/**
				Sets the user's status (pre-defined or custom) and an optional message.
				The predefined status values can be found in class UserStatus.
				State UserStatus.Invisible will make you offline for everyone and send no message.
				@param status predefined states are in class UserStatus. Other values can be used at will
				@param message optional string message or null
				@param skipMessage If true, the message gets ignored. It can be null but won't replace any current message.
				@returns true if command sent*/
		bool Client::opSetOnlineStatus(int status, const Common::Object& message, bool skipMessage)
		{
			if(mState != ClientState::ConnectedToFrontEnd)
			{
				EGLOG(DebugLevel::ERRORS, L"setOnlineStatus: client in wrong state: %d", mState);
				return false;
			}
			return mPeer.opSetOnlineStatus(status, message, skipMessage);
		}
		
			/**
				Adds users to the list on the Chat Server which will send you status updates for those.
				@param userIds list of friend user names
				@returns true if command sent*/
		bool Client::opAddFriends(const Common::JVector<Common::JString>& userIds)
		{
			if(mState != ClientState::ConnectedToFrontEnd)
			{
				EGLOG(DebugLevel::ERRORS, L"opAddFriends: client in wrong state: %d", mState);
				return false;
			}
			return mPeer.opAddFriends(userIds);
		}
		
			/**
				Removes users from the list on the Chat Server which will send you status updates for those.
				@param userIds list of friend user names
				@return true if command sent*/
		bool Client::opRemoveFriends(const Common::JVector<Common::JString>& userIds)
		{
			if(mState != ClientState::ConnectedToFrontEnd)
			{
				EGLOG(DebugLevel::ERRORS, L"opRemoveFriends: client in wrong state: %d", mState);
				return false;
			}
			return mPeer.opRemoveFriends(userIds);
		}
		
			/**
				Returns user id (name in chat).
				@returns user id*/
		const JString& Client::getUserId()
		{
			return mUserId;
		}
		
			/**
				Sets user id (name in chat).
				Use before #connect() call to override value set in #Client().
				@param userId new user id*/
		void Client::setUserId(const JString& userId)
		{
			mUserId = userId;
		}
		
			/**
				Returns client state*/
		ClientState::ClientState Client::getState(void) const
		{
			return mState;
		}
		
			/**
				Returns cause of last disconnect event.
				@returns disconnect cause constant from Chat::DisconnectCause.
				@sa Chat::DisconnectCause*/
		int Client::getDisconnectedCause(void) const
		{
			return mDisconnectedCause;
		}
		
			/**
				Returns chat (Name Server) region.*/
		const JString& Client::getRegion(void) const
		{
			return mRegion;
		}
		
			/**
				Sets chat (Name Server) region.
				Set it before connect() call.
				@param region region*/
		void Client::setRegion(const JString& value)
		{
			mRegion = value;
		}
		
			/**
				Returns list of subscribed public channels.
				@returns list of subscribed channels*/
		const JVector<Channel>& Client::getPublicChannels(void) const
		{
			return mPublicChannels;
		}
		
			/**
				Returns list of private chats that client currently has.
				@returns list of private chats*/
		const JVector<Channel>& Client::getPrivateChannels(void) const
		{
			return mPrivateChannels;
		}
		
			/**
				Search subscribed public channels by channel name.
				@param channelName channel name to search
				@returns found channel or NULL otherwise*/
		const Channel* Client::getPublicChannel(const JString& name) const
		{
			for(unsigned int i=0; i<mPublicChannels.getSize(); ++i)
				if(mPublicChannels[i].getName() == name)
					return &(mPublicChannels[i]);
			return NULL;
		}
		
			/**
				Search private chat by user name.
				@param userName user name to search
				@returns found chat or NULL otherwise*/
		const Channel* Client::getPrivateChannel(const JString& userName) const
		{
			for(unsigned int i=0; i<mPrivateChannels.getSize(); ++i)
				if(mPrivateChannels[i].getName() == userName)
					return &(mPrivateChannels[i]);
			return NULL;
		}

		void Client::onOperationResponse(const Photon::OperationResponse& operationResponse)
		{
			EGLOG(operationResponse.getReturnCode()?DebugLevel::ERRORS:DebugLevel::INFO, operationResponse.toString(true, true));
			switch(operationResponse.getOperationCode())
			{
			case OperationCode::AUTHENTICATE:
				handleAuthResponse(operationResponse);
				break;
			case OperationCode::PUBLISH:
				break;
			case OperationCode::SEND_PRIVATE:
				break;
			case OperationCode::CHANNEL_HISTORY:
				break;
			case OperationCode::UPDATE_STATUS:
				break;
			case OperationCode::ADD_FRIENDS:
				break;
			case OperationCode::REMOVE_FRIENDS:
				break;
			default:
				break;
			}
		}

		void Client::onStatusChanged(int statusCode)
		{
			switch(statusCode)
			{
			case Photon::StatusCode::CONNECT:
				{
					mPeer.establishEncryption();
					if(mState == ClientState::ConnectingToNameServer)
					{
						mState = ClientState::ConnectedToNameServer;
						mListener.onStateChange(mState);
						EGLOG(DebugLevel::INFO, L"connected to nameserver");
					}
					else if(mState == ClientState::ConnectingToFrontEnd)
					{
						mPeer.opAuthenticateOnFrontEnd(mAuthenticationValues.getSecret());
						EGLOG(DebugLevel::INFO, L"connected to frontend");
					}
				}
				break;
			case Photon::StatusCode::DISCONNECT:
				{
					if(mState == ClientState::Authenticated)
					{
						mPeer.connect(mFrontEndAddress);
						mState = ClientState::ConnectingToFrontEnd;
						mListener.onStateChange(mState);
					}
					else
					{
						mState = ClientState::Disconnected;
						mListener.onStateChange(mState);
						mListener.disconnectReturn();
					}
				}
				break;
			case Photon::StatusCode::ENCRYPTION_ESTABLISHED:
				if(!mDidAuthenticate)
				{
					if(mPeer.opAuthenticateOnNameServer(mAppID, mAppVersion, mRegion, mUserId, mAuthenticationValues))
						mDidAuthenticate = true;
					else
					{
						mDidAuthenticate = false;
						mListener.debugReturn(DebugLevel::ERRORS, JString(L"Error calling OpAuthenticate! Did not work. Check log output, CustomAuthenticationValues and if you're connected. State: ") + mState);
					}
				}
				break;
			case Photon::StatusCode::ENCRYPTION_FAILED_TO_ESTABLISH:
				handleConnectionFlowError(mState, statusCode, "Encryption failed to establish");
				break;
				// cases till next break should set mDisconnectedCause below
			case Photon::StatusCode::EXCEPTION:
			case Photon::StatusCode::EXCEPTION_ON_CONNECT:
			case Photon::StatusCode::INTERNAL_RECEIVE_EXCEPTION:
			case Photon::StatusCode::TIMEOUT_DISCONNECT:
			case Photon::StatusCode::DISCONNECT_BY_SERVER:
			case Photon::StatusCode::DISCONNECT_BY_SERVER_USER_LIMIT:
			case Photon::StatusCode::DISCONNECT_BY_SERVER_LOGIC:
				mListener.connectionErrorReturn(statusCode);
				if(mPeer.getPeerState() != Photon::PeerState::DISCONNECTED && mPeer.getPeerState() != Photon::PeerState::DISCONNECTING)
					disconnect();
				break;
				break;
			case Photon::StatusCode::SEND_ERROR:
				mListener.clientErrorReturn(statusCode);
				break;
			case Photon::StatusCode::QUEUE_OUTGOING_RELIABLE_WARNING:
			case Photon::StatusCode::QUEUE_OUTGOING_UNRELIABLE_WARNING:
			case Photon::StatusCode::QUEUE_OUTGOING_ACKS_WARNING:
			case Photon::StatusCode::QUEUE_INCOMING_RELIABLE_WARNING:
			case Photon::StatusCode::QUEUE_INCOMING_UNRELIABLE_WARNING:
			case Photon::StatusCode::QUEUE_SENT_WARNING:
				mListener.warningReturn(statusCode);
				break;
			case ErrorCode::OPERATION_INVALID:
			case ErrorCode::INTERNAL_SERVER_ERROR:
				mListener.serverErrorReturn(statusCode);
				break;
			default:
				EGLOG(DebugLevel::ERRORS, L"received unknown status-code from server");
				break;
			}

			// above cases starting from StatusCode::EXCEPTION till next break should set mDisconnectedCause here
			switch(statusCode)
			{
			case Photon::StatusCode::DISCONNECT_BY_SERVER_USER_LIMIT:
				mDisconnectedCause = DisconnectCause::DISCONNECT_BY_SERVER_USER_LIMIT;
				break;
			case Photon::StatusCode::EXCEPTION_ON_CONNECT:
				mDisconnectedCause = DisconnectCause::EXCEPTION_ON_CONNECT;
				break;
			case Photon::StatusCode::DISCONNECT_BY_SERVER:
				mDisconnectedCause = DisconnectCause::DISCONNECT_BY_SERVER;
				break;
			case Photon::StatusCode::DISCONNECT_BY_SERVER_LOGIC:
				mDisconnectedCause = DisconnectCause::DISCONNECT_BY_SERVER_LOGIC;
				break;
			case Photon::StatusCode::TIMEOUT_DISCONNECT:
				mDisconnectedCause = DisconnectCause::TIMEOUT_DISCONNECT;
				break;
			case Photon::StatusCode::EXCEPTION:
			case Photon::StatusCode::INTERNAL_RECEIVE_EXCEPTION:
				mDisconnectedCause = DisconnectCause::EXCEPTION;
				break;
			default:
				break;
			}
		}

		void Client::onEvent(const Photon::EventData& eventData)
		{
			EGLOG(DebugLevel::ALL, L"%s", eventData.toString(true, true).cstr()); // don't print out the payload here, as that can get too expensive for big events
			switch(eventData.getCode())
			{
			case EventCode::CHAT_MESSAGES:
				{
					const ValueObject<JString*>& vSenders = eventData.getParameterForCode(ParameterCode::SENDERS);
					const ValueObject<Object*>& vMessages = eventData.getParameterForCode(ParameterCode::MESSAGES);

					JVector<JString> senders(*vSenders.getDataAddress(), vSenders.getSizes()[0]);
					JVector<Object> messages(*vMessages.getDataAddress(), vMessages.getSizes()[0]);

					const JString& channelName = ValueObject<JString>(eventData.getParameterForCode(ParameterCode::CHANNEL)).getDataCopy();

					Channel* pCh = getChannel(mPublicChannels, channelName);
					if(pCh)
					{
						pCh->add(senders, messages);
						mListener.onGetMessages(channelName, senders, messages);
					}
					else
						EGLOG(DebugLevel::WARNINGS, L"Got message from unsubscribed channel " + channelName);
				}
				break;
			case EventCode::PRIVATE_MESSAGE:
				{
					const Object& message = eventData.getParameterForCode(ParameterCode::MESSAGE);
					const JString& sender = ValueObject<JString>(eventData.getParameterForCode(ParameterCode::SENDER)).getDataCopy();

					JString channelName = sender;
					if(sender == mUserId) 
						channelName = ValueObject<JString>(eventData.getParameterForCode(ParameterCode::USER_ID)).getDataCopy();

					getOrAddChannel(mPrivateChannels, channelName, true)->add(sender, message);
					mListener.onPrivateMessage(sender, message, channelName);
				}
				break;
			case EventCode::STATUS_UPDATE:
				{
					const Object& message = eventData.getParameterForCode(ParameterCode::MESSAGE);
					const JString& user = ValueObject<JString>(eventData.getParameterForCode(ParameterCode::SENDER)).getDataCopy();
					int status = ValueObject<int>(eventData.getParameterForCode(ParameterCode::STATUS)).getDataCopy();

					bool gotMessage = eventData.getParameters().contains(ParameterCode::MESSAGE);
					mListener.onStatusUpdate(user, status, gotMessage, message);
				}
				break;
			case EventCode::SUBSCRIBE:
				handleSubscribeResponse(eventData);
				break;
			case EventCode::UNSUBSCRIBE:
				handleUnsubscribeResponse(eventData);
				break;
			default:
				break;
			}
		}

		void Client::debugReturn(DebugLevel::DebugLevel debugLevel, const JString& string)
		{
			mListener.debugReturn(debugLevel, string);
		}

		void Client::handleConnectionFlowError(ClientState::ClientState oldState, int errorCode, const JString& errorString)
		{
			mState = ClientState::Disconnecting;
			mListener.onStateChange(mState);
			mPeer.disconnect();
		}

		void Client::handleAuthResponse(const Photon::OperationResponse& operationResponse)
		{
			ClientState::ClientState oldState = mState;
			if(operationResponse.getReturnCode())
			{
				EGLOG(DebugLevel::ERRORS, L"Authentication failed with errorcode %d: %ls", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());

				switch(operationResponse.getReturnCode())
				{
				case ErrorCode::INVALID_AUTHENTICATION:
					mDisconnectedCause = DisconnectCause::INVALID_AUTHENTICATION;
					break;
				case ErrorCode::CUSTOM_AUTHENTICATION_FAILED:
					mDisconnectedCause = DisconnectCause::CUSTOM_AUTHENTICATION_FAILED;
					break;
				case ErrorCode::INVALID_REGION:
					mDisconnectedCause = DisconnectCause::INVALID_REGION;
					break;
				case ErrorCode::MAX_CCU_REACHED:
					mDisconnectedCause = DisconnectCause::MAX_CCU_REACHED;
					break;
				case ErrorCode::OPERATION_DENIED:
					mDisconnectedCause = DisconnectCause::OPERATION_NOT_ALLOWED_IN_CURRENT_STATE;
					break;
				default:
					break;
				}
				handleConnectionFlowError(oldState, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
			}
			else
			{
				if(mState == ClientState::ConnectedToNameServer)
				{
					mState = ClientState::Authenticated;
					mListener.onStateChange(mState);
					AuthenticationValuesSecretSetter::setSecret(mAuthenticationValues, ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::SECRET)).getDataCopy());
					mFrontEndAddress = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ADDRESS)).getDataCopy();
					EGLOG(DebugLevel::INFO, L"Server returned secret %ls and front end address %ls", mAuthenticationValues.getSecret().cstr(), mFrontEndAddress.cstr());
					mPeer.disconnect();
				}
				else if(mState == ClientState::ConnectingToFrontEnd)
				{
					mState = ClientState::ConnectedToFrontEnd;
					mListener.onStateChange(mState);
				}
			}
		}

		void Client::handleSubscribeResponse(const Photon::EventData& eventData)
		{
			ValueObject<JString*> vChannels = eventData.getParameterForCode(ParameterCode::CHANNELS);
			ValueObject<bool*> vResults = eventData.getParameterForCode(ParameterCode::SUBSCRIBE_RESULTS);
			int size = vChannels.getSizes()[0];
			JVector<JString> channels(size);
			JVector<bool> results(size);
			for(int i=0; i<size; ++i)
			{
				channels.addElement((*vChannels.getDataAddress())[i]);
				results.addElement(false);
				if((int)i < size)
				{
					bool result = (*vResults.getDataAddress())[i];
					results[i] = result;
					if(result)
						getOrAddChannel(mPublicChannels, channels[i], false);
				}
			}
			mListener.subscribeReturn(channels, results);
		}

		void Client::handleUnsubscribeResponse(const Photon::EventData& eventData)
		{
			ValueObject<JString*> vChannels = eventData.getParameterForCode(ParameterCode::CHANNELS);
			int size = vChannels.getSizes()[0];
			JVector<JString> channels(size);
			for(int i=0; i<size; ++i)
			{
				channels.addElement((*vChannels.getDataAddress())[i]);
				removeChannel(mPublicChannels, channels[i]);
			}
			mListener.unsubscribeReturn(channels);
		}

		Channel* Client::getChannel(JVector<Channel>& channels, const JString& name)
		{
			for(unsigned int i=0; i<channels.getSize(); ++i)
				if(channels[i].getName() == name)
					return &(channels[i]);
			return NULL;
		}

		Channel* Client::getOrAddChannel(JVector<Channel>& channels, const JString& name, bool isPrivate)
		{
			Channel* pCh = getChannel(channels, name);
			if(pCh)
				return pCh;
			else
			{
				channels.addElement(Channel(name, isPrivate));
				return &(channels[channels.getSize() - 1]);
			}
		}

		void Client::removeChannel(JVector<Channel>& channels, const JString& name)
		{
			for(unsigned int i=0; i<channels.getSize();)
			{
				if(channels[i].getName() == name)
					channels.removeElementAt(i);
				else
					++i;
			}
		}
	}
}