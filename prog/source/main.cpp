#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

#include <windows.h>
#include <mmsystem.h>
#include <conio.h>

void disable_visual_input()
{
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    mode &= ~ENABLE_LINE_INPUT;
    SetConsoleMode(hStdin, mode);
}

std::vector<std::string> get_files_in_directory(const std::string& directory)
{
    std::vector<std::string> files;
    std::string search_term = directory + "\\*.mp3";

    WIN32_FIND_DATA find_data;
    HANDLE find_handle = FindFirstFile(search_term.c_str(), &find_data);
    if (find_handle == INVALID_HANDLE_VALUE)
        return files;

    do
    {
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        files.push_back(find_data.cFileName);
    } while (FindNextFile(find_handle, &find_data) != 0);

    FindClose(find_handle);

    return files;
}

std::vector<std::string> initialize_song_list(const std::string& directory)
{
    std::vector<std::string> files = get_files_in_directory(directory);
    if (files.empty())
    {
        std::cout << "No files found in directory: " << directory << std::endl;
        return files;
    }

    return files;
}

int initialize_volume()
{
    std::ifstream file_check("C:\\Users\\conno\\Documents\\Programming\\C++\\TerminalMusicPlayer\\volume.txt");
    bool file_exists = file_check.good();
    if (!file_exists)
    {
        std::ofstream volume_file("C:\\Users\\conno\\Documents\\Programming\\C++\\TerminalMusicPlayer\\volume.txt");
        if (!volume_file.is_open())
        {
            std::cout << "Failed to open volume file" << std::endl;
            return 1;
        }
        
        volume_file << 100;
        volume_file.close();
        return 100;
    }
    else
    {
        std::ifstream volume_file("C:\\Users\\conno\\Documents\\Programming\\C++\\TerminalMusicPlayer\\volume.txt");
        if (!volume_file.is_open())
        {
            std::cout << "Failed to open volume file" << std::endl;
            return 1;
        }

        std::string line;
        std::getline(volume_file, line);
        return std::stoi(line);
    }
}

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

void pause_or_play(bool& is_paused)
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
}

void increase_volume(int& volume)
{
    volume += 50;
    if (volume > 1000)
        volume = 1000;
    else if (volume < 0)
        volume = 0;

    set_volume(volume);

    std::ofstream volume_file("C:\\Users\\conno\\Documents\\Programming\\C++\\TerminalMusicPlayer\\volume.txt");
    if (!volume_file.is_open())
    {
        std::cout << "Failed to open volume file" << std::endl;
        exit(1);
    }

    volume_file << volume;
}

void decrease_volume(int& volume)
{
    volume -= 50;
    if (volume > 1000)
        volume = 1000;
    else if (volume < 0)
        volume = 0;

    set_volume(volume);

    std::ofstream volume_file("C:\\Users\\conno\\Documents\\Programming\\C++\\TerminalMusicPlayer\\volume.txt");
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

int main()
{
    disable_visual_input();

    std::string directory = "C:\\Users\\conno\\Documents\\Programming\\C++\\TerminalMusicPlayer\\Songs";
    std::vector<std::string> files = initialize_song_list(directory);
    if (files.empty())
        return 1;

    int volume = initialize_volume();

    int current_song = 0;
    while (true)
    {
        current_song++;

        int random_index;
        std::string song = generate_song(files, random_index);
        std::string path = "\"" + directory + "\\" + song + "\"";

        open_song(path);
        set_volume(volume);
        play_song(path);

        std::string song_name = song.substr(0, song.length() - 4);
        std::cout << " " << current_song << " | " << random_index + 1 << "/" << files.size() << " | " << song_name << std::endl;

        std::string command;
        bool is_paused = false;
        while (!song_ended())
        {
            if (_kbhit())
            {
                char key = _getch();
                if (key == 'p')
                {
                    pause_or_play(is_paused);
                }
                else if (key == 'u')
                {
                    increase_volume(volume);
                }
                else if (key == 'd')
                {
                    decrease_volume(volume);
                }
                else if (key == 'n')
                    break;
                else if (key == 'q')
                {
                    close_song(path);
                    return 0;
                }
            }

            Sleep(100);
        }

        close_song(path);
    }

    return 0;
}
