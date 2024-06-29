#pragma once

#include <minwindef.h>

namespace tmp::player
{
  double get_progress();
  double get_duration();
  float get_decibels();
  void set_volume(int volume);

  void play();
  void pause();
  void resume();
  bool song_ended();

  void open();
  void close();

  extern bool song_playing;
}
