#include <iostream>
#include <memory>
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
    float decibels = -14.0f;

    std::string command =
      "ffmpeg -i \"" + app.current_song_path + "\" -filter:a volumedetect -f null /dev/null 2>&1";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe)
    {
      std::cout << "Failed to open pipe!" << std::endl;
      exit(1);
    }
    std::string ffmpeg_output;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) ffmpeg_output += buffer;
    pipe.reset();

    std::regex ffmpeg_regex("mean_volume: -?[0-9]+.[0-9]+");
    std::smatch ffmpeg_match;
    if (!std::regex_search(ffmpeg_output, ffmpeg_match, ffmpeg_regex)) return decibels;
    ffmpeg_output = ffmpeg_match[0];

    std::regex decibels_regex("-?[0-9]+.[0-9]+");
    std::smatch decibels_match;
    if (std::regex_search(ffmpeg_output, decibels_match, decibels_regex))
      decibels = std::stof(decibels_match[0]);

    return decibels;
  }
  void set_volume(int volume)
  {
    float real_volume = static_cast<float>(volume);
    real_volume = real_volume * (current_song_decibels / -14.0f);
    if (real_volume > MIX_MAX_VOLUME)
      real_volume = 128;
    else if (real_volume < 0)
      real_volume = 0;

    Mix_VolumeMusic((int)real_volume);
  }

  void play() { Mix_PlayMusic(music, 0); }
  void seek(double seconds) { Mix_SetMusicPosition(seconds); }
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
