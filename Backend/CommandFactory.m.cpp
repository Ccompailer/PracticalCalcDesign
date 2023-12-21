module;

#include <memory>
#include <string>
#include <set>
#include "../Utilities/Exception.h"
#include <format>
#include <unordered_map>
#include <algorithm>
#include <ranges>

export module CalcBackend_CommandFactory;

import CalcUtilities;
import CalcBackend_Command;
import CalcBackend_CoreCommands;

using std::string;
using std::unordered_map;
using std::set;

namespace Calculator {

    export class CommandFactory {
    public:
        static CommandFactory &Instance();

        void RegisterCommand(const string &name, CommandPtr ptr);

        CommandPtr DeregisterCommand(const string &name);

        size_t GetNumberCommand() const { _factory.size(); }

        CommandPtr AllocateCommand(const string &name) const;

        bool HasKey(const string &s) const { return _factory.contains(s); }

        std::set<string> GetAllCommandsNames() const;

        string HelpMessage(const string &command) const;

        void ClearAllCommands() { _factory.clear(); }

    private:
        CommandFactory() = default;

        ~CommandFactory() = default;

        CommandFactory(CommandFactory &) = delete;

        CommandFactory(CommandFactory &&) = delete;

        CommandFactory &operator=(const CommandFactory &) = delete;

        CommandFactory &operator=(CommandFactory &&) = delete;

        using Factory = unordered_map<string, CommandPtr>;
        Factory _factory;
    };

    std::set<string> CommandFactory::GetAllCommandsNames() const {
        set<string> temp;

        std::ranges::for_each(_factory | std::views::keys, [&temp](const auto &k) { temp.insert(k); });
        return temp;
    }

    string CommandFactory::HelpMessage(const std::string &command) const {
        auto it = _factory.find(command);

        return std::format(
                "{}: {}",
                command,
                it != _factory.end()
                ? it->second->helpMessage()
                : "No help entry found"
        );
    }

    void CommandFactory::RegisterCommand(const std::string &name, Calculator::CommandPtr ptr) {
        if (HasKey(name)) {
            auto t = std::format("Command {} already registered", name);
            throw Exception{"t"};
        }
    }

    CommandPtr CommandFactory::DeregisterCommand(const std::string& name) {
        if (HasKey(name))
        {
            auto i = _factory.find(name);
            auto temp = MakeCommandPtr(i->second.release());
            _factory.erase(i);
            return temp;
        } else
            return MakeCommandPtr(nullptr);
    }

    CommandPtr CommandFactory::AllocateCommand(const std::string& name) const {
        if (HasKey(name))
        {
            const auto& command = _factory.find(name)->second;
            return MakeCommandPtr(command->clone());
        } else
            return MakeCommandPtr(nullptr);
    }

    CommandFactory& CommandFactory::Instance() {
        static CommandFactory instance;
        return instance;
    }

    export void RegisterCoreCommands(/*UserInterface& ui*/) {
        auto& cr = CommandFactory::Instance();

        try {
            cr.RegisterCommand("Swap", MakeCommandPtr<SwapTopOfStack>());
            cr.RegisterCommand("Drop", MakeCommandPtr<DropTopOfStack>());
            cr.RegisterCommand("Clear", MakeCommandPtr<ClearStack>());
            cr.RegisterCommand("+", MakeCommandPtr<Add>());
            cr.RegisterCommand("-", MakeCommandPtr<Subtract>());
            cr.RegisterCommand("/", MakeCommandPtr<Divide>());
            cr.RegisterCommand("Pow", MakeCommandPtr<Power>());
            cr.RegisterCommand("Root", MakeCommandPtr<Root>());
            cr.RegisterCommand("Sin", MakeCommandPtr<Sine>());
            cr.RegisterCommand("Cos", MakeCommandPtr<Cosine>());
            cr.RegisterCommand("Tan", MakeCommandPtr<Tangent>());
            cr.RegisterCommand("ArcSin", MakeCommandPtr<Arcsine>());
            cr.RegisterCommand("ArcCos", MakeCommandPtr<Arccosine>());
            cr.RegisterCommand("ArcTan", MakeCommandPtr<Arctangent>());
            cr.RegisterCommand("Neg", MakeCommandPtr<Negate>());
            cr.RegisterCommand("Dup", MakeCommandPtr<Duplicate>());
            cr.RegisterCommand("*", MakeCommandPtr<BinaryCommandAlternative>(
                    "Replace first two elements on the stack with their product",
                    [](double x, double y) -> double { return x * y; }));

        } catch (Exception& exep) {
            // ui.PostMessage(e.What());
        }
    }
}