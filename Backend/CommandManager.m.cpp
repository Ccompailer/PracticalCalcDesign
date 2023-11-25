module;

#include <stack>
#include <vector>
#include <list>
#include <memory>

export module CalcBackend_CommandManager;

import CalcBackend_Command;

using std::unique_ptr;
using std::make_unique;
using std::stack;
using std::list;
using std::vector;

namespace CommandModule {

    export class CommandManager {

        class CommandManagerStrategy;
        class UndoRedoStackStrategy;
        class UndoRedoListStrategyVector;
        class UndoRedoListStrategy;

    public:
        enum class UndoRedoStrategy {
            ListStrategy, StackStrategy, ListStrategyVector
        };

        explicit CommandManager(UndoRedoStrategy st = UndoRedoStrategy::StackStrategy);

        ~CommandManager() = default;

        size_t GetUndoSize() const;
        size_t GetRedoSize() const;
        void ExecuteCommand(CommandPtr ptr);
        void Undo();
        void Redo();

    private:
        CommandManager(CommandManager &) = delete;
        CommandManager(CommandManager &&) = delete;
        CommandManager &operator=(const CommandManager &) = delete;
        CommandManager &operator=(CommandManager &&) = delete;

        unique_ptr<CommandManagerStrategy> _strategy;
    };

    class CommandManager::CommandManagerStrategy {
    public:
        virtual ~CommandManagerStrategy() = default;
        virtual size_t GetRedoSize() const = 0;
        virtual size_t GetUndoSize() const = 0;
        virtual void ExecuteCommand(CommandPtr ptr) = 0;
        virtual void Undo() = 0;
        virtual void Redo() = 0;
    };

    class CommandManager::UndoRedoStackStrategy :
            public CommandManager::CommandManagerStrategy {
    public:
        size_t GetRedoSize() const override { return _redoStack.size(); }
        size_t GetUndoSize() const override { return _undoStack.size(); }
        void ExecuteCommand(CommandPtr ptr) override;
        void Undo() override;
        void Redo() override;

    private:
        void FlushStack(stack<CommandPtr> &st);

        stack<CommandPtr> _undoStack;
        stack<CommandPtr> _redoStack;
    };

    void CommandManager::UndoRedoStackStrategy::ExecuteCommand(CommandPtr ptr) {
        ptr->execute();

        _undoStack.push(std::move(ptr));
        FlushStack(_redoStack);
    }

    void CommandManager::UndoRedoStackStrategy::Redo() {
        if (GetRedoSize() == 0)
            return;

        auto &c = _redoStack.top();
        c->execute();

        _undoStack.push(std::move(c));
        _redoStack.pop();
    }

    void CommandManager::UndoRedoStackStrategy::Undo() {
        if (GetUndoSize() == 0)
            return;

        auto &c = _undoStack.top();
        c->undo();

        _redoStack.push(std::move(c));
        _undoStack.pop();
    }

    void CommandManager::UndoRedoStackStrategy::FlushStack(stack<CommandPtr> &st) {
        while (!st.empty()) {
            st.pop();
        }
    }

    class CommandManager::UndoRedoListStrategyVector : public CommandManager::CommandManagerStrategy {
    public:
        UndoRedoListStrategyVector()
        : _cur{-1}
        ,_undoSize{0}
        ,_redoSize{0}
        {}

        size_t GetUndoSize() const override { return _undoSize; }
        size_t GetRedoSize() const override { return _redoSize; }
        void ExecuteCommand(CommandPtr ptr) override;
        void Undo() override;
        void Redo() override;

    private:
        void Flush();

        int _cur;
        size_t _undoSize;
        size_t _redoSize;
        vector<CommandPtr> _undoRedoList;
    };

    void CommandManager::UndoRedoListStrategyVector::ExecuteCommand(CommandPtr ptr) {
        ptr->execute();

        Flush();
        _undoRedoList.emplace_back(std::move(ptr));
        _cur = _undoRedoList.size() - 1;
        ++_undoSize;
        _redoSize = 0;
    }

    void CommandManager::UndoRedoListStrategyVector::Undo() {
        if (GetUndoSize() == 0)
            return;

        _undoRedoList[_cur]->undo();
        --_cur;
        --_undoSize;
        ++_redoSize;
    }

    void CommandManager::UndoRedoListStrategyVector::Redo() {
        if (GetRedoSize() == 0)
            return;

        ++_cur;
        _undoRedoList[_cur]->execute();
        --_redoSize;
        ++_undoSize;
    }

    void CommandManager::UndoRedoListStrategyVector::Flush() {
        if (!_undoRedoList.empty())
            _undoRedoList.erase(_undoRedoList.begin() + _cur + 1, _undoRedoList.end());
    }

    class CommandManager::UndoRedoListStrategy : public CommandManager::CommandManagerStrategy {
    public:
        UndoRedoListStrategy();

        size_t GetRedoSize() const override;

        size_t GetUndoSize() const override;

        void ExecuteCommand(CommandPtr ptr) override;

        void Undo() override;

        void Redo() override;

    private:
        void Flush();

        size_t _undoSize;
        size_t _redoSize;
        list<CommandPtr> _undoRedoList;
        list<CommandPtr>::iterator _cur;
    };

    CommandManager::UndoRedoListStrategy::UndoRedoListStrategy()
            : _undoSize{0}, _redoSize{0} {

        _undoRedoList.push_back(MakeCommandPtr(nullptr));
        _cur = _undoRedoList.end();
    }

    void CommandManager::UndoRedoListStrategy::ExecuteCommand(CommandModule::CommandPtr ptr) {
        ptr->execute();

        Flush();
        _undoRedoList.emplace_back(std::move(ptr));
        ++_undoSize;
        _redoSize = 0;
        _cur = _undoRedoList.end();
        --_cur;
    }

    void CommandManager::UndoRedoListStrategy::Undo() {
        if (_undoSize == 0)
            return;

        --_undoSize;
        ++_redoSize;
        (*_cur)->undo();
        --_cur;
    }

    void CommandManager::UndoRedoListStrategy::Redo() {
        if (_redoSize == 0)
            return;

        --_redoSize;
        ++_undoSize;
        ++_cur;
        (*_cur)->execute();
    }

    void CommandManager::UndoRedoListStrategy::Flush() {
        if (_cur != _undoRedoList.end())
        {
            auto i = _cur;
            ++i;
            _undoRedoList.erase(i, _undoRedoList.end());
        }
    }

    CommandManager::CommandManager(CommandModule::CommandManager::UndoRedoStrategy st) {
        switch (st) {
            case UndoRedoStrategy::ListStrategy:
                _strategy = make_unique<UndoRedoListStrategy>();
                break;
            case UndoRedoStrategy::StackStrategy:
                _strategy = make_unique<UndoRedoStackStrategy>();
                break;
            case UndoRedoStrategy::ListStrategyVector:
                _strategy = make_unique<UndoRedoListStrategyVector>();
                break;
        }
    }

    size_t CommandManager::GetRedoSize() const {
        return _strategy->GetRedoSize();
    }

    size_t CommandManager::GetUndoSize() const {
        return _strategy->GetUndoSize();
    }

    void CommandManager::ExecuteCommand(CommandPtr ptr) {
        _strategy->ExecuteCommand(std::move(ptr));
    }

    void CommandManager::Undo() {
        _strategy->Undo();
    }

    void CommandManager::Redo() {
        _strategy->Redo();
    }
}
