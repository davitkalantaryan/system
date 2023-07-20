#
# file:			sys_common.pri
# path:			prj/common/common_qt/sys_common.pri    
# created on:		2023 Jan 10
# Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
#

message ("$${PWD}/resolve_common.pri")
systemkResolveCommonIncluded = 1

isEmpty( systemkRepositoryRoot ) {
        systemkRepositoryRoot = $$(systemkRepositoryRoot)
        isEmpty(systemkRepositoryRoot) {
            systemkRepositoryRoot = $${PWD}/../../..
        }
}

isEmpty( repositoryRoot ) {
        repositoryRoot = $$(repositoryRoot)
        isEmpty(repositoryRoot) {
            repositoryRoot = $${systemkRepositoryRoot}
        }
}

isEmpty(artifactRoot) {
    artifactRoot = $$(artifactRoot)
    isEmpty(artifactRoot) {
        artifactRoot = $${repositoryRoot}
    }
}

isEmpty( cinternalRepoRoot ) {
        cinternalRepoRoot = $$(cinternalRepoRoot)
        isEmpty(cinternalRepoRoot) {
            cinternalRepoRoot=$${systemkRepositoryRoot}/contrib/cinternal
        }
}

