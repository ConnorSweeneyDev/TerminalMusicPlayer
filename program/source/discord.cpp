#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "discord.hpp"

namespace discord
{
    std::string presence;

    void update_presence(const std::string& song_name, const int& session_songs, const std::string& cwd)
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

        std::string client_id = "";
        std::string id_path = cwd + "\\user\\client_id.txt";
        std::ifstream id_file(id_path);
        if (!id_file.is_open())
        {
            std::cout << "Failed to open client ID file" << std::endl;
            return;
        }

        std::string line;
        std::getline(id_file, line);
        client_id = line;

        id_file.close();

        presence = "[Identifiers]\nClientID=" + client_id + "\n\n[State]\nState=" + std::to_string(session_songs) + " Song" + optional_s + " This Session | Playing | 0%" + "\nDetails=" + song_name + "\nStartTimestamp=\nEndTimestamp=\n\n[Images]\nLargeImage=\"\"\nLargeImageTooltip=\nSmallImage=\"\"\nSmallImageTooltip=";

        config_file << presence;
        config_file.close();
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
        config_file.close();
    }

    void update_progress_status(const std::string& progress, const std::string& cwd)
    {
        std::string config_path = cwd + "\\config.ini";

        std::ofstream config_file(config_path);
        if (!config_file.is_open())
        {
            std::cout << "Failed to open config file" << std::endl;
            return;
        }

        size_t percent_pos = presence.find("%");
        size_t space_pos = presence.rfind(" ", percent_pos);

        std::string old_progress = presence.substr(space_pos + 1, percent_pos - space_pos - 1);
        presence = std::regex_replace(presence, std::regex(old_progress + "%"), progress + "%");

        config_file << presence;
        config_file.close();
    }

    void close()
    {
        system("pwsh -Command \"Stop-Process -Name \"easyrp\" -Force\"");
    }
}
