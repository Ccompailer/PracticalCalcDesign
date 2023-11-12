module;

#include <string>
#include <stack>
#include <cassert>
#include <iostream>
#include "../Utilities/Exception.h"
#include <vector>
#include <cmath>

export module CalcBackend_CoreCommands;

import CalcBackend_Stack;
import CalcBackend_Command;
import CalcUtilities;

using std::vector;
using std::string;

#define CLONE(X) X* cloneImp() const override { return new X { *this }; }
#define HELP(X) const char* helpMessageImp() const noexcept override { return X; }


double eps = 1e-12;

bool TopOfStackIsBetween(double lb, double ub) {
    assert(lb <= ub);

    auto v = Stack::Instance().GetElements(1);
    auto d = v.back();

    return d >= lb && d <= ub;
}

bool PassesPowerTest(double y, double x) {
    if(y == 0 && x < 0)
        return false;

    if(double intPart; y < 0 && std::modf(x, &intPart) != 0.0)
        return false;

    return true;
}

export class EnterNumber : public Command
{
public:
    explicit EnterNumber(double d)
    : Command {}
    , _number(d)
    { }

    explicit EnterNumber(const EnterNumber& rhs)
    : Command(rhs)
    , _number(rhs._number)
    { }

    ~EnterNumber() = default;

private:
    EnterNumber(EnterNumber&&) = delete;
    EnterNumber& operator=(const EnterNumber&) = delete;
    EnterNumber& operator=(EnterNumber&&) = delete;

    void executeImp() noexcept override {
        Stack::Instance().Push(_number);
    }

    void undoImp() noexcept override {
        Stack::Instance().Pop();
    }

    CLONE(EnterNumber);
    HELP("Adds a number to the stack");

    double _number;
};

export class SwapTopOfStack : public Command
{
public:
    SwapTopOfStack() = default;

    explicit SwapTopOfStack(const SwapTopOfStack& rhs)
    : Command {rhs}
    { }

    ~SwapTopOfStack() = default;

private:
    SwapTopOfStack(SwapTopOfStack&&) = delete;
    SwapTopOfStack& operator=(const SwapTopOfStack&) = delete;
    SwapTopOfStack& operator=(SwapTopOfStack&&) = delete;

    void checkPreconditionsImp() const override {
        if(Stack::Instance().Size() < 2)
            throw Exception{"Stack must have 2 elements"};
    }

    void executeImp() noexcept override {
        Stack::Instance().SwapTop();
    }

    void undoImp() noexcept override {
        Stack::Instance().SwapTop();
    }

    CLONE(SwapTopOfStack);
    HELP("Swap the top two elements of the stack");
};

export class DropTopOfStack : public Command
{
public:
    DropTopOfStack() = default;
    ~DropTopOfStack() = default;

    explicit DropTopOfStack(const DropTopOfStack& rhs)
    : Command {rhs}
    , _droppedNumber(rhs._droppedNumber)
    { }

private:
    DropTopOfStack(DropTopOfStack&&) = delete;
    DropTopOfStack& operator=(const DropTopOfStack&) = delete;
    DropTopOfStack& operator=(DropTopOfStack&&) = delete;

    void checkPreconditionsImp() const override {
        if(Stack::Instance().Size() < 1)
            throw Exception{"Stack must have 1 element"};
    }

    void executeImp() noexcept override {
        _droppedNumber = Stack::Instance().Pop();
    }

    void undoImp() noexcept override {
        Stack::Instance().Push(_droppedNumber);
    }

    CLONE(DropTopOfStack);
    HELP("Drop the top element from the stack");

    double _droppedNumber;
};

export class ClearStack : public Command
{
public:
    ClearStack() = default;
    ~ClearStack() = default;

    explicit ClearStack(const ClearStack& rhs)
    : Command {rhs}
    , _stack(rhs._stack)
    { }

private:
    ClearStack(ClearStack&&) = delete;
    ClearStack& operator=(const ClearStack&) = delete;
    ClearStack& operator=(ClearStack&&) = delete;

    void executeImp() noexcept override {
        const auto n = Stack::Instance().Size();

        if(n == 0)
            return;

        for (auto i = 0u; i < n - 1; ++i) {
            _stack.push(Stack::Instance().Pop(true));
        }

        _stack.push(Stack::Instance().Pop(false));
    }

    void undoImp() noexcept override {
        const auto n = _stack.size();

        if(n == 0)
            return;

        for(auto i = 0u; i < n - 1; ++i) {
            Stack::Instance().Push(_stack.top(), true);
        }

        Stack::Instance().Push(_stack.top(), false);
    }

    CLONE(ClearStack);
    HELP("Clear the stack");

    std::stack<double> _stack;
};

export class Add : public BinaryCommand
{
public:
    Add() = default;
    ~Add() = default;

    explicit Add(const Add& rhs)
    : BinaryCommand {rhs}
    { }

private:
    Add(Add&&) = delete;
    Add& operator=(const Add&) = delete;
    Add& operator=(Add&&) = delete;

    double binaryOperation(double next, double top)
    const noexcept override {
        return next + top;
    }

    CLONE(Add);
    HELP("Replace first two elements of the stack with their sum");
};

export class Subtract : public BinaryCommand
{
public:
    Subtract() = default;
    ~Subtract() = default;

    explicit Subtract(const Subtract& rhs)
    : BinaryCommand {rhs}
    { }

private:
    Subtract(Subtract&&) = delete;
    Subtract& operator=(const Subtract&) = delete;
    Subtract& operator=(Subtract&&) = delete;

    double binaryOperation(double next, double top)
    const noexcept override {
        return next - top;
    }

    CLONE(Subtract);
    HELP("Replace first two elements of the stack with their difference");
};

export class Divide : public BinaryCommand
{
public:
    Divide() = default;
    ~Divide() = default;

    explicit Divide(const Divide& rhs)
    : BinaryCommand {rhs}
    { }

private:
    Divide(Divide&&) = delete;
    Divide& operator=(const Divide&) = delete;
    Divide& operator=(Divide&&) = delete;

    void checkPreconditionsImp() const override {
        BinaryCommand::checkPreconditionsImp();

        if(auto v = Stack::Instance().GetElements(1); v.front() == 0. || v.front() == -0.)
            throw Exception{"Division by zero"};
    }

    double binaryOperation(double next, double top)
    const noexcept override {
        return next / top;
    }

    CLONE(Divide);
    HELP("Replace first two elements of the stack with their quotient";)
};










