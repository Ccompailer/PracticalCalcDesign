module;

#include <memory>

module PlatformFactory;

#ifdef POSIX
    import PosixFactory;
#elif WIN32
    import WindowsFactory;
#endif

import DynamicLoader;

namespace Calculator {

    PlatformFactory::PlatformFactory() {

    }

    PlatformFactory& PlatformFactory::Instance() {
        #ifdef POSIX
            static PosixFactory instance;
        #elif WIN32
            static WindowsFactory instance;
        #endif

        return instance;
    }

    PlatformFactory::~PlatformFactory() {

    }
}
