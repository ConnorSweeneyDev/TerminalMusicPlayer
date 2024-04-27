#pragma once

#include <string>

namespace discord
{
    void update_presence(const std::string& song_name, const int& current_song, const int& total_songs, const int& session_songs, const std::string& cwd);
    void update_pause_status(const bool& is_paused, const std::string& cwd);
    void close_discord();
}
