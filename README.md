# TerminalMusicPlayer

A fast and simple music player for the terminal. 'p' to pause, 'n' to go next, 'u' and 'd' for volume up and down, 'q' to quit.
Alternatively, media controls for pause, skip and stop work too. Also has discord integration, showing information about the song
being played, the current session, the status of the player and song progress.

https://github.com/ConnorSweeneyDev/TerminalMusicPlayer/assets/75945279/b4bbb381-3121-4223-ac8b-b01c84be278a

*Showcase: song history, progress bar, volume indicator, blue signifies unpaused, red signifies paused.*

I made this because it was annoying to have to use a mouse to control my music, however this does have the limitation of being windows and mp3 only. 

## Dependencies
- Windows.
- PowerShell (pwsh.exe) &rightarrow; Run `winget install --id Microsoft.WindowsTerminal`.
- MinGW 64-bit &rightarrow; Use my [version](https://github.com/brechtsanders/winlibs_mingw/releases/download/13.2.0posix-17.0.6-11.0.1-ucrt-r5/winlibs-x86_64-posix-seh-gcc-13.2.0-llvm-17.0.6-mingw-w64ucrt-11.0.1-r5.zip) of MinGW or replace the dlls in `binary/` with your own.
- A discord app set up on the [developer portal](https://discord.com/developers/applications). Add a new application and enable rich presence
  for it, and make note of the application id.
- A folder anywhere on your computer with mp3 files in it.

To run this successfully, create `user/client_id.txt` and put the application id from the portal in it. After that create `user/songs_directory.txt` and
put the path to your songs folder in it. Then you can build and run the C++ code using the batch files in `script/`. Optionally, you can supply specific
mp3 files from the selected folder as arguments to the program to put them at the top of the play queue.

## Known Issues
- Title and artist get cut off if they are longer than 30 characters.
- The screen can be made bigger (not smaller) before the song list starts scrolling, but after that any more resizing causes glitchy output.
