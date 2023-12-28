module;

#include <string>
#include <iostream>
#include <memory>
#include <string_view>

export module UserInterface;

import CalcUtilities;

using std::unique_ptr;
using std::string;
using std::string_view;
using std::istream;
using std::ostream;

namespace Calculator {

    export class UserInterface : protected Publisher {
    public:
        UserInterface() {RegisterEvent(CommandEntered());}
        virtual ~UserInterface() = default;
        virtual void PostMessage(string_view message) = 0;
        virtual void StackChanged() = 0;

        using Publisher::Attach;
        using Publisher::Detach;

        static string CommandEntered() {return "CommandIssued";}
    };

    export class Cli : public UserInterface {
    public:
        Cli(istream&, ostream&);
        ~Cli() = default;

        void Execute(bool suppressStartupMessage = false, bool echo = false);

    private:
        void PostMessage(std::string_view message) override;
        void StackChanged() override;
        void StartupMessage();

        Cli(const Cli&) = delete;
        Cli(Cli&&) = delete;
        Cli& operator=(Cli&) = delete;
        Cli& operator=(Cli&&) = delete;

        istream _istream;
        ostream _ostream;
    };
}
