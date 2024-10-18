CXX = g++
ifeq ($(DEBUG), 1)
  CXXFLAGS = -ggdb3 -MD -MP -O2 -std=c++20 -DDEBUG -D_FORTIFY_SOURCE=2 -fno-common -fstack-protector-strong
else
  CXXFLAGS = -s -O3 -std=c++20 -DNDEBUG -fno-common -fstack-protector-strong -ffunction-sections -fdata-sections -flto=auto -Wl,--gc-sections
endif

WARNINGS = -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wcast-qual -Wcast-align -Wfloat-equal -Wlogical-op -Wduplicated-cond -Wshift-overflow=2 -Wformat=2
INCLUDES = -Iprogram/include
SYSTEM_INCLUDES = -isystemexternal/include -isystemexternal/include/sdl2 -isystemexternal/include/taglib
LIBRARIES = -Lexternal/library/sdl2 -Lexternal/library/taglib -static -Wl,-Bstatic -lgcc -lstdc++ -lssp -lwinpthread -lwinmm -lmingw32 -Wl,-Bdynamic -lSDL2main -lSDL2 -lSDL2_mixer -ltag
