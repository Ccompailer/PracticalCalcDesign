#include <string>

using std::string;
using std::string_view;

namespace Calculator {

    class Exception {
    public:
        explicit Exception(string_view msg) : _msg(msg) {}

        const string &What() const { return _msg; }

    private:
        string _msg;
    };
}
