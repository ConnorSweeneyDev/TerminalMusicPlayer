directories:
	@if [ ! -d "$(BINARY_DIRECTORY)" ]; then mkdir -p $(BINARY_DIRECTORY); echo "Write | $(BINARY_DIRECTORY)"; fi
	@if [ ! -d "$(OBJECT_DIRECTORY)" ]; then mkdir -p $(OBJECT_DIRECTORY); echo "Write | $(OBJECT_DIRECTORY)"; fi

$(OBJECT_DIRECTORY)/%.o: $(PROGRAM_SOURCE_DIRECTORY)/%.cpp $(PROGRAM_INCLUDE_DIRECTORY)/%.hpp | directories
	@$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) $(SYSTEM_INCLUDES) -c $< -o $@
	@echo "CXX   | $< -> $@"
$(OBJECT_DIRECTORY)/%.o: $(PROGRAM_SOURCE_DIRECTORY)/%.cpp | directories
	@$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) $(SYSTEM_INCLUDES) -c $< -o $@
	@echo "CXX   | $< -> $@"
$(OUTPUT): $(OBJECT_FILES) | directories
	@$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) $(SYSTEM_INCLUDES) $(OBJECT_FILES) $(LIBRARIES) -o $(OUTPUT)
	@echo "Link  | $(OBJECT_FILES) -> $(OUTPUT)"
-include $(DEPENDENCY_FILES)

delete:
	@if [ -d $(OBJECT_DIRECTORY) ]; then rm -r $(OBJECT_DIRECTORY); echo "RM    | $(OBJECT_DIRECTORY)"; fi
	@if [ -f $(OUTPUT) ]; then rm -r $(OUTPUT); echo "RM    | $(OUTPUT)"; fi
