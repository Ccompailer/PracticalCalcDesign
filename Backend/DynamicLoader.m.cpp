module;

#include <string>

export module DynamicLoader;

using std::string;

namespace Calculator {
    export class DynamicLoader {
    public:
        virtual ~DynamicLoader() = default;

        virtual class Plugin* AllocatePlugin(const string &pluginName) = 0;

        virtual void deallocatePlugin(class Plugin*) = 0;

        static string GetPluginAllocateName() { return "AllocPlugin"; }

        static string GetPluginDeallocateName() { return "DeallocPlugin"; }
    };

    export extern "C" { typedef void* (*PluginAllocator)(void); }
    export extern "C" { typedef void (*PluginDeallocator)(void*); }
}