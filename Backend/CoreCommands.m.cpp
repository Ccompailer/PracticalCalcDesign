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












