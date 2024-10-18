directories:
	@if [ ! -d "$(BINARY_DIRECTORY)" ]; then mkdir -p $(BINARY_DIRECTORY); echo "WRITE | $(BINARY_DIRECTORY)"; fi
	@if [ ! -d "$(OBJECT_DIRECTORY)" ]; then mkdir -p $(OBJECT_DIRECTORY); echo "WRITE | $(OBJECT_DIRECTORY)"; fi

$(OBJECT_DIRECTORY)/%.o: $(PROGRAM_SOURCE_DIRECTORY)/%.cpp
	@$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) $(SYSTEM_INCLUDES) -c $< -o $@
	@echo "CXX   | $@"
-include $(DEPENDENCY_FILES)

$(OUTPUT_FILE): $(OBJECT_FILES)
	@$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) $(SYSTEM_INCLUDES) $^ $(LIBRARIES) -o $@
	@echo "LINK  | $@"

delete:
	@if [ -d $(OBJECT_DIRECTORY) ]; then rm -r $(OBJECT_DIRECTORY); echo "RM    | $(OBJECT_DIRECTORY)"; fi
	@if [ -f $(OUTPUT_FILE) ]; then rm -r $(OUTPUT_FILE); echo "RM    | $(OUTPUT_FILE)"; fi
