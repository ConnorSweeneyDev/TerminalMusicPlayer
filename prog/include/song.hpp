#pragma once

#include <string>
#include <vector>

#include <windows.h>

namespace song
{
    std::string generate(const std::vector<std::string>& files, int& random_index);
    void open(const std::string& path);
    std::string display_info(const std::string& song, int current_song, int random_index, int total_songs, const std::string& cwd);
    DWORD get_progress();
    void display_progress(const std::string& length);
    void progress_cleanup();
    void play(const std::string& path);
    void pause_or_play(bool& is_paused, const std::string& cwd);
    void set_volume(int volume);
    void increase_volume(int& volume, const std::string& cwd);
    void decrease_volume(int& volume, const std::string& cwd);
    void close(const std::string& path);
    bool ended();
}
