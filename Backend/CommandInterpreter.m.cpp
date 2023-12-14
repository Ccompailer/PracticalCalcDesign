module;

#include <string>
#include <memory>

export module Calc_CommandDispatcher:CommandInterpreter;

import CalcUtilities;

using std::string;

export namespace Calculator {

    class CommandInterpreter {
         class CommandInterpreterImpl;

    public:
        explicit CommandInterpreter();

        ~CommandInterpreter();

        void commandEntered(const string &command);

    private:
        CommandInterpreter(const CommandInterpreter &) = delete;

        CommandInterpreter(CommandInterpreter &&) = delete;

        CommandInterpreter &operator=(const CommandInterpreter &) = delete;

        CommandInterpreter &operator=(CommandInterpreter &&) = delete;


        std::unique_ptr<CommandInterpreterImpl> pimpl_;
    };
}
