include make/flags.mk
include make/files.mk

build: $(OUTPUT)
prepare: directories
utility: compile_commands clang-format
clean: delete

include make/build.mk
include make/utility.mk
