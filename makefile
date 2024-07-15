CXX = g++
# RELEASE FLAGS:
CXXFLAGS = -s -O3 -std=c++20 -DNDEBUG -D_FORTIFY_SOURCE=2 -fstack-protector-strong
# DEBUG FLAGS:
# CXXFLAGS = -g -O2 -std=c++20 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fstack-protector-strong

WARNINGS = -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wcast-qual -Wcast-align -Wfloat-equal -Wlogical-op -Wduplicated-cond -Wshift-overflow=2 -Wformat=2
INCLUDES = -Iprogram/include
SYSTEM_INCLUDES = -isystemexternal/include -isystemexternal/include/sdl2 -isystemexternal/include/taglib
LIBRARIES = -Lexternal/library/sdl2 -Lexternal/library/taglib -lwinmm -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -ltag
OUTPUT = binary/TerminalMusicPlayer.exe

PROGRAM_SOURCE_DIRECTORY = program/source
OBJECTS_DIRECTORY = binary/object
CPP_SOURCES = $(wildcard $(PROGRAM_SOURCE_DIRECTORY)/*.cpp)
OBJECTS = $(patsubst $(PROGRAM_SOURCE_DIRECTORY)/%.cpp,$(OBJECTS_DIRECTORY)/%.o,$(CPP_SOURCES))

COMMANDS_DIRECTORY = compile_commands.json
FORMAT_DIRECTORY = .clang-format
STYLE = BasedOnStyle: LLVM
TAB_WIDTH = IndentWidth: 2
INITIALIZER_WIDTH = ConstructorInitializerIndentWidth: 2
CONTINUATION_WIDTH = ContinuationIndentWidth: 2
BRACES = BreakBeforeBraces: Allman
LANGUAGE = Language: Cpp
LIMIT = ColumnLimit: 100
BLOCKS = AllowShortBlocksOnASingleLine: true
FUNCTIONS = AllowShortFunctionsOnASingleLine: true
IFS = AllowShortIfStatementsOnASingleLine: true
LOOPS = AllowShortLoopsOnASingleLine: true
CASE_LABELS = AllowShortCaseLabelsOnASingleLine: true
PP_DIRECTIVES = IndentPPDirectives: BeforeHash
NAMESPACE_INDENTATION = NamespaceIndentation: All
NAMESPACE_COMMENTS = FixNamespaceComments: false
INDENT_CASE_LABELS = IndentCaseLabels: true
BREAK_TEMPLATE_DECLARATIONS = AlwaysBreakTemplateDeclarations: false

all: compile_commands clang-format object $(OUTPUT)

compile_commands:
	@echo "[" > $(COMMANDS_DIRECTORY)
	@for source in $(CPP_SOURCES); do echo -e "\t{ \"directory\": \"$(CURDIR)\", \"command\": \"$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) $(SYSTEM_INCLUDES) -c $$source -o $(OBJECTS_DIRECTORY)/$$(basename $$source .cpp).o\", \"file\": \"$$source\" },"; done >> $(COMMANDS_DIRECTORY)
	@sed -i "$$ s/,$$//" $(COMMANDS_DIRECTORY)
	@echo "]" >> $(COMMANDS_DIRECTORY)
	@echo "Wrote    | $(COMMANDS_DIRECTORY)"

clang-format:
	@echo -e "---\n$(STYLE)\n$(TAB_WIDTH)\n$(INITIALIZER_WIDTH)\n$(CONTINUATION_WIDTH)\n$(BRACES)\n---\n$(LANGUAGE)\n$(LIMIT)\n$(BLOCKS)\n$(FUNCTIONS)\n$(IFS)\n$(LOOPS)\n$(CASE_LABELS)\n$(PP_DIRECTIVES)\n$(NAMESPACE_INDENTATION)\n$(NAMESPACE_COMMENTS)\n$(INDENT_CASE_LABELS)\n$(BREAK_TEMPLATE_DECLARATIONS)\n..." > $(FORMAT_DIRECTORY)
	@find program -type f \( -name "*.cpp" -o -name "*.hpp" \) -print0 | xargs -0 -I{} sh -c 'clang-format -i "{}"'
	@echo "Wrote    | $(FORMAT_DIRECTORY)"

object:
	@if [ ! -d "$(OBJECTS_DIRECTORY)" ]; then mkdir -p $(OBJECTS_DIRECTORY); echo "Created  | $(OBJECTS_DIRECTORY)"; fi

$(OUTPUT): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) $(SYSTEM_INCLUDES) $(OBJECTS) $(LIBRARIES) -o $(OUTPUT)
	@echo "Linked   | $(OBJECTS) -> $(OUTPUT)"
$(OBJECTS_DIRECTORY)/%.o: $(PROGRAM_SOURCE_DIRECTORY)/%.cpp
	@$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) $(SYSTEM_INCLUDES) -c $< -o $@
	@echo "Compiled | $< -> $@"

clean:
	@if [ -d "$(OBJECTS_DIRECTORY)" ]; then rm -r $(OBJECTS_DIRECTORY); fi
	@if [ -f $(OUTPUT) ]; then rm -r $(OUTPUT); fi
