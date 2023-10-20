#
# file:			system_all.pro
# path:			workspaces/system_all_qt/system_all.pro
# created on:		2023 Aug 31
# Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
#

message("!!! $${_PRO_FILE_}")

TEMPLATE = subdirs
#CONFIG += ordered

include ( "$${PWD}/../../prj/common/common_qt/flagsandsys_common.pri" )

SUBDIRS		+=	"$${systemRepositoryRoot}/prj/tests/create_exe_test_qt/create_exe_test.pro"
SUBDIRS		+=	"$${systemRepositoryRoot}/prj/tests/any_quick_test_qt/any_quick_test.pro"
SUBDIRS		+=	"$${systemRepositoryRoot}/prj/tests/system_unit_test_mult/system_unit_test.pro"

cinternalFromHere{
        SUBDIRS	+= "$${cinternalRepoRoot}/workspaces/cinternal_all_qt/cinternal_all.pro"
}


greaterThan(QT_MAJOR_VERSION, 4):QT += widgets

OTHER_FILES += $$files($${systemRepositoryRoot}/docs/*.md,true)
OTHER_FILES += $$files($${systemRepositoryRoot}/docs/*.txt,true)
OTHER_FILES += $$files($${systemRepositoryRoot}/scripts/*.sh,true)
OTHER_FILES += $$files($${systemRepositoryRoot}/scripts/*.bat,true)
OTHER_FILES += $$files($${systemRepositoryRoot}/.github/*.yml,true)

OTHER_FILES	+=	\
        $${systemRepositoryRoot}/.gitattributes		\
	$${systemRepositoryRoot}/.gitignore			\
	$${systemRepositoryRoot}/LICENSE			\
	$${systemRepositoryRoot}/README.md
