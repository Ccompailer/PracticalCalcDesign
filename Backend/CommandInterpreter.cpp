module;

#include <format>
#include <regex>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <stack>
#include <string_view>
#include <string>
#include <iterator>
#include <set>

module Calc_CommandDispatcher:CommandInterpreter;

import CalcBackend_Command;
import CalcUtilities;
import CalcBackend_CoreCommands;

using std::string;
using std::unique_ptr;
using std::pmr::set;
using std::string_view;

class CommandInterpreter::CommandInterpreterImp {
public:
    explicit CommandInterpreterImp() = default; // Use UserInterface& object in the ctor;

    void executeCommand(const string& command);

private:
    bool IsNum(const string&, double& d);
    void HandlCommand();
    void PrintHelp() const;

    //2 private field CommandManager and UserInterface&;
};