TEMPLATE = lib
TARGET = netspeak4
VERSION = 1.3.8

QMAKE_CXXFLAGS += -fPIC

COMMON_INCLUDE = netspeak4-application-cpp.pri
!include(../../$$COMMON_INCLUDE) {
  error("Could not find $$COMMON_INCLUDE file")
}

HEADERS += \
    $$PWD/../../src/netspeak/jni/org_netspeak_Netspeak.h \
    $$PWD/../../src/netspeak/jni/jni_utils.hpp \
    $$PWD/../../src/netspeak/jni/jni_md.h \
    $$PWD/../../src/netspeak/jni/jni.h

SOURCES += \
    $$PWD/../../src/netspeak/jni/org_netspeak_Netspeak.cpp \
    $$PWD/../../src/netspeak/jni/jni_utils.cpp
