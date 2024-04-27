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
    std::string generate(const std::vector<std::string>& files, int& random_index)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, files.size() - 1);
        random_index = dis(gen);

        return files[random_index];
    }

    void open(const std::string& path)
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

    std::string display_info(const std::string& song, int current_song, int random_index, int total_songs, const std::string& cwd)
    {
        std::string song_name = song;
        if (song.find("---") != std::string::npos)
            song_name = std::regex_replace(song_name, std::regex("---"), "|");
        if (song.find("___") != std::string::npos)
            song_name = std::regex_replace(song_name, std::regex("___"), ":");
        song_name = std::regex_replace(song_name, std::regex(".mp3"), "");
        std::cout << " " << current_song << " | " << random_index + 1 << "/" << total_songs << " | " << song_name << std::endl;

        discord::update_presence(song_name, random_index, total_songs, current_song, cwd);

        std::string length;
        auto it = std::find(song_name.rbegin(), song_name.rend(), ' ');
        if (it != song_name.rend())
        {
            ++it;
            std::string result(it.base(), song_name.end());
            length = result;
        }

        return length;
    }

    DWORD get_progress()
    {
        MCI_STATUS_PARMS status;
        status.dwItem = MCI_STATUS_POSITION;

        mciSendCommand(mciGetDeviceID("mp3"), MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, reinterpret_cast<DWORD_PTR>(&status));
        return status.dwReturn;
    }

    int bar_width = 100;
    int display_status_bar(const std::string& length, int volume)
    {
        size_t colon_pos = length.find(':');
        int minutes = std::stoi(length.substr(0, colon_pos));
        int seconds = std::stoi(length.substr(colon_pos + 1));
        int time = minutes * 60 + seconds;

        DWORD progress = song::get_progress();
        progress = progress / 1000;
        int progress_percent = int((double)progress / time * 100.0);

        std::string progress_blank_space;
        if (progress_percent < 10) progress_blank_space = "     ";
        else if (progress_percent < 100 && progress_percent >= 10) progress_blank_space = "    ";
        else progress_blank_space = "   ";

        int volume_percent;
        if (volume == 0) volume_percent = 0;
        else if (volume == 50) volume_percent = 5;
        else volume_percent = volume / 10;

        std::string volume_blank_space;
        if (volume_percent < 10) volume_blank_space = "  ";
        else if (volume_percent < 100 && volume_percent >= 10) volume_blank_space = " ";
        else volume_blank_space = "";

        int pos = (double)progress / time * bar_width;
        std::cout << "\r [";
        for (int i = 0; i < bar_width; ++i)
        {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << ".";
        }
        std::cout << "] " << progress_percent << "%" << progress_blank_space << "Vol: " << volume_percent << "%" << volume_blank_space << "\r";
        std::cout.flush();

        return progress_percent;
    }

    void progress_cleanup()
    {
        std::cout << "\r";
        for (int i = 0; i < bar_width + 20; ++i)
            std::cout << " ";
        std::cout << "\r";
        std::cout.flush();
    }

    void play(const std::string& path)
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
        if (is_paused)
            system("color 09");
        else
            system("color 04");

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
        volume_file.close();
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
        volume_file.close();
    }

    void close(const std::string& path)
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

    bool ended()
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
