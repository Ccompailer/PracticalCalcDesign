module;

#include <memory>

export module WindowsFactory;

import PlatformFactory;

namespace Calculator {

    export class WindowsFactory : public PlatformFactory {
    public:
        WindowsFactory() = default;
        std::unique_ptr<DynamicLoader> CreatedDynamicLoader() override;
    };
}
