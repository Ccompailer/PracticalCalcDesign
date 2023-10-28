module;

#include <string>
#include <any>

export module CalcUtilities:Observer;

using std::string;
using std::any;
using std::string_view;

export class Observer
{
public:
    explicit Observer(string_view name);
    virtual ~Observer() = default;

    void Notify(const any& data);
    const string& Name() const { return _observerName; }

private:
    virtual  void NotifyImpl(const any& data) = 0;

    string _observerName;
};

Observer::Observer(std::string_view name) : _observerName(name) { }

void Observer::Notify(const std::any& data) {
    NotifyImpl(data);
}
