module;

#include <format>
#include <regex>
#include <algorithm>
#include <fstream>
#include <string_view>
#include <string>
#include <set>

module Calc_CommandDispatcher:CommandInterpreter;

import CalcBackend_Command;
import CalcUtilities;
import CalcBackend_CoreCommands;

using std::string;
using std::unique_ptr;
using std::pmr::set;
using std::string_view;

class CommandInterpreter::CommandInterpreterImpl {
public:
    explicit CommandInterpreterImpl(UserInterface&ui);

    void executeCommand(const string&command);

private:
    bool isNum(const string&, double&d);

    void handleCommand(CommandPtr command);

    void printHelp() const;

    CommandManager manager_;
    UserInterface&ui_;
};

CommandInterpreter::CommandInterpreterImpl::CommandInterpreterImpl(UserInterface&ui)
    : ui_(ui) {
}

void CommandInterpreter::CommandInterpreterImpl::executeCommand(const string&command) {
    string_view sv{command};

    // entry of a number simply goes onto the the stack
    if (double d; isNum(command, d))
        manager_.executeCommand(MakeCommandPtr<EnterNumber>(d));
    else if (command == "undo")
        manager_.undo();
    else if (command == "redo")
        manager_.redo();
    else if (command == "help")
        printHelp();
    else if (command.size() > 6 && sv.starts_with("proc:")) {
        string filename{sv.substr(5, command.size() - 5)};
        handleCommand(MakeCommandPtr<StoredProcedure>(ui_, filename));
    }
    else {
        if (auto c = CommandFactory::Instance().allocateCommand(command)) {
            handleCommand(std::move(c));
        }
        else {
            auto t = std::format("Command {} is not a known command", command);
            ui_.postMessage(t);
        }
    }

    return;
}

void CommandInterpreter::CommandInterpreterImpl::handleCommand(CommandPtr c) {
    try {
        manager_.executeCommand(std::move(c));
    }
    catch (Exception&e) {
        ui_.postMessage(e.what());
    }

    return;
}

void CommandInterpreter::CommandInterpreterImpl::printHelp() const {
    string help = "\n"
            "undo: undo last operation\n"
            "redo: redo last operation\n";

    set<string> allCommands = CommandFactory::Instance().getAllCommandNames();
    for (auto i: allCommands) {
        help += CommandFactory::Instance().helpMessage(i) + "\n";
    }

    ui_.postMessage(help);

    return;
}

// uses a C++11 regular expression to check if this is a valid double number
// if so, converts it into one and returns it
bool CommandInterpreter::CommandInterpreterImpl::isNum(const string&s, double&d) {
    if (s == "+" || s == "-") return false;

    std::regex dpRegex("((\\+|-)?[[:digit:]]*)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?");
    bool isNumber{std::regex_match(s, dpRegex)};

    if (isNumber) {
        d = std::stod(s);
    }

    return isNumber;
}

void CommandInterpreter::commandEntered(const string&command) {
    pimpl_->executeCommand(command);

    return;
}

CommandInterpreter::CommandInterpreter(UserInterface&ui)
    : pimpl_{std::make_unique<CommandInterpreterImpl>(ui)} {
}

CommandInterpreter::~CommandInterpreter() {
}

}
