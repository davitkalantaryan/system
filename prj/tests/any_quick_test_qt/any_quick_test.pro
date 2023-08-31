#
# file:			any_quick_test.pro
# path:			prj/tests/any_quick_test_qt/any_quick_test.pro
# created on:	2021 Mar 07
# created by:	Davit Kalantaryan
#

message("!!! $${_PRO_FILE_}")
include ( "$${PWD}/../../common/common_qt/flagsandsys_common.pri" )
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
}


SOURCES	+=		\
	"$${PWD}/../../../src/tests/main_any_quick_test.cpp"			\
	"$${PWD}/../../../src/core/cpputils_bigint.cpp"				\
	"$${PWD}/../../../src/core/cpputils_functional_old.cpp"			\
	"$${PWD}/../../../src/core/cpputils_hashtbl.cpp"			\
	"$${PWD}/../../../src/core/cpputils_inscopecleaner.cpp"

HEADERS += $$files($${cpputilsRepoRoot}/include/*.h,true)
HEADERS += $$files($${cpputilsRepoRoot}/include/*.hpp,true)

OTHER_FILES += $$files($${PWD}/../any_quick_test_mkfl/*.Makefile,false)
