module;

#include <string_view>
#include <functional>
#include "../Utilities/Exception.h"

module CalcBackend_Command;

import CalcUtilities;
import CalcBackend_Stack;

using std::string_view;

void Command::execute() {
    checkPreconditionsImp();
    executeImp();
}

void Command::undo() {
    undoImp();
}

Command* Command::clone() const {
    return cloneImp();
}

void Command::deallocate() {
    delete this;
}

void Command::checkPreconditionsImp() const { }

BinaryCommand::BinaryCommand(const BinaryCommand& rhs) :
Command(rhs), _top(rhs._top), _next(rhs._next)
{ }

void BinaryCommand::checkPreconditionsImp() const {
    if(Stack::Instance().Size() < 2)
        throw Exception {"Stack must have least two elements"};
}

void BinaryCommand::executeImp() noexcept {
    _top = Stack::Instance().Pop(true);
    _next = Stack::Instance().Pop(true);
    Stack::Instance().Push(binaryOperation(_next, _top));
}

void BinaryCommand::undoImp() noexcept {
    Stack::Instance().Pop(true);
    Stack::Instance().Push(_next, true);
    Stack::Instance().Push(_top);
}

UnaryCommand::UnaryCommand(const UnaryCommand& rhs) :
Command(rhs), _top(rhs._top) { }

void UnaryCommand::checkPreconditionsImp() const {
    if(Stack::Instance().Size() < 1)
        throw Exception {"Stack must have at least one element"};
}

void UnaryCommand::executeImp() noexcept {
    _top = Stack::Instance().Pop(true);
    Stack::Instance().Push(unaryOperation(_top));
}

void UnaryCommand::undoImp() noexcept {
    Stack::Instance().Pop(true);
    Stack::Instance().Push(_top);
}

void PluginCommand::checkPreconditionsImp() const {
    if(const char* p = checkPluginPreconditions())
        throw Exception {p};
}

PluginCommand* PluginCommand::cloneImp() const {
    if(auto p = clonePluginImp())
        return p;
    else
        throw Exception {"Problem cloning a plugin command"};
}

BinaryCommandAlternative::BinaryCommandAlternative(
        std::string_view help,
        std::function<BinaryCommandAlternative::BinaryCommandOp> func) :
_helpMessage(help), _command(func) { }

BinaryCommandAlternative::BinaryCommandAlternative(const BinaryCommandAlternative& rhs) :
Command(rhs),
_top(rhs._top),
_next(rhs._next),
_helpMessage(rhs._helpMessage),
_command(rhs._command)
{ }

const char* BinaryCommandAlternative::helpMessageImp() const noexcept {
    return _helpMessage.c_str();
}

BinaryCommandAlternative* BinaryCommandAlternative::cloneImp() const {
    return new BinaryCommandAlternative(*this);
}

void BinaryCommandAlternative::executeImp() noexcept {
    _top = Stack::Instance().Pop(true);
    _next = Stack::Instance().Pop(true);
    Stack::Instance().Push(_command(_next, _top));
}

void BinaryCommandAlternative::undoImp() noexcept {
    Stack::Instance().Pop(true);
    Stack::Instance().Push(_next, true);
    Stack::Instance().Push(_top);
}
