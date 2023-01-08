// @author: lx
// @time: 2022/11/1
// @brief: 

#ifndef GAME_PLATFORMUTILS_H
#define GAME_PLATFORMUTILS_H

#include <string>

namespace Asura {
    class FileDialogs {
    public:
        static std::string openFile(const char* filter = "");
        static std::string saveFile(const char* filter = "");
    };
}


#endif //GAME_PLATFORMUTILS_H
