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

export class CommandManager {
    class CommandManagerStrategy;
    class UndoRedoStackStrategy;
    class UndoRedoListStrategyVector;
    class UndoRedoListStrategy;

public:
    enum class UndoRedoStrategy { ListStrategy, StackStrategy, ListStrategyVector };

    explicit CommandManager(UndoRedoStrategy st = UndoRedoStrategy::StackStrategy);
    ~CommandManager() = default;

    size_t GetUndoSize() const;
    size_t GetRedoSize() const;

    void ExecuteCommand(/*Pointer CommandPtr*/);
    void undo();
    void redo();

private:
    CommandManager(CommandManager&) = delete;
    CommandManager(CommandManager&&) = delete;
    CommandManager& operator=(const CommandManager&) = delete;
    CommandManager& operator=(CommandManager&&) = delete;
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
    void FlushStack(stack<CommandPtr>& st);

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

    auto& c = _redoStack.top();
    c->execute();

    _undoStack.push(std::move(c));
    _redoStack.pop();
}

void CommandManager::UndoRedoStackStrategy::Undo() {
    if (GetUndoSize() == 0)
        return;

    auto& c = _undoStack.top();
    c->undo();

    _redoStack.push(std::move(c));
    _undoStack.pop();
}

void CommandManager::UndoRedoStackStrategy::FlushStack(stack<CommandPtr> &st) {
    while (!st.empty())
    {
        st.pop();
    }
}

class CommandManager::UndoRedoListStrategyVector : public CommandManager::CommandManagerStrategy {
public:
    UndoRedoListStrategyVector()
    : _cur{-1},
    _undoSize{0},
    _redoSize{0}
    { }

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