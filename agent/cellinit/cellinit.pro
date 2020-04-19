PARTY_LIBS += \
    gflags \

#amster 需要加载的lib库
AMSTER_LIB_DIRS += \
    $$PWD/../../bbs/bb \

include(../agent-config.pri)

HEADERS += \
    cellinitagent.h

SOURCES += \
    cellinitagent.cpp
