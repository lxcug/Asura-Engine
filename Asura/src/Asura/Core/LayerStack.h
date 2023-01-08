// @author: lx
// @time: 2022/10/13
// @brief: 

#pragma once
#ifndef GAME_LAYERSTACK_H
#define GAME_LAYERSTACK_H

#include "Asura/Core/Core.h"
#include "Asura/Core/Layer.h"

#include <vector>
#include <algorithm>


namespace Asura {
    // a wrap of vector<Layer*>
    class ASURA_API LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        /**
         * @brief push a layer into layerStack
         * @param layer
         */
        void pushLayer(Layer* layer);

        /**
         * @brief push a layer into layerStack as overlay(end of vector)
         * @param overlay
         */
        void pushOverlay(Layer* overlay);

        /**
         * @brief pop a layer, should delete memory
         * @param layer
         */
        void popLayer(Layer* layer);

        /**
         * @brief pop a overlay
         * @param overlay
         */
        void popOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() {
            return m_layers.begin();
        }
        std::vector<Layer*>::iterator end() {
            return m_layers.end();
        }

        [[nodiscard]] std::vector<Layer*>::const_iterator begin() const {
            return m_layers.cbegin();
        }
        [[nodiscard]] std::vector<Layer*>::const_iterator end() const {
            return m_layers.cend();
        }

        std::vector<Layer*>::reverse_iterator rbegin() {
            return m_layers.rbegin();
        }
        std::vector<Layer*>::reverse_iterator rend() {
            return m_layers.rend();
        }

        [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rbegin() const {
            return m_layers.rbegin();
        }
        [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rend() const {
            return m_layers.rend();
        }



    private:
        std::vector<Layer*> m_layers;
        unsigned m_layerInsertIndex = 0;
    };
}


#endif //GAME_LAYERSTACK_H
