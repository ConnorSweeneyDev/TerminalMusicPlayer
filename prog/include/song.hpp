#pragma once

#include <string>
#include <vector>

std::string generate_song(const std::vector<std::string>& files, int& random_index);
void open_song(const std::string& path);
void display_song_info(const std::string& song, int current_song, int random_index, int total_songs);
void play_song(const std::string& path);
void pause_or_play(bool& is_paused);
void set_volume(int volume);
void increase_volume(int& volume);
void decrease_volume(int& volume);
void close_song(const std::string& path);
bool song_ended();
