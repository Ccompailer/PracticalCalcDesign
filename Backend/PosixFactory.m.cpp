module;

#include <memory>

export module PosixFactory;

import PlatformFactory;

namespace Calculator {

    class PosixFactory : public PlatformFactory {
    public:
        PosixFactory();

        std::unique_ptr<DynamicLoader> CreatedDynamicLoader() override;
    };
}
