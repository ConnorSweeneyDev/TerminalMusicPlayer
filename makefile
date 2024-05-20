RM = rm -r
CXX = g++
CC = gcc

# RELEASE FLAGS:
# CXXFLAGS = -s -O3 -std=c++20 -DNDEBUG -D_FORTIFY_SOURCE=2 -fstack-protector-strong

# DEBUG FLAGS:
CXXFLAGS = -g -O0 -std=c++20 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fstack-protector-strong

WARNINGS = -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wcast-qual -Wcast-align -Wfloat-equal -Wlogical-op -Wduplicated-cond -Wshift-overflow=2 -Wformat=2
INCLUDES = -Iprogram/include
LIBRARIES = -lwinmm 
CPP_SOURCES = $(wildcard program/source/*.cpp)
OUTPUT = binary/TerminalMusicPlayer.exe

OBJECTS_DIR = object
FLAGS_DIR = compile_flags.txt

$(shell if [ ! -d "$(OBJECTS_DIR)" ]; then mkdir -p $(OBJECTS_DIR); fi)
OBJECTS = $(patsubst program/source/%.cpp,$(OBJECTS_DIR)/%.o,$(CPP_SOURCES))

all: compile_flags $(OUTPUT)

compile_flags:
	@echo -n > $(FLAGS_DIR)
	@for flag in $(CXXFLAGS); do echo $$flag; done >> $(FLAGS_DIR)
	@for flag in $(WARNINGS); do echo $$flag; done >> $(FLAGS_DIR)
	@for flag in $(INCLUDES); do echo $$flag; done >> $(FLAGS_DIR)
	@for flag in $(LIBRARIES); do echo $$flag; done >> $(FLAGS_DIR)
	@echo -o$(OUTPUT) >> $(FLAGS_DIR)
	@echo "$(FLAGS_DIR) updated."

$(OUTPUT): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) $(OBJECTS) $(LIBRARIES) -o $(OUTPUT)
$(OBJECTS_DIR)/%.o: program/source/%.cpp
	$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) -c $< -o $@

clean:
	@if [ -d "$(OBJECTS_DIR)" ]; then $(RM) $(OBJECTS_DIR); fi
	@if [ -f $(OUTPUT) ]; then $(RM) $(OUTPUT); fi
