module;

#include <string>
#include <memory>

export module Calc_CommandDispatcher:CommandInterpreter;

import CalcUtilities;

using std::string;

class CommandInterpreter {
    class CommandInterpreterImp;

public:
    explicit CommandInterpreter() = default; // Need impl this ctor with UserInterface& object
    ~CommandInterpreter() = default;

    void CommandEntered(const string& command);

private:
    CommandInterpreter(const CommandInterpreter&) = delete;
    CommandInterpreter(CommandInterpreter&&) = delete;
    CommandInterpreter& operator=(const CommandInterpreter&) = delete;
    CommandInterpreter& operator=(CommandInterpreter&&) = delete;

    std::unique_ptr<CommandInterpreterImp> _pimp;
};