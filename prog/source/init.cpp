#include <iostream>
#include <fstream>
#include <processthreadsapi.h>
#include <string>
#include <vector>

#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include <conio.h>

#include "init.hpp"

namespace init
{
    void setup_console()
    {
        system("cls");

        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        GetConsoleMode(hStdin, &mode);
        mode &= ~ENABLE_LINE_INPUT;
        SetConsoleMode(hStdin, mode);
    }

    void setup_discord()
    {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        TCHAR cmdLine[] = _T("pwsh -ExecutionPolicy Bypass -Command Start-Process -d \"C:\\Users\\conno\\Documents\\Programming\\C++\\TerminalMusicPlayer\" -FilePath \"C:\\Users\\conno\\Documents\\Programming\\C++\\TerminalMusicPlayer\\easyrp.exe\" -Confirm:$false -WindowStyle Hidden");
        if (!CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
        {
            printf("CreateProcess failed (%d).\n", GetLastError());
            exit(1);
        }
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
}
