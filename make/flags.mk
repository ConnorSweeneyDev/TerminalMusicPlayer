DEBUG := 0

CXX = g++
ifeq ($(DEBUG), 1)
  CXXFLAGS = -g -MD -MP -O2 -std=c++20 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fstack-protector-strong
else
  CXXFLAGS = -s -O3 -std=c++20 -DNDEBUG -D_FORTIFY_SOURCE=2 -fstack-protector-strong
endif

WARNINGS = -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wcast-qual -Wcast-align -Wfloat-equal -Wlogical-op -Wduplicated-cond -Wshift-overflow=2 -Wformat=2
INCLUDES = -Iprogram/include
SYSTEM_INCLUDES = -isystemexternal/include -isystemexternal/include/sdl2 -isystemexternal/include/taglib
LIBRARIES = -Lexternal/library/sdl2 -Lexternal/library/taglib -static -Wl,-Bstatic -lgcc -lstdc++ -lssp -lwinpthread -lwinmm -lmingw32 -Wl,-Bdynamic -lSDL2main -lSDL2 -lSDL2_mixer -ltag
OUTPUT = binary/TerminalMusicPlayer.exe
