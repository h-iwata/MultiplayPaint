/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2015 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

namespace ExitGames
{
	namespace LoadBalancing
	{
		namespace PeerStates
		{
			enum PeerStates
			{
				Uninitialized,
				PeerCreated,
				ConnectingToNameserver,
				ConnectedToNameserver,
				DisconnectingFromNameserver,
				Connecting,
				Connected,
				WaitingForCustomAuthenticationNextStepCall,
				Authenticated,
				JoinedLobby,
				DisconnectingFromMasterserver,
				ConnectingToGameserver,
				ConnectedToGameserver,
				AuthenticatedOnGameServer,
				Joining,
				Joined,
				Leaving,
				Left,
				DisconnectingFromGameserver,
				ConnectingToMasterserver,
				ConnectedComingFromGameserver,
				AuthenticatedComingFromGameserver,
				Disconnecting,
				Disconnected,
			};
			/** @file */
		}
	}
}