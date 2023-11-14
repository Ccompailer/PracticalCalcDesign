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

export class Power : public BinaryCommand
{
public:
    Power() = default;
    ~Power() = default;

    explicit Power(const Power& rhs)
    : BinaryCommand {rhs}
    { }

private:
    Power(Power&&) = delete;
    Power& operator=(Power&&) = delete;
    Power& operator=(const Power&) = delete;

    void checkPreconditionsImp() const override {
        BinaryCommand::checkPreconditionsImp();

        if(auto v = Stack::Instance().GetElements(2); v[0] < 0 || v[1] < 0)
            throw Exception{"Invalid result"};
    }

    double binaryOperation(double next, double top)
    const noexcept override {
        return std::pow(next, top);
    }

    CLONE(Power);
    HELP("Replace first two elements of the stack, y, x, with y^x. Note, x is top of stack");
};

export class Root : public BinaryCommand
{
public:
    Root() = default;
    ~Root() = default;

    explicit Root(const Root& rhs)
    : BinaryCommand {rhs}
    { }

private:
    Root(Root&&) = delete;
    Root& operator=(const Root&) = delete;
    Root& operator=(Root&&) = delete;

    void checkPreconditionsImp() const override {
        BinaryCommand::checkPreconditionsImp();

        if(auto v = Stack::Instance().GetElements(2); v[0] < 0 || v[1] < 0)
            throw Exception{"Invalid result"};
    }

    double binaryOperation(double next, double top)
    const noexcept override {
        return std::pow(next, 1. / top);
    }

    CLONE(Root);
    HELP("Replace first two elements of the stack, y, x, with x root of y. Note, x is top of stack")
};

export class Sine : public UnaryCommand
{
public:
    Sine() = default;
    ~Sine() = default;

    explicit Sine(const Sine& rhs)
    : UnaryCommand {rhs}
    { }

private:
    Sine(Sine&&) = delete;
    Sine& operator=(const Sine&) = delete;
    Sine& operator=(Sine&&) = delete;

    double unaryOperation(double top)
    const noexcept override {
        return std::sin(top);
    }

    CLONE(Sine);
    HELP("Replace the first element x, on the stack with sin(x). x must be in radians");
};

export class Cosine : public UnaryCommand
{
public:
    Cosine() = default;
    ~Cosine() = default;

    explicit Cosine(const Cosine& rhs)
    : UnaryCommand {rhs}
    { }

private:
    Cosine(Cosine&&) = delete;
    Cosine& operator=(const Cosine&) = delete;
    Cosine& operator=(Cosine&&) = delete;

    double unaryOperation(double top)
    const noexcept override {
        return std::cos(top);
    }

    CLONE(Cosine);
    HELP("Replace the first element, x, on the stack with cos(x). x must be in radians");
};

export class Tangent : public UnaryCommand
{
public:
    Tangent() = default;
    ~Tangent() = default;

    explicit Tangent(const Tangent& rhs)
    : UnaryCommand {ths}
    { }

private:
    Tangent(Tangent&&) = delete;
    Tangent& operator=(const Tangent&) = delete;
    Tangent& operator=(Tangent&&) = delete;

    void checkPreconditionsImp() const override {
        UnaryCommand::checkPreconditionsImp();

        auto v = Stack::Instance().GetElements(1);
        double d { v.back() + M_PI / 2. };
        double r { std::fabs(d) / std::fabs(M_PI) };
        int w { static_cast<int>(std::floor(r + eps)) };

        r = r - w;

        if(r < eps && r > -eps)
            throw Exception{"Infinite result"};
    }

    double unaryOperation(double top)
    const noexcept override {
        return std::tan(top);
    }

    CLONE(Tangent);
    HELP("Replace the first element, x, on the stack with tan(x). x must be in radians");
};
