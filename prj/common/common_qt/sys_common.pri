# File sys_common.pri
# File created : 12 Feb 2017
# Created by : Davit Kalantaryan (davit.kalantaryan@desy.de)
# This file can be used to produce Makefile for daqadcreceiver application
# for PITZ
#

#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-variable
#QMAKE_CXXFLAGS_WARN_ON += -Wno-sign-compare
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-function
#QMAKE_CXXFLAGS_WARN_ON -= -Wunused-function

STATIC_LIB_EXTENSION	= a
LIB_PREFIX	= lib
TARGET_PATH_EXTRA	=

isEmpty( cpputilsRepoRoot ) {
	cpputilsRepoRoot += $${PWD}/../../..
}

isEmpty( repositoryRoot ) {
	repositoryRoot = $${cpputilsRepoRoot}
}

defineReplace(cpputilsFindFilesRecursive){
	# todo: we should switch to _sp scheme
	#findFilesInit = $$system($$system_quote($$system_path($${cpputilsRepoRoot}/scripts/findfiles_sp)) $$system_quote($$system_path($$1)) $$2)
	#for(var, $$list($${findFilesInit})) {
	#	length = $$str_size($$var)
	#	stripedVar = $$str_member($$var,1,$$num_add($$length, -2))
	#	returnVar += $$stripedVar
	#}
	#return ($$returnVar)
	
	findFilesInit = $$system($$system_quote($$system_path($${cpputilsRepoRoot}/scripts/findfiles_no_sp)) $$system_quote($$system_path($$1)) $$2)
	return ($$findFilesInit)	
}

contains( TEMPLATE, lib ) {
    TARGET_PATH=lib
    #message("Shared library creation")
} else {
    TARGET_PATH=bin
    #message("Binary file creation")
}


PRJ_PWD_TMP = $$PRJ_PWD
isEmpty(PRJ_PWD_TMP) {
    PRJ_PWD = $${PWD}/../../..
}


win32 {
	STATIC_LIB_EXTENSION = lib
	LIB_PREFIX =
    contains(QMAKE_TARGET.arch, x86_64) {
        ## Windows x64 (64bit) specific build here
        CODENAME = win_x64
        SYSTEM_PATH = sys/win_x64

    } else {
        ## Windows x86 (32bit) specific build here
        CODENAME = win_x86
        SYSTEM_PATH = sys/win_x86

    }

} else:mac {
    CODENAME = mac
    SYSTEM_PATH = sys/mac
} else:android {
    # ANDROID_TARGET_ARCH values 1."armeabi-v7a", 2."arm64-v8a", 3."x86", 4."x86_64"
    # x86 and x86_64 are simulators
    message("------------ ANDROID_TARGET_ARCH=$$ANDROID_TARGET_ARCH")
    CODENAME = android_$${ANDROID_TARGET_ARCH}
    SYSTEM_PATH = sys/android_$${ANDROID_TARGET_ARCH}

    #contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
	##ANDROID_EXTRA_LIBS += $$PWD/../platform/android/openssl/armeabi-v7a/libcrypto.so
	##ANDROID_EXTRA_LIBS += $$PWD/../platform/android/openssl/armeabi-v7a/libssl.so
	#CODENAME = android_armeabi-v7a
	#SYSTEM_PATH = sys/android_armeabi-v7a
    #}

} else:linux {
    DEFINES += LINUX
    CODENAME = $$system(lsb_release -c | cut -f 2)
    SYSTEM_PATH = sys/$$CODENAME
} else:ios {
    CODENAME = ios
    SYSTEM_PATH = sys/$$CODENAME
} else:wasm {
	STATIC_LIB_EXTENSION	= ba
    CODENAME = wasm
    SYSTEM_PATH = sys/$$CODENAME
	TARGET_PATH_EXTRA = /$${TARGET}
} else {
    CODENAME = unknownCodeName
    SYSTEM_PATH = sys/$$CODENAME
}

CONFIGURATION=Profile
nameExtension=

CONFIG(debug, debug|release) {
	nameExtension=d
	CONFIGURATION=Debug
	DEFINES += CPPUTILS_DEBUG
} else:CONFIG(release, debug|release) {
	CONFIGURATION=Release
}

message("!!! sys_common.pri: SYSTEM_PATH/CONFIGURATION=$${PRJ_PWD}/$${SYSTEM_PATH}/$${CONFIGURATION}")

# Debug:DESTDIR = debug1
SYSTEM_PATH=$$SYSTEM_PATH/$$CONFIGURATION
MAKEFILE = Makefile.$${TARGET}.$${CODENAME}.$$CONFIGURATION
DESTDIR = $${PRJ_PWD}/$${SYSTEM_PATH}/$${TARGET_PATH}$${TARGET_PATH_EXTRA}
OBJECTS_DIR = $${PRJ_PWD}/$${SYSTEM_PATH}/.objects/$${TARGET}
MOC_DIR = $${PWD}/$$CODENAME/$$CONFIGURATION
UI_DIR  = $${PWD}/$$CODENAME/$$CONFIGURATION


#CONFIG += debug
#CONFIG += c++11
#QMAKE_CXXFLAGS += -std=c++0x
# greaterThan(QT_MAJOR_VERSION, 4):QT += widgets
#QT -= core
#QT -= gui
