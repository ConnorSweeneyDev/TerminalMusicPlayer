#pragma once

#include <minwindef.h>

namespace tmp::player
{
  void play();
  void pause();
  void resume();
  void set_volume(int volume);
  double get_progress();
  double get_duration();
  bool song_ended();

  void open();
  void close();

  extern bool song_playing;
}
