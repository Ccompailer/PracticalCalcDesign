module;

#include <any>
#include <memory>
#include <vector>
#include <set>
#include <ranges>
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
    using _observersList = unordered_map<string, unique_ptr<Observer>>;
    using _events = unordered_map<string, _observersList>;

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

};
