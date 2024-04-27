#pragma once

#include <string>
#include <vector>

namespace init
{
    std::string setup_console();
    void setup_discord(const std::string& cwd);
    std::vector<std::string> get_files_in_directory(const std::string& directory);
    std::vector<std::string> initialize_song_list(const std::string& directory);
    int initialize_volume(const std::string& cwd);
}
