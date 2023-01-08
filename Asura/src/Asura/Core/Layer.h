// @author: lx
// @time: 2022/10/13
// @brief: 

#pragma once
#ifndef GAME_LAYER_H
#define GAME_LAYER_H

#include "Asura/Core/Core.h"
#include "Events/Event.h"
#include "Core/Timestep.h"


namespace Asura {
    /**
     *@brief layer can be created by user and push into layerStack of Application, and will be updated and
     * rendered in run loop.
     */
    class ASURA_API Layer {
    public:
        explicit Layer(std::string name="Layer");
        virtual ~Layer();

        /**
         * @brief called by pushLayer | pushOverlay
         */
        virtual void onAttach() {}

        /**
         * @brief called by popLayer | popOverlay
         */
        virtual void onDetach() {}

        /**
         * @brief called by run function in Application
         */
        virtual void onUpdate(Timestep timestep) {}

        /**
         * @brief called by onEvent function in Application
         * @param event
         */
        virtual void onEvent(Event& event) {}

        /**
         * @brief called by run function in Application
         */
        virtual void onImGuiRender() {}

        [[nodiscard]] inline const std::string& getName() const {
            return m_debugName;
        }

    private:
        std::string m_debugName;
    };
}




#endif //GAME_LAYER_H
