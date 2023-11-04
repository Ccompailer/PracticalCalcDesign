module;

#include <string>
#include <string_view>
#include <memory>
#include <functional>
#include <concepts>

export module CalcBackend_Command;

using std::string_view;
using std::string;
using std::unique_ptr;

export class Command
{
public:
    Command* clone() const;
    virtual ~Command() = default;
    void execute();
    void undo();
    const char* helpMessage() const;
    virtual void deallocate();

protected:
    Command();
    Command(Command&);

private:
    virtual void checkPreconditionsImp() const;
    virtual Command* cloneImp() const = 0;
    virtual void executeImp() noexcept = 0;
    virtual void undoImp() noexcept = 0;
    virtual const char* helpMessageImp() const noexcept = 0;

    Command(Command&&) = delete;
    Command& operator=(Command&) = delete;
    Command& operator=(Command&&) = delete;
};

class BinaryCommand : public Command
{
public:
    virtual ~BinaryCommand() = default;

protected:
    void checkPreconditionsImp() const override;

    BinaryCommand() = default;
    BinaryCommand(const BinaryCommand&);

private:
    BinaryCommand(BinaryCommand&&) = delete;
    BinaryCommand& operator=(BinaryCommand&) = delete;
    BinaryCommand& operator=(BinaryCommand&&) = delete;

    void executeImp() noexcept final override;
    void undoImp() noexcept final override;

    virtual double binaryOperation(double next, double top) const noexcept = 0;

    double _next;
    double _top;
};