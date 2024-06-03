#pragma once

#include <minwindef.h>

namespace tmp::player
{
  bool song_ended();
  void open();
  void set_volume(int volume);
  void play();
  void close();
  void resume();
  void pause();
  DWORD get_progress();

  extern bool song_playing;
}
