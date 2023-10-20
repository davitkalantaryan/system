#
# repo:		system
# file:		flagsandsys_common.windows.Makefile
# created on:	2020 Dec 14
# created by:	Davit Kalantaryan (davit.kalantaryan@desy.de)
# purpose:	This file can be only as include
#

!IFNDEF systemFlagsAndSysCommonIncluded
systemFlagsAndSysCommonIncluded		= 1
!IFNDEF systemRepositoryRoot
systemRepositoryRoot	= $(MAKEDIR)\..\..\..
!ENDIF
!include <$(systemRepositoryRoot)\prj\common\common_mkfl\raw\flagsandsys_common_raw.windows.Makefile>
!ENDIF
