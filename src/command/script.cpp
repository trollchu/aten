/*
	*** Script Commands
	*** src/command/script.cpp
	Copyright T. Youngs 2007-2018

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

#include "command/commands.h"
#include "parser/commandnode.h"
#include "main/aten.h"
#include "gui/mainwindow.h"

ATEN_USING_NAMESPACE

// List available scripts
bool Commands::function_ListScripts(CommandNode* c, Bundle& obj, ReturnValue& rv)
{
	if (aten_.nScripts() == 0) Messenger::print("No scripts loaded.");
	else Messenger::print("Currently loaded scripts:");
	for (Program* prog = aten_.scripts(); prog != NULL; prog = prog->next) Messenger::print("  %s (%s)", qPrintable(prog->filename()), qPrintable(prog->name()));
	rv.reset();
	return true;
}

// Load script from disk
bool Commands::function_LoadScript(CommandNode* c, Bundle& obj, ReturnValue& rv)
{
	Program* prog = aten_.addScript();
	if (!prog->generateFromFile(c->argc(0), "ScriptFile"))
	{
		aten_.removeScript(prog);
		return false;
	}
	if (c->hasArg(1)) prog->setName(c->argc(1));
	else prog->setName(c->argc(0));
	rv.reset();

	// Update GUI
	aten_.atenWindow()->updateWidgets(AtenWindow::ToolsPanelTarget);

	return true;
}

// Run specified script
bool Commands::function_RunScript(CommandNode* c, Bundle& obj, ReturnValue& rv)
{
	// Find the script...
	Program* prog;
	for (prog = aten_.scripts(); prog != NULL; prog = prog->next) if (c->argc(0) == prog->name()) break;
	if (prog != NULL)
	{
		Messenger::print("Executing script '%s':", qPrintable(c->argc(0)));
		ReturnValue result;
		prog->execute(result);
	}
	else Messenger::print("Couldn't find script '%s'.", qPrintable(c->argc(0)));
	rv.reset();
	return true;
}
