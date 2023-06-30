#
# repo:                 cpputils
# file:			sys_common.pri
# path:			prj/common/common_qt/sys_common.pri    
# created on:		2023 Jan 10
# Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
#

message ("$${PWD}/sys_common.pri")
cpputilsSysCommonIncluded = 1

isEmpty( cpputilsResolveCommonIncluded ) {
        include("$${PWD}/resolve_common.pri")
        cpputilsResolveCommonIncluded = 1
}

isEmpty( cinternalSysCommonIncluded ) {
        include ( "$${cinternalRepoRoot}/prj/common/common_qt/sys_common.pri" )
        cinternalSysCommonIncluded = 1
}
