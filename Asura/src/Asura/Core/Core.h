// @author: lx
// @time: 2022/10/11
// @brief:

#ifndef GAME_CORE_H
#define GAME_CORE_H


#ifdef ASURA_PLATFORM_WINDOWS
    #ifdef ASURA_DYNAMIC_LINK
        #ifdef ASURA_BUILD_DLL
            #define ASURA_API __declspec(dllexport)
        #else
            #define ASURA_API __declspec(dllimport)
        #endif  // ASURA_BUILD_DLL
    #else
        #define ASURA_API
    #endif  // ASURA_DYNAMIC_LINK
#else
    #error Asura only support Windows!
#endif  // ASURA_PLATFORM_WINDOWS

#ifdef ASURA_DEBUG
#define HZ_ENABLE_ASSERTS
#endif


// std::bind return a functional object
#define BIND_EVENT_FUN(fun) std::bind(&fun, this, std::placeholders::_1)


#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"


#include <memory>
namespace Asura {
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> createScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> createRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#include "Debug/Instrumentor.h"


#endif //GAME_CORE_H
