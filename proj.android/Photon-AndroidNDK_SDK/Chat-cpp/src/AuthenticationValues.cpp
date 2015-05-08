/* Exit Games Photon Chat - C++ Client Lib
 * Copyright (C) 2004-2015 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "Chat-cpp/inc/AuthenticationValues.h"

/** @file Chat-cpp/inc/AuthenticationValues.h */

namespace ExitGames
{
	namespace Chat
	{
		using namespace Common;

		AuthenticationValues::AuthenticationValues(nByte type, const Common::JString& parameters, const Common::JVector<nByte>& data)
			: mType(type)
			, mParameters(parameters)
			, mData(data)
		{
		}

		AuthenticationValues::AuthenticationValues(const JVector<nByte>& data)
			: mType(CustomAuthenticationType::CUSTOM)
			, mData(data)
		{
		}

		AuthenticationValues::AuthenticationValues(const JString& username, const JString& token, const JVector<nByte>& data)
			: mType(CustomAuthenticationType::CUSTOM)
			, mParameters(JString(L"username=")+username+"&token="+token)
			, mData(data)
		{
		}

		nByte AuthenticationValues::getType(void) const
		{
			return mType;
		}

		const Common::JString& AuthenticationValues::getParameters(void) const
		{
			return mParameters;
		}

		const Common::JVector<nByte>& AuthenticationValues::getData(void) const
		{
			return mData;
		}

		const Common::JString& AuthenticationValues::getSecret(void) const
		{
			return mSecret;
		}

		void AuthenticationValues::setSecret(const JString& secret)
		{
			mSecret = secret;
		}
	}
}