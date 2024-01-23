module;

#include "../Utilities/Exception.h"
#include <format>
#include <regex>
#include <algorithm>
#include <fstream>
#include <string_view>
#include <string>
#include <set>

module CommandInterpreter;

import CalcBackend_Command;
import CalcBackend_CommandManager;
import CalcBackend_CommandFactory;
import CalcUtilities;
import UserInterface;
import CalcBackend_CoreCommands;

using std::string;
using std::unique_ptr;
using std::pmr::set;
using std::string_view;


namespace Calculator {

    class CommandInterpreter::CommandInterpreterImpl {
    public:
        explicit CommandInterpreterImpl(UserInterface& ui);

        void executeCommand(const string &command);

    private:
        bool isNum(const string &, double &d);

        void handleCommand(CommandPtr command);

        void printHelp() const;

        CommandManager _manager;
        UserInterface& _ui;
    };


    CommandInterpreter::CommandInterpreterImpl::CommandInterpreterImpl(UserInterface &ui)
            : _ui(ui) {
    }

    void CommandInterpreter::CommandInterpreterImpl::executeCommand(const string &command) {
        string_view sv{command};

        if (double d; isNum(command, d))
            _manager.ExecuteCommand(MakeCommandPtr<EnterNumber>(d));
        else if (command == "undo")
            _manager.Undo();
        else if (command == "redo")
            _manager.Redo();
        else if (command == "help")
            printHelp();
        else if (command.size() > 6 && sv.starts_with("proc:")) {
            string filename{sv.substr(5, command.size() - 5)};
            handleCommand(MakeCommandPtr<StoredProcedure>(ui_, filename));
        } else {
            if (auto c = CommandFactory::Instance().AllocateCommand(command))
                handleCommand(std::move(c));
            else {
                auto t = std::format("Command {} is not a known command", command);
                _ui.PostMessage(t);
            }
        }
    }

    void CommandInterpreter::CommandInterpreterImpl::handleCommand(CommandPtr c) {
        try {
            _manager.ExecuteCommand(std::move(c));
        }
        catch (Exception &e) {
            _ui.PostMessage(e.What());
        }
    }

    void CommandInterpreter::CommandInterpreterImpl::printHelp() const {
        string help = "\n"
                      "undo: undo last operation\n"
                      "redo: redo last operation\n";

        auto allCommands = CommandFactory::Instance().GetAllCommandsNames();
        for (const auto &i: allCommands) {
            help += CommandFactory::Instance().HelpMessage(i) + "\n";
        }

        _ui.PostMessage(help);
    }

    bool CommandInterpreter::CommandInterpreterImpl::isNum(const string &s, double &d) {
        if (s == "+" || s == "-") return false;

        std::regex dpRegex("((\\+|-)?[[:digit:]]*)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?");
        bool isNumber{std::regex_match(s, dpRegex)};

        if (isNumber)
            d = std::stod(s);

        return isNumber;
    }

    CommandInterpreter::CommandInterpreter(UserInterface& ui) : pimpl_ {std::make_unique<CommandInterpreterImpl>(ui)} {

    }

    void CommandInterpreter::commandEntered(const string &command) {
        pimpl_->executeCommand(command);
    }

    CommandInterpreter::~CommandInterpreter() {

    }
}
