import os
import math
from mutagen.mp3 import MP3

directory = "Songs"

replace_chars = {
    "<": "",
    ">": "",
    ":": "",
    "/": "",
    "\\": "",
    "|": "",
    "?": "",
    "*": "",
    "‘": "'",
    "’": "'",
    "“": "'",
    "”": "'",
    "\"": "'",

    "á": "a",
    "å": "a",
    "é": "e",
    "í": "i",
    "ó": "o",
    "Ö": "O",
    "ú": "u",
    "ñ": "n",
    "ç": "c",
    "ä": "a",
    "ë": "e",
    "ï": "i",
    "ö": "o",
    "ü": "u",
    "Á": "A",
    "É": "E",
    "Í": "I",
    "Ó": "O",
    "Ú": "U",
    "Ñ": "N",
    "Ç": "C",
    "Ä": "A",
    "Ë": "E",
    "Ï": "I",
    "Ü": "U",
    "ß": "B",
    "ø": "o",
    "Ø": "O",
    "æ": "ae",
    "Æ": "AE",
    "œ": "oe",
    "Œ": "OE",
    "Å": "A",
    "Þ": "D",
    "þ": "b",
    "ð": "d",
    "Ý": "Y",
    "ý": "y",
    "ÿ": "y",
    "Ÿ": "Y",
    "Š": "S",
    "š": "s",
    "Ž": "Z",
    "ž": "z",
}

for filename in os.listdir(directory):
    file_path = os.path.join(directory, filename)

    try:
        audio = MP3(file_path)
        title = audio["TIT2"][0] if "TIT2" in audio else "Unknown Title"
        artist = audio["TPE1"][0] if "TPE1" in audio else "Unknown Artist"
        duration = audio.info.length
        duration = str(math.trunc(duration // 60)) + "___" + str(math.trunc(duration % 60)).zfill(2)

        title = ''.join(replace_chars.get(char, char) for char in title)
        artist = ''.join(replace_chars.get(char, char) for char in artist)

        spaces = ["  ", "   ", "    "]
        for space in spaces:
            if space in title:
                title = title.replace(space, " ")
            if space in artist:
                artist = artist.replace(space, " ")

        new_filename = title + " --- " + artist + " --- " + duration + ".mp3"
        new_file_path = os.path.join(directory, new_filename)
        os.rename(file_path, new_file_path)

        print("Renamed " + filename + " --> " + new_filename)
    except Exception as e:
        print(f"Error renaming {filename}: {e}")
