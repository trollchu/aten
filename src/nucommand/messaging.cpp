/*
	*** Messaging Commands
	*** src/nucommand/messaging.cpp
	Copyright T. Youngs 2007-2009

	This file is part of Aten.

	Aten is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Aten is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Aten.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "nucommand/commands.h"
#include "parser/commandnode.h"

// Write line to msg output and stop
bool NuCommand::function_Error(NuCommandNode *c, Bundle &obj, NuReturnValue &rv)
{
	NuFormat *fmt = c->format();
	if (fmt == NULL)
	{
		printf("Warning - No format defined in 'error' command.\n");
		return FALSE;
	}
	if (fmt->writeToString()) msg.print("%s\n",fmt->string());
	return FALSE;
}

// Print formatted string
bool NuCommand::function_Printf(NuCommandNode *c, Bundle &obj, NuReturnValue &rv)
{
	NuFormat *fmt = c->format();
	if (fmt == NULL)
	{
		printf("Warning - No format defined in 'printf' command.\n");
		return FALSE;
	}
	if (fmt->writeToString()) msg.print("%s\n",fmt->string());
	return TRUE;
}

// Print formatted string (in verbose output only)
bool NuCommand::function_Verbose(NuCommandNode *c, Bundle &obj, NuReturnValue &rv)
{
	NuFormat *fmt = c->format();
	if (fmt == NULL)
	{
		printf("Warning - No format defined in 'verbose' command.\n");
		return FALSE;
	}
	if (!msg.isOutputActive(Messenger::Verbose)) return TRUE;
	if (fmt->writeToString()) msg.print(Messenger::Verbose, "%s\n",fmt->string());
	return TRUE;
}

// Write line to msg output
bool NuCommand::function_Warn(NuCommandNode *c, Bundle &obj, NuReturnValue &rv)
{
	NuFormat *fmt = c->format();
	if (fmt == NULL)
	{
		printf("Warning - No format defined in 'warn' command.\n");
		return FALSE;
	}
	if (fmt->writeToString()) msg.print("Warning: %s\n",fmt->string());
	return TRUE;
}