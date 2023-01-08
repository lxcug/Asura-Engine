// @author: lx
// @time: 2022/10/11
// @brief: 

#ifndef GAME_LOG_H
#define GAME_LOG_H

#include "Core/Core.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


// Log system of Asura (a wrapper of spdlog)
namespace Asura {
    class ASURA_API Log {
    public:
        static void init();

        /**
         * @brief logger for Asura Core
         * @return core logger
         */
        inline static Ref<spdlog::logger>& getCoreLogger() {
            return s_coreLogger;
        }

        /**
         * @brief logger for Client
         * @return client logger
         */
        inline static Ref<spdlog::logger>& getClientLogger() {
            return s_clientLogger;
        }

    private:
        static Ref<spdlog::logger> s_coreLogger;
        static Ref<spdlog::logger> s_clientLogger;
    };
}


template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}



// use macros to simplify the use of core logger
#define ASURA_CORE_INFO(...)    ::Asura::Log::getCoreLogger()->info(__VA_ARGS__)
#define ASURA_CORE_TRACE(...)   ::Asura::Log::getCoreLogger()->trace(__VA_ARGS__)
#define ASURA_CORE_WARN(...)    ::Asura::Log::getCoreLogger()->warn(__VA_ARGS__)
#define ASURA_CORE_ERROR(...)   ::Asura::Log::getCoreLogger()->error(__VA_ARGS__)
#define ASURA_CORE_FATAL(...)   ::Asura::Log::getCoreLogger()->critical(__VA_ARGS__)

// use macros to simplify the use of client logger
#define ASURA_INFO(...)    ::Asura::Log::getClientLogger()->info(__VA_ARGS__)
#define ASURA_TRACE(...)   ::Asura::Log::getClientLogger()->trace(__VA_ARGS__)
#define ASURA_WARN(...)    ::Asura::Log::getClientLogger()->warn(__VA_ARGS__)
#define ASURA__ERROR(...)   ::Asura::Log::getClientLogger()->error(__VA_ARGS__)
#define ASURA_FATAL(...)   ::Asura::Log::getClientLogger()->critical(__VA_ARGS__)

// Assert marco
#ifdef ASURA_ENABLE_ASSERT
#define ASURA_CORE_ASSERT(x, ...) if(!(x)) { ASURA_CORE_FATAL("Assertion Failed: {0}", __VA_ARGS__); raise(SIGABRT); }
#define ASURA_ASSERT(x, ...) if((!x)) { ASURA_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGABRT); }
#else
#define ASURA_CORE_ASSERT(x, ...)
#define ASURA_ASSERT(x, ...)
#endif

#endif //GAME_LOG_H
