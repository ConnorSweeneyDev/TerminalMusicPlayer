#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "discord.hpp"

namespace discord
{
    std::string presence;

    void update_presence(const std::string& song_name, const int& current_song, const int& total_songs, const int& session_songs, const std::string& cwd)
    {
        std::string config_path = cwd + "\\config.ini";

        std::ofstream config_file(config_path);
        if (!config_file.is_open())
        {
            std::cout << "Failed to open config file" << std::endl;
            return;
        }

        std::string optional_s = "";
        if (session_songs != 1)
            optional_s = "s";

        presence = "[Identifiers]\nClientID=1233543047732662354\n\n[State]\nState=" + song_name + "\nDetails=" + std::to_string(current_song) + "/" + std::to_string(total_songs) + " | " + std::to_string(session_songs) + " song" + optional_s + " this session. | Playing." + "\nStartTimestamp=\nEndTimestamp=\n\n[Images]\nLargeImage=\"\"\nLargeImageTooltip=\nSmallImage=\"\"\nSmallImageTooltip=";

        config_file << presence;
    }

    void update_pause_status(const bool& is_paused, const std::string& cwd)
    {
        std::string config_path = cwd + "\\config.ini";

        std::ofstream config_file(config_path);
        if (!config_file.is_open())
        {
            std::cout << "Failed to open config file" << std::endl;
            return;
        }

        if (is_paused)
            presence = std::regex_replace(presence, std::regex("Playing"), "Paused");
        else
            presence = std::regex_replace(presence, std::regex("Paused"), "Playing");

        config_file << presence;
    }

    void close_discord()
    {
        system("pwsh -Command \"Stop-Process -Name \"easyrp\" -Force\"");
    }
}
