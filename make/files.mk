OUTPUT_FILE = binary/TerminalMusicPlayer.exe
PROGRAM_SOURCE_DIRECTORY = program/source
PROGRAM_INCLUDE_DIRECTORY = program/include
BINARY_DIRECTORY = binary
BINARY_OBJECT_DIRECTORY = binary/object
CPP_SOURCE_FILES = $(wildcard $(PROGRAM_SOURCE_DIRECTORY)/*.cpp)
OBJECT_FILES = $(patsubst $(PROGRAM_SOURCE_DIRECTORY)/%.cpp,$(BINARY_OBJECT_DIRECTORY)/%.o,$(CPP_SOURCE_FILES))
DEPENDENCY_FILES := $(patsubst $(BINARY_OBJECT_DIRECTORY)/%.o,$(BINARY_OBJECT_DIRECTORY)/%.d,$(OBJECT_FILES))
