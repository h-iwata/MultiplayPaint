/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2015 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

namespace ExitGames
{
	namespace Common
	{
		namespace DebugLevel
		{
			/** Amount of DebugReturn callbacks. Each debug level includes output for lower ones: OFF, ERRORS, WARNINGS, INFO, ALL. */
			enum DebugLevel
			{
				OFF,      ///<No debug out.
				ERRORS,   ///<Only error descriptions.
				WARNINGS, ///<Warnings and errors.
				INFO,     ///<Information about internal workflows, warnings and errors.
				ALL       ///<Most complete workflow description (but lots of debug output), info, warnings and errors.
			};
			/** @file */
		}
	}
}