#include "Instrumentor.h"
#include <algorithm>
#include "Core/Log.h"


Asura::Instrumentor::Instrumentor(): m_currSession(nullptr), m_profileCount(0) {

}

void Asura::Instrumentor::beginSession(const std::string &name, const std::string &filePath) {
    m_outputStream.open(filePath);
    writeHeader();
    m_currSession = Asura::createScope<InstrumentationSession>(name);
}

void Asura::Instrumentor::endSession() {
    writeFooter();
    m_outputStream.close();
    m_currSession.reset();
    ASURA_CORE_ASSERT(!m_currSession, "Current session doesn't release correctly");
    m_profileCount = 0;
}

void Asura::Instrumentor::writeProfile(const Asura::ProfileResult& profileResult) {
    if (m_profileCount++ > 0)
        m_outputStream << ",";

    std::string name = profileResult.Name;
    std::replace(name.begin(), name.end(), '"', '\'');

    m_outputStream << "{";
    m_outputStream << R"("cat":"function",)";
    m_outputStream << "\"duration\":" << (profileResult.ElapsedTime.count()) << ',';
    m_outputStream << R"("name":")" << name << "\",";
    m_outputStream << R"("ph":"X",)";
    m_outputStream << "\"pid\":0,";
    m_outputStream << "\"tid\":" << profileResult.ThreadID << ",";
    m_outputStream << "\"ts\":" << profileResult.Start.count();
    m_outputStream << "}";

    m_outputStream.flush();
}

void Asura::Instrumentor::writeHeader() {
    m_outputStream << R"({"otherData": {},"traceEvents":[)";
    m_outputStream.flush();
}

void Asura::Instrumentor::writeFooter() {
    m_outputStream << R"(]})";
    m_outputStream.flush();
}



Asura::InstrumentationTimer::InstrumentationTimer(std::string name): m_name(std::move(name)), m_stop(false) {
    m_startTime = std::chrono::steady_clock::now();
}

Asura::InstrumentationTimer::~InstrumentationTimer() {
    if(!m_stop) {
        stop();
    }
}

void Asura::InstrumentationTimer::stop() {
    auto endTime = std::chrono::steady_clock::now();
    long long start = std::chrono::time_point_cast<std::chrono::microseconds>
            (m_startTime).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::microseconds>
            (endTime).time_since_epoch().count();
    auto highResStart = std::chrono::duration<double, std::micro>{ m_startTime.time_since_epoch() };
    auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>
            (endTime).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime).time_since_epoch();
    Instrumentor::getInstrumentor().writeProfile({m_name, highResStart, elapsedTime, std::this_thread::get_id()});
    m_stop = true;
}
