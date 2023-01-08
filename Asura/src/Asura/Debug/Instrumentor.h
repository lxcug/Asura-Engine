// @author: lx
// @time: 2022/10/21
// @brief: 

#pragma once
#ifndef GAME_INSTRUMENTOR_H
#define GAME_INSTRUMENTOR_H

#include "Core/Core.h"

#include <fstream>
#include <chrono>
#include <thread>
#include <iomanip>


namespace Asura {
    struct ProfileResult {
        std::string Name;
        std::chrono::duration<double, std::micro> Start;
        std::chrono::microseconds ElapsedTime;
        std::thread::id ThreadID;
    };

    struct InstrumentationSession {
        std::string Name;

        InstrumentationSession(std::string name): Name(std::move(name)) {}
    };

    class Instrumentor {
    public:
        Instrumentor();

        void beginSession(const std::string& name, const std::string& filePath="results.json");
        void endSession();

        void writeProfile(const ProfileResult& profileResult);

        void writeHeader();
        void writeFooter();

        static Instrumentor& getInstrumentor() {
            static Instrumentor instance;
            return instance;
        }

    private:
        Scope<InstrumentationSession> m_currSession;
        std::ofstream m_outputStream;
        int m_profileCount;
    };


    class InstrumentationTimer {
    public:
        InstrumentationTimer(std::string name);
        ~InstrumentationTimer();

        void stop();

    private:
        std::string m_name;
        bool m_stop = false;
        std::chrono::steady_clock::time_point m_startTime;
    };
}


#define ASURA_PROFILE 0

#if ASURA_PROFILE
#define ASURA_PROFILE_BEGIN_SESSION(name, filePath) Asura::Instrumentor::getInstrumentor().beginSession(name, filePath)
#define ASURA_PROFILE_END_SESSION()  Asura::Instrumentor::getInstrumentor().endSession()
#define ASURA_PROFILE_SCOPE(name) Asura::InstrumentationTimer timer##__LINE__(name);
#define ASURA_PROFILE_FUNCTION() ASURA_PROFILE_SCOPE( __FUNCTION__)
#else
#define ASURA_PROFILE_BEGIN_SESSION(name, filePath)
	#define ASURA_PROFILE_END_SESSION()
	#define ASURA_PROFILE_SCOPE(name)
	#define ASURA_PROFILE_FUNCTION()
#endif


#endif //GAME_INSTRUMENTOR_H
