#include <cstddef>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <random>
#include <regex>
#include <conio.h>

#include "app.hpp"
#include "platform.hpp"
#include "discord.hpp"
#include "player.hpp"

namespace tmp
{
    App app;

    void App::play_song()
    {
        current_song++;
        choose_random_song();
        display_song();

        Player::open();
        Player::set_volume(volume);
        Player::play();
        Player::song_playing = true;
        current_song_paused = false;
    }

    void App::resume_or_pause()
    {
        if (current_song_paused)
        {
            Player::resume();
            system("color 09");
            current_song_paused = false;
        }
        else
        {
            Player::pause();
            system("color 04");
            current_song_paused = true;
        }
        Discord::update_pause(current_song_paused);
    }

    void App::increase_volume()
    {
        volume += 20;
        if (volume > 1000)
            volume = 1000;
        else if (volume < 0)
            volume = 0;

        Player::set_volume(volume);

        std::ofstream volume_file(volume_path);
        if (!volume_file.is_open())
        {
            std::cout << "Failed to open volume file!" << std::endl;
            exit(1);
        }

        volume_file << volume;
        volume_file.close();
    }

    void App::decrease_volume()
    {
        volume -= 20;
        if (volume > 1000)
            volume = 1000;
        else if (volume < 0)
            volume = 0;

        Player::set_volume(volume);

        std::ofstream volume_file(volume_path);
        if (!volume_file.is_open())
        {
            std::cout << "Failed to open volume file!" << std::endl;
            exit(1);
        }

        volume_file << volume;
        volume_file.close();
    }

    void App::close_song()
    {
        if (current_song_paused)
            resume_or_pause();

        Player::close();
        Player::song_playing = false;
    }

    void App::quit_app()
    {
        Player::song_playing = false;
        running = false;
    }

    void App::read_input()
    {
        if (GetAsyncKeyState(VK_MEDIA_PLAY_PAUSE))
            resume_or_pause();
        else if (GetAsyncKeyState(VK_MEDIA_NEXT_TRACK))
            close_song();
        else if (GetAsyncKeyState(VK_MEDIA_STOP))
            quit_app();

        if (_kbhit())
        {
            char key = (char)_getch();

            if (key == 'p')
                resume_or_pause();
            else if (key == 'u')
                increase_volume();
            else if (key == 'd')
                decrease_volume();
            else if (key == 'n')
                close_song();
            else if (key == 'q')
                quit_app();
            else if (key == '
        }

        Sleep(wait_time);
    }

    void App::display_info()
    {
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &current_song_buffer_info))
        {
            bar_width = current_song_buffer_info.srWindow.Right - current_song_buffer_info.srWindow.Left - 19;
            bar_height = current_song_buffer_info.srWindow.Bottom - current_song_buffer_info.srWindow.Top - 1;
        }

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, static_cast<short>(current_song) });

        size_t colon_pos = current_song_display_length.find(':');
        int minutes = std::stoi(current_song_display_length.substr(0, colon_pos));
        int seconds = std::stoi(current_song_display_length.substr(colon_pos + 1));
        int time = minutes * 60 + seconds;

        DWORD progress = Player::get_progress();
        progress = progress / 1000;
        int progress_percent = int((double)progress / time * 100.0);
        std::string progress_percent_blank_space;
        if (progress_percent < 10) progress_percent_blank_space = "     ";
        else if (progress_percent < 100 && progress_percent >= 10) progress_percent_blank_space = "    ";
        else progress_percent_blank_space = "   ";

        int volume_percent;
        if (volume == 0) volume_percent = 0;
        else if (volume == 50) volume_percent = 5;
        else volume_percent = volume / 10;
        std::string volume_percent_blank_space;
        if (volume_percent < 10) volume_percent_blank_space = "  ";
        else if (volume_percent < 100 && volume_percent >= 10) volume_percent_blank_space = " ";
        else volume_percent_blank_space = "";

        std::string new_lines_blank_space;
        for (int i = 0; i < bar_width; ++i)
            new_lines_blank_space += " ";
        std::string new_lines;
        for (int i = 0; i < bar_height - current_song; ++i)
            new_lines += "\n" + new_lines_blank_space;
        if (bar_height - current_song + 1 > 0) new_lines += "\n";

        int pos = (int)((double)progress / time * bar_width);
        std::cout << "\r" + new_lines + " [";
        for (int i = 0; i < bar_width; ++i)
        {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << ".";
        }
        if (progress_percent > 100) progress_percent = 100;
        std::cout << "] " << progress_percent << "%" << progress_percent_blank_space << "Vol: " << volume_percent << "%" << volume_percent_blank_space << "\r";
        std::cout.flush();

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, static_cast<short>(current_song) });

        current_song_progress = progress_percent;
        Discord::update_progress(current_song_progress);
    }

    void App::init()
    {
        Platform::init();
        Discord::init();

        songs_directory = Platform::working_directory + "\\Songs";
        load_files();
        if (files.empty())
        {
            cleanup();
            exit(1);
        }

        volume_path = Platform::working_directory + "\\user\\volume.txt";
        init_volume();
    }

    void App::cleanup()
    {
        Player::close();
        Platform::cleanup();
        Discord::cleanup();
    }

    void App::load_files()
    {
        std::string search_term = songs_directory + "\\*.mp3";

        WIN32_FIND_DATA find_data;
        HANDLE find_handle = FindFirstFile(search_term.c_str(), &find_data);
        if (find_handle == INVALID_HANDLE_VALUE)
            files = std::vector<std::string>();

        do
        {
            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                continue;

            files.push_back(find_data.cFileName);
        } while (FindNextFile(find_handle, &find_data) != 0);

        FindClose(find_handle);
    }

    void App::init_volume()
    {
        std::ifstream file_check(volume_path);
        bool file_exists = file_check.good();
        if (!file_exists)
        {
            std::ofstream volume_file(volume_path);
            if (!volume_file.is_open())
            {
                std::cout << "Failed to open volume file!" << std::endl;
                volume = 100;
            }

            volume_file << 100;
            volume_file.close();
            volume = 100;
        }
        else
        {
            std::ifstream volume_file(volume_path);
            if (!volume_file.is_open())
            {
                std::cout << "Failed to open volume file!" << std::endl;
                volume = 100;
            }

            std::string line;
            std::getline(volume_file, line);
            volume_file.close();
            volume = std::stoi(line);
        }
    }

    void App::choose_random_song()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, (int)files.size() - 1);
        current_song_index = dis(gen);

        current_song_name = files[(size_t)current_song_index];
        current_song_path = "\"" + songs_directory + "\\" + current_song_name + "\"";
    }

    void App::display_song()
    {
        int total_songs = (int)files.size();

        if (current_song_name.find("---") != std::string::npos)
            current_song_name = std::regex_replace(current_song_name, std::regex("---"), "|");
        if (current_song_name.find("___") != std::string::npos)
            current_song_name = std::regex_replace(current_song_name, std::regex("___"), ":");
        current_song_name = std::regex_replace(current_song_name, std::regex(".mp3"), "");
        std::string info = " " + std::to_string(current_song) + " | " + std::to_string(current_song_index + 1) + "/" + std::to_string(total_songs) + " | " + current_song_name;

        int screen_width = 0;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &current_song_buffer_info))
            screen_width = current_song_buffer_info.srWindow.Right - current_song_buffer_info.srWindow.Left + 1;
        std::string blank_space;
        for (int i = 0; i < screen_width - (int)info.size(); ++i)
            blank_space += " ";

        std::cout << info << blank_space << std::endl;

        Discord::update_presence(current_song_name);

        std::string length;
        auto it = std::find(current_song_name.rbegin(), current_song_name.rend(), ' ');
        if (it != current_song_name.rend())
        {
            ++it;
            std::string result(it.base(), current_song_name.end());
            length = result;
        }

        current_song_display_length = length;
    }
}
