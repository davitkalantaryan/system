#
# repo:		    system
# file:		    flagsandsys_common_pure.unix.Makefile
# created on:	    2020 Dec 14
# created by:	    Davit Kalantaryan (davit.kalantaryan@desy.de)
# purpose:	    This file can be only as include
#

ifndef systemRepositoryRoot
        mkfile_path		=  $(abspath $(lastword $(MAKEFILE_LIST)))
        mkfile_dir		=  $(shell dirname $(mkfile_path))
        systemRepositoryRoot	:= $(shell curDir=`pwd` && cd $(mkfile_dir)/../../../.. && pwd && cd ${curDir})
endif

ifndef artifactRoot
        artifactRoot	= $(systemRepositoryRoot)
endif

include $(systemRepositoryRoot)/contrib/cinternal/prj/common/common_mkfl/flagsandsys_common.unix.Makefile

COMMON_FLAGS	+= -I$(systemRepositoryRoot)/include

LIBS	+= -L$(systemRepositoryRoot)/sys/$(lsbCode)/$(Configuration)/lib
LIBS	+= -L$(systemRepositoryRoot)/sys/$(lsbCode)/$(Configuration)/tlib
