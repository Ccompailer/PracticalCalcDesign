module;

#include <memory>
#include <functional>
#include <concepts>

export module CalcBackend_Command;

using std::string_view;
using std::string;
using std::unique_ptr;

export namespace Calculator {

    class Command {
    public:
        Command *clone() const;

        virtual ~Command() = default;

        void execute();

        void undo();

        const char *helpMessage() const;

        virtual void deallocate();

    protected:
        Command();

        Command(const Command &);

    private:
        virtual void checkPreconditionsImp() const;

        virtual Command *cloneImp() const = 0;

        virtual void executeImp() noexcept = 0;

        virtual void undoImp() noexcept = 0;

        virtual const char *helpMessageImp() const noexcept = 0;

        Command(Command &&) = delete;

        Command &operator=(Command &) = delete;

        Command &operator=(Command &&) = delete;
    };

    class BinaryCommand : public Command {
    public:
        virtual ~BinaryCommand() = default;

    protected:
        void checkPreconditionsImp() const override;

        BinaryCommand() = default;

        BinaryCommand(const BinaryCommand &);

    private:
        BinaryCommand(BinaryCommand &&) = delete;

        BinaryCommand &operator=(BinaryCommand &) = delete;

        BinaryCommand &operator=(BinaryCommand &&) = delete;

        void executeImp() noexcept final override;

        void undoImp() noexcept final override;

        virtual double binaryOperation(double next, double top) const noexcept = 0;

        double _next;
        double _top;
    };

    class UnaryCommand : public Command {
    public:
        virtual ~UnaryCommand() = default;

    protected:
        void checkPreconditionsImp() const override;

        UnaryCommand() = default;

        UnaryCommand(const UnaryCommand &);

    private:
        UnaryCommand(UnaryCommand &&) = delete;

        UnaryCommand &operator=(UnaryCommand &) = delete;

        UnaryCommand &operator=(UnaryCommand &&) = delete;

        void executeImp() noexcept final override;

        void undoImp() noexcept final override;

        virtual double unaryOperation(double top) const noexcept = 0;

        double _top;
    };

    class PluginCommand : public Command {
    public:
        virtual ~PluginCommand() = default;

    private:
        virtual const char *checkPluginPreconditions() const noexcept = 0;

        virtual PluginCommand *clonePluginImp() const noexcept = 0;

        void checkPreconditionsImp() const override final;

        PluginCommand *cloneImp() const override final;
    };

    class BinaryCommandAlternative final : public Command {
        using BinaryCommandOp = double(double, double);

    public:
        BinaryCommandAlternative(string_view help, std::function<BinaryCommandOp> f);

        ~BinaryCommandAlternative() = default;

    private:
        BinaryCommandAlternative(BinaryCommandAlternative &&) = delete;

        BinaryCommandAlternative &operator=(BinaryCommandAlternative &) = delete;

        BinaryCommandAlternative &operator=(BinaryCommandAlternative &&) = delete;

        void checkPreconditionsImp() const override;

        const char *helpMessageImp() const noexcept override;

        void executeImp() noexcept override;

        void undoImp() noexcept override;

        BinaryCommandAlternative *cloneImp() const override;

        BinaryCommandAlternative(const BinaryCommandAlternative &);

        double _top;
        double _next;
        string _helpMessage;
        std::function<BinaryCommandOp> _command;
    };

    inline void CommandDeleter(Command *ptr) {
        if (ptr)
            ptr->deallocate();
    }

    using CommandPtr = unique_ptr<Command, decltype(&CommandDeleter)>;

    template<typename T, typename... Args>
    auto MakeCommandPtr(Args &&... args) requires std::derived_from<T, Command> {
        return CommandPtr{new T{std::forward<Args>(args)...}, &CommandDeleter};
    }

    inline auto MakeCommandPtr(Command *ptr) {
        return CommandPtr{ptr, &CommandDeleter};
    }

}
