// @author: lx
// @time: 2022/10/11
// @brief: 

#ifndef GAME_ENTRYPOINT_CPP
#define GAME_ENTRYPOINT_CPP

#ifdef ASURA_PLATFORM_WINDOWS

#include "Core.h"
#include "Asura/Core/Application.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"


// entry point
int main(int argc, char** argv) {
    ASURA_PROFILE_BEGIN_SESSION("Startup", "AsuraProfile-Startup.json");
    Asura::Log::init();
    ASURA_CORE_INFO("Start Asura Engine");
    Asura::Application* application = Asura::createApplication({argc, argv});
    ASURA_PROFILE_END_SESSION();

    ASURA_PROFILE_BEGIN_SESSION("Runtime", "AsuraProfile-Runtime.json");
    application->run();
    ASURA_PROFILE_END_SESSION();


    ASURA_PROFILE_BEGIN_SESSION("Shutdown", "AsuraProfile-Shutdown.json");
    delete application;
    ASURA_PROFILE_END_SESSION();

    return 0;
}

#endif  // ASURA_PLATFORM_WINDOWS

#endif //GAME_ENTRYPOINT_CPP
