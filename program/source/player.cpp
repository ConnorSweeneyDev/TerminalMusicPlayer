#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <windows.h>

#include <SDL_mixer.h>

#include "app.hpp"
#include "player.hpp"

namespace tmp::player
{
  Mix_Music *music = nullptr;
  bool song_playing = false;
  float current_song_decibels;

  double get_progress() { return Mix_GetMusicPosition(music); }
  double get_duration() { return Mix_MusicDuration(music); }
  float get_decibels()
  {
    system(("pwsh -Command \"ffmpeg -i '" + app.current_song_path +
            "' -af volumedetect -f null / 2>user\\ffmpeg.txt\"")
             .c_str());
    system("pwsh -Command \"Select-String -Path user\\ffmpeg.txt -Pattern 'mean_volume: "
           "-?[0-9]+.[0-9]+' -AllMatches | ForEach-Object { $_.Matches.Value } | Set-Content -Path "
           "user\\decibels.txt\"");

    std::ifstream decibels_file("user\\decibels.txt");
    if (!decibels_file.is_open())
    {
      std::cout << "Failed to open decibels file!" << std::endl;
      return 0;
    }
    std::string line;
    std::getline(decibels_file, line);
    decibels_file.close();

    std::regex decibels_regex("-?[0-9]+.[0-9]+");
    std::smatch match;
    std::regex_search(line, match, decibels_regex);
    float decibels = std::stof(match[0]);

    return decibels;
  }
  void set_volume(int volume)
  {
    float real_volume = static_cast<float>(volume);
    real_volume = real_volume * (current_song_decibels / -14.0f);
    if (real_volume > MIX_MAX_VOLUME) real_volume = 128;
    if (real_volume < 0) real_volume = 0;

    Mix_VolumeMusic((int)real_volume);
  }

  void play() { Mix_PlayMusic(music, 0); }
  void pause() { Mix_PauseMusic(); }
  void resume() { Mix_ResumeMusic(); }
  bool song_ended() { return Mix_PlayingMusic() == 0; }

  void open()
  {
    music = Mix_LoadMUS(app.current_song_path.c_str());
    if (music == nullptr)
    {
      std::cout << "Error loading file: " << app.current_song_path << ": " << Mix_GetError()
                << std::endl;
      app.cleanup();
      exit(1);
    }

    current_song_decibels = get_decibels();
  }
  void close()
  {
    Mix_FreeMusic(music);
    music = nullptr;
  }
}
