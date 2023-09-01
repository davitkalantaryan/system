#
# repo:		system
# file:		flagsandsys_common_pure.windows.Makefile
# created on:	2020 Dec 14
# created by:	Davit Kalantaryan (davit.kalantaryan@desy.de)
# purpose:	This file can be only as include
#

!IFNDEF MakeFileDir
MakeFileDir			= $(MAKEDIR)\..
!ENDIF

!IFNDEF systemRepositoryRoot
systemRepositoryRoot	= $(MakeFileDir)\..\..\..
!ENDIF

!IFNDEF artifactRoot
artifactRoot	= $(systemRepositoryRoot)
!ENDIF

!IFNDEF cinternalRepoRoot
cinternalRepoRoot	= $(systemRepositoryRoot)\contrib\cinternal
!ENDIF

!include <$(cinternalRepoRoot)\prj\common\common_mkfl\flagsandsys_common.windows.Makefile>

CFLAGS				= $(CFLAGS) /I"$(systemRepositoryRoot)\include"

LFLAGS				= $(LFLAGS) /LIBPATH:"$(systemRepositoryRoot)\sys\win_$(Platform)\$(Configuration)\lib"
LFLAGS				= $(LFLAGS) /LIBPATH:"$(systemRepositoryRoot)\sys\win_$(Platform)\$(Configuration)\tlib"
