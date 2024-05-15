#include <iostream>
#include <fstream>
#include <processthreadsapi.h>
#include <string>

#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include <conio.h>
#include <direct.h>

#include "init.hpp"

namespace init
{
    void cursor_visible(bool is_visible)
    {
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursor_info;
        GetConsoleCursorInfo(out, &cursor_info);
        cursor_info.bVisible = is_visible;
        SetConsoleCursorInfo(out, &cursor_info);
    }

    std::string console()
    {
        system("cls");
        system("color 09");

        cursor_visible(false);

        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        GetConsoleMode(hStdin, &mode);
        mode &= ~ENABLE_LINE_INPUT;
        SetConsoleMode(hStdin, mode);

        char cwd[FILENAME_MAX];
        if (_getcwd(cwd, sizeof(cwd)) == NULL) {
            std::cerr << "Error getting current working directory" << std::endl;
            exit(1);
        }

        return std::string(cwd);
    }

    void discord(const std::string& cwd)
    {
        std::ifstream file_check(cwd + "\\config.ini");
        bool file_exists = file_check.good();
        if (!file_exists)
        {
            std::ofstream config_file(cwd + "\\config.ini");
            if (!config_file.is_open())
                std::cout << "Failed to open config file" << std::endl;

            config_file << "";
            config_file.close();
        }

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        std::string cmd_line = "pwsh -ExecutionPolicy Bypass -Command Start-Process -d \"" + cwd + "\" -FilePath \"" + cwd + "\\binary\\easyrp.exe\" -Confirm:$false -WindowStyle Hidden";

        TCHAR cmd_line_tchar[MAX_PATH];
        #ifdef UNICODE
            _tcscpy_s(cmd_line_tchar, cmd_line.c_str());
        #else
            strcpy_s(cmd_line_tchar, cmd_line.c_str());
        #endif

        if (!CreateProcess(NULL, cmd_line_tchar, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
        {
            std::cout << "CreateProcess failed: " << GetLastError() << ".\n";
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

    std::vector<std::string> song_list(const std::string& directory)
    {
        std::vector<std::string> files = get_files_in_directory(directory);
        if (files.empty())
        {
            std::cout << "No files found in directory: " << directory << std::endl;
            return files;
        }

        return files;
    }

    int volume(const std::string& cwd)
    {
        std::ifstream file_check(cwd + "\\user\\volume.txt");
        bool file_exists = file_check.good();
        if (!file_exists)
        {
            std::ofstream volume_file(cwd + "\\user\\volume.txt");
            if (!volume_file.is_open())
            {
                std::cout << "Failed to open volume file" << std::endl;
                return -1;
            }

            volume_file << 100;
            volume_file.close();
            return 100;
        }
        else
        {
            std::ifstream volume_file(cwd + "\\user\\volume.txt");
            if (!volume_file.is_open())
            {
                std::cout << "Failed to open volume file" << std::endl;
                return -1;
            }

            std::string line;
            std::getline(volume_file, line);
            volume_file.close();
            return std::stoi(line);
        }
    }
}
