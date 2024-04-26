#pragma once

#include <string>
#include <vector>

void disable_visual_input();
std::vector<std::string> get_files_in_directory(const std::string& directory);
std::vector<std::string> initialize_song_list(const std::string& directory);
int initialize_volume();
