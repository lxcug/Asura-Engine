#include "LayerStack.h"

Asura::LayerStack::LayerStack() {
}

Asura::LayerStack::~LayerStack() {
    for(Layer* layer : m_layers) {
        delete layer;
    }
}

void Asura::LayerStack::pushLayer(Asura::Layer *layer) {
    m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
    m_layerInsertIndex++;
    layer->onAttach();
}

void Asura::LayerStack::pushOverlay(Asura::Layer* overlay) {
    m_layers.emplace_back(overlay);
    overlay->onAttach();
}

void Asura::LayerStack::popLayer(Asura::Layer *layer) {
    auto it = std::find(m_layers.begin(), m_layers.begin()+m_layerInsertIndex, layer);
    if(it != m_layers.begin() + m_layerInsertIndex) {
        layer->onDetach();
        m_layers.erase(it);
        m_layerInsertIndex--;
    }
}

void Asura::LayerStack::popOverlay(Asura::Layer* overlay) {
    auto it = std::find(m_layers.begin()+m_layerInsertIndex, m_layers.end(), overlay);
    if(it != m_layers.end()) {
        overlay->onDetach();
        m_layers.erase(it);
    }
}
