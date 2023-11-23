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

export class CommandManeger {
    class CommandManegerStrategy;
    class UndoRedoStackStrategy;
    class UndoRedoListStrategyVector;
    class UndoRedoListStrategy;

public:
    enum class UndoRedoStrategy { ListStrategy, StackStrategy, ListStrategyVector };

    explicit CommandManeger(UndoRedoStrategy st = UndoRedoStrategy::StackStrategy);
    ~CommandManeger() = default;

    size_t GetUndoSize() const;
    size_t GetRedoSize() const;

    void ExecuteCommand(/*Pointer CommandPtr*/);
    void undo();
    void redo();

private:
    CommandManeger(CommandManeger&) = delete;
    CommandManeger(CommandManeger&&) = delete;
    CommandManeger& operator=(const CommandManeger&) = delete;
    CommandManeger& operator=(CommandManeger&&) = delete;
};

class CommandManeger::CommandManegerStrategy {
public:
    virtual ~CommandManegerStrategy() = default;
    virtual size_t GetRedoSize() const = 0;
    virtual size_t GetUndoSize() const = 0;
    virtual void ExecuteCommand(CommandPtr ptr) = 0;
    virtual void Undo() = 0;
    virtual void Redo() = 0;
};

class CommandManeger::UndoRedoStackStrategy :
    public CommandManeger::CommandManegerStrategy {
public:
    size_t GetRedoSize() const override { return _redoStack.size(); }
    size_t GetUndoSize() const override { return _undoStack.size(); }

private:
    stack<CommandPtr> _undoStack;
    stack<CommandPtr> _redoStack;
};
