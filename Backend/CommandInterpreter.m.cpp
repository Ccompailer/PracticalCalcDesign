module;

#include <string>
#include <memory>

export module CommandInterpreter;

import CalcUtilities;
import UserInterface;

using std::string;

export namespace Calculator {

    class CommandInterpreter {
        class CommandInterpreterImpl;

    public:
        explicit CommandInterpreter(UserInterface&);
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
