#
# repo:         system
# name:         flagsandsys_common.pri
# path:         prj/common/common_qt/flagsandsys_common.pri
# created on:   2023 Jun 21
# created by:   Davit Kalantaryan (davit.kalantaryan@desy.de)
# usage:	Use this qt include file to calculate some platform specific stuff
#


message("!!! $${PWD}/flagsandsys_common.pri")

isEmpty(systemFlagsAndSysCommonIncluded){
    systemFlagsAndSysCommonIncluded = 1

    systemRepositoryRoot = $${PWD}/../../..

    isEmpty(artifactRoot) {
        artifactRoot = $$(artifactRoot)
        isEmpty(artifactRoot) {
            artifactRoot = $${systemRepositoryRoot}
        }
    }

    include("$${systemRepositoryRoot}/contrib/cinternal/prj/common/common_qt/flagsandsys_common.pri")
    SYSTEM_QT_INSTALL_PATH = $$[QT_INSTALL_PREFIX]
    SYSTEM_QT_INSTALL_DIR = $$dirname(SYSTEM_QT_INSTALL_PATH)
    message("SYSTEM_QT_INSTALL_DIR: $${SYSTEM_QT_INSTALL_DIR}")

    INCLUDEPATH += $${systemRepositoryRoot}/include

    exists($${systemRepositoryRoot}/sys/$${CODENAME}/$$CONFIGURATION/lib) {
        LIBS += -L$${systemRepositoryRoot}/sys/$${CODENAME}/$$CONFIGURATION/lib
    }
    exists($${systemRepositoryRoot}/sys/$${CODENAME}/$$CONFIGURATION/tlib) {
        LIBS += -L$${systemRepositoryRoot}/sys/$${CODENAME}/$$CONFIGURATION/tlib
    }

    OTHER_FILES += $$files($${PWD}/../common_mkfl/*.Makefile,true)
}
