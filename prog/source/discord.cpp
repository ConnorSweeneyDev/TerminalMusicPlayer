#include <iostream>
#include <fstream>
#include <string>

#include "discord.hpp"

namespace discord
{
    void update_presence(const std::string& song_name, const int& current_song, const int& total_songs, const int& session_songs)
    {
        std::string config_path = "C:\\Users\\conno\\Documents\\Programming\\C++\\TerminalMusicPlayer\\config.ini";

        std::ofstream config_file(config_path);
        if (!config_file.is_open())
        {
            std::cout << "Failed to open config file" << std::endl;
            return;
        }

        std::string optional_s = "";
        if (session_songs != 1)
            optional_s = "s";

        config_file << "[Identifiers]\nClientID=1233543047732662354\n\n[State]\nState=" << song_name << "\nDetails=" << current_song << "/" << total_songs << " | " << session_songs << " song" << optional_s << " this session." << "\nStartTimestamp=\nEndTimestamp=\n\n[Images]\nLargeImage=\"\"\nLargeImageTooltip=\nSmallImage=\"\"\nSmallImageTooltip=";
    }
}
