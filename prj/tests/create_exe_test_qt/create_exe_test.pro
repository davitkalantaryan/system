#
# file:		create_exe_test.pro
# path:		prj/tests/create_exe_test_qt/create_exe_test.pro
# created on:	2021 Sep 26
# creatd by:	Davit Kalantaryan (davit.kalantaryan@desy.de)
#




include ("$${PWD}/../../common/common_qt/sys_common.pri")

# we reevaluate this
DESTDIR = "$${PRJ_PWD}/$${SYSTEM_PATH}/test"

QT -= gui
QT -= core
QT -= widgets
CONFIG -= qt

LIBS += -ldl


INCLUDEPATH += $${cpputilsRepoRoot}/include
INCLUDEPATH += $${cpputilsRepoRoot}/src/include


CET_SRC_DIR = $${repositoryRoot}/src/tests/create_exe
CORE_SRC_DIR = $${repositoryRoot}/src/core

SOURCES_FROM_CET1 = $$cpputilsFindFilesRecursive($$CET_SRC_DIR, .cpp)
HEADERS_FROM_CET1 = $$cpputilsFindFilesRecursive($$CET_SRC_DIR, .hpp)
SOURCES_FROM_CORE = $$cpputilsFindFilesRecursive($$CORE_SRC_DIR, .cpp)
HEADERS_FROM_CORE = $$cpputilsFindFilesRecursive($$CORE_SRC_DIR, .hpp)
HEADERS_FROM_INC1 = $$cpputilsFindFilesRecursive($${cpputilsRepoRoot}/include, .hpp)
HEADERS_FROM_INC2 = $$cpputilsFindFilesRecursive($${cpputilsRepoRoot}/include, .h)
HEADERS_FROM_SINC1 = $$cpputilsFindFilesRecursive($${cpputilsRepoRoot}/src/include, .hpp)
HEADERS_FROM_SINC2 = $$cpputilsFindFilesRecursive($${cpputilsRepoRoot}/src/include, .h)

SOURCES +=	$${SOURCES_FROM_CET1}
SOURCES +=	$${SOURCES_FROM_CORE}
HEADERS +=	$${HEADERS_FROM_CET1}
HEADERS +=	$${HEADERS_FROM_CORE}
HEADERS +=	$${HEADERS_FROM_INC1}
HEADERS +=	$${HEADERS_FROM_INC2}
HEADERS +=	$${HEADERS_FROM_SINC1}
HEADERS +=	$${HEADERS_FROM_SINC2}
