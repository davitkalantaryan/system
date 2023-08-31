

targetName=system_all

mkfile_path		=  $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir		=  $(shell dirname $(mkfile_path))

ifndef systemRepositoryRoot
        systemRepositoryRoot	:= $(shell curDir=`pwd` && cd $(mkfile_dir)/../.. && pwd && cd ${curDir})
endif


all:
	$(MAKE) -f $(systemRepositoryRoot)/prj/tests/system_unit_test_mult/system_unit_test.unix.Makefile			&& \
	$(MAKE) -f $(systemRepositoryRoot)/prj/tests/any_quick_test_mkfl/any_quick_test.unix.Makefile

.PHONY: clean
clean:
	$(MAKE) -f $(systemRepositoryRoot)/prj/tests/system_unit_test_mult/system_unit_test.unix.Makefile clean
	$(MAKE) -f $(systemRepositoryRoot)/prj/tests/any_quick_test_mkfl/any_quick_test.unix.Makefile clean
	