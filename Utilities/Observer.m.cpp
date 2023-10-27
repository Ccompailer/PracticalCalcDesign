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

    void notify(const any& data);
    const string& name() const { return _observerName; }

private:
    virtual  void notifyImpl(const any& data) = 0;

    string _observerName;
};

Observer::Observer(std::string_view name) : _observerName(name) { }

void Observer::notify(const std::any& data) {
    notifyImpl(data);
    return;
}