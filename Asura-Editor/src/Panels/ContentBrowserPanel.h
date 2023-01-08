// @author: lx
// @time: 2022/11/3
// @brief: 

#ifndef GAME_CONTENTBROWSERPANEL_H
#define GAME_CONTENTBROWSERPANEL_H

#include <filesystem>
#include "Renderer/Texture.h"


namespace Asura {
    class ContentBrowserPanel {
    public:
        ContentBrowserPanel();

        void onImGuiRender();

    private:
        std::filesystem::path m_currentDir;
        Ref<Texture2D> m_directoryIcon;
        Ref<Texture2D> m_fileIcon;
    };

    static const std::filesystem::path& getAssetPath() {
        static std::filesystem::path path = "E:/Asura/Asura-Editor/assets";
        return path;
    }
}

#endif //GAME_CONTENTBROWSERPANEL_H
