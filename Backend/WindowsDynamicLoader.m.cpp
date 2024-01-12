module;

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
    public:
        WindowsDynamicLoader();
        ~WindowsDynamicLoader();

    private:
        HINSTANCE _handle;

    };

    WindowsDynamicLoader::WindowsDynamicLoader()
        : DynamicLoader {}, _handle {nullptr} {

    }

    WindowsDynamicLoader::~WindowsDynamicLoader() {
        if (_handle)
            FreeLibrary(_handle);
    }
}