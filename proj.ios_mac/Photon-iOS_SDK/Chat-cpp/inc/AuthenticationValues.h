/* Exit Games Photon Chat - C++ Client Lib
 * Copyright (C) 2004-2015 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Chat-cpp/inc/Enums/CustomAuthenticationType.h"

namespace ExitGames
{
	namespace Chat
	{
		namespace Internal
		{
			class AuthenticationValuesSecretSetter;
		}

		class AuthenticationValues
		{
		public:
			AuthenticationValues(nByte type=CustomAuthenticationType::NONE, const Common::JString& parameters=Common::JString(), const Common::JVector<nByte>& data=Common::JVector<nByte>());
			AuthenticationValues(const Common::JVector<nByte>& data);
			AuthenticationValues(const Common::JString& username, const Common::JString& token, const Common::JVector<nByte>& data=Common::JVector<nByte>());

			nByte getType(void) const;
			const Common::JString& getParameters(void) const;
			const Common::JVector<nByte>& getData(void) const;
			const Common::JString& getSecret(void) const;
		private:
			void setSecret(const Common::JString& secret);

			nByte mType;
			Common::JString mParameters;
			Common::JVector<nByte> mData;
			Common::JString mSecret;

			friend class Internal::AuthenticationValuesSecretSetter;
		};
	}
}