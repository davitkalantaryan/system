#
# repo:         systemk
# file:			flags_common.pri
# path:			prj/common/common_qt/flags_common.pri  
# created on:		2023 Jan 10
# Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
#

message ("$${PWD}/flags_common.pri")
systemkFlagsCommonIncluded = 1

isEmpty( systemkResolveCommonIncluded ) {
        include("$${PWD}/resolve_common.pri")
        systemkResolveCommonIncluded = 1
}

isEmpty( cinternalFlagsCommonIncluded ) {
        include ( "$${cinternalRepoRoot}/prj/common/common_qt/flags_common.pri" )
        cinternalFlagsCommonIncluded = 1
}

INCLUDEPATH += $${systemkRepositoryRoot}/include
