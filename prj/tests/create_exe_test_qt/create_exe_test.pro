#
# file:		create_exe_test.pro
# path:		prj/tests/create_exe_test_qt/create_exe_test.pro
# created on:	2021 Sep 26
# creatd by:	Davit Kalantaryan (davit.kalantaryan@desy.de)
#


message("!!! $${_PRO_FILE_}")
include ( "$${PWD}/../../common/common_qt/flagsandsys_common_private.pri" )
DESTDIR     = "$${artifactRoot}/sys/$${CODENAME}/$$CONFIGURATION/test"


QT -= gui
QT -= core
QT -= widgets
CONFIG -= qt
CONFIG += console

win32 {
} else {
	LIBS += -ldl
}


CET_SRC_DIR = $${systemRepositoryRoot}/src/tests/create_exe
CORE_SRC_DIR = $${systemRepositoryRoot}/src/core

SOURCES +=	$$files($${CORE_SRC_DIR}/*.c,true)
SOURCES +=	$$files($${CORE_SRC_DIR}/*.cpp,true)
SOURCES +=	$$files($${CET_SRC_DIR}/*.c,true)
SOURCES +=	$$files($${CET_SRC_DIR}/*.cpp,true)
