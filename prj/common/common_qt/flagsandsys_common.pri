#
# repo:			system
# name:			flagsandsys_common.pri
# path:			prj/common/common_qt/flagsandsys_common.pri
# created on:   2023 Jun 21
# created by:   Davit Kalantaryan (davit.kalantaryan@desy.de)
# usage:		Use this qt include file to calculate some platform specific stuff
#


message("!!! $${PWD}/flagsandsys_common.pri")

isEmpty(systemFlagsAndSysCommonIncluded){
    systemFlagsAndSysCommonIncluded = 1

    systemRepositoryRoot = $${PWD}/../../..
        include("$${systemRepositoryRoot}/contrib/cinternal/prj/common/common_qt/flagsandsys_common.pri")

    isEmpty(artifactRoot) {
        artifactRoot = $$(artifactRoot)
		isEmpty(artifactRoot) {
		        artifactRoot = $${systemRepositoryRoot}
		}
    }

    INCLUDEPATH += $${systemRepositoryRoot}/include

    LIBS	+= -L$${systemRepositoryRoot}/sys/$${CODENAME}/$$CONFIGURATION/lib
    LIBS	+= -L$${systemRepositoryRoot}/sys/$${CODENAME}/$$CONFIGURATION/tlib

    OTHER_FILES += $$files($${PWD}/../common_mkfl/*.Makefile,true)
}
