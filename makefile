RM = rm -r
CXX = g++

# RELEASE FLAGS:
CXXFLAGS = -s -O3 -std=c++20 -DNDEBUG -D_FORTIFY_SOURCE=2 -fstack-protector-strong

# DEBUG FLAGS:
# CXXFLAGS = -g -O0 -std=c++20 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fstack-protector-strong

WARNINGS = -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wcast-qual -Wcast-align -Wfloat-equal -Wlogical-op -Wduplicated-cond -Wshift-overflow=2 -Wformat=2
INCLUDES = -Iprogram/include
LIBRARIES = -lwinmm 
CPP_SOURCES = $(wildcard program/source/*.cpp)
OUTPUT = binary/TerminalMusicPlayer.exe

OBJECTS_DIR = object
COMMANDS_DIR = compile_commands.json
FORMAT_DIR = .clang-format

$(shell if [ ! -d "$(OBJECTS_DIR)" ]; then mkdir -p $(OBJECTS_DIR); fi)
OBJECTS = $(patsubst program/source/%.cpp,$(OBJECTS_DIR)/%.o,$(CPP_SOURCES))

all: compile_commands clang-format $(OUTPUT)

compile_commands:
	@echo -n > $(COMMANDS_DIR)
	@echo "[" >> $(COMMANDS_DIR)
	@for source in $(CPP_SOURCES); do echo "    { \"directory\": \"$(CURDIR)\", \"command\": \"$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) $(LIBRARIES) -c $$source -o $(OBJECTS_DIR)/$$(basename $$source .cpp).o\", \"file\": \"$$source\" },"; done >> $(COMMANDS_DIR)
	@sed -i "$$ s/,$$//" $(COMMANDS_DIR)
	@echo "]" >> $(COMMANDS_DIR)
	@echo "$(COMMANDS_DIR) updated."

clang-format:
	@echo -n > $(FORMAT_DIR)
	@echo "---" >> $(FORMAT_DIR)
	@echo "BasedOnStyle: LLVM" >> $(FORMAT_DIR)
	@echo "IndentWidth: 2" >> $(FORMAT_DIR)
	@echo "ConstructorInitializerIndentWidth: 2" >> $(FORMAT_DIR)
	@echo "ContinuationIndentWidth: 2" >> $(FORMAT_DIR)
	@echo "BreakBeforeBraces: Allman" >> $(FORMAT_DIR)
	@echo "---" >> $(FORMAT_DIR)
	@echo "Language: Cpp" >> $(FORMAT_DIR)
	@echo "ColumnLimit: 100" >> $(FORMAT_DIR)
	@echo "AllowShortBlocksOnASingleLine: true" >> $(FORMAT_DIR)
	@echo "AllowShortFunctionsOnASingleLine: true" >> $(FORMAT_DIR)
	@echo "AllowShortIfStatementsOnASingleLine: true" >> $(FORMAT_DIR)
	@echo "AllowShortLoopsOnASingleLine: true" >> $(FORMAT_DIR)
	@echo "AllowShortCaseLabelsOnASingleLine: true" >> $(FORMAT_DIR)
	@echo "IndentPPDirectives: BeforeHash" >> $(FORMAT_DIR)
	@echo "NamespaceIndentation: All" >> $(FORMAT_DIR)
	@echo "FixNamespaceComments: false" >> $(FORMAT_DIR)
	@echo "AlwaysBreakTemplateDeclarations: false" >> $(FORMAT_DIR)
	@echo "..." >> $(FORMAT_DIR)
	@clang-format -i program/**/*
	@echo "$(FORMAT_DIR) updated."

$(OUTPUT): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) $(OBJECTS) $(LIBRARIES) -o $(OUTPUT)
$(OBJECTS_DIR)/%.o: program/source/%.cpp
	$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) -c $< -o $@

clean:
	@if [ -d "$(OBJECTS_DIR)" ]; then $(RM) $(OBJECTS_DIR); fi
	@if [ -f $(OUTPUT) ]; then $(RM) $(OUTPUT); fi
