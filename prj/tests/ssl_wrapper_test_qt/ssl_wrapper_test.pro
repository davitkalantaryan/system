#
# file:			any_quick_test.pro
# path:			prj/tests/any_quick_test_qt/any_quick_test.pro
# created on:	2021 Mar 07
# created by:	Davit Kalantaryan
#

message("!!! $${_PRO_FILE_}")
include ( "$${PWD}/../../common/common_qt/flagsandsys_common_private.pri" )
DESTDIR     = "$${ArifactFinal}/test"

#QT -= gui
#QT -= core
#QT -= widgets
#CONFIG -= qt
QT += network
CONFIG += console

DEFINES += CPPUTILS_DO_NOT_USE_STD_FUNCTION


win32{
	LIBS += -lWs2_32
} else {
	LIBS += -pthread
	LIBS += -ldl
}

INCLUDEPATH += $${SYSTEM_QT_INSTALL_PATH}/Src/qtwebengine/src/3rdparty/chromium/third_party/boringssl/src/include

QMAKE_CXXFLAGS -= $${CinternalStrongWarings}
QMAKE_CFLAGS -= $${CinternalStrongWarings}

# SOURCES += $$files($${PWD}/../../../src/core/*.c,true)
# SOURCES += $$files($${PWD}/../../../src/core/*.cpp,true)

SOURCES	+=		\
        "$${systemRepositoryRoot}/src/tests/main_ssl_wrapper_test.cpp"  \
        "$${systemRepositoryRoot}/src/core/system_find_symbol_address.cpp"  \
        "$${cinternalRepoRoot}/src/core/cinternal_core_logger.c"

HEADERS += $$files($${systemRepositoryRoot}/include/*.h,true)
HEADERS += $$files($${systemRepositoryRoot}/include/*.hpp,true)

OTHER_FILES += $$files($${PWD}/../any_quick_test_mkfl/*.Makefile,false)
