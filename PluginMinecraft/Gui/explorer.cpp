#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

#include "imgui.h"
#include "explorer.h"

static std::string currentPath = "/data/data/com.mojang.minecraftpe";
static std::string selectedFile;

struct Entry
{
    std::string name;
    bool isDir;
};

void DrawFileExplorer()
{
    ImGui::Begin("File Explorer");

    ImGui::Text("%s", currentPath.c_str());

    if (currentPath != "/")
    {
        ImGui::SameLine();

        if (ImGui::Button("Up"))
        {
            size_t pos = currentPath.find_last_of('/');

            if (pos == 0)
                currentPath = "/";
            else
                currentPath = currentPath.substr(0, pos);
        }
    }

    ImGui::Separator();

    ImGui::BeginChild("Files");

    std::vector<Entry> files;

    DIR* dir = opendir(currentPath.c_str());

    if (!dir)
    {
        ImGui::TextColored(
            ImVec4(1.f, 0.f, 0.f, 1.f),
            "Cannot open directory."
        );
    }
    else
    {
        dirent* ent;

        while ((ent = readdir(dir)) != nullptr)
        {
            if (!strcmp(ent->d_name, ".") ||
                !strcmp(ent->d_name, ".."))
                continue;

            std::string full = currentPath + "/" + ent->d_name;

            bool isDir = false;

            if (ent->d_type == DT_DIR)
            {
                isDir = true;
            }
            else if (ent->d_type == DT_UNKNOWN)
            {
                struct stat st{};

                if (stat(full.c_str(), &st) == 0)
                    isDir = S_ISDIR(st.st_mode);
            }

            files.push_back({
                ent->d_name,
                isDir
            });
        }

        closedir(dir);

        std::sort(
            files.begin(),
            files.end(),
            [](const Entry& a, const Entry& b)
            {
                if (a.isDir != b.isDir)
                    return a.isDir > b.isDir;

                return a.name < b.name;
            }
        );

        for (const auto& f : files)
        {
            std::string full = currentPath + "/" + f.name;

            std::string label =
                (f.isDir ? "📁 " : "📄 ") + f.name;

            bool selected = (selectedFile == full);

            if (ImGui::Selectable(label.c_str(), selected))
            {
                selectedFile = full;

                if (f.isDir &&
                    ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    currentPath = full;
                }
            }
        }
    }

    ImGui::EndChild();

    if (ImGui::BeginPopup("File"))
    {
        ImGui::Text("%s", selectedFile.c_str());

        if (ImGui::Button("Copy Path"))
        {
            ImGui::SetClipboardText(selectedFile.c_str());
        }

        ImGui::EndPopup();
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) &&
        !selectedFile.empty())
    {
        ImGui::OpenPopup("File");
    }

    ImGui::End();
}