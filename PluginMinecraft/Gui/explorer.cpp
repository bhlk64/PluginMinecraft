#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <algorithm>

static std::string currentPath = "/data/data/com.mojang.minecraftpe";

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
        if (ImGui::Button(".."))
        {
            size_t pos = currentPath.find_last_of('/');
            if (pos == 0)
                currentPath = "/";
            else
                currentPath = currentPath.substr(0, pos);
        }
    }

    std::vector<Entry> files;

    DIR* dir = opendir(currentPath.c_str());

    if (dir)
    {
        dirent* ent;

        while ((ent = readdir(dir)))
        {
            if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
                continue;

            std::string full = currentPath + "/" + ent->d_name;

            struct stat st{};
            bool isDir = false;

            if (stat(full.c_str(), &st) == 0)
                isDir = S_ISDIR(st.st_mode);

            files.push_back({
                ent->d_name,
                isDir
            });
        }

        closedir(dir);
    }

    std::sort(files.begin(), files.end(),
        [](const Entry& a, const Entry& b)
        {
            if (a.isDir != b.isDir)
                return a.isDir > b.isDir;

            return a.name < b.name;
        });

    for (auto& f : files)
    {
        std::string label = f.isDir
            ? "[DIR] " + f.name
            : f.name;

        if (ImGui::Selectable(label.c_str()))
        {
            if (f.isDir)
            {
                if (currentPath == "/")
                    currentPath += f.name;
                else
                    currentPath += "/" + f.name;
            }
            else
            {
                ImGui::OpenPopup("File");
            }
        }
    }

    ImGui::End();
}