#include <iostream>
#include <string>
#include <windows.h>

#include <SDL_mixer.h>

#include "app.hpp"
#include "player.hpp"

namespace tmp::player
{
  Mix_Music *music = nullptr;
  bool song_playing = false;

  void play() { Mix_PlayMusic(music, 0); }
  void pause() { Mix_PauseMusic(); }
  void resume() { Mix_ResumeMusic(); }
  void set_volume(int volume) { Mix_VolumeMusic(volume); }
  double get_progress() { return Mix_GetMusicPosition(music); }
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
  }
  void close()
  {
    Mix_FreeMusic(music);
    music = nullptr;
  }
}
