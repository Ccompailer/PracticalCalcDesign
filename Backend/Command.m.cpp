module;

export module CalcBackend:Command;

export class Command
{
public:
    Command* clone() const;
    virtual ~Command();
    void execute();
    void undo();
    const char* helpMessage() const;

protected:
    Command();
    Command(Command&);

private:
    virtual void checkPreconditionsImp() const;
    virtual Command* cloneImp() const = 0;
    virtual void executeImp() noexcept = 0;
    virtual void undoImp() noexcept = 0;
    virtual const char* helpMessageImp() const noexcept = 0;
};
