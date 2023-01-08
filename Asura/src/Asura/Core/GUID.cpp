#include "GUID.h"

#include <random>
#include <unordered_map>


namespace Asura {
    static std::random_device s_randomDevice;
    static std::mt19937_64 s_engine(s_randomDevice());
    static std::uniform_int_distribution<uint64_t> s_uniformDistribution;

    GUID::GUID(): m_GUID(s_uniformDistribution(s_engine)) {

    }

    GUID::GUID(uint64_t uuid): m_GUID(uuid) {

    }
}