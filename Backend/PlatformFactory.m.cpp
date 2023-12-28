module;

#include <memory>

export module PlatformFactory;

namespace Calculator {

    export class PlatformFactory {
    public:
        static PlatformFactory& Instance();
        virtual ~PlatformFactory();
        virtual std::unique_ptr<class DynamicLoader> CreatedDynamicLoader() = 0;

    protected:
        PlatformFactory();

    private:
        PlatformFactory(const PlatformFactory&) = delete;
        PlatformFactory(PlatformFactory&&) = delete;
        PlatformFactory& operator=(PlatformFactory&) = delete;
        PlatformFactory& operator=(PlatformFactory&&) = delete;
    };
}
