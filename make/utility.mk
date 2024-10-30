COMPILE_COMMANDS_FILE := compile_commands.json

CLANGD_FILE := .clangd
UNUSED_INCLUDES := UnusedIncludes: Strict
MISSING_INCLUDES := MissingIncludes: Strict
IGNORE_HEADERS := IgnoreHeader: inl

CLANG_FORMAT_FILE := .clang-format
STYLE := BasedOnStyle: LLVM
TAB_WIDTH := IndentWidth: 2
INITIALIZER_WIDTH := ConstructorInitializerIndentWidth: 2
CONTINUATION_WIDTH := ContinuationIndentWidth: 2
BRACES := BreakBeforeBraces: Allman
LANGUAGE := Language: Cpp
LIMIT := ColumnLimit: 120
BLOCKS := AllowShortBlocksOnASingleLine: true
FUNCTIONS := AllowShortFunctionsOnASingleLine: true
IFS := AllowShortIfStatementsOnASingleLine: true
LOOPS := AllowShortLoopsOnASingleLine: true
CASE_LABELS := AllowShortCaseLabelsOnASingleLine: true
PP_DIRECTIVES := IndentPPDirectives: BeforeHash
NAMESPACE_INDENTATION := NamespaceIndentation: All
NAMESPACE_COMMENTS := FixNamespaceComments: false
INDENT_CASE_LABELS := IndentCaseLabels: true
BREAK_TEMPLATE_DECLARATIONS := AlwaysBreakTemplateDeclarations: false
TARGET_FORMAT_FILES := $(wildcard $(PROGRAM_SOURCE_DIRECTORY)/*.cpp) $(wildcard $(PROGRAM_INCLUDE_DIRECTORY)/*.hpp)

compile_commands:
	@echo "[" > $(COMPILE_COMMANDS_FILE)
	@for source in $(CPP_SOURCE_FILES); do echo -e "\t{ \"directory\": \"$(CURDIR)\", \"command\": \"$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) $(SYSTEM_INCLUDES) -c $$source -o $(OBJECT_DIRECTORY)/$$(basename $$source .cpp).o\", \"file\": \"$$source\" },"; done >> $(COMPILE_COMMANDS_FILE)
	@sed -i "$$ s/,$$//" $(COMPILE_COMMANDS_FILE)
	@echo "]" >> $(COMPILE_COMMANDS_FILE)
	@echo "WRITE | $(COMPILE_COMMANDS_FILE)"

clangd:
	@echo -e "Diagnostics:\n  $(UNUSED_INCLUDES)\n  $(MISSING_INCLUDES)\n  Includes:\n    $(IGNORE_HEADERS)" > $(CLANGD_FILE)
	@echo -e "WRITE | $(CLANGD_FILE)"

clang-format: $(TARGET_FORMAT_FILES)
	@echo -e "---\n$(STYLE)\n$(TAB_WIDTH)\n$(INITIALIZER_WIDTH)\n$(CONTINUATION_WIDTH)\n$(BRACES)\n---\n$(LANGUAGE)\n$(LIMIT)\n$(BLOCKS)\n$(FUNCTIONS)\n$(IFS)\n$(LOOPS)\n$(CASE_LABELS)\n$(PP_DIRECTIVES)\n$(NAMESPACE_INDENTATION)\n$(NAMESPACE_COMMENTS)\n$(INDENT_CASE_LABELS)\n$(BREAK_TEMPLATE_DECLARATIONS)\n..." > $(CLANG_FORMAT_FILE)
	@echo "WRITE | $(CLANG_FORMAT_FILE)"
	@for file in $^; do clang-format -i $$file; echo -e "FMT   | $$file"; done
