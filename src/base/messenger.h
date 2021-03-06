/*
	*** Messaging Routines
	*** src/base/messenger.h
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

#ifndef ATEN_MESSENGER_H
#define ATEN_MESSENGER_H

#include "base/namespace.h"
#include "base/task.hui"
#include "base/message.h"
#include "templates/reflist.h"
#include <QDateTime>

// Forward Declarations (Aten)
class AtenProgress;

ATEN_BEGIN_NAMESPACE

// Global messaging and program output levels
class Messenger
{
	public:
	// Constructor
	Messenger();
	// Messaging output types
	enum OutputType { All, Calls, Commands, Parse, Typing, Verbose, nOutputTypes };
	// Convert text string to debug mode
	static OutputType outputType(QString s, bool reportError = false);
	// Convert debug mode to text string
	static const char* outputType(OutputType ot);


	/*
	 * Output Control
	 */
	private:
	// Bitvector of active output levels
	static int outputTypes_;
	// Call level, used to provide indent for enter/exit info
	static int callLevel_;
	// Quiet mode
	static bool quiet_;
	// Print all messages to console (i.e. when GUI does not yet exist)
	static bool printToConsole_;

	public:
	// Add an output type to the output bitvector
	static void addOutputType(Messenger::OutputType outputType);
	// Remove an output type from the output bitvector
	static void removeOutputType(Messenger::OutputType outputType);
	// Returns whether the specified output type is active
	static bool isOutputActive(Messenger::OutputType outputType);
	// Set status of quiet mode
	static void setQuiet(bool quiet);
	// Return status of quiet mode
	static bool isQuiet();
	// Set status console printing
	static void setPrintToConsole(bool printToConsole);


	/*
	 * Messaging functions
	 */
	private:
	// Maximum size of string list buffer
	static int bufferSize_;
	// List of recent messages
	static QList<Message> messageBuffer_;
	// Logpoint for message buffer
	static int messageBufferPoint_;

	private:
	// Add message to buffer
	static void addToBuffer(QString message, Message::MessageType type = Message::NormalMessage);
	// Output message to console
	static void outputMessage(QString message);

	public:
	// Clear message buffer
	static void clearMessageBuffer();
	// Return list of messages in buffer
	static QList<Message>& messageBuffer();
	// Return number of lines currently in buffer
	static int nMessagesBuffered();
	// Print formatted normal message
	static void print(const char* fmtString, ...);
	// Print formatted warning message
	static void warn(const char* fmtString, ...);
	// Print formatted warning message
	static void error(const char* fmtString, ...);
	// Print normal message
	static void print(QString message);
	// Print warning message
	static void warn(QString message);
	// Print error message
	static void error(QString message);
	// Print normal message in specific output level
	static void print(Messenger::OutputType outputType, const char* fmtString, ...);
	// Print warning message in specific output level
	static void warn(Messenger::OutputType outputType, const char* fmtString, ...);
	// Print error message in specific output level
	static void error(Messenger::OutputType outputType, const char* fmtString, ...);
	// Entrance to subroutine
	static void enter(const char* callName);
	// Exit from subroutine
	static void exit(const char* callName);


	/*
	 * Progress Indication
	 */
	private:
	// Pointer to custom progress dialog in GUI (if available)
	static AtenProgress* atenProgress_;
	// Stack of current tasks
	static RefList<Task,int> tasks_;
	// Point at which task list was updated (new task added, old task completed, or list cleared)
	static int taskPoint_;
	// Current CLI progress text
	static QString cliProgressText_;
	// Buffer point at which a message was last backprinted as a result of showCLIProgress
	static int backPrintedMessagePoint_;
	// Time at which first (main) task was started
	static QDateTime tasksStartTime_;
	// Minimum amount of time that needs to have elapsed (in msecs) before the progress dialog is shown
	static int minimumDialogShowTime_;

	private:
	// Whether the progress indicator should be shown
	static bool progressIndicatorRequired();
	// Show/update CLI progress indicator
	static void showCLIProgress();

	public:
	// Set pointer to custom progress dialog in GUI
	static void setAtenProgress(AtenProgress* atenProgress);
	// Push new task onto stack
	static Task* initialiseTask(QString title, int totalSteps);
	// Push new command task onto stack
	static Task* initialiseCommandTask(QString title, QString command, QString args, QString outputFile = QString());
	// Return number of current tasks
	static int nTasks();
	// Return list of current tasks
	static RefListItem<Task,int>* tasks();
	// Flag all tasks as being canceled
	static void cancelAllTasks();
	// Return task log point
	static int taskPoint();
	// Update specified task
	static bool updateTaskProgress(Task* task, int newCurrentStep);
	// Update specified task
	static bool incrementTaskProgress(Task* task, int deltaSteps = 1);
	// Terminate specifed task
	static void terminateTask(Task* task);
};

ATEN_END_NAMESPACE

#endif
