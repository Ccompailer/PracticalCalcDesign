module;

#include <memory>

module WindowsFactory;

import DynamicLoader;

namespace Calculator {
    class DynamicLoader;

    std::unique_ptr<DynamicLoader> WindowsFactory::CreatedDynamicLoader() {
        return nullptr; // Нужно будет сделать WindowsDynamicLoader который будет наследоваться от DynamicLoader
    }
}
