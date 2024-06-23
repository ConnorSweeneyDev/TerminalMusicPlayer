# TerminalMusicPlayer
A fast and simple music player for the terminal. 'p' to pause, 'n' to go next, 'u' and 'd' for
volume up and down, 'q' to quit. Alternatively, media controls for pause, skip and stop work too.
Also has discord integration, showing information about the song being played, the current session,
the status of the player and song progress.

https://github.com/ConnorSweeneyDev/TerminalMusicPlayer/assets/75945279/b4bbb381-3121-4223-ac8b-b01c84be278a

*Showcase: song history, progress bar, volume indicator, blue signifies unpaused, red signifies
paused.*

I made this because it was annoying to have to use a mouse to control my music, however this does
have the limitation of being windows and mp3 only. 

## Dependencies
- Windows.
- PowerShell (pwsh.exe) &rightarrow; Run `winget install --id Microsoft.WindowsTerminal`.
- MinGW 64-bit &rightarrow; Use my version of [MinGW](https://winlibs.com/) (look at
  `external/mingw_version_info.txt` or replace `libgcc_s_seh-1.dll` and `libstdc++-6.dll` in the
  `binary` folder with your own, as long as they are 64-bit.
- A discord app set up on the [developer portal](https://discord.com/developers/applications). Add a
  new application and enable rich presence for it, and make note of the application id.
- A folder anywhere on your computer with mp3 files in it.

To run this successfully, create `user/client_id.txt` and put the application id from the portal in
it. After that create `user/songs_directory.txt` and put the path to your songs folder in it. Then
you can build and run the C++ code using the batch files in `script/`. Optionally, you can supply
specific mp3 files from the selected folder as arguments to the program to put them at the top of
the play queue.

## Known Issues
- The screen can be made bigger (not smaller) before the song list starts scrolling, but after that
  any more resizing causes glitchy output.

## Updating SDL
On top of the having the dependencies installed, go to the releases page and download the file
ending `mingw.zip`. Extract this and go to `x86_64-w64-mingw32` and you will have access to three
important folders:
- `bin` which contains the file that can replace the one in `binary` folder of this project.
- `include/SDL2` which contains files that can replace the contents of the `external/include/sdl2`
  folder of this project.
- `lib` which contains the files (not the folders) to replace the contents of the
  `external/library/sdl2` folder of this project.

## Updating TagLib
