/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2015 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancing-cpp/inc/Peer.h"
#include "Common-cpp/inc/Enums/DebugLevel.h"
#include "Photon-cpp/inc/Enums/ReceiverGroup.h"
#include "Photon-cpp/inc/Enums/EventCache.h"
#include "Photon-cpp/inc/Enums/ParameterCode.h"
#include "LoadBalancing-cpp/inc/Enums/CustomAuthenticationType.h"
#include "LoadBalancing-cpp/inc/Internal/AuthenticationValuesSecretSetter.h"
#include "LoadBalancing-cpp/inc/Internal/Utils.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/JoinType.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/OperationCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/ParameterCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Player.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Room.h"

/** @file LoadBalancing-cpp/inc/Peer.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;
		using namespace Common::MemoryManagement;
		using namespace Photon;
		using namespace Lite;
		using namespace ParameterCode;
		using namespace Internal;



		Peer::Peer(Photon::PhotonListener& listener, nByte connectionProtocol)
			: PhotonPeer(listener, connectionProtocol)
		{
		}

		Peer::~Peer(void)
		{
		}

		bool Peer::opJoinLobby(const JString& lobbyName, nByte lobbyType)
		{
			OperationRequestParameters op;
			if(lobbyName.length()) 
			{
				op.put(ParameterCode::LOBBY_NAME, ValueObject<JString>(lobbyName));
				if(lobbyType != LobbyType::DEFAULT)
					op.put(ParameterCode::LOBBY_TYPE, ValueObject<nByte>(lobbyType));
			};
			return opCustom(OperationRequest(OperationCode::JOIN_LOBBY, op), true);
		}

		bool Peer::opLeaveLobby(void)
		{
			return opCustom(OperationRequest(OperationCode::LEAVE_LOBBY), true);
		}

		bool Peer::opCreateRoom(const JString& gameID, bool isVisible, bool isOpen, nByte maxPlayers, const Hashtable& customRoomProperties, const Hashtable& customLocalPlayerProperties, const JVector<JString>& propsListedInLobby, const Common::JString& lobbyName, nByte lobbyType, int playerTtl, int emptyRoomTtl)
		{
			return opCustom(OperationRequest(OperationCode::CREATE_ROOM, opCreateRoomImplementation(gameID, isVisible, isOpen, maxPlayers, customRoomProperties, customLocalPlayerProperties, propsListedInLobby, lobbyName, lobbyType, playerTtl, emptyRoomTtl)), true);
		}

		bool Peer::opJoinRoom(const JString& gameID, const Hashtable& customLocalPlayerProperties, bool createIfNotExists, int playerNumber, int cacheSliceIndex, int playerTtl, int emptyRoomTtl)
		{
			return gameID.length() ? opCustom(OperationRequest(OperationCode::JOIN_ROOM, opJoinRoomImplementation(gameID, customLocalPlayerProperties, createIfNotExists, playerNumber, cacheSliceIndex, playerTtl, emptyRoomTtl)), true) : false;
		}

		bool Peer::opJoinRandomRoom(const Hashtable& customRoomProperties, nByte maxPlayers, nByte matchingType, const Common::JString& lobbyName, nByte lobbyType, const Common::JString& sqlLobbyFilter)
		{
			Hashtable roomProps(Utils::stripToCustomProperties(customRoomProperties));
			if(maxPlayers)
				roomProps.put(Properties::Room::MAX_PLAYERS, maxPlayers);
				
			OperationRequestParameters op;

			if(roomProps.getSize())
				op.put(ParameterCode::ROOM_PROPERTIES, ValueObject<Hashtable>(roomProps));
			if(matchingType != MatchmakingMode::FILL_ROOM)
				op.put(ParameterCode::MATCHMAKING_TYPE, ValueObject<nByte>(matchingType));
			if(lobbyName.length()) 
			{
				op.put(ParameterCode::LOBBY_NAME, ValueObject<JString>(lobbyName));
				if(lobbyType != LobbyType::DEFAULT)
					op.put(ParameterCode::LOBBY_TYPE, ValueObject<nByte>(lobbyType));
			}
			if(sqlLobbyFilter.length()) 
				op.put(ParameterCode::DATA, ValueObject<JString>(sqlLobbyFilter));

			return opCustom(OperationRequest(OperationCode::JOIN_RANDOM_ROOM, op), true);
		}

		bool Peer::opLeaveRoom(bool willComeBack)
		{
			OperationRequestParameters op;
			if(willComeBack)
				op.put(ParameterCode::IS_COMING_BACK, ValueObject<bool>(willComeBack));
			return opCustom(OperationRequest(OperationCode::LEAVE), true);
		}

		bool Peer::opRaiseEvent(bool reliable, const Common::Object& parameters, nByte eventCode, nByte channelID, nByte eventCaching, const int* targetPlayers, short numTargetPlayers, nByte receiverGroup, nByte interestGroup, bool forwardToWebhook, int cacheSliceIndex)
		{
			OperationRequestParameters op;
			if(eventCaching == EventCache::SLICE_INC_INDEX || eventCaching == EventCache::SLICE_SET_INDEX || eventCaching == EventCache::SLICE_PURGE_INDEX || eventCaching == EventCache::SLICE_PURGE_UP_TO_INDEX)
			{
				op.put(ParameterCode::CACHE, ValueObject<nByte>(eventCaching));
				if(eventCaching != EventCache::SLICE_INC_INDEX)
					op.put(ParameterCode::CACHE_SLICE_INDEX, ValueObject<int>(cacheSliceIndex));
				return opCustom(OperationRequest(OperationCode::RAISE_EVENT, op), true, channelID);
			}
			op.put(ParameterCode::DATA, parameters);
			op.put(ParameterCode::CODE, ValueObject<nByte>(eventCode));
			if(eventCaching != EventCache::DO_NOT_CACHE)
				op.put(ParameterCode::CACHE, ValueObject<nByte>(eventCaching));
			if(targetPlayers)
				op.put(ParameterCode::PLAYER_LIST, ValueObject<const int*>(targetPlayers, numTargetPlayers));
			if(receiverGroup != ReceiverGroup::OTHERS)
				op.put(ParameterCode::RECEIVER_GROUP, ValueObject<nByte>(receiverGroup));
			if(interestGroup)
				op.put(ParameterCode::GROUP, ValueObject<nByte>(interestGroup));
			if(forwardToWebhook)
				op.put(ParameterCode::EVENT_FORWARD, ValueObject<bool>(forwardToWebhook));
			return opCustom(OperationRequest(OperationCode::RAISE_EVENT, op), reliable, channelID);
		}

		bool Peer::opAuthenticate(const JString& appID, const JString& appVersion, bool encrypted, const JString& userID, const AuthenticationValues& authenticationValues, bool lobbyStats, const JString& regionCode)
		{
			OperationRequestParameters op;
			if(lobbyStats)
				op.put(ParameterCode::LOBBY_STATS, ValueObject<bool>(true));
			if(authenticationValues.getSecret().length())
			{
				op.put(ParameterCode::SECRET, ValueObject<JString>(authenticationValues.getSecret()));
				EGLOG(DebugLevel::INFO, OperationRequest(OperationCode::AUTHENTICATE, op).toString(true));
				return opCustom(OperationRequest(OperationCode::AUTHENTICATE, op), true, 0, false); // the secret already is encrypted, so we don't need to encrypt it again
			}
			op.put(ParameterCode::APPLICATION_ID, ValueObject<JString>(appID));
			op.put(ParameterCode::APP_VERSION, ValueObject<JString>(appVersion));
			if(regionCode.length())
				op.put(ParameterCode::REGION, ValueObject<JString>(regionCode));
			if(userID.length())
				op.put(ParameterCode::USER_ID, ValueObject<JString>(userID));
			if(authenticationValues.getType() != CustomAuthenticationType::NONE)
			{
				op.put(ParameterCode::CLIENT_AUTHENTICATION_TYPE, ValueObject<nByte>(authenticationValues.getType()));
				if(authenticationValues.getParameters().length())
					op.put(ParameterCode::CLIENT_AUTHENTICATION_PARAMETERS, ValueObject<JString>(authenticationValues.getParameters()));
				if(authenticationValues.getData().getSize())
					op.put(ParameterCode::CLIENT_AUTHENTICATION_DATA, ValueObject<const nByte*>(authenticationValues.getData().getCArray(), static_cast<int>(authenticationValues.getData().getSize())));
			}
			EGLOG(DebugLevel::INFO, OperationRequest(OperationCode::AUTHENTICATE, op).toString(true));
			return opCustom(OperationRequest(OperationCode::AUTHENTICATE, op), true, 0, encrypted);
		}

		bool Peer::opChangeGroups(const JVector<nByte>* pGroupsToRemove, const JVector<nByte>* pGroupsToAdd)
		{
			OperationRequestParameters op;
			if(pGroupsToRemove)
				op.put(Lite::ParameterCode::REMOVE, ValueObject<const nByte*>(pGroupsToRemove->getCArray(), pGroupsToRemove->getSize()));
			if(pGroupsToAdd)
				op.put(Lite::ParameterCode::ADD, ValueObject<const nByte*>(pGroupsToAdd->getCArray(), pGroupsToAdd->getSize()));

			return opCustom(OperationRequest(OperationCode::CHANGE_GROUPS, op), true, 0, false);
		}

		bool Peer::opWebRpc(const JString& uriPath, const Object& parameters)
		{
			OperationRequestParameters op;
			op.put(ParameterCode::URI_PATH, ValueObject<JString>(uriPath));
			op.put(ParameterCode::RPC_CALL_PARAMS, parameters);
			return opCustom(OperationRequest(OperationCode::RPC, op), true, 0, false);
		}

		bool Peer::opFindFriends(const JString* friendsToFind, short numFriendsToFind)
		{
			if(!friendsToFind || !numFriendsToFind)
				return false;
			OperationRequestParameters op;
			op.put(ParameterCode::FIND_FRIENDS_REQUEST_LIST, ValueObject<const JString*>(friendsToFind, numFriendsToFind));

			return opCustom(OperationRequest(OperationCode::FIND_FRIENDS, op), true);
		}

		bool Peer::opLobbyStats(const Common::JVector<LoadBalancing::LobbyStatsRequest>& lobbiesToQuery)
		{
			if(!lobbiesToQuery.getSize())
				return opCustom(OperationRequest(OperationCode::LOBBY_STATS), true);

			unsigned int size = lobbiesToQuery.getSize();
			OperationRequestParameters op;
			JString* pNames = allocateArray<JString>(size);
			nByte* pTypes = allocateArray<nByte>(size);
			for(unsigned int i=0; i<size; ++i)
			{
				pNames[i] = lobbiesToQuery[i].getName();
				pTypes[i] = lobbiesToQuery[i].getType();
			}
			op.put(ParameterCode::LOBBY_NAME, ValueObject<const JString*>(pNames, size));
			op.put(ParameterCode::LOBBY_TYPE, ValueObject<const nByte*>(pTypes, size));
			bool res = opCustom(OperationRequest(OperationCode::LOBBY_STATS, op), true);
			deallocateArray(pNames);
			deallocateArray(pTypes);
			return res;
		}

		bool Peer::opGetRegions(bool encrypted, const JString& appID)
		{
			OperationRequestParameters op;
			op.put(ParameterCode::APPLICATION_ID, ValueObject<JString>(appID));

			return opCustom(OperationRequest(OperationCode::GET_REGIONS, op), true, 0, encrypted);
		}

		bool Peer::opSetPropertiesOfPlayer(int playerNr, const Hashtable& properties)
		{
			OperationRequestParameters op;
			op.put(ParameterCode::PROPERTIES, ValueObject<Hashtable>(properties));
			op.put(ParameterCode::PLAYERNR, ValueObject<int>(playerNr));
			op.put(ParameterCode::BROADCAST, ValueObject<bool>(true));

			return opCustom(OperationRequest(OperationCode::SET_PROPERTIES, op), true);
		}

		bool Peer::opSetPropertiesOfRoom(const Hashtable& properties, bool webForward)
		{
			OperationRequestParameters op;
			op.put(ParameterCode::PROPERTIES, ValueObject<Hashtable>(properties));
			if(webForward) 
				op.put(ParameterCode::EVENT_FORWARD, ValueObject<bool>(webForward));
			op.put(ParameterCode::BROADCAST, ValueObject<bool>(true));

			return opCustom(OperationRequest(OperationCode::SET_PROPERTIES, op), true);
		}

		OperationRequestParameters Peer::opCreateRoomImplementation(const JString& gameID, bool isVisible, bool isOpen, nByte maxPlayers, const Hashtable& customRoomProperties, const Hashtable& customLocalPlayerProperties, const JVector<JString>& propsListedInLobby, const JString& lobbyName, nByte lobbyType, int playerTtl, int emptyRoomTtl)
		{
			OperationRequestParameters op;
			if(gameID.length())
				op.put(ParameterCode::ROOM_NAME, ValueObject<JString>(gameID));

			Hashtable roomProps(Utils::stripToCustomProperties(customRoomProperties));
			if(!isOpen)
				roomProps.put(Properties::Room::IS_OPEN, isOpen);
			if(!isVisible)
				roomProps.put(Properties::Room::IS_VISIBLE, isVisible);
			if(maxPlayers)
				roomProps.put(Properties::Room::MAX_PLAYERS, maxPlayers);
			JString* propsListedInLobbyArr = allocateArray<JString>(propsListedInLobby.getSize());
			for(unsigned int i=0; i<propsListedInLobby.getSize(); ++i)
				propsListedInLobbyArr[i] = propsListedInLobby[i];
			roomProps.put(Properties::Room::PROPS_LISTED_IN_LOBBY, propsListedInLobbyArr, propsListedInLobby.getSize());
			deallocateArray(propsListedInLobbyArr);
			op.put(ParameterCode::ROOM_PROPERTIES, ValueObject<Hashtable>(roomProps));
			Hashtable playerProperties = Utils::stripToCustomProperties(customLocalPlayerProperties);
			if(playerProperties.getSize())
				op.put(ParameterCode::PLAYER_PROPERTIES, ValueObject<Hashtable>(playerProperties));
			op.put(ParameterCode::BROADCAST, ValueObject<bool>(true));
			op.put(ParameterCode::CLEANUP_CACHE_ON_LEAVE, ValueObject<bool>(true));
			if(lobbyName.length())
			{
				op.put(ParameterCode::LOBBY_NAME, ValueObject<JString>(lobbyName));
				if(lobbyType != LobbyType::DEFAULT)
					op.put(ParameterCode::LOBBY_TYPE, ValueObject<nByte>(lobbyType));
			}
			if(playerTtl > 0)
				op.put(ParameterCode::PLAYER_TTL, ValueObject<int>(playerTtl));
            if(emptyRoomTtl > 0)
				op.put(ParameterCode::EMPTY_ROOM_TTL, ValueObject<int>(emptyRoomTtl));
			op.put(ParameterCode::CHECK_USER_ON_JOIN, ValueObject<bool>(true));

			return op;
		}

		OperationRequestParameters Peer::opJoinRoomImplementation(const JString& gameID, const Hashtable& customLocalPlayerProperties, bool createIfNotExists, int playerNumber, int cacheSliceIndex, int playerTtl, int emptyRoomTtl)
		{
			OperationRequestParameters op;
			op.put(ParameterCode::ROOM_NAME, ValueObject<JString>(gameID));
			
			Hashtable playerProps = Utils::stripToCustomProperties(customLocalPlayerProperties);
			if(playerProps.getSize())
				op.put(ParameterCode::PLAYER_PROPERTIES, ValueObject<Hashtable>(playerProps));
			op.put(ParameterCode::BROADCAST, ValueObject<bool>(true));

			if(createIfNotExists)
				op.put(ParameterCode::CREATE_IF_NOT_EXISTS, ValueObject<bool>(true));
			if(playerNumber > 0)
				op.put(ParameterCode::PLAYERNR, ValueObject<int>(playerNumber));
			else
			{
				if(playerTtl > 0)
					op.put(ParameterCode::PLAYER_TTL, ValueObject<int>(playerTtl));
				if(emptyRoomTtl > 0)
					op.put(ParameterCode::EMPTY_ROOM_TTL, ValueObject<int>(emptyRoomTtl));
			}
			if(cacheSliceIndex > 0)
				op.put(ParameterCode::CACHE_SLICE_INDEX, ValueObject<int>(cacheSliceIndex));
			if(createIfNotExists)
				op.put(ParameterCode::CHECK_USER_ON_JOIN, ValueObject<bool>(true));

			return op;
		}
	}
}