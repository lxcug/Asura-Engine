// @author: lx
// @time: 2022/11/2
// @brief: 

#ifndef GAME_TIMER_H
#define GAME_TIMER_H

#include "Core/Core.h"
#include "Core/Log.h"


namespace Asura {
    class Timer {
    public:
        Timer() {
            reset();
        }

        void reset() {
            m_startTime = std::chrono::steady_clock::now();
        }

        float elapsed() {
            return (float)(std::chrono::steady_clock::now() - m_startTime).count();
        }

        float elapsedMillis() {
            return elapsed() * 1000.f;
        }

    private:
        std::chrono::steady_clock::time_point m_startTime;
    };
}


#endif //GAME_TIMER_H
