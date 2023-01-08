#include "ContentBrowserPanel.h"
#include "imgui/imgui.h"


Asura::ContentBrowserPanel::ContentBrowserPanel(): m_currentDir(Asura::getAssetPath()) {
    m_directoryIcon = Texture2D::createTexture2D("../../Asura-Editor/resources/icons/ContentBrowser/DirectoryIcon.png");
    m_fileIcon = Texture2D::createTexture2D("../../Asura-Editor/resources/icons/ContentBrowser/FileIcon.png");
}

void Asura::ContentBrowserPanel::onImGuiRender() {
    ImGui::Begin("Content Browser");

    static float padding = 16.0f;
    static float thumbnailSize = 100.0f;
    float cellSize = thumbnailSize + padding;

    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / cellSize);
    if(columnCount < 1)
        columnCount = 1;

    ImGui::Columns(columnCount, nullptr, false);

    for(auto& directoryEntry : std::filesystem::directory_iterator(m_currentDir)) {
        const auto& path = directoryEntry.path();
        auto relativePath = std::filesystem::relative(path, Asura::getAssetPath());
        std::string filenameString = relativePath.filename().string();

        ImGui::PushID(filenameString.c_str());
        Ref<Texture2D> icon = directoryEntry.is_directory() ? m_directoryIcon : m_fileIcon;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        uint64_t textureID = icon->getRendererID();
        ImGui::ImageButton((ImTextureID)textureID,
                           { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

        if(ImGui::BeginDragDropSource()) {
            const wchar_t* itemPath = relativePath.c_str();
            ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath,
                                      (wcslen(itemPath) + 1) * sizeof(wchar_t));
            ImGui::EndDragDropSource();
        }

        if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            if (directoryEntry.is_directory())
                m_currentDir /= path.filename();
        }

        ImGui::PopStyleColor();
        ImGui::TextWrapped(filenameString.c_str());
        ImGui::NextColumn();
        ImGui::PopID();
    }
    ImGui::Columns(1);
    if(m_currentDir != std::filesystem::path(Asura::getAssetPath())) {
        if(ImGui::Button("<-")) {
            m_currentDir = m_currentDir.parent_path();
        }
    }
//    ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
//    ImGui::SliderFloat("Padding", &padding, 0, 32);

    // TODO: status bar
    ImGui::End();
}
