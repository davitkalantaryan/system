#
# file:			any_quick_test.pro
# path:			prj/tests/any_quick_test_qt/any_quick_test.pro
# created on:	2021 Mar 07
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

DEFINES += CPPUTILS_DO_NOT_USE_STD_FUNCTION

win32{
	LIBS += -lWs2_32
} else {
	LIBS += -pthread
	LIBS += -ldl
}

SOURCES += $$files($${PWD}/../../../src/core/*.c,true)
SOURCES += $$files($${PWD}/../../../src/core/*.cpp,true)

SOURCES	+=		\
        "$${PWD}/../../../src/tests/main_any_quick_test.c"

HEADERS += $$files($${systemRepositoryRoot}/include/*.h,true)
HEADERS += $$files($${systemRepositoryRoot}/include/*.hpp,true)

OTHER_FILES += $$files($${PWD}/../any_quick_test_mkfl/*.Makefile,false)
