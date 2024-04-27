#pragma once

#include <string>
#include <vector>

namespace song
{
    std::string generate_song(const std::vector<std::string>& files, int& random_index);
    void open_song(const std::string& path);
    void display_song_info(const std::string& song, int current_song, int random_index, int total_songs, const std::string& cwd);
    void play_song(const std::string& path);
    void pause_or_play(bool& is_paused, const std::string& cwd);
    void set_volume(int volume);
    void increase_volume(int& volume, const std::string& cwd);
    void decrease_volume(int& volume, const std::string& cwd);
    void close_song(const std::string& path);
    bool song_ended();
}
