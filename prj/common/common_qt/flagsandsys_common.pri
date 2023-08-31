#
# repo:			cpputils
# name:			flagsandsys_common.pri
# path:			prj/common/common_qt/flagsandsys_common.pri
# created on:   2023 Jun 21
# created by:   Davit Kalantaryan (davit.kalantaryan@desy.de)
# usage:		Use this qt include file to calculate some platform specific stuff
#


message("!!! $${PWD}/flagsandsys_common.pri")

isEmpty(cpputilsFlagsAndSysCommonIncluded){
    cpputilsFlagsAndSysCommonIncluded = 1

    cpputilsRepoRoot = $${PWD}/../../..
	include("$${cpputilsRepoRoot}/contrib/cinternal/prj/common/common_qt/flagsandsys_common.pri")

    isEmpty(artifactRoot) {
        artifactRoot = $$(artifactRoot)
		isEmpty(artifactRoot) {
			artifactRoot = $${cpputilsRepoRoot}
		}
    }

    INCLUDEPATH += $${cpputilsRepoRoot}/include
}
