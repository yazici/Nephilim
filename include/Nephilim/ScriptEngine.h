#ifndef NephilimScriptEngine_h__
#define NephilimScriptEngine_h__

#include <Nephilim/Platform.h>
#include "Strings.h"
#include "ScriptEngineTask.h"

#include <map>
#include <vector>

NEPHILIM_NS_BEGIN	

class GameCore;
/**
	\ingroup Scripting
	\class ScriptEngine
	\brief Represents an entity capable of handling commands

	Its perfectly possible to register custom commands to it, so it even becomes an in-game console language.
*/	
class NEPHILIM_API ScriptEngine{
public:
	/// Creates an empty script engine and registers the default ones immediately
	ScriptEngine();

	/// Safely destroys itself and its child tasks
	~ScriptEngine();

	/// Run a command
	void run(const String &command);

	/// Hides all the default tasks at once, that means they cannot be executed anymore
	void hideDefaultTasks();

	/// Shows all the default tasks at once, that means you can call them again
	void showDefaultTasks();

	/// Hide a task from execution
	void hideTask(const String &task_name);

	/// Enables a task for execution back again
	void showTask(const String &task_name);

	/// Registers a task instance as an available command, responding by
	/// the name specified, case-insensitive
	void registerTask(String &name, ScriptEngineTask* task);

private:
	/// Registers all default commands
	void registerDefaultCommands();

	/// All commands registered
	std::map<String, ScriptEngineTask*> myCommands;

	/// All commands in execution
	std::vector<ScriptEngineTask*> myExecutionList;

	friend class GameCore;
	GameCore *myParent;
};

NEPHILIM_NS_END
#endif // NephilimScriptEngine_h__