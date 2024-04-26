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

int main()
{
    disable_visual_input();

    std::string directory = "C:\\Users\\conno\\Documents\\Programming\\C++\\MusicPlayer\\Songs";
    std::vector<std::string> files = get_files_in_directory(directory);
    if (files.empty())
    {
        std::cout << "No files found in directory: " << directory << std::endl;
        return 1;
    }

    int volume = 100;
    std::ifstream file_check("C:\\Users\\conno\\Documents\\Programming\\C++\\MusicPlayer\\volume.txt");
    bool file_exists = file_check.good();
    if (!file_exists)
    {
        std::ofstream volume_file("C:\\Users\\conno\\Documents\\Programming\\C++\\MusicPlayer\\volume.txt");
        if (!volume_file.is_open())
        {
            std::cout << "Failed to open volume file" << std::endl;
            return 1;
        }
        
        volume_file << volume;
        volume_file.close();
    }
    else
    {
        std::ifstream volume_file("C:\\Users\\conno\\Documents\\Programming\\C++\\MusicPlayer\\volume.txt");
        if (!volume_file.is_open())
        {
            std::cout << "Failed to open volume file" << std::endl;
            return 1;
        }

        std::string line;
        std::getline(volume_file, line);
        volume = std::stoi(line);
    }

    int current_song = 0;
    while (true)
    {
        current_song++;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, files.size() - 1);
        int random_index = dis(gen);
        std::string song = files[random_index];
        std::string path = "\"" + directory + "\\" + song + "\"";

        std::string command = "open " + path + " type mpegvideo alias mp3";
        MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char error_message[256];
            mciGetErrorString(error, error_message, sizeof(error_message));
            std::cout << "Error opening file: " << path << ": " << error_message << std::endl;
            command = "close mp3";
            mciSendString(command.c_str(), NULL, 0, NULL);
            return 1;
        }

        command = "setaudio mp3 volume to " + std::to_string(volume);
        error = mciSendString(command.c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char error_message[256];
            mciGetErrorString(error, error_message, sizeof(error_message));
            std::cout << "Error setting volume: " << error_message << std::endl;
            command = "close mp3";
            mciSendString(command.c_str(), NULL, 0, NULL);
            return 1;
        }

        command = "play mp3";
        error = mciSendString(command.c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char error_message[256];
            mciGetErrorString(error, error_message, sizeof(error_message));
            std::cout << "Error playing file: " << path << ": " << error_message << std::endl;
            command = "close mp3";
            mciSendString(command.c_str(), NULL, 0, NULL);
            return 1;
        }
        std::string song_name = song.substr(0, song.length() - 4);
        std::cout << " " << current_song << " | " << random_index + 1 << "/" << files.size() << " | " << song_name << std::endl;

        bool is_paused = false;
        while (!song_ended())
        {
            if (_kbhit())
            {
                char key = _getch();
                if (key == 'p')
                {
                    if (is_paused)
                    {
                        command = "play mp3";
                        is_paused = false;
                    }
                    else
                    {
                        command = "pause mp3";
                        is_paused = true;
                    }
                    mciSendString(command.c_str(), NULL, 0, NULL);
                }
                else if (key == 'u')
                {
                    volume += 50;
                    if (volume > 1000)
                        volume = 1000;
                    else if (volume < 0)
                        volume = 0;

                    command = "setaudio mp3 volume to " + std::to_string(volume);
                    mciSendString(command.c_str(), NULL, 0, NULL);

                    std::ofstream volume_file("C:\\Users\\conno\\Documents\\Programming\\C++\\MusicPlayer\\volume.txt");
                    if (!volume_file.is_open())
                    {
                        std::cout << "Failed to open volume file" << std::endl;
                        return 1;
                    }

                    volume_file << volume;
                }
                else if (key == 'd')
                {
                    volume -= 50;
                    if (volume > 1000)
                        volume = 1000;
                    else if (volume < 0)
                        volume = 0;

                    command = "setaudio mp3 volume to " + std::to_string(volume);
                    mciSendString(command.c_str(), NULL, 0, NULL);

                    std::ofstream volume_file("C:\\Users\\conno\\Documents\\Programming\\C++\\MusicPlayer\\volume.txt");
                    if (!volume_file.is_open())
                    {
                        std::cout << "Failed to open volume file" << std::endl;
                        return 1;
                    }

                    volume_file << volume;
                }
                else if (key == 'n')
                {
                    break;
                }
                else if (key == 'q')
                {
                    command = "close mp3";
                    mciSendString(command.c_str(), NULL, 0, NULL);
                    return 0;
                }
            }
            Sleep(100);
        }

        command = "close mp3";
        error = mciSendString(command.c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char error_message[256];
            mciGetErrorString(error, error_message, sizeof(error_message));
            std::cout << "Error closing file: " << path << ": " << error_message << std::endl;
            return 1;
        }
    }

    return 0;
}
