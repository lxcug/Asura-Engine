#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"


void Asura::Log::init() {
    // set log format
    spdlog::set_pattern("%^[%T] %n: %v%$");

    Asura::Log::s_coreLogger = spdlog::stdout_color_mt("Asura");
    Asura::Log::s_coreLogger->set_level(spdlog::level::trace);

    Asura::Log::s_clientLogger = spdlog::stdout_color_mt("App");
    Asura::Log::s_clientLogger->set_level(spdlog::level::trace);
}


Asura::Ref<spdlog::logger> Asura::Log::s_coreLogger;
Asura::Ref<spdlog::logger> Asura::Log::s_clientLogger;
