/* Exit Games Photon Chat - C++ Client Lib
 * Copyright (C) 2004-2015 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

namespace ExitGames
{
	namespace Chat
	{
		namespace ClientState
		{
			/** Possible states for a Client.*/
			enum ClientState
			{
				Uninitialized,               ///<Peer is created but not used yet.
				ConnectingToNameServer,      ///<Connecting to Name Server (includes connect, authenticate and joining the lobby)
				ConnectedToNameServer,       ///<Connected to Name Server.
				Authenticating,              ///<Authenticating.
				Authenticated,               ///<Authenticated.
				DisconnectingFromNameServer, ///<Transition from Name to Chat Server.
				ConnectingToFrontEnd,        ///<Transition to Chat Server.
				ConnectedToFrontEnd,         ///<Connected to Chat Server. Subscribe to channels and chat here.
				Disconnecting,               ///<The client disconnects (from any server).
				Disconnected                 ///<The client is no longer connected (to any server). Connect to Name Server to go on.
			};
			/** @file */
		}
	}
}