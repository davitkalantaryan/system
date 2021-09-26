

TEMPLATE = subdirs
#CONFIG += ordered

repositoryRoot = $${PWD}/../..

include ( "$${repositoryRoot}/prj/common/common_qt/sys_common.pri" )

SUBDIRS		+=	"$${repositoryRoot}/prj/tests/create_exe_test_qt/create_exe_test.pro"

UNIX_SCRIPTS	= $$cpputilsFindFilesRecursive($${repositoryRoot}/scripts, .sh)
WINDOWS_SCRIPTS	= $$cpputilsFindFilesRecursive($${repositoryRoot}/scripts, .bat)
MDS_IN_DOCS	= $$cpputilsFindFilesRecursive($${repositoryRoot}/docs, .md)
TXT_IN_DOCS	= $$cpputilsFindFilesRecursive($${repositoryRoot}/docs, .txt)
#YML_GH_WORKS	= $$cpputilsFindFilesRecursive($${repositoryRoot}/.github, .yml)
#YML_GL_WORKS	= $$cpputilsFindFilesRecursive($${repositoryRoot}/.gitlab, .yml)

OTHER_FILES += $$UNIX_SCRIPTS
OTHER_FILES += $$WINDOWS_SCRIPTS
OTHER_FILES += $$MDS_IN_DOCS
OTHER_FILES += $$TXT_IN_DOCS
OTHER_FILES += $$YML_GH_WORKS
OTHER_FILES += $$YML_GL_WORKS

OTHER_FILES	+=	\
	"$${repositoryRoot}/.gitattributes"					\
	"$${repositoryRoot}/.gitignore"						\
	"$${repositoryRoot}/.gitmodules"					\
	"$${repositoryRoot}/LICENSE"						\
	"$${repositoryRoot}/README.md"						\
	\
	"$${PWD}/../../scripts/findfiles_no_sp"					\
	"$${PWD}/../../scripts/findfiles_sp"					\
	\
	"$${PWD}/../../prj/common/common_mkfl/unix.common.Makefile"		\
	"$${PWD}/../../prj/common/common_mkfl/windows.common.Makefile"		\
