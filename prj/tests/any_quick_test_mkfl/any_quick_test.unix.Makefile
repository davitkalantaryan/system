
targetName=any_quick_test

mkfile_path		=  $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir		=  $(shell dirname $(mkfile_path))

firstTarget: all

include $(mkfile_dir)/../../common/common_mkfl/flagsandsys_common_private.unix.Makefile

CORE_SRCS_DIR=$(systemRepositoryRoot)/src/core
CORE_SRCS_CPP	= $(shell find $(CORE_SRCS_DIR) -name "*.cpp")
CORE_SRCS_C	= $(shell find $(CORE_SRCS_DIR) -name "*.c")

SOURCES += $(systemRepositoryRoot)/src/tests/main_any_quick_test.c

LIBS += -pthread
LIBS += -ldl
CPPFLAGS += -std=c++11

all: $(artifactRoot)/sys/$(lsbCode)/$(Configuration)/test/$(targetName)

$(artifactRoot)/sys/$(lsbCode)/$(Configuration)/test/$(targetName): \
                $(SOURCES:%=$(artifactRoot)/sys/$(lsbCode)/$(Configuration)/.objects/$(targetName)/%.o)	    \
		$(CORE_SRCS_CPP:%=$(artifactRoot)/sys/$(lsbCode)/$(Configuration)/.objects/$(targetName)/%.o)		\
		$(CORE_SRCS_C:%=$(artifactRoot)/sys/$(lsbCode)/$(Configuration)/.objects/$(targetName)/%.o)
	@mkdir -p $(@D)
	@$(LINK) $^ $(LIBS) $(LFLAGS) -o $@

.PHONY: clean
clean:
	@rm -rf $(artifactRoot)/sys/$(lsbCode)/$(Configuration)/.objects/$(targetName)
	@rm -f  $(artifactRoot)/sys/$(lsbCode)/$(Configuration)/test/$(targetName)
	@echo "  " cleaning of $(targetName) complete !!!
