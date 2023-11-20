module;

#include <stack>
#include <vector>
#include <list>
#include <memory>

export module CalcBackend_CommandManager;

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
