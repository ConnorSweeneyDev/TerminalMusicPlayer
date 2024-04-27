# TerminalMusicPlayer

A fast and simple music player for the terminal. 'p' to pause, 'n' to go next, 'u' and 'd' for volume up and down, 'q' to quit.
Also has discord integration, showing information about the song being played, the current session and the status of the player.

I made this because it was annoying to have to use a mouse to control my music, however this does have some limitations - it is
windows only and mp3 only, and if any of the files have any of the newest IDvX tags the program will crash due to using some old 
windows functions. The only tags that are required on the mp3 files are `title` and `artist`, the rest are unused.

To run this successfully, put a folder called `Songs` in the root of the project, then run `update_song_names` and make sure there
are no error messages, this will rename all the files to the correct format. Then rename `config_template.ini` to `config.ini` and
build and run the C++ code.

## Dependencies
- Windows
- PowerShell (pwsh.exe)
- MinGW 64-bit
- Mutagen
- EasyRP
- A discord bot set up on the developer portal
