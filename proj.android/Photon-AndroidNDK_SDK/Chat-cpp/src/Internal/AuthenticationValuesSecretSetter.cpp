/* Exit Games Photon Chat - C++ Client Lib
 * Copyright (C) 2004-2015 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "Chat-cpp/inc/AuthenticationValues.h"
#include "Chat-cpp/inc/Internal/AuthenticationValuesSecretSetter.h"

namespace ExitGames
{
	namespace Chat
	{
		namespace Internal
		{
			void AuthenticationValuesSecretSetter::setSecret(AuthenticationValues& authenticationValues, const Common::JString& secret)
			{
				authenticationValues.setSecret(secret);
			}
		}
	}
}