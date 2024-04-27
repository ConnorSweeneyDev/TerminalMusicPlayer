#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <regex>

#include <windows.h>
#include <mmsystem.h>

#include "song.hpp"
#include "discord.hpp"

namespace song
{
    std::string generate_song(const std::vector<std::string>& files, int& random_index)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, files.size() - 1);
        random_index = dis(gen);

        return files[random_index];
    }

    void open_song(const std::string& path)
    {
        std::string command = "open " + path + " type mpegvideo alias mp3";
        MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char error_message[256];
            mciGetErrorString(error, error_message, sizeof(error_message));
            std::cout << "Error opening file: " << path << ": " << error_message << std::endl;
            command = "close mp3";
            mciSendString(command.c_str(), NULL, 0, NULL);
            exit(1);
        }
    }

    void display_song_info(const std::string& song, int current_song, int random_index, int total_songs, const std::string& cwd)
    {
        std::string song_name = song;
        if (song.find("---") != std::string::npos)
            song_name = std::regex_replace(song_name, std::regex("---"), "|");
        if (song.find("___") != std::string::npos)
            song_name = std::regex_replace(song_name, std::regex("___"), ":");
        song_name = std::regex_replace(song_name, std::regex(".mp3"), "");
        std::cout << " " << current_song << " | " << random_index + 1 << "/" << total_songs << " | " << song_name << std::endl;

        discord::update_presence(song_name, random_index, total_songs, current_song, cwd);
    }

    void play_song(const std::string& path)
    {
        std::string command = "play mp3";
        MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char error_message[256];
            mciGetErrorString(error, error_message, sizeof(error_message));
            std::cout << "Error playing file: " << path << ": " << error_message << std::endl;
            command = "close mp3";
            mciSendString(command.c_str(), NULL, 0, NULL);
            exit(1);
        }
    }

    void pause_or_play(bool& is_paused, const std::string& cwd)
    {
        std::string command;
        if (is_paused)
            command = "play mp3";
        else
            command = "pause mp3";
        is_paused = !is_paused;

        MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char error_message[256];
            mciGetErrorString(error, error_message, sizeof(error_message));
            std::cout << "Error pausing or resuming: " << error_message << std::endl;
            exit(1);
        }

        discord::update_pause_status(is_paused, cwd);
    }

    void set_volume(int volume)
    {
        std::string command = "setaudio mp3 volume to " + std::to_string(volume);
        MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char error_message[256];
            mciGetErrorString(error, error_message, sizeof(error_message));
            std::cout << "Error setting volume: " << error_message << std::endl;
            command = "close mp3";
            mciSendString(command.c_str(), NULL, 0, NULL);
            exit(1);
        }
    }

    void increase_volume(int& volume, const std::string& cwd)
    {
        volume += 50;
        if (volume > 1000)
            volume = 1000;
        else if (volume < 0)
            volume = 0;

        set_volume(volume);

        std::ofstream volume_file(cwd + "\\volume.txt");
        if (!volume_file.is_open())
        {
            std::cout << "Failed to open volume file" << std::endl;
            exit(1);
        }

        volume_file << volume;
    }

    void decrease_volume(int& volume, const std::string& cwd)
    {
        volume -= 50;
        if (volume > 1000)
            volume = 1000;
        else if (volume < 0)
            volume = 0;

        set_volume(volume);

        std::ofstream volume_file(cwd + "\\volume.txt");
        if (!volume_file.is_open())
        {
            std::cout << "Failed to open volume file" << std::endl;
            exit(1);
        }

        volume_file << volume;
    }

    void close_song(const std::string& path)
    {
        std::string command = "close mp3";
        MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char error_message[256];
            mciGetErrorString(error, error_message, sizeof(error_message));
            std::cout << "Error closing file: " << path << ": " << error_message << std::endl;
            exit(1);
        }
    }

    bool song_ended()
    {
        MCI_STATUS_PARMS status;
        status.dwItem = MCI_STATUS_MODE;
        UINT device_id = mciGetDeviceID("mp3");
        MCIERROR error = mciSendCommand(device_id, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&status);
        if (error != 0)
        {
            char error_message[256];
            mciGetErrorString(error, error_message, sizeof(error_message));
            std::cout << "Error getting status: " << error_message << std::endl;
            return true;
        }

        return status.dwReturn == MCI_MODE_STOP;
    }
}
