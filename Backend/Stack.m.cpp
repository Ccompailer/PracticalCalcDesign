module;

#include <vector>
#include <string>
#include <deque>
#include "../Utilities/Exception.h"

export module CalcBackend_Stack;

import CalcUtilities;

using std::string;
using std::vector;
using std::deque;

namespace Calculator {
    export class StackErrorData {
    public:
        enum class ErrorConditions {
            Empty, TooFewArguments
        };

        explicit StackErrorData(ErrorConditions e) : _err(e) {}

        static const char *Message(ErrorConditions ec);

        const char *Message() const;

        ErrorConditions Error() const { return _err; }

    private:
        ErrorConditions _err;
    };

    export class Stack : private Publisher {
    public:
        static Stack &Instance();

        void Push(double, bool suppressChangeEvent = false);

        double Pop(bool suppressChangeEvent = false);

        void SwapTop();

        vector<double> GetElements(size_t n) const;

        void GetElements(size_t n, vector<double> &) const;

        using Publisher::Attach;
        using Publisher::Detach;

        size_t Size() const { return _stack.size(); }

        void Clear();

        static string StackChanged();

        static string StackError();

    private:
        Stack();

        ~Stack() = default;

        Stack(const Stack &) = delete;

        Stack(Stack &&) = delete;

        Stack &operator=(Stack &) = delete;

        Stack &operator=(Stack &&) = delete;

        deque<double> _stack;
    };

    string Stack::StackChanged() {
        return "Stack changed!";
    }

    string Stack::StackError() {
        return "Error";
    }

    const char *StackErrorData::Message(StackErrorData::ErrorConditions ec) {
        switch (ec) {
            case ErrorConditions::Empty:
                return "Attempting to pop empty stack";
            case ErrorConditions::TooFewArguments:
                return "Need at least two stack element to swap top";
            default:
                return "Unknown error";
        }
    }

    const char *StackErrorData::Message() const {
        return Message(_err);
    }

    void Stack::Push(double d, bool suppressChangeEvent) {
        _stack.push_back(d);

        if (!suppressChangeEvent)
            Raise(Stack::StackChanged(), nullptr);
    }

    double Stack::Pop(bool suppressChangeEvent) {
        if (_stack.empty()) {
            Raise(
                    Stack::StackError(),
                    StackErrorData{StackErrorData::ErrorConditions::Empty}
            );
            throw Exception{
                    StackErrorData::Message(StackErrorData::ErrorConditions::Empty)
            };
        } else {
            auto value = _stack.back();
            _stack.pop_back();

            if (!suppressChangeEvent)
                Raise(Stack::StackChanged(), nullptr);

            return value;
        }
    }

    void Stack::SwapTop() {
        if (_stack.size() < 2) {
            Raise(
                    Stack::StackError(),
                    StackErrorData{StackErrorData::ErrorConditions::TooFewArguments}
            );
            throw Exception{
                    StackErrorData::Message(StackErrorData::ErrorConditions::TooFewArguments)
            };
        } else {
            auto first = _stack.back();
            _stack.pop_back();
            auto second = _stack.back();
            _stack.pop_back();
            _stack.push_back(first);
            _stack.push_back(second);

            Raise(Stack::StackChanged(), nullptr);
        }
    }

    void Stack::GetElements(size_t n, vector<double> &vec) const {
        if (n > _stack.size())
            n = _stack.size();

        vec.insert(vec.end(), _stack.rbegin(), _stack.rbegin() + n);
    }

    vector<double> Stack::GetElements(size_t n) const {
        vector<double> vec;

        GetElements(n, vec);
        return vec;
    }

    void Stack::Clear() {
        _stack.clear();
        Raise(Stack::StackChanged(), nullptr);
    }

    Stack &Stack::Instance() {
        static Stack instance;
        return instance;
    }

    Stack::Stack() {
        RegisterEvent(StackChanged());
        RegisterEvent(StackError());
    }
}