module;

#include <cstdlib>
#include <vector>
#include <windows.h>
#include <string>

export module WindowsDynamicLoader;

import DynamicLoader;
import CalcUtilities;

using std::string;
using std::vector;

namespace Calculator {
    export class WindowsDynamicLoader : public DynamicLoader {

    };
}