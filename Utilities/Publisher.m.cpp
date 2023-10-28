module;

#include <any>
#include <memory>
#include <vector>
#include <set>
#include <ranges>
#include <format>
#include <algorithm>
#include <unordered_map>

export module CalcUtilities:Publisher;

import :Observer;
import :Exception;

using std::string;
using std::vector;
using std::set;
using std::unique_ptr;
using std::shared_ptr;
using std::any;
using std::unordered_map;

namespace views = std::views;
namespace ranges = std::ranges;

export class Publisher
{
    using ObserversList = unordered_map<string, unique_ptr<Observer>>;
    using Events = unordered_map<string, ObserversList>;

public:
    Publisher() = default;

    void Attach(
        const string& eventName,
        unique_ptr<Observer> observer
    );
    unique_ptr<Observer> Detach(
        const string& eventName,
        const string& observerName
    );
    set<string> ListEvents() const;
    set<string> ListEventObservers(const string& eventName) const;

protected:
    void Raise(const string& eventName) const;
    void Raise(const string& eventName, any data) const;
    void RegisterEvent(const string& eventName);
    void RegisterEvents(const vector<string> eventNames);

private:
    Events::const_iterator FindChackedEvent(const string& eventName) const;
    Events::iterator FindChackedEvent(const string& eventName);

    Events _events;
};

Publisher::Events::const_iterator Publisher::FindChackedEvent(const string& eventName) const {
    auto ev = _events.find(eventName);

    if(ev == _events.end())
    {
        throw Exception {
            std::format("Publisher doesn't support this event '{}'", eventName)
        };
    }
    return ev;
}

Publisher::Events::iterator Publisher::FindChackedEvent(const string& eventName) {
    auto ev = _events.find(eventName);

    if(ev == _events.end())
    {
        throw Exception {
                std::format("Publisher doesn't support this event '{}'", eventName)
        };
    }
    return ev;
}

void Publisher::Attach(
    const string& eventName,
    unique_ptr<Observer> observer
) {
    auto& obsList = FindChackedEvent(eventName)->second;

    if(obsList.contains(observer->Name()))
        throw Exception("Observer already attached to publisher");

    obsList.insert(std::pair(observer->Name(), std::move(observer)));
}

unique_ptr<Observer> Publisher::Detach(
    const string& eventName,
    const string& observerName
) {
    auto& obsList = FindChackedEvent(eventName)->second;
    auto observ = obsList.find(observerName);

    if(observ == obsList.end())
        throw Exception("Cannot detach observer because observer not found");

    auto temp = std::move(observ->second);

    obsList.erase(observ);
    return temp;
}

void Publisher::Raise(const string &eventName) const {
    Raise(eventName, any{});
}

void Publisher::Raise(const string &eventName, any data) const {
    const auto& obsList = FindChackedEvent(eventName)->second;
    ranges::for_each(
        views::values(obsList),
        [data](const auto& v){ v->Notify(data); }
    );
}

void Publisher::RegisterEvent(const string& eventName) {
    if(_events.contains(eventName))
            throw Exception("Event already registered");

    _events[eventName] = ObserversList{};
}

void Publisher::RegisterEvents(const vector<string> eventNames) {
    ranges::for_each(
        eventNames,
        [this](const auto& i){ RegisterEvent(i); }
    );
}

set<string> Publisher::ListEvents() const {
    set<string> temp;
    ranges::for_each(
        _events,
        [&temp](const auto& i){ temp.insert(i.first); }
    );
    return temp;
}

set<string> Publisher::ListEventObservers(const string &eventName) const {
    auto ev = FindChackedEvent(eventName);
    set<string> temp;

    ranges::for_each(
        ev->second | views::keys,
        [&temp](const auto& k){ temp.insert(k); }
    );
    return temp;
}
