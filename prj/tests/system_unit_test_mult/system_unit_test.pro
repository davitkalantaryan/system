#
# file:			system_unit_test.pro
# path:			prj/tests/system_unit_test_mult/system_unit_test.pro
# created on:	2021 Mar 27
# created by:	Davit Kalantaryan
#

message("!!! $${_PRO_FILE_}")
include ( "$${PWD}/../../common/common_qt/flagsandsys_common_private.pri" )
DESTDIR     = "$${artifactRoot}/sys/$${CODENAME}/$$CONFIGURATION/test"

QT -= gui
QT -= core
QT -= widgets
CONFIG -= qt
CONFIG += console

win32{
	LIBS += -lWs2_32
} else {
	LIBS += -pthread
	LIBS += -ldl
}


DEFINES += CINTERNAL_UNIT_TEST_USE_GTEST_LIKE_MACROSES


SOURCES	+= $$files($${systemRepositoryRoot}/src/core/*.c,true)
SOURCES	+= $$files($${systemRepositoryRoot}/src/core/*.cpp,true)
SOURCES += $$files($${systemRepositoryRoot}/src/tests/unit_test/*.cpp)

SOURCES +=  \
    "$${cinternalRepoRoot}/src/core/cinternal_core_logger.c"				\
    "$${cinternalRepoRoot}/src/core/cinternal_core_unit_test_checks.c"			\
    "$${cinternalRepoRoot}/src/core/cinternal_core_unit_test_main.c"



HEADERS =  $$files($${systemRepositoryRoot}/include/*.h,true)
HEADERS += $$files($${systemRepositoryRoot}/include/*.hpp,true)

OTHER_FILES += $$files($${PWD}/*.Makefile)
