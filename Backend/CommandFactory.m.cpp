module;

#include <memory>
#include <string>
#include <set>
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

namespace CommandModule {

    export class CommandFactory {
    public:
        static CommandFactory& Instance();
        void RegisterCommand(const string& name, CommandPtr ptr);
        CommandPtr DeregisterCommand(const string& name);
        size_t GetNumberCommand() const { _factory.size(); }
        CommandPtr AllocateCommand(const string& name) const;
        bool HasKey(const string& s) const { return _factory.contains(s); }
        std::set<string> GetAllCommandsNames() const;
        string HelpMessage(const string& command) const;
        void ClearAllCommands() { _factory.clear(); }

    private:
        CommandFactory() = default;
        ~CommandFactory() = default;
        CommandFactory(CommandFactory&) = delete;
        CommandFactory(CommandFactory&&) = delete;
        CommandFactory& operator=(const CommandFactory&) = delete;
        CommandFactory& operator=(CommandFactory&&) = delete;

        using Factory = unordered_map<string, CommandPtr>;
        Factory _factory;
    };
}