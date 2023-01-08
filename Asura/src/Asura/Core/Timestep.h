// @author: lx
// @time: 2022/10/16
// @brief: 

#pragma once
#ifndef GAME_TIMESTEP_H
#define GAME_TIMESTEP_H


namespace Asura {
    class Timestep {
    public:
        Timestep(float time=.0f): m_time(time) {}

        operator float() const {
            return m_time;
        }

        [[nodiscard]] float getSeconds() const {
            return m_time;
        };
        [[nodiscard]] float getMilliSeconds() const {
            return m_time * 1000.f;
        }

    private:
        float m_time;
    };
}


#endif //GAME_TIMESTEP_H
