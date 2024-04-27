#pragma once

#include <string>
#include <vector>

namespace init
{
    std::string console();
    void discord(const std::string& cwd);
    std::vector<std::string> get_files_in_directory(const std::string& directory);
    std::vector<std::string> song_list(const std::string& directory);
    int volume(const std::string& cwd);
}
