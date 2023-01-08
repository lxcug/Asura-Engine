// @author: lx
// @time: 2022/11/4
// @brief: 

#ifndef GAME_GUID_H
#define GAME_GUID_H

#include <hashtable.h>

namespace Asura {
    class GUID {
    public:
        GUID();
        GUID(uint64_t uuid);
        GUID(const GUID&) = default;

        operator uint64_t() const {
            return m_GUID;
        }
    private:
        uint64_t m_GUID;
    };

}

namespace std {
    template<>
    struct hash<Asura::GUID>
    {
        std::size_t operator()(const Asura::GUID& uuid) const
        {
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };

}


#endif //GAME_GUID_H
